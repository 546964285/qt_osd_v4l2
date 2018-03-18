#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "qbattery.h"
#include "backplay.h"

QObject * dstWnd;

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{ 
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景

    QPalette  palette;
    //palette.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));//设置背景为透明
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景有颜色
    setPalette(palette);

    ui->setupUi(this);
    ui->label->setText(tr(""));//清除控件文字内容
    dstWnd = ui->label;

    ui->battery->setValue(50);//电池电量设置

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

void MainWindow::rcdstarstop()//视频录制开始停止控制
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

//void MainWindow::capture_ok()
//{
//    qDebug() <<"in slot capture_ok()" << endl;

//    QTime delayTime = QTime::currentTime().addMSecs(1000);  //在当前时间上增加1S
//    while( QTime::currentTime() < delayTime)
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

//    capt_btn->setIcon(QIcon(*capt_btn->releasePicture));
//}

//void MainWindow::capture_fail()
//{
//    qDebug() <<"in slot capture_fail()" << endl;

//    QTime delayTime1 = QTime::currentTime().addMSecs(1000);  //在当前时间上增加1S
//    while( QTime::currentTime() < delayTime1)
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//    capt_btn->setIcon(QIcon(*capt_btn->leavePicture));
//}

void MainWindow::msecSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() <dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::capture_ok()
{
    qDebug() <<"in slot capture_ok()" << endl;

    //    SleeperThread::msleep(1000);
    this->msecSleep(1000);
    capt_btn->setIcon(QIcon(*capt_btn->releasePicture));
    v4l2thread.capture_lock=false;
}

void MainWindow::capture_fail()
{
    qDebug() <<"in slot capture_fail()" << endl;

    //    SleeperThread::msleep(1000);
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

int MainWindow::GetI2CValue()
{
    int fd2,ret;
    char *i2c_device = "/dev/bq40z50_control";
    unsigned char buffer[2];

    printf("opening %s!\n",i2c_device);
    if((fd2 = open(i2c_device,O_RDWR|O_NDELAY))<0)
    printf("APP open %s failed\n",i2c_device);
    else{
             printf("APP open %s success!\n",i2c_device);
    }
    buffer[0] = 13;
    printf("buffer0 is-------------------- %d \n",buffer[0]);
    //buffer[0] = atoi(argv2[1]);
    //printf("buffer0 is-------------------- %d \n",buffer[0]);
    ret = read(fd2,buffer,2);
    if(ret<0)
    printf("i2c read failed!\n");
    else{
           printf("i2c read success Buffer[0] is %d!\n",buffer[0]);
           printf("i2c read success Buffer[1] is %d!\n",buffer[1]);
           printf("i2c read success reg 0x09 data is %d!\n",buffer[1]*256+buffer[0]);
    }

    printf("i2c read failed Buffer[0] is %d!\n",buffer[0]);
    printf("i2c read failed Buffer[1] is %d!\n",buffer[1]);
    printf("i2c read failed reg 0x09 data is %d!\n",buffer[1]*256+buffer[0]);

    //close(fd2);
    ret=buffer[1]*256+buffer[0];
    return ret;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int keyValue;
    keyValue=event->key();
    qDebug() <<"pressed keyValue= "<< keyValue << endl;

    switch (keyValue)
    {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            //ui->battery->addValue();

            ui->battery->setValue(GetI2CValue());
            qDebug() <<"pressed --Key_Return";
        }
        break;
        case Qt::Key_A:
        {
            ui->battery->subValue();
        }
        break;

        case Qt::Key_B:
        emit call_dialog();
        break;

        case Qt::Key_Z:
        //  capt_btn->keyPressEvent(event);
        //  capt_btn->setIcon(QIcon(*capt_btn->enterPicture));
        if(v4l2thread.capture_lock==false)
        {
            emit call_capture();
        }
        break;

        case Qt::Key_X:
        emit call_rcdstarstop();
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

    qDebug() << "released keyValue= " << keyValue << endl;

    switch (keyValue)
    {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            //ui->pushButton->setDown(false);
        }
        break;
        case Qt::Key_A:
        {
            //ui->pushButton_2->setDown(false);
        }
        break;
        //case Qt::Key_Z:
        //{
            //capt_btn->keyReleaseEvent(event);
        //}
        //break;
        default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}

void MainWindow::UpdateRTC()
{
    current_time = QDateTime::currentDateTime();
    current_time_str = current_time.toString("yyyy-MM-dd hh:mm:ss");
    //qDebug() << current_time_str;
    ui->label_2->setText(current_time_str);
    //update();
}



