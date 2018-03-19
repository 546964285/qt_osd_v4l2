#include "dlglanguage.h"
#include "ui_dlglanguage.h"

DlgLanguage::DlgLanguage(QWidget *parent) :QDialog(parent),ui(new Ui::DlgLanguage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    qApp->installTranslator(&appTranslator);

    m_ColumnNum=0;
    m_PageNum=0;
    SlotFreshThisWindow();

    ui->LabelText1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中

}

DlgLanguage::~DlgLanguage()
{
    delete ui;
}

void DlgLanguage::SlotFreshThisWindow()
{
    ReadiniRemember();//读取ini文件
    ControlInit();//控件初始化
}

void DlgLanguage::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;

    ui->LabelTitle->clear();//上标题栏清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();

    ui->LabelStatusBar->clear();//下状态栏清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();

    QString Setting =tr("Language");//字
    ui->LabelSetting->setText(Setting);
    Font.setPointSize(40);//大小
    ui->LabelSetting->setFont(Font);

    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelSetting->setPalette(Palette);
    Setting ="1/3";//字
    ui->LabelPageNum->setText(Setting);
    Font.setPointSize(30);//大小
    ui->LabelPageNum->setFont(Font);
    Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelPageNum->setPalette(Palette);

    LabelClear();
    LabelSet4_Show();LabelSet3_Show();LabelSet2_Show();LabelSet1_Show();
    LabelChange();
}

void DlgLanguage::LabelClear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
    ui->LabelSet3->clear();//清空
    ui->LabelSet4->clear();//清空
}

void DlgLanguage::LabelSet1_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("English");//字
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Language5");//字
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Language9");//字
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

void DlgLanguage::LabelSet2_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Chinese");//字 语言设置
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Language6");//字
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Language10");//字
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

void DlgLanguage::LabelSet3_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet3->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet3->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Language3");//字
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Language7");//字
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Language11");//字
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

void DlgLanguage::LabelSet4_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet4->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet4->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Language4");//字
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Language8");//字
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Language12");//字
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

void DlgLanguage::InitLanguage()
{
    ReadiniRemember();
}

void DlgLanguage::Translate(QString Language)
{
    QDir qmDir(":/translations");
    QString qmPath = qmDir.absolutePath();
    appTranslator.load("QtOsdArm_"+Language,qmPath);
    ControlInit();//控件初始化
    emit STranslatedlgf1();
}

void DlgLanguage::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgLanguage Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgLanguage::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgLanguage pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                if((m_PageNum==0)&&(m_ColumnNum==0))//处于 英语 栏
                {
                    Translate("en");
                    WriteiniRemember("en");//将语言设置存入ini文件
                }
                else if((m_PageNum==0)&&(m_ColumnNum==1))//处于 中文 栏
                {
                    Translate("cn");
                    WriteiniRemember("cn");//将语言设置存入ini文件
                }
                else if((m_PageNum==2)&&(m_ColumnNum==3))//处于 德语 栏
                {
                }
                else if((m_PageNum==1)&&(m_ColumnNum==1))//处于 法语 栏
                {
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
                emit SShowdlgf1();
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                if(m_ColumnNum>0)
                   m_ColumnNum--;
                else
                {
                   m_ColumnNum=3;
                   if(m_PageNum>0)
                   {
                        m_PageNum--;
                        ControlInit();//控件重新初始化
                   }
                   else
                   {
                        m_PageNum=2;
                        ControlInit();//控件重新初始化
                   }
                }
                LabelChange();
            }
        }
        break;
        case Qt::Key_Right:
        {
            if(key_flg==true)
            {
                if(m_ColumnNum<3)
                   m_ColumnNum++;
                else
                {
                   m_ColumnNum=0;
                   if(m_PageNum<2)
                   {
                        m_PageNum++;
                        ControlInit();//控件重新初始化
                   }
                   else
                   {
                        m_PageNum=0;
                        ControlInit();//控件重新初始化
                   }
                }
                LabelChange();
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

void DlgLanguage::WriteiniRemember(QString  StrLanguage)//将语言设置存入ini文件
{
    //创建或者打开指定路径下的.ini文件
    QSettings *ConfigIniWrite = new QSettings("./qt_osd_arm.ini", QSettings::IniFormat);
    ConfigIniWrite->setValue("language/set1"    , StrLanguage);//language   下的set1内容设置
    delete ConfigIniWrite;//写入完成后删除指针
}

void DlgLanguage::ReadiniRemember()//读取ini文件并翻译
{
    QSettings   *ConfigIniRead    =    new QSettings("./qt_osd_arm.ini", QSettings::IniFormat);
    QString     StrLanguageSet1   =    ConfigIniRead->value("language/set1").toString();
    qDebug() <<"StrLanguageSet1   is"  << StrLanguageSet1;

    static  QString Str;
    if(StrLanguageSet1!=Str)//只有不等于的时候才翻译，因为翻译比较卡
    {
        if(StrLanguageSet1=="en")
            Translate("en");
        else if(StrLanguageSet1=="cn")
            Translate("cn");

        Str=StrLanguageSet1;
    }

    if(StrLanguageSet1=="en")//刷新显示栏
    {
        m_ColumnNum=0;
        m_PageNum=0;
    }
    else if(StrLanguageSet1=="cn")
    {
        m_ColumnNum=1;
        m_PageNum=0;
    }

    delete ConfigIniRead;//读入完成后删除指针
}

void DlgLanguage::LabelChange()
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
