#include "dlgshowdev.h"
#include "ui_dlgshowdev.h"

DlgShowDev::DlgShowDev(QWidget *parent) :QDialog(parent),ui(new Ui::DlgShowDev)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
    ControlInit();//控件初始化
}

DlgShowDev::~DlgShowDev()
{
    delete ui;
}

void DlgShowDev::SlotFreshThisWindow()
{
    ControlInit();//控件初始化
}

void DlgShowDev::SlotTranslateThisWindow()
{
    ControlInit();//控件初始化
}

void DlgShowDev::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "dlgshowdev Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgShowDev::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgShowDev pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                hide();
                emit SShowdlgf1();;
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                hide();
                emit SShowdlgf1();;
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                hide();
                emit SShowdlgf1();;
            }
        }
        break;
        case Qt::Key_Right:
        {
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

void DlgShowDev::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;
    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();
    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();
    QString Setting =tr("Version");//字
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
    Setting ="AH30151122";//字
    ui->LabelText1->setText(Setting);
    Font.setPointSize(50);//大小
    ui->LabelText1->setFont(Font);
    Palette.setColor(QPalette::WindowText,Qt::white);//自己颜色
    ui->LabelText1->setPalette(Palette);
    ui->LabelText1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelText1->setStyleSheet("QLabel{border:5px solid rgb(40, 40, 255);}");//Label显示周框
    Setting =tr("ShangHai AoHua Co.,LTD");//字
    ui->LabelCopyright->setText(Setting);
    Font.setPointSize(30);//大小
    ui->LabelCopyright->setFont(Font);
    Palette.setColor(QPalette::WindowText,Qt::white);//自己颜色
    ui->LabelCopyright->setPalette(Palette);
    ui->LabelCopyright->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
}
