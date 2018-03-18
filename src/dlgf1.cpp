#include "dlgf1.h"
#include "ui_dlgf1.h"

DlgF1::DlgF1(QWidget *parent) :QDialog(parent),ui(new Ui::DlgF1)
{
    dlgshowdev.setModal(false);//false非模态/true模态
    dlgshowdev.hide();
    dlglanguage.setModal(false);
    dlglanguage.hide();
    dlgtakepicnum.setModal(false);
    dlgtakepicnum.hide();
    dlgpicorvid.setModal(false);
    dlgpicorvid.hide();
    dlgstandby.setModal(false);
    dlgstandby.hide();
    dlgvidsavetime.setModal(false);
    dlgvidsavetime.hide();
    dlgtvmode.setModal(false);
    dlgtvmode.hide();
    dlgrecovery.setModal(false);
    dlgrecovery.hide();
    dlgoff.setModal(false);
    dlgoff.hide();
    dlgcolor.setModal(false);
    dlgcolor.hide();
    dlgtime.setModal(false);
    dlgtime.hide();

    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);

    connect(&dlgshowdev,     SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));//显示当前窗口
    connect(&dlglanguage,    SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgtakepicnum,  SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgpicorvid,    SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgstandby,     SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgvidsavetime, SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgtvmode,      SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgrecovery,    SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgoff,         SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgcolor,       SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));
    connect(&dlgtime,        SIGNAL(SShowdlgf1()),   this,          SLOT(SlotShowThisWindow()));

    connect(&dlglanguage, SIGNAL(STranslatedlgf1()), this,          SLOT(SlotTranslateThisWindow()));//语言设置窗口翻译以后刷新其他窗口

    connect(this, SIGNAL(STranslateOtherWindow()), &dlgshowdev,     SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgtakepicnum,  SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgpicorvid,    SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgstandby,     SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgvidsavetime, SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgtvmode,      SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgrecovery,    SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgoff,         SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgcolor,       SLOT(SlotTranslateThisWindow()));
    connect(this, SIGNAL(STranslateOtherWindow()), &dlgtime,        SLOT(SlotTranslateThisWindow()));

    connect(this, SIGNAL(SFreshDlgShowDevWindow()),    &dlgshowdev,     SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgTakePicNumWindow()), &dlgtakepicnum,  SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgPicOrVidWindow()),   &dlgpicorvid,    SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgStandbyWindow()),    &dlgstandby,     SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgVidSaveTimeWindow()),&dlgvidsavetime, SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgTVModeWindow()),     &dlgtvmode,      SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgRecoveryWindow()),   &dlgrecovery,    SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgOffWindow()),        &dlgoff,         SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgColorWindow()),      &dlgcolor,       SLOT(SlotFreshThisWindow()));
    connect(this, SIGNAL(SFreshDlgTimeWindow()),       &dlgtime,        SLOT(SlotFreshThisWindow()));


    connect(&dlgshowdev,     SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));//显示当前窗口
    connect(&dlglanguage,    SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgtakepicnum,  SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgpicorvid,    SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgstandby,     SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgvidsavetime, SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgtvmode,      SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgrecovery,    SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgoff,         SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgcolor,       SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));
    connect(&dlgtime,        SIGNAL(SShowMainWindow()),     this,   SLOT(SlotShowMainWindow()));

    connect(&dlgpicorvid, SIGNAL(SSendNewFolderPath(QString )), this, SLOT(SlotSendNewFolderPath(QString )));

    m_ColumnNum=0;
    m_PageNum=0;
    ControlInit();//控件初始化

    key_flg=false;
}

DlgF1::~DlgF1()
{
    delete ui;
}

void DlgF1::SlotSendNewFolderPath(QString StrNewFolderPath)
{
    qDebug() << "DlgF1 Got Singnal StrNewFolderPath is " << StrNewFolderPath;
    emit SSendNewFolderPath(StrNewFolderPath);
}

void DlgF1::SlotShowMainWindow()//为了接收它底层菜单发上来的信号
{
    emit SShowMainWindow();
}

void DlgF1::SlotShowThisWindow()
{    
    ControlInit();//控件初始化
    show();
}
void DlgF1::SlotFreshThisWindow()
{    
    m_ColumnNum=0;
    m_PageNum=0;
    ControlInit();//控件初始化
    //emit SFreshOtherWindow();
}

void DlgF1::SlotTranslateThisWindow()
{
    m_ColumnNum=0;
    m_PageNum=0;
    ControlInit();//控件初始化
    emit STranslateOtherWindow();
}

