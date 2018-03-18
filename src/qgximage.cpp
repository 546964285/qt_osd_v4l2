#include "qgximage.h"
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QSettings>
#include "qv4l2.h"
//#include "GxPublic.h"

QGxImage::QGxImage(QWidget *parent, Qt::WindowFlags f): QLabel(parent, f)
{
//    setAutoFillBackground(true);
//    QPalette pal = palette();
//    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
//    setPalette(pal);
}

QGxImage::~QGxImage()
{

}

void QGxImage::paintEvent(QPaintEvent * )
{
    if(m_pImage1!=NULL)
    {
        QPainter painter(this);
//        painter.setCompositionMode(QPainter::CompositionMode_Clear);
//        painter.fillRect(0,0,384,384,Qt::SolidPattern);
        painter.drawImage(QRect(0,0,384, 384),*m_pImage1);
    }
}

void QGxImage::customEvent(QEvent *e)
{
    if(e->type() != V4l2Event)
    {
        return;
    }

    if(m_pImage1)
    {
        delete m_pImage1;
        m_pImage1 = NULL;
    }

    m_totalFrames++;

    QV4l2MsgData *pMsgData = (QV4l2MsgData*)((QChildEvent*)e)->child();

    m_pImage1 = new QImage(pMsgData->pBuf, 384, 384, QImage::Format_RGB888);
//    m_pImage1 = new QImage(pMsgData->pBuf, 640, 480, QImage::Format_RGB888);

    delete pMsgData;
    delete e;

    QPainter painter(m_pImage1);

    painter.setPen(QPen(QBrush(Qt::blue), 4.0, Qt::SolidLine));

    update();
}

int QGxImage::GetFps()
{
    int fps;
    fps = m_totalFrames - m_lastTotalFrames;
    m_lastTotalFrames = m_totalFrames;

    return fps;
}
