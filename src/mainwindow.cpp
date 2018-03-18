#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "qbattery.h"
#include "backplay.h"

QObject * dstWnd;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 隐藏标题栏
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置窗体背景
    setAutoFillBackground(true);

    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));
    setPalette(palette);

    ui->setupUi(this);

    ui->label->setText(tr(""));

    dstWnd = ui->label;

    ui->battery->setValue(50);

    current_time = QDateTime::currentDateTime();
    current_time_str = current_time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << current_time_str << "-=-=-=-=-=-=-=-";

    QFont font("Arial",12,QFont::Bold,false);
    ui->label_2->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_2->setPalette(pe);
    ui->label_2->setText(current_time_str);

    updateRTC_timer=new QTimer(this);
    updateRTC_timer->setInterval(50);
    connect(updateRTC_timer,SIGNAL(timeout()),this,SLOT(UpdateRTC()));
    updateRTC_timer->start();

    capt_btn=new Button(this);
    capt_btn->setButtonPicture(QPixmap(":/images/capt1.png"));
    capt_btn->setPressPicture(QPixmap(":/images/capt2.png"));
    capt_btn->setReleasePicture(QPixmap(":/images/capt4.png"));
    capt_btn->setEnterPicture(QPixmap(":/images/capt3.png"));
    capt_btn->setLeavePicture(QPixmap(":/images/capt5.png"));
    //capt_btn->setGeometry(QRect(470,170,71,71));
    capt_btn->set_X_Y_width_height(490,170,71,71);


    record_btn=new Button(this);
    record_btn->setButtonPicture(QPixmap(":/images/start_r.png"));
    record_btn->setPressPicture(QPixmap(":/images/recording.jpg"));
    record_btn->setReleasePicture(QPixmap(":/images/recording.jpg"));
    record_btn->setEnterPicture(QPixmap(":/images/recording.jpg"));
    record_btn->setLeavePicture(QPixmap(":/images/stop_r.png"));
    record_btn->set_X_Y_width_height(490,280,71,71);

    v4l2thread.capture_lock=false;
    v4l2thread.video_recording=false;
    v4l2thread.start();
    std::cout << "thread 1 running" << std::endl;
    connect(this, SIGNAL(call_dialog()), &v4l2thread, SLOT(blank_osd1()));

    connect(this, SIGNAL(call_dialog()), this, SLOT(call_testdialog()));

    connect(this, SIGNAL(call_capture()), this, SLOT(capture()));
    connect(this, SIGNAL(call_capture()), &v4l2thread, SLOT(video0_capture()));

    connect(&v4l2thread,SIGNAL(capture_ok()),this,SLOT(capture_ok()));
    connect(&v4l2thread,SIGNAL(capture_fail()),this,SLOT(capture_fail()));

    connect(this, SIGNAL(call_rcdstarstop()), this, SLOT(rcdstarstop()));
    connect(this, SIGNAL(call_rcdstarstop()), &v4l2thread, SLOT(rcdstarstop()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::rcdstarstop()
{
    qDebug() <<"in slot rcdstarstop() @ mainwindow";

    if(v4l2thread.video_recording==false)
    {
        v4l2thread.video_recording=true;
        record_btn->setIcon(QIcon(*record_btn->enterPicture));
        qDebug() << "recording is true now";
    }
    else
    {
        v4l2thread.video_recording=false;
        record_btn->setIcon(QIcon(*record_btn->leavePicture));
        qDebug() << "recording is false now";
    }
}

void MainWindow::call_testdialog()
{
    qDebug() <<"in slot call_testdialog()" << endl;

    MainMenu mainmenu(this);
    mainmenu.exec();
}

void MainWindow::msecSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() <dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::capture_ok()
{
    qDebug() <<"in slot capture_ok()" << endl;
    this->msecSleep(1000);
    capt_btn->setIcon(QIcon(*capt_btn->releasePicture));
    v4l2thread.capture_lock=false;
}

void MainWindow::capture_fail()
{
    qDebug() <<"in slot capture_fail()" << endl;
    this->msecSleep(1000);
    capt_btn->setIcon(QIcon(*capt_btn->leavePicture));
    v4l2thread.capture_lock=false;
}


void MainWindow::capture()
{
    qDebug() <<"in slot capture()" << endl;
    capt_btn->setIcon(QIcon(*capt_btn->enterPicture));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        oldPos = event->globalPos();
        press = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (press)
    {
        this->move(this->pos() + event->globalPos() - oldPos);
        oldPos = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    press = false;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int keyValue;
    keyValue=event->key();

    qDebug() << "DlgLanguage Pressed Key is" << QKeySequence(event->modifiers() + event->key());

    switch (keyValue)
    {
        case Qt::Key_Return:
        {
                ui->battery->addValue();
        }
        break;
        case Qt::Key_M://M键盘，主要用于返回上一层
        {
        }
        break;
        case Qt::Key_Left://
        {
        }
        break;
        case Qt::Key_Right://
        {
        }
        break;
        case Qt::Key_T://白平衡测试按钮，按下记录白色
        {
        }
        break;
        case Qt::Key_P://Play按键
        {
        }
        break;
        case Qt::Key_C://Captrue按键
        {
        }
        break;
        case Qt::Key_A://板子未带
        {
            ui->battery->subValue();//电池电量调整
        }
        break;

        case Qt::Key_B://板子未带
        emit call_dialog();
        break;

        case Qt::Key_Z://板子未带
        if(v4l2thread.capture_lock==false)
        {
            emit call_capture();//拍照
        }
        break;

        case Qt::Key_X://板子未带
        emit call_rcdstarstop();//录像
        break;

        default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int keyValue;
    keyValue=event->key();
    qDebug() <<"released keyValue= "<< keyValue << endl;

    switch (keyValue) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
    }
    break;
    case Qt::Key_A:
    {
    }
    break;

    default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}

void MainWindow::UpdateRTC()
{
    current_time = QDateTime::currentDateTime();
    current_time_str = current_time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_2->setText(current_time_str);
}



