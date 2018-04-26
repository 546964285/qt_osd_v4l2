#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "qbattery.h"
#include "backplay.h"
#include <QProcess>
#include <QFile>
#include <sys/types.h>
#include <sys/stat.h>

#include "dlgbackground.h"

#define FIFO_P42Qt "./fifoP42Qt"
#define FIFO_Qt2P4 "./fifoQt2P4"
#define FIFOSIZE   20
int fd_P42Qt;
int fd_Qt2P4;


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

    connect(this, SIGNAL(SFreeze()),   &v4l2thread, SLOT(SlotFreeze()));
    connect(this, SIGNAL(SReFreeze()), &v4l2thread, SLOT(SlotReFreeze()));
    m_Freezed_f=false;

    connect(this, SIGNAL(SWhiteBalance()), &v4l2thread, SLOT(SlotWhiteBalance()));

    connect(this, SIGNAL(SRunAgain()), &v4l2thread, SLOT(SlotRunAgain()));

    connect(this, SIGNAL(SSendNewFolderPath(QString )), &v4l2thread, SLOT(SlotSendNewFolderPath(QString )));

    connect(this, SIGNAL(SFreshDlgf1Window()), &dlgf1,SLOT(SlotFreshThisWindow()));

    show();
    key_flg=false;

    g_PowerOnTimeStr=current_time.toString("hhmm");
    qDebug()<< "g_PowerOnTimeStr :" << g_PowerOnTimeStr;

    OpenI2CDevice();
    SystemScanTimer=new QTimer;//主定时器
    connect(SystemScanTimer,SIGNAL(timeout()),this,SLOT(SlotSystemScan()));
    SystemScanTimer->start(1000);//定时1秒
}

MainWindow::~MainWindow()
{
}

