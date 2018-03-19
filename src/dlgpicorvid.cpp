#include "dlgpicorvid.h"
#include "ui_dlgpicorvid.h"

DlgPicOrVid::DlgPicOrVid(QWidget *parent) :QDialog(parent),ui(new Ui::DlgPicOrVid)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
    m_RowNum=0;
    ControlInit();//控件初始化
    connect(&dlgpicfile, SIGNAL(SShowMainWindow()), this, SLOT(SlotShowMainWindow()));
    connect(&dlgvidfile, SIGNAL(SShowMainWindow()), this, SLOT(SlotShowMainWindow()));

    connect(&dlgpicfile, SIGNAL(SSendNewFolderPath(QString )), this, SLOT(SlotSendNewFolderPath(QString )));
    connect(this, SIGNAL(SFreshOtherWindow()), &dlgpicfile, SLOT(SlotFreshThisWindow()));
}

DlgPicOrVid::~DlgPicOrVid()
{
    delete ui;
}

void DlgPicOrVid::SlotSendNewFolderPath(QString StrNewFolderPath)
{
    qDebug() << "DlgPicOrVid Got Singnal StrNewFolderPath is " << StrNewFolderPath;
    emit SSendNewFolderPath(StrNewFolderPath);
}

void DlgPicOrVid::SlotFreshThisWindow()
{
    ControlInit();//控件初始化
}

void DlgPicOrVid::SlotTranslateThisWindow()
{
    ControlInit();//控件初始化
}

void DlgPicOrVid::SlotShowDlgF1()//为了接收它底层菜单发上来的信号
{
    emit SShowdlgf1();
}

void DlgPicOrVid::SlotShowMainWindow()//为了接收它底层菜单发上来的信号
{
    emit SShowMainWindow();
}

void DlgPicOrVid::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;

    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();
    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();
    QString Setting =tr("backplay");//字
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
    Label12Clear();LabelSet2_Show();LabelSet1_Show();
    Label12Change();
}

void DlgPicOrVid::LabelSet1_Show()
{
    Label12Clear();//清除Lable1234内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting=tr("picture");//字
    ui->LabelText1->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelText1->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText1->setPalette(g_Palette);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText2->setPalette(g_Palette);
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

void DlgPicOrVid::LabelSet2_Show()
{
    Label12Clear();//清除Lable1234内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting=tr("video");//字
    ui->LabelText2->setText(Setting);
    g_Font.setPointSize(40);//大小
    ui->LabelText2->setFont(g_Font);
    g_Palette.setColor(QPalette::WindowText,Qt::blue);//自己颜色
    ui->LabelText2->setPalette(g_Palette);
    g_Palette.setColor(QPalette::WindowText,Qt::white);//其它颜色
    ui->LabelText1->setPalette(g_Palette);
    ui->LabelSet2->setStyleSheet("QLabel{border:5px solid rgb(0, 255, 0);}");//Label显示周框
    ui->LabelSet1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");//Label显示周框
}

// 键盘事件
void DlgPicOrVid::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgPicOrVid Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgPicOrVid::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgPicOrVid pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)//处于照片栏
                {
                    emit SFreshOtherWindow();//刷新照片库
                    hide();
                    dlgpicfile.show();//显示 文件 对话框
                }
                else if(m_RowNum==1)//处于视频栏
                {
                    hide();
                    dlgvidfile.show();// 显示 版本 对话框
                }
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                hide();
                m_RowNum=0;
                ControlInit();//控件重新初始化
                Label12Change();
                SShowdlgf1();
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)
                   m_RowNum=1;
                else
                {
                   m_RowNum=0;
                }
                ControlInit();//控件重新初始化
                Label12Change();
            }
        }
        break;
        case Qt::Key_Right:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)
                   m_RowNum=1;
                else
                {
                   m_RowNum=0;
                }
                ControlInit();//控件重新初始化
                Label12Change();
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

void DlgPicOrVid::Label12Clear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
}

void DlgPicOrVid::Label12Change()
{
    switch(m_RowNum)//第几栏，高亮
    {
        case 0: LabelSet1_Show();  break;
        case 1: LabelSet2_Show();  break;
        default:break;
    }
}


