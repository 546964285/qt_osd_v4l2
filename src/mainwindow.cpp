#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "qbattery.h"
#include "backplay.h"

#include "dlgbackground.h"

QObject * dstWnd;//屏蔽掉好像也没事

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{ 
    dlgf1.setModal(false);//false非模态/true模态
    dlgf1.hide();
    dlgbackground.setModal(false);
    dlgbackground.show();

    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景有颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);

    ui->setupUi(this);
    ui->label->setText(tr(""));//清除控件文字内容
    ui->battery->setValue(70); //电池电量设置

    current_time = QDateTime::currentDateTime();//显示当前时间
    current_time_str = current_time.toString("yyyy-MM-dd hh:mm:ss");

    QFont font("Arial",15,QFont::Bold,false);
    ui->label_2->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_2->setPalette(pe);
    ui->label_2->setText(current_time_str);
    ui->label_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中

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
    capt_btn->set_X_Y_width_height(490,230,71,71);

    record_btn=new Button(this);
    record_btn->setButtonPicture(QPixmap(":/images/start_r.png"));
    record_btn->setPressPicture(QPixmap(":/images/recording.jpg"));
    record_btn->setReleasePicture(QPixmap(":/images/recording.jpg"));
    record_btn->setEnterPicture(QPixmap(":/images/recording.jpg"));
    record_btn->setLeavePicture(QPixmap(":/images/stop_r.png"));
    record_btn->set_X_Y_width_height(490,340,71,71);

    v4l2thread.capture_lock=false;
    v4l2thread.video_recording=false;
    v4l2thread.start();
    std::cout << "thread 1 running" << std::endl;

    connect(this, SIGNAL(call_capture()), this, SLOT(capture()));
    connect(this, SIGNAL(call_capture()), &v4l2thread, SLOT(video0_capture()));
    connect(&v4l2thread,SIGNAL(capture_ok()),this,SLOT(capture_ok()));
    connect(&v4l2thread,SIGNAL(capture_fail()),this,SLOT(capture_fail()));
    connect(this, SIGNAL(call_rcdstarstop()), this, SLOT(rcdstarstop()));
    connect(this, SIGNAL(call_rcdstarstop()), &v4l2thread, SLOT(rcdstarstop()));

    connect(this, SIGNAL(SShowOsd1()), &v4l2thread, SLOT(trans_osd1()));//显示Osd1视频窗口 信号
    connect(this, SIGNAL(SHideOsd1()), &v4l2thread, SLOT(blank_osd1()));//隐藏Osd1视频窗口 信号
    connect(&dlgf1, SIGNAL(SShowMainWindow()), this, SLOT(SlotShowMainWindow()));

    connect(&dlgf1, SIGNAL(SSendNewFolderPath(QString )), this, SLOT(SlotSendNewFolderPath(QString )));
    connect(this, SIGNAL(SSendNewFolderPath(QString )), &v4l2thread, SLOT(SlotSendNewFolderPath(QString )));

    connect(this, SIGNAL(SFreshDlgf1Window()), &dlgf1,SLOT(SlotFreshThisWindow()));

    show();
    key_flg=false;

    g_PowerOnTimeStr=current_time.toString("hhmm");
    qDebug()<< "g_PowerOnTimeStr :" << g_PowerOnTimeStr;
}

MainWindow::~MainWindow()
{
}

void MainWindow::SlotSendNewFolderPath(QString StrNewFolderPath)
{
    qDebug() << "MainWindow Got Singnal StrNewFolderPath is " << StrNewFolderPath;
    emit SSendNewFolderPath(StrNewFolderPath);
}

void MainWindow::SlotShowMainWindow()//显示主窗口对话框
{
    show();
    emit SShowOsd1();//显示Osd1视频窗口 信号
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
    this->msecSleep(500);
    capt_btn->setIcon(QIcon(*capt_btn->releasePicture));
    v4l2thread.capture_lock=false;
}

