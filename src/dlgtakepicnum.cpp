#include "dlgtakepicnum.h"
#include "ui_dlgtakepicnum.h"

DlgTakePicNum::DlgTakePicNum(QWidget *parent) :QDialog(parent),ui(new Ui::DlgTakePicNum)
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

DlgTakePicNum::~DlgTakePicNum()
{
    delete ui;
}

void DlgTakePicNum::SlotFreshThisWindow()
{
    ReadiniRemember();//读取ini文件
    ControlInit();//控件初始化
}

void DlgTakePicNum::SlotTranslateThisWindow()
{
    ControlInit();//控件初始化
}

void DlgTakePicNum::ControlInit()//控件初始化
{
    QFont    Font;
    QPalette Palette;
    ui->LabelTitle->clear();//清空
    ui->LabelTitle->setPixmap(QPixmap(":/images/Title.png"));
    ui->LabelTitle->show();
    ui->LabelStatusBar->clear();//清空
    ui->LabelStatusBar->setPixmap(QPixmap(":/images/StatusBar.png"));
    ui->LabelStatusBar->show();
    QString Setting =tr("Sequence");//字  拍照设置
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

void DlgTakePicNum::LabelSet1_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet1->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet1->show();
    QString Setting=tr("single (1)");//字
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

void DlgTakePicNum::LabelSet2_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet2->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet2->show();
    QString Setting=tr("double(2)");//字
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

void DlgTakePicNum::LabelSet3_Show()
{
    Label123Clear();//清除Lable123内容
    ui->LabelSet3->setPixmap(QPixmap(":/images/ButtonYellow.png"));
    ui->LabelSet3->show();
    QString Setting=tr("triple  (3)");//字
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

// 键盘事件
void DlgTakePicNum::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgTakePicNum key's content" << QKeySequence(event->modifiers() + event->key());
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

void DlgTakePicNum::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgTakePicNum pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                if(m_RowNum==0)//处于单拍栏
                {
                    hide();
                    WriteiniRemember("1");//存入ini文件
                    emit SShowdlgf1();                
                }
                else if(m_RowNum==1)//处于2连拍栏
                {
                    hide();
                    WriteiniRemember("2");//存入ini文件
                    emit SShowdlgf1();
                }
                else if(m_RowNum==2)//处于3连拍栏
                {
                    hide();
                    WriteiniRemember("3");//存入ini文件
                    emit SShowdlgf1();
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

void DlgTakePicNum::WriteiniRemember(QString  Str)//存入ini文件
{
    //创建或者打开指定路径下的.ini文件
    QSettings *ConfigIniWrite = new QSettings("/media/mmcblk0p1/qt_osd_arm.ini", QSettings::IniFormat);
    ConfigIniWrite->setValue("takepicnum/set1",Str);//takepicnum   下的set1内容设置
    delete ConfigIniWrite;//写入完成后删除指针
}

void DlgTakePicNum::ReadiniRemember()//读取ini文件
{
    QSettings   *ConfigIniRead      =    new QSettings("/media/mmcblk0p1/qt_osd_arm.ini", QSettings::IniFormat);
    QString     StrtakepicnumSet1   =    ConfigIniRead->value("takepicnum/set1").toString();
    qDebug() <<"StrtakepicnumSet1   is"  << StrtakepicnumSet1;
    if(StrtakepicnumSet1=="1")
    {
        m_RowNum=0;
    }
    else if(StrtakepicnumSet1=="2")
    {
        m_RowNum=1;
    }
    else if(StrtakepicnumSet1=="3")
    {
        m_RowNum=2;
    }
    else
    {
        m_RowNum=0;
        WriteiniRemember("1");//将设置存入ini文件
    }

    delete ConfigIniRead;//读入完成后删除指针
}

void DlgTakePicNum::Label123Clear()
{
    ui->LabelSet1->clear();//清空
    ui->LabelSet2->clear();//清空
    ui->LabelSet3->clear();//清空
}

void DlgTakePicNum::Label123Change()
{
    switch(m_RowNum)//第几栏，高亮
    {
        case 0: LabelSet1_Show();  break;
        case 1: LabelSet2_Show();  break;
        case 2: LabelSet3_Show();  break;
        default:break;
    }
}