void MainWindow::SlotSystemScan()
{
    GetI2CValue();
    ui->battery->setValue(m_Battery);
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

QFont Font;
QPalette Palette;

void MainWindow::OpenI2CDevice()
{
    char *i2c_device = "/dev/stm32_control";
    qDebug() << "opening I2C /dev/stm32_control!" << i2c_device;
    if((I2Cfd = open(i2c_device,O_RDWR|O_NDELAY))<0)
        qDebug() << "APP open I2C  failed!!" << i2c_device;
    else
        qDebug() << "APP open I2C  success!" << i2c_device;
}

void MainWindow::GetI2CValue()
{
    static bool ShowOsd1_f=false;
    unsigned char buffer[2];
    buffer[0] = 240;
    if(read(I2Cfd,buffer,2)<0)
        qDebug() << "i2c read Address 240 failed!"<< endl;
    m_bq40z50=buffer[0];
    //qDebug() << "Address 0xF0 , m_bq40z50 State is-----" << m_bq40z50;

    buffer[0] = 241;
    if(read(I2Cfd,buffer,2)<0)
        qDebug() << "i2c read Address 241 failed!"<< endl;
    m_Battery=buffer[0];
    //qDebug() << "Address 0xF1 , m_Battery Value is-----" << m_Battery;

    buffer[0] = 242;
    if(read(I2Cfd,buffer,2)<0)
        qDebug() << "i2c read Address 242 failed!"<< endl;
    m_Charge=buffer[0];
    //qDebug() << "Address 0xF2 , m_Charge State is------" << m_Charge;

    buffer[0] = 243;
    if(read(I2Cfd,buffer,2)<0)
        qDebug() << "i2c read Address 243 failed!"<< endl;
    m_OnOff=buffer[0];
    //qDebug() << "Address 0xF3 , m_OnOff State  is------" << m_OnOff;

    if(m_bq40z50==1)
    {
       // qDebug() << "MCU Read bq40z50 Success!!!!!";
    }
    else
    {
        qDebug() << "MCU Read bq40z50 failed!!!!!!";
    }

    if(m_Charge==2)
    {
        emit SHideOsd1();//隐藏Osd1视频窗口 信号
        ui->label->setText(tr("充电中......"));//清除控件文字内容
        Font.setPointSize(50);//大小
        ui->label->setFont(Font);
        Palette.setColor(QPalette::WindowText,Qt::white);//颜色
        ui->label->setPalette(Palette);
        ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中

        ShowOsd1_f=false;
    }
    else
    {
        if(ShowOsd1_f==false)
        {
            ShowOsd1_f=true;
            ui->label->setText(tr(""));//清除控件文字内容
            emit SShowOsd1();//显示Osd1视频窗口 信号
        }
    }

    if((m_OnOff==2)||(m_Battery<3))
    {        
        emit SHideOsd1();//隐藏Osd1视频窗口 信号
        ui->label->setText(tr("关机中......"));//清除控件文字内容
        Font.setPointSize(50);//大小
        ui->label->setFont(Font);
        Palette.setColor(QPalette::WindowText,Qt::white);//颜色
        ui->label->setPalette(Palette);
        ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中

        this->msecSleep(2000);

        //写关机命令
        buffer[0] = 244;
        buffer[1] = 2;
        if(write(I2Cfd,buffer,2)<0)
        {
            qDebug() << "OFF Machine ,i2c write failed!!" << endl;
        }
        else
        {
            qDebug() << "OFF Machine ,i2c write Success!" << endl;
        }
    }
}


//ipipe base address is 0x01C70800
//0x01C709E0    R
//0x01C709E4    Gr
//0x01C709E8    Gb
//0x01C709Ec    B
bool MainWindow::RgbAlgorithm()//算法
{
    QString PicturePath="Balance/Capture.jpg";
    unsigned int Red_ki=0,Green_ki=0,Blue_ki=0;
    bool    FunctionRedOk=false,FunctionGreenOk=false,FunctionBlueOk=false;
    QImage  *P_Image;
    QImage  LoadImage;
    LoadImage.load(PicturePath);
    LoadImage=LoadImage.scaled(120, 100, Qt::IgnoreAspectRatio, Qt::FastTransformation);//快速缩放
    P_Image=&LoadImage;
    unsigned int RedAverage=0 ,GreenAverage=0 ,BlueAverage=0;
    for(int y=0; y<100; y++)//高度
    {
        QRgb *line = (QRgb *)P_Image->scanLine(y);
        for(int x = 0; x<120; x++)//宽度
        {
            RedAverage=RedAverage+(qRed(line[x]));
            GreenAverage=GreenAverage+(qGreen(line[x]));
            BlueAverage=BlueAverage+(qBlue(line[x]));
        }
    }

    QFile file("Balance/Capture.jpg");
    if (file.exists())
    {
        file.remove();
        qDebug() << "file.remove();" ;
    }

    RedAverage   = RedAverage/12000;
    GreenAverage = GreenAverage/12000;
    BlueAverage  = BlueAverage/12000;
    qDebug()<< "Red   is...." <<RedAverage;
    qDebug()<< "Green is...." <<GreenAverage;
    qDebug()<< "Blue  is...." <<BlueAverage;
    double k=RedAverage;
    k=((255/k)-1)*1000;
    unsigned int ki=k;
    if((ki<1000)&&(ki>1))
    {
        qDebug()<< "Red ki is...." <<ki;
        Red_ki=ki;
        FunctionRedOk=true;
    }
    else
    {
        qDebug()<< "Red ki fault return 10K";
        FunctionRedOk=false;
    }
    k=GreenAverage;
    k=((255/k)-1)*1000;
    ki=k;
    if((ki<1000)&&(ki>1))
    {
        qDebug()<< "Green ki is...." <<ki;
        Green_ki=ki;
        FunctionGreenOk=true;
    }
    else
    {
        qDebug()<< "Green ki fault return 10K";
        FunctionGreenOk=false;
    }
    k=BlueAverage;
    k=((255/k)-1)*1000;
    ki=k;
    if((ki<1000)&&(ki>1))
    {
        qDebug()<< "Blue ki is...." <<ki;
        Blue_ki=ki;
        FunctionBlueOk=true;
    }
    else
    {
        qDebug()<< "Blue ki fault return 10K";
        FunctionBlueOk=false;
    }
    if((FunctionRedOk==true)&&(FunctionGreenOk==true)&&(FunctionBlueOk==true))
    {
        Red_ki=512+Red_ki;
        QString Str_Command = QString::number(Red_ki, 10);
        Str_Command = "devmem2 0x01C709E0  w " + Str_Command;
        qDebug() << "Str_Command Change Red Gain is" << Str_Command;
        QProcess ::execute(Str_Command);

        Green_ki=512+Green_ki;
        Str_Command = QString::number(Green_ki, 10);
        Str_Command = "devmem2 0x01C709E4  w " + Str_Command;
        qDebug() << "Str_Command Change GreenR Gain is" << Str_Command;
        QProcess ::execute(Str_Command);

        Str_Command = QString::number(Green_ki, 10);
        Str_Command = "devmem2 0x01C709E8  w " + Str_Command;
        qDebug() << "Str_Command Change GreenB Gain is" << Str_Command;
        QProcess ::execute(Str_Command);

        Blue_ki=512+Blue_ki;
        Str_Command = QString::number(Blue_ki, 10);
        Str_Command = "devmem2 0x01C709Ec  w " + Str_Command;
        qDebug() << "Str_Command Change Blue Gain is" << Str_Command;
        QProcess ::execute(Str_Command);

        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::ClearRGBGain()
{
    QString Str_Command;

    Str_Command = "devmem2 0x01C709E0  w 512";
    qDebug() << "Str_Command Change Red Gain is" << Str_Command;
    QProcess ::execute(Str_Command);


    Str_Command = "devmem2 0x01C709E4  w 512" + Str_Command;
    qDebug() << "Str_Command Change GreenR Gain is" << Str_Command;
    QProcess ::execute(Str_Command);


    Str_Command = "devmem2 0x01C709E8  w 512" + Str_Command;
    qDebug() << "Str_Command Change GreenB Gain is" << Str_Command;
    QProcess ::execute(Str_Command);


    Str_Command = "devmem2 0x01C709Ec  w 512" + Str_Command;
    qDebug() << "Str_Command Change Blue Gain is" << Str_Command;
    QProcess ::execute(Str_Command);
}



void MainWindow::OpenFifo()
{
    printf("Qt OpenFifo() function.....!\n");
//    if((mkfifo(FIFO_P42Qt,O_CREAT|O_EXCL|660)<0)&&(errno!=EEXIST))
//    {
//        printf("Qt cannot create FIFO_P42Qt fifo!\n");
//    }
//    if((mkfifo(FIFO_Qt2P4,O_CREAT|O_EXCL|660)<0)&&(errno!=EEXIST))
//    {
//        printf("Qt cannot create FIFO_Qt2P4 fifo!\n");
//    }

    fd_P42Qt=open(FIFO_P42Qt,O_RDONLY|O_NONBLOCK);
    if(fd_P42Qt<0)
    {
        printf("Qt open fifo file error!\n");
    }
    else
    {
        printf("Qt open fifo file success!\n");
    }

    fd_Qt2P4=open(FIFO_Qt2P4,O_WRONLY,0);
    if(fd_Qt2P4<0)
    {
        printf("Qt open fifo file error!\n");
    }
    else
    {
        printf("Qt open fifo file success!\n");
    }
}

void MainWindow::SendMessageFifo(char *bewrite)
{
    int wtnum=write(fd_Qt2P4,bewrite,strlen(bewrite));
    if(wtnum<0)
    {
        printf("Qt fifoserver write error!\n");
    }
    else
    {
        printf("Qt fifoserver write successfully!\n");
    }
}

int MainWindow::ReciveMessageFifo()
{
    char buf[FIFOSIZE+1];
    int  rdnum=read(fd_P42Qt,buf,FIFOSIZE);
    if(rdnum<0)
    {
        printf("Qt fifoserver read error!\n");
    }
    buf[rdnum]=0;
    printf("Qt after read(),buf has data:\n%s\n",buf);
    if(strncmp(buf,"end",3)==0)
    {
        printf("Qt Mp4Player read fifo stop!\n");
        return 1;
    }
    else
    {
        return 0;
    }
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



void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "MainWindow Released key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(m_Freezed_f==false)
            {
                m_Freezed_f=true;
                emit SFreeze();
                qDebug() << "MainWindow emit SFreeze();m_Freezed_f=true;" ;
            }
            else
            {
                m_Freezed_f=false;
                emit SReFreeze();
                qDebug() << "MainWindow emit SReFreeze();m_Freezed_f=false;" ;
            }
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
            //emit  SHideOsd1();//隐藏Osd1视频窗口 信号

            emit SFreeze();
            this->msecSleep(200);
            //emit SReFreeze();

            //QProcess ::execute("./my_mp4dec /media/mmcblk0p1/DCIM/11250117/07040001.mp4 &");
            system("./my_mp4dec /media/mmcblk0p1/DCIM/11250117/07040001.mp4");
            OpenFifo();
            SendMessageFifo("play");
        }
        break;
        case Qt::Key_Right:
        {
            //emit SRunAgain();
            emit SReFreeze();
        }
        break;
        case Qt::Key_T:
        {
//            ui->label->setText(tr(""));//清除控件文字内容
//            emit SShowOsd1();//显示Osd1视频窗口 信号

//            QString month="03",day="14",year="2015",hour="13",min="14";
//            QString str1;
//            //指令date -s "2012-05-18 04:53:00"可以设置时分秒
//            str1 = "date -s \"" +year + "-" + month + "-" + day +" "+ hour + ":" + min + ":" + "00\"";
//            qDebug() << "change time strkk" << str1;
//            QProcess ::execute(str1);
            ClearRGBGain();
            emit SWhiteBalance();
            this->msecSleep(1000);
            RgbAlgorithm();//算法
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