void MainWindow::capture_fail()
{
    qDebug() <<"in slot capture_fail()" << endl;
    this->msecSleep(500);
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
    if(press)
    {
        this->move(this->pos() + event->globalPos() - oldPos);
        oldPos = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    press = false;
}

//地址0xF0（240）--------0为单片机与bq40z50通信异常，1为正常
//地址0xF1（241）--------电池电量0-100%
//地址0xF2（242）--------1为放电，2为充电
//地址0xF3（243）--------1为开机请求，2为关机请求
//地址0xF4（244）--------2为应用程序保存完数据，可以断电


void MainWindow::GetI2CValue()
{
    int fd2,ret;
    char *i2c_device = "/dev/stm32_control";
    unsigned char buffer[2];

    printf("opening %s!\n",i2c_device);
    if((fd2 = open(i2c_device,O_RDWR|O_NDELAY))<0)
        printf("APP open I2C %s failed\n",i2c_device);
    else
        printf("APP open I2C %s success!\n",i2c_device);

    buffer[0] = 240;
    ret = read(fd2,buffer,2);
    if(ret<0)
        printf("i2c read failed!\n");
    m_bq40z50=buffer[0];
    printf("Address 0xF0 , m_bq40z50 State is %d!\n",m_bq40z50);

    buffer[0] = 241;
    ret = read(fd2,buffer,2);
    if(ret<0)
        printf("i2c read failed!\n");
    m_Battery=buffer[0];
    printf("Address 0xF1 , m_Battery Value is %d!\n",m_Battery);

    buffer[0] = 242;
    ret = read(fd2,buffer,2);
    if(ret<0)
        printf("i2c read failed!\n");
    m_Charge=buffer[0];
    printf("Address 0xF2 , m_Charge State is %d!\n",m_Charge);

    buffer[0] = 243;
    ret = read(fd2,buffer,2);
    if(ret<0)
        printf("i2c read failed!\n");
    m_OnOff=buffer[0];
    printf("Address 0xF3 , m_OnOff State is %d!\n",m_OnOff);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "MainWindow pressed key is" << QKeySequence(event->modifiers() + event->key());
    key_flg=false;
    emit SKeyPressed();
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_M:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_Left:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_Right:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_T:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_P://录像
        {
            key_flg=true;
        }
        break;
        case Qt::Key_C:  //Captrue
        {
            key_flg=true;
        }
        break;
        default:QWidget::keyPressEvent(event);break;
    }
}

QFont Font;
QPalette Palette;

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "MainWindow Released key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            GetI2CValue();
            ui->battery->setValue(m_Battery);
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                key_flg=false;
                hide();
                emit  SHideOsd1();//隐藏Osd1视频窗口 信号
                emit  SFreshDlgf1Window();
                dlgf1.show();    //菜单对话框显示
            }
        }
        break;
        case Qt::Key_Left:
        {
            emit SHideOsd1();//隐藏Osd1视频窗口 信号
            ui->label->setText(tr("充电中......"));//清除控件文字内容
            Font.setPointSize(50);//大小
            ui->label->setFont(Font);
            Palette.setColor(QPalette::WindowText,Qt::white);//颜色
            ui->label->setPalette(Palette);
            ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
        }
        break;
        case Qt::Key_Right:
        {
            //g_PowerOnTimeStr=current_time.toString("hhmm");
            //qDebug()<< "g_PowerOnTimeStr :" << g_PowerOnTimeStr;

            emit SHideOsd1();//隐藏Osd1视频窗口 信号
            ui->label->setText(tr("关机中......"));//清除控件文字内容
            Font.setPointSize(50);//大小
            ui->label->setFont(Font);
            Palette.setColor(QPalette::WindowText,Qt::white);//颜色
            ui->label->setPalette(Palette);
            ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
        }
        break;
        case Qt::Key_T:
        {
            ui->label->setText(tr(""));//清除控件文字内容
            emit SShowOsd1();//显示Osd1视频窗口 信号

            QString month="3",day="4",year="5",hour="13",min="14";
            QString strkk;

            strkk = "date -s " + month + "/" + day + "/" + year;//设置时间，还有问题
            system(strkk.toLatin1().data());
            strkk = "date -s " + hour + ":" + min + ":" + "00";//这句可以
            system(strkk.toLatin1().data());
            system("clock -w"); //强制写入到CMOS

        }
        break;
        case Qt::Key_P:  //录像
        {
                if(key_flg==true)
                {
                    key_flg=false;
                    emit call_rcdstarstop();
                }
        }
        break;
        case Qt::Key_C:  //Captrue
        {
                if(key_flg==true)
                {
                     key_flg=false;
                     if(v4l2thread.capture_lock==false)
                     {
                         emit call_capture();
                     }
                }
        }
        break;
        default:QWidget::keyReleaseEvent(event);break;
      }
      key_flg=false;
}

void MainWindow::UpdateRTC()
{
    current_time = QDateTime::currentDateTime();
    current_time_str = current_time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_2->setText(current_time_str);
}



