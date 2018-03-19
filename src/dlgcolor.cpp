#include "dlgcolor.h"
#include "ui_dlgcolor.h"

DlgColor::DlgColor(QWidget *parent) :QDialog(parent),ui(new Ui::DlgColor)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);

    m_ColumnNum=0;//栏目
    m_Config1Num=0;m_Config2Num=0;m_Config3Num=0;m_Config4Num=0;//对应数字
    SlotFreshThisWindow();

     ui->LabelText1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
     ui->LabelText2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
     ui->LabelText3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
     ui->LabelText4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText11->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText12->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText13->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText14->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中

}

DlgColor::~DlgColor()
{
    delete ui;
}

void DlgColor::SlotFreshThisWindow()
{
    ReadiniRemember();//读取ini文件
    ControlInit();//控件初始化
}

void DlgColor::SlotTranslateThisWindow()
{
    ReadiniRemember();//读取ini文件
    ControlInit();//控件初始化
}

void DlgColor::SlotShowMainWindow()//为了接收它底层菜单发上来的信号
{
    emit SShowMainWindow();
}

void DlgColor::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;

    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    Font.setPointSize(30);//大小
    QString ConfigText;

    ConfigText =tr("亮度");//字
    ui->LabelText11->setPalette(Palette);
    ui->LabelText11->setText(ConfigText);
    ui->LabelText11->setFont(Font);

    ConfigText =tr("色度");//字
    ui->LabelText12->setPalette(Palette);
    ui->LabelText12->setText(ConfigText);
    ui->LabelText12->setFont(Font);

    ConfigText =tr("对比度");//字
    ui->LabelText13->setPalette(Palette);
    ui->LabelText13->setText(ConfigText);
    ui->LabelText13->setFont(Font);

    ConfigText =tr("锐化度");//字
    ui->LabelText14->setPalette(Palette);
    ui->LabelText14->setText(ConfigText);
    ui->LabelText14->setFont(Font);

    ui->LabelTitle->clear();//上标题栏清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();

    ui->LabelStatusBar->clear();//下状态栏清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();

    QString Setting =tr("Color");//字
    ui->LabelSetting->setText(Setting);
    Font.setPointSize(40);//大小
    ui->LabelSetting->setFont(Font);

    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelSetting->setPalette(Palette);
    Setting ="3/3";//字
    ui->LabelPageNum->setText(Setting);
    Font.setPointSize(30);//大小
    ui->LabelPageNum->setFont(Font);
    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelPageNum->setPalette(Palette);

    LabelClear();
    LabelSet4_Show();LabelSet3_Show();LabelSet2_Show();LabelSet1_Show();
    LabelChange();
}

void DlgColor::LabelClear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
    ui->LabelSet3->clear();//清空
    ui->LabelSet4->clear();//清空
}

void DlgColor::LabelSet1_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting;
    if((m_Config1Num>=0)&&(m_Config1Num<16))
    {
        Setting = QString::number(m_Config1Num, 10);//字
    }
    ui->LabelText1->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelText1->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText1->setPalette(g_Palette);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText2->setPalette(g_Palette);
    ui->LabelText3->setPalette(g_Palette);
    ui->LabelText4->setPalette(g_Palette);
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet4->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgColor::LabelSet2_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting;
    if((m_Config2Num>=0)&&(m_Config2Num<16))
    {
        Setting = QString::number(m_Config2Num, 10);//字
    }
    ui->LabelText2->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelText2->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText2->setPalette(g_Palette);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText1->setPalette(g_Palette);
    ui->LabelText3->setPalette(g_Palette);
    ui->LabelText4->setPalette(g_Palette);
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet4->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgColor::LabelSet3_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet3->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet3->show();
    QString Setting;
    if((m_Config3Num>=0)&&(m_Config3Num<16))
    {
        Setting = QString::number(m_Config3Num, 10);//字
    }
    ui->LabelText3->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelText3->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText3->setPalette(g_Palette);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText1->setPalette(g_Palette);
    ui->LabelText2->setPalette(g_Palette);
    ui->LabelText4->setPalette(g_Palette);
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet4->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgColor::LabelSet4_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet4->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet4->show();
    QString Setting;
    if((m_Config4Num>=0)&&(m_Config4Num<16))
    {
        Setting = QString::number(m_Config4Num, 10);//字
    }
    ui->LabelText4->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelText4->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText4->setPalette(g_Palette);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText1->setPalette(g_Palette);
    ui->LabelText2->setPalette(g_Palette);
    ui->LabelText3->setPalette(g_Palette);
    ui->LabelSet4->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
    ui->LabelSet3->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