void DlgF1::ControlInit()//控件初始化
{
    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();

    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();

    QString Setting =tr("Setting");//字
    ui->LabelSetting->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelSetting->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelSetting->setPalette(g_Palette);

    if(m_PageNum==0)//页码切换
        Setting ="1/3";//字
    else if(m_PageNum==1)
        Setting ="2/3";//字
    else if(m_PageNum==2)
        Setting ="3/3";//字
    ui->LabelPageNum->setText(Setting);
    g_Font.setPointSize(30);//大小
    ui->LabelPageNum->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelPageNum->setPalette(g_Palette);

    LabelClear();
    LabelSet4_Show();LabelSet3_Show();LabelSet2_Show();LabelSet1_Show();
    LabelChange();
}

void DlgF1::LabelClear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
    ui->LabelSet3->clear();//清空
    ui->LabelSet4->clear();//清空
}

void DlgF1::LabelSet1_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Playback");//字
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Auto Power Off");//字
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Image rotation");//字
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

void DlgF1::LabelSet2_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Language");//字 语言设置
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Sequence");//字 拍照设置
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Color");//字  色彩
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

void DlgF1::LabelSet3_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet3->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet3->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Date/Time");//字 日期/时间
    }
    else if(m_PageNum==1)
    {
        Setting =tr("Save time");//字 录像保存时间
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Default Setting");//字 默认设置
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

void DlgF1::LabelSet4_Show()
{
    LabelClear();//清除Lable1234内容
    ui->LabelSet4->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet4->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting =tr("Standby");//字 待机时间
    }
    else if(m_PageNum==1)
    {
        Setting =tr("TV Mode");//字 TV模式
    }
    else if(m_PageNum==2)
    {
        Setting =tr("Version");//字 版本
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

// 键盘按下事件
void DlgF1::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "dlgf1 Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgF1::keyReleaseEvent(QKeyEvent *event)// 按键释放事件
{
    qDebug() << "dlgf1 Released Key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                key_flg=false;
                if((m_PageNum==0)&&(m_ColumnNum==0))//处于      回放     栏
                {
                    dlgpicorvid.show();
                    hide();
                }
                else if((m_PageNum==0)&&(m_ColumnNum==1))//处于 语言设置  栏
                {
                    dlglanguage.show();
                    hide();
                }
                else if((m_PageNum==0)&&(m_ColumnNum==2))//处于 日期/时间 栏
                {
                    dlgtime.show();
                    hide();
                }
                else if((m_PageNum==0)&&(m_ColumnNum==3))//处于 待机时间  栏
                {
                    dlgstandby.show();
                    hide();
                }
                else if((m_PageNum==1)&&(m_ColumnNum==0))//处于 自动关机  栏
                {
                    dlgoff.show();
                    hide();
                }
                else if((m_PageNum==1)&&(m_ColumnNum==1))//处于 拍照设置  栏
                {
                    emit SFreshDlgTakePicNumWindow();                 
                    dlgtakepicnum.show();
                    hide();
                    emit SFreshDlgTakePicNumWindow();
                }
                else if((m_PageNum==1)&&(m_ColumnNum==2))//处于 录像时间  栏
                {
                    dlgvidsavetime.show();
                    hide();
                }
                else if((m_PageNum==1)&&(m_ColumnNum==3))//处于 TV模式    栏
                {
                    dlgtvmode.show();
                    hide();
                }
                else if((m_PageNum==2)&&(m_ColumnNum==0))//处于 影音旋转   栏
                {
                    //hide();
                }
                else if((m_PageNum==2)&&(m_ColumnNum==1))//处于 色彩设置   栏
                {
                    dlgcolor.show();
                    hide();
                }
                else if((m_PageNum==2)&&(m_ColumnNum==2))//处于 默认设置   栏
                {
                    dlgrecovery.show();
                    hide();
                }
                else if((m_PageNum==2)&&(m_ColumnNum==3))//处于 版本信息    栏
                {
                    emit SFreshDlgShowDevWindow();
                    dlgshowdev.show();
                    hide();
                }
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                key_flg=false;
                hide();
                emit SShowMainWindow();
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                key_flg=false;
                if(event->isAutoRepeat()) return;
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
                key_flg=false;
                if(event->isAutoRepeat()) return;// 按键重复时不做处理
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
                    emit SShowMainWindow();
                }
        }
        break;
        default:QWidget::keyReleaseEvent(event);break;
      }
      key_flg=false;
}



void DlgF1::LabelChange()
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
