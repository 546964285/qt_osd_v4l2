#include "dlgtvmode.h"
#include "ui_dlgtvmode.h"

DlgTVMode::DlgTVMode(QWidget *parent) :QDialog(parent),ui(new Ui::DlgTVMode)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
    m_RowNum=0;
    ControlInit();//控件初始化
}

DlgTVMode::~DlgTVMode()
{
    delete ui;
}

void DlgTVMode::SlotFreshThisWindow()
{
    ControlInit();//控件初始化
}

void DlgTVMode::SlotTranslateThisWindow()
{
    ControlInit();//控件初始化
}

void DlgTVMode::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgTVMode Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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
        case Qt::Key_C://Captrue
        {
            key_flg=true;
        }
        break;
        default:QWidget::keyPressEvent(event);break;
    }
}

void DlgTVMode::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgTVMode pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)//处于5分钟未操作，待机
                {
                    m_RowNum=0;
                    hide();
                    emit SShowdlgf1();
                }
                else if(m_RowNum==1)//处于10分钟未操作，待机
                {
                    m_RowNum=0;
                    hide();
                    emit SShowdlgf1();
                }
                else if(m_RowNum==2)//处于15分钟未操作，待机
                {
                    m_RowNum=0;
                    hide();
                    emit SShowdlgf1();
                }
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                m_RowNum=0;
                hide();
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
                    m_RowNum=0;
                    hide();
                    emit SShowMainWindow();
                }
        }
        break;
        default:QWidget::keyReleaseEvent(event);break;
      }
      key_flg=false;
}




void DlgTVMode::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;
    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();
    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();
    QString Setting =tr("TV Mode");//字  拍照设置
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

void DlgTVMode::LabelSet1_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting=tr("Cancel");//字
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

void DlgTVMode::LabelSet2_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting=tr("MP4 Show");//字
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

void DlgTVMode::LabelSet3_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet3->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet3->show();
    QString Setting=tr("TV  Show");//字
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

void DlgTVMode::Label123Clear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
    ui->LabelSet3->clear();//清空
}

void DlgTVMode::Label123Change()
{
    switch(m_RowNum)//第几栏，高亮
    {
        case 0: LabelSet1_Show();  break;
        case 1: LabelSet2_Show();  break;
        case 2: LabelSet3_Show();  break;
        default:break;
    }
}

