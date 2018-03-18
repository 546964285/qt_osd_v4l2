#include "dlgvidfile.h"
#include "ui_dlgvidfile.h"

DlgVidFile::DlgVidFile(QWidget *parent) :QDialog(parent),ui(new Ui::DlgVidFile)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
}

DlgVidFile::~DlgVidFile()
{
    delete ui;
}

void DlgVidFile::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgVidFile Pressed Key is" << QKeySequence(event->modifiers() + event->key());
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

void DlgVidFile::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgVidFile pressed key is" << QKeySequence(event->modifiers() + event->key());
    switch (event->key())
    {
        case Qt::Key_Return:
        {
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