// 键盘事件
void DlgColor::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgColor Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgColor::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgColor pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                if(m_ColumnNum==0)
                {
                    m_ColumnNum=1;
                }
                else if(m_ColumnNum==1)
                {
                    m_ColumnNum=2;
                }
                else if(m_ColumnNum==2)
                {
                    m_ColumnNum=3;
                }
                else if(m_ColumnNum==3)
                {
                    m_ColumnNum=0;
                }
                ControlInit();//控件初始化
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                hide();
                m_ColumnNum=0;
                ControlInit();//控件初始化
                emit SShowdlgf1();
            }
        }
        break;
        case Qt::Key_Left://加
        {
            if(key_flg==true)
            {
                if(m_ColumnNum==0)
                {
                    if(m_Config1Num<15)
                    {
                        m_Config1Num++;
                        WriteiniRemember(m_ColumnNum,m_Config1Num);//存入ini文件
                    }
                }
                else if(m_ColumnNum==1)
                {
                    if(m_Config2Num<15)
                    {
                        m_Config2Num++;
                        WriteiniRemember(m_ColumnNum,m_Config2Num);//存入ini文件
                    }
                }
                else if(m_ColumnNum==2)
                {
                    if(m_Config3Num<15)
                    {
                        m_Config3Num++;
                        WriteiniRemember(m_ColumnNum,m_Config3Num);//存入ini文件
                    }
                }
                else if(m_ColumnNum==3)
                {
                    if(m_Config4Num<15)
                    {
                        m_Config4Num++;
                        WriteiniRemember(m_ColumnNum,m_Config4Num);//存入ini文件
                    }
                }
                ControlInit();//控件初始化
            }
        }
        break;
        case Qt::Key_Right://减
        {
            if(key_flg==true)
            {
                if(m_ColumnNum==0)
                {
                    if(m_Config1Num!=0)
                    {
                        m_Config1Num--;
                        WriteiniRemember(m_ColumnNum,m_Config1Num);//存入ini文件
                    }
                }
                else if(m_ColumnNum==1)
                {
                    if(m_Config2Num!=0)
                    {
                        m_Config2Num--;
                        WriteiniRemember(m_ColumnNum,m_Config2Num);//存入ini文件
                    }
                }
                else if(m_ColumnNum==2)
                {
                    if(m_Config3Num!=0)
                    {
                        m_Config3Num--;
                        WriteiniRemember(m_ColumnNum,m_Config3Num);//存入ini文件
                    }
                }
                else if(m_ColumnNum==3)
                {
                    if(m_Config4Num!=0)
                    {
                        m_Config4Num--;
                        WriteiniRemember(m_ColumnNum,m_Config4Num);//存入ini文件
                    }
                }
                ControlInit();//控件初始化
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
                    m_ColumnNum=0;
                    ControlInit();//控件初始化
                    emit SShowMainWindow();
                }
        }
        break;
        default:QWidget::keyReleaseEvent(event);break;
      }
      key_flg=false;
}

void DlgColor::WriteiniRemember(int n,int Colornum)//存入ini文件
{
    QString StrColor=QString::number(Colornum, 10);//数字转字符串

    //创建或者打开指定路径下的.ini文件
    QSettings *ConfigIniWrite = new QSettings("./qt_osd_arm.ini", QSettings::IniFormat);

    if(n==0)
    {
        ConfigIniWrite->setValue("color/set1"    , StrColor);//color  下的set1内容设置
    }
    else if(n==1)
    {
        ConfigIniWrite->setValue("color/set2"    , StrColor);//color  下的set2内容设置
    }
    else if(n==2)
    {
        ConfigIniWrite->setValue("color/set3"    , StrColor);//color  下的set3内容设置
    }
    else if(n==3)
    {
        ConfigIniWrite->setValue("color/set4"    , StrColor);//color  下的set4内容设置
    }

    delete ConfigIniWrite;//写入完成后删除指针
}

void DlgColor::ReadiniRemember()//读取ini文件
{
    QSettings   *ConfigIniRead  =    new QSettings("./qt_osd_arm.ini", QSettings::IniFormat);
    QString     StrColorSet1    =    ConfigIniRead->value("color/set1").toString();
    QString     StrColorSet2    =    ConfigIniRead->value("color/set2").toString();
    QString     StrColorSet3    =    ConfigIniRead->value("color/set3").toString();
    QString     StrColorSet4    =    ConfigIniRead->value("color/set4").toString();

    qDebug() <<"StrColorSet1    is"  << StrColorSet1;
    qDebug() <<"StrColorSet2    is"  << StrColorSet2;
    qDebug() <<"StrColorSet3    is"  << StrColorSet3;
    qDebug() <<"StrColorSet4    is"  << StrColorSet4;
    bool ok;
    m_Config1Num=StrColorSet1.toInt(&ok,10);
    m_Config2Num=StrColorSet2.toInt(&ok,10);
    m_Config3Num=StrColorSet3.toInt(&ok,10);
    m_Config4Num=StrColorSet4.toInt(&ok,10);
    delete ConfigIniRead;//读入完成后删除指针
}

void DlgColor::LabelChange()
{
    switch(m_ColumnNum)//第几栏，高亮
    {
        case 0: LabelSet1_Show();  break;
        case 1: LabelSet2_Show();  break;
        case 2: LabelSet3_Show();  break;
        case 3: LabelSet4_Show();  break;
        default:break;
    }
}
