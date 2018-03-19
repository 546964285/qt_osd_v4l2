#include "dlgshowpic.h"
#include "ui_dlgshowpic.h"

DlgShowPic::DlgShowPic(QWidget *parent) :QDialog(parent),ui(new Ui::DlgShowPic)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(50,50,50));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
    ui->LabelShowPic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    m_keylock=false;
}

DlgShowPic::~DlgShowPic()
{
    delete ui;
}

void DlgShowPic::msecSleep(int msec)//非阻塞式延时
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() <dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void DlgShowPic::SlotPictureShow(QString PicturePath)//根据路径显示图片/这是个槽
{
    m_keylock=true;
    QPixmap  Image;
    Image.load(PicturePath);
    Image=Image.scaled(480, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//平滑缩放
    ui->LabelShowPic->clear();//清空
    ui->LabelShowPic->setPixmap(Image);//放图    
    ui->LabelShowPic->show(); //显示
    unsigned int i;
    for(i=0;i<80;i=i+8)
    {
        ui->LabelShowPic->move(i, 0);
        this->msecSleep(1);        
    }
    m_keylock=false;
}

void DlgShowPic::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgShowPic Pressed Key is" << QKeySequence(event->modifiers() + event->key());
    key_flg=false;
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
            if(m_keylock==false)
            {
                key_flg=true;
            }
        }
        break;
        case Qt::Key_Right:
        {
            if(m_keylock==false)
            {
                key_flg=true;
            }
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

void DlgShowPic::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgShowPic pressed key is" << QKeySequence(event->modifiers() + event->key());
    emit SKeyPressed();
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                hide();
                emit SShowMainWindow();
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                hide();
                emit SShowMainWindow();
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                emit SNextOrPre(2);
            }
        }
        break;
        case Qt::Key_Right:
        {
            if(key_flg==true)
            {
                emit SNextOrPre(1);
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

