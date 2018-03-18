#include "dlgvidsavetime.h"
#include "ui_dlgvidsavetime.h"

DlgVidSaveTime::DlgVidSaveTime(QWidget *parent) :QDialog(parent),ui(new Ui::DlgVidSaveTime)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
    m_RowNum=0;
    SlotFreshThisWindow();
}

DlgVidSaveTime::~DlgVidSaveTime()
{
    delete ui;
}

void DlgVidSaveTime::SlotFreshThisWindow()
{
    ReadiniRemember();//读取ini文件
    ControlInit();//控件初始化
}

void DlgVidSaveTime::SlotTranslateThisWindow()
{
    ReadiniRemember();//读取ini文件
    ControlInit();//控件初始化
}

void DlgVidSaveTime::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgVidSaveTime Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgVidSaveTime::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgVidSaveTime pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)//处于5分钟未操作，待机
                {
                    hide();
                    emit SShowdlgf1();
                    WriteiniRemember("5");//存入ini文件
                }
                else if(m_RowNum==1)//处于10分钟未操作，待机
                {
                    hide();
                    emit SShowdlgf1();
                    WriteiniRemember("10");//存入ini文件
                }
                else if(m_RowNum==2)//处于15分钟未操作，待机
                {
                    hide();
                    emit SShowdlgf1();
                    WriteiniRemember("15");//存入ini文件
                }
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                hide();
                SlotFreshThisWindow();//读取以及刷新窗口
                SShowdlgf1();
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)
                {
                   m_RowNum=2;
                }
                else if(m_RowNum==1)
                {
                   m_RowNum=0;
                }
                else if(m_RowNum==2)
                {
                   m_RowNum=1;
                }
                ControlInit();//控件重新初始化
                Label123Change();
            }
        }
        break;
        case Qt::Key_Right:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)
                {
                   m_RowNum=1;
                }
                else if(m_RowNum==1)
                {
                   m_RowNum=2;
                }
                else if(m_RowNum==2)
                {
                   m_RowNum=0;
                }
                ControlInit();//控件重新初始化
                Label123Change();
            }
        }
        break;
        case Qt::Key_T:
        {
        }
        break;
        case Qt::Key_P:  //录像
        {
        }
        break;
        case Qt::Key_C:  //Captrue
        {
                if(key_flg==true)
                {
                    hide();
                    SlotFreshThisWindow();//读取以及刷新窗口
                    emit SShowMainWindow();
                }
        }
        break;
        default:QWidget::keyReleaseEvent(event);break;
      }
      key_flg=false;
}


void DlgVidSaveTime::WriteiniRemember(QString  Strvidsavetime)//存入ini文件
{
    //创建或者打开指定路径下的.ini文件
    QSettings *ConfigIniWrite = new QSettings("/media/mmcblk0p1/qt_osd_arm.ini", QSettings::IniFormat);
    ConfigIniWrite->setValue("vidsavetime/set1", Strvidsavetime);//vidsavetime   下的set1内容设置
    delete ConfigIniWrite;//写入完成后删除指针
}

void DlgVidSaveTime::ReadiniRemember()//读取ini文件
{
    QSettings   *ConfigIniRead        =    new QSettings("/media/mmcblk0p1/qt_osd_arm.ini", QSettings::IniFormat);
    QString     StrVidsavetimeSet1    =    ConfigIniRead->value("vidsavetime/set1").toString();
    qDebug() <<"StrVidsavetimeSet1    is"  << StrVidsavetimeSet1;
    if(StrVidsavetimeSet1=="5")
    {
        m_RowNum=0;
    }
    else if(StrVidsavetimeSet1=="10")
    {
        m_RowNum=1;
    }
    else if(StrVidsavetimeSet1=="15")
    {
        m_RowNum=2;
    }
    else
    {
        m_RowNum=0;
        WriteiniRemember("5");//将自动关机时间设置存入ini文件
    }

    delete ConfigIniRead;//读入完成后删除指针
}

void DlgVidSaveTime::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;
    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();
    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();
    QString Setting =tr("Save Time");//字  拍照设置
    ui->LabelSetting->setText(Setting);
    Font.setPointSize(40);//大小
    ui->LabelSetting->setFont(Font);
    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelSetting->setPalette(Palette);
    Setting ="2/3";//字
    ui->LabelPageNum->setText(Setting);
    Font.setPointSize(30);//大小
    ui->LabelPageNum->setFont(Font);
    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelPageNum->setPalette(Palette);
    Label123Clear();LabelSet3_Show();LabelSet2_Show();LabelSet1_Show();
    Label123Change();
}

void DlgVidSaveTime::LabelSet1_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting=tr("  5 minutes");//字
    ui->LabelText1->setText(Setting);
    m_Font.setPointSize(40);//大小
    ui->LabelText1->setFont(m_Font);
    m_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText1->setPalette(m_Palette);
    m_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText2->setPalette(m_Palette);
    ui->LabelText3->setPalette(m_Palette);
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgVidSaveTime::LabelSet2_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting=tr("10 minutes");//字
    ui->LabelText2->setText(Setting);
    m_Font.setPointSize(40);//大小
    ui->LabelText2->setFont(m_Font);
    m_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText2->setPalette(m_Palette);
    m_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText1->setPalette(m_Palette);
    ui->LabelText3->setPalette(m_Palette);
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgVidSaveTime::LabelSet3_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet3->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet3->show();
    QString Setting=tr("15 minutes");//字
    ui->LabelText3->setText(Setting);
    m_Font.setPointSize(40);//大小
    ui->LabelText3->setFont(m_Font);
    m_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText3->setPalette(m_Palette);
    m_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText1->setPalette(m_Palette);
    ui->LabelText2->setPalette(m_Palette);
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgVidSaveTime::Label123Clear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
    ui->LabelSet3->clear();//清空
}

void DlgVidSaveTime::Label123Change()
{
    switch(m_RowNum)//第几栏，高亮
    {
        case 0: LabelSet1_Show();  break;
        case 1: LabelSet2_Show();  break;
        case 2: LabelSet3_Show();  break;
        default:break;
    }
}
