#include "dlgf1.h"
#include "ui_dlgf1.h"

DlgF1::DlgF1(QWidget *parent) :QDialog(parent),ui(new Ui::DlgF1)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景

    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);

    m_ColumnNum=0;
    m_PageNum=0;
    ControlInit();//控件初始化
}

DlgF1::~DlgF1()
{
    delete ui;
}

void DlgF1::ControlInit()//控件初始化
{
    g_PicturefileName=":/images/Title.png";//显示上标题栏
    g_Pixmap.load(g_PicturefileName);
    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(g_Pixmap);
    ui->LabelTitle->show();

    g_PicturefileName=":/images/StatusBar.png";//显示下状态栏
    g_Pixmap.load(g_PicturefileName);
    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(g_Pixmap);
    ui->LabelStatusBar->show();

    QString Setting =(tr("Setting"));//字
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
    LabelClear();LabelSet4_Show();LabelSet3_Show();LabelSet2_Show();LabelSet1_Show();
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
    g_PicturefileName=":/images/ButtonYellow.png";
    g_Pixmap.load(g_PicturefileName);
    LabelClear();//清除Lable1234内容
    ui->LabelSet1->setPixmap(g_Pixmap);
    ui->LabelSet1->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting ="Playback";//字
    }
    else if(m_PageNum==1)
    {
        Setting ="Auto Power Off";//字
    }
    else if(m_PageNum==2)
    {
        Setting ="Image rotation";//字
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
    g_PicturefileName=":/images/ButtonYellow.png";
    g_Pixmap.load(g_PicturefileName);
    LabelClear();//清除Lable1234内容
    ui->LabelSet2->setPixmap(g_Pixmap);
    ui->LabelSet2->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting ="Language";//字 语言设置
    }
    else if(m_PageNum==1)
    {
        Setting ="Sequence";//字 拍照设置
    }
    else if(m_PageNum==2)
    {
        Setting ="Color";//字  色彩
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
    g_PicturefileName=":/images/ButtonYellow.png";
    g_Pixmap.load(g_PicturefileName);
    LabelClear();//清除Lable1234内容
    ui->LabelSet3->setPixmap(g_Pixmap);
    ui->LabelSet3->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting ="Date/Time";//字 日期/时间
    }
    else if(m_PageNum==1)
    {
        Setting ="Save time";//字 录像保存时间
    }
    else if(m_PageNum==2)
    {
        Setting ="Default Setting";//字 默认设置
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
    g_PicturefileName=":/images/ButtonYellow.png";
    g_Pixmap.load(g_PicturefileName);
    LabelClear();//清除Lable1234内容
    ui->LabelSet4->setPixmap(g_Pixmap);
    ui->LabelSet4->show();
    QString Setting;
    if(m_PageNum==0)
    {
        Setting ="Standby";//字 待机时间
    }
    else if(m_PageNum==1)
    {
        Setting ="TV Mode";//字 TV模式
    }
    else if(m_PageNum==2)
    {
        Setting ="Version";//字 版本
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
    qDebug() << "press key's content" << QKeySequence(event->modifiers() + event->key());

    if(event->key() == Qt::Key_Right)//箭头向下键
    {
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
    else if(event->key() == Qt::Key_Left)//箭头向上键
    {
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
    else if(event->key() == Qt::Key_Return)//回车键
    {
        hide();
        emit SShowMainWindow();

        if((m_PageNum==0)&&(m_ColumnNum==0))//处于回放栏
        {
            //dialogPicOrVidwindow.show();//显示 文件 对话框
            //hide();
        }
        else if((m_PageNum==2)&&(m_ColumnNum==3))//处于版本栏
        {
            //hide();
            //dialogdevwindow.show();// 显示 版本 对话框
        }
        else if((m_PageNum==1)&&(m_ColumnNum==1))//处于 拍照设置 栏
        {
            //hide();
            //DlgTakePicNum.show();//显示 拍照设置  对话框
        }
    }
    else if(event->key() == Qt::Key_M)//M键
    {
        hide();
        emit SShowMainWindow();
    }
}

// 按键释放事件
void DlgF1::keyReleaseEvent(QKeyEvent *event)
{
    //如果是处理两个普通按键，得避免自动重复
    if(event->key() == Qt::Key_Up)
    {
        if(event->isAutoRepeat()) return;
    }
    else if(event->key() == Qt::Key_Left)
    {
        if(event->isAutoRepeat()) return;
    }
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
