#include "qgximage.h"
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QSettings>
#include "GxPublic.h"

QGxImage::QGxImage(QWidget *parent, Qt::WindowFlags f): QLabel(parent, f)
{
   m_pV4l2 = NULL;

   m_pImage1 = NULL;

   m_totalFrames = 0;
   m_lastTotalFrames = 0;
}

QGxImage::~QGxImage()
{
    if(m_pV4l2)
    {
        delete m_pV4l2;
    }
}

void QGxImage::paintEvent(QPaintEvent*)
{
    if(m_pImage1)
    {
        QPainter painter(this);
        painter.drawImage(QRect(0,0,m_PanelWidth, m_PanelHeight),*m_pImage1);
    }
}

void QGxImage::InitImage(u_int32_t nWidth, u_int32_t nHeight)
{
    m_PanelWidth = this->width();
    m_PanelHeight = this->height();

    m_nBmpWidth = nWidth;
    m_nBmpHeight = nHeight;

//    m_testRC.ptStar.setX(GxReadFromIni(tr("pos/ptStarX")).toInt());
//    m_testRC.ptStar.setY(GxReadFromIni(tr("pos/ptStarY")).toInt());
//    m_testRC.ptEnd.setX(GxReadFromIni(tr("pos/ptEndX")).toInt());
//    m_testRC.ptEnd.setY(GxReadFromIni(tr("pos/ptEndY")).toInt());
//    m_testRC.rad = 6;
//    m_testRC.MoveID = -1;
//    UpdatePolyRect();
}

//void QGxImage::mousePressEvent(QMouseEvent *ev)
//{
//    if(ev->button() != Qt::LeftButton)
//    {
//        return;
//    }

//    int x = ev->x() * m_nBmpWidth / m_PanelWidth;
//    int y = ev->y() * m_nBmpHeight / m_PanelHeight;
//    int rad = m_testRC.rad;

//    if (((abs(x - m_testRC.ptStar.x())) <= rad) && ((abs(y - m_testRC.ptStar.y())) <= rad))
//    {
//            m_testRC.MoveID = 0;

//    }
//    else if (((abs(x - m_testRC.ptEnd.x())) <= rad) && ((abs(y - m_testRC.ptEnd.y())) <= rad))
//    {
//            m_testRC.MoveID = 1;
//    }
//}

//void QGxImage::mouseMoveEvent(QMouseEvent *ev)
//{
//    if(m_testRC.MoveID == -1)
//    {
//        return;

//    }

//    int x = ev->x() * m_nBmpWidth / m_PanelWidth;
//    int y = ev->y() * m_nBmpHeight / m_PanelHeight;

//    switch(m_testRC.MoveID)
//    {
//    case 0:	// ptStar
//    {
//        m_testRC.ptStar.setX(x);
//        m_testRC.ptStar.setY(y);
//        UpdatePolyRect();
//        break;
//    }
//    case 1:	// ptEnd
//    {
//        m_testRC.ptEnd.setX(x);
//        m_testRC.ptEnd.setY(y);
//        UpdatePolyRect();
//        break;
//    }
//    }
//}

//void QGxImage::mouseReleaseEvent(QMouseEvent *ev)
//{
//    if(ev->button() != Qt::LeftButton || m_testRC.MoveID == -1)
//    {
//        return;
//    }

//    m_testRC.MoveID = -1;
//    g_IsLineChanged = true;
//}

//void QGxImage::UpdatePolyRect()
//{
//    m_testRC.rc.setRect(m_testRC.ptStar.x(), m_testRC.ptStar.y(),
//                        m_testRC.ptEnd.x() - m_testRC.ptStar.x(),
//                        m_testRC.ptEnd.y() - m_testRC.ptStar.y());
//}

//void QGxImage::SaveRect()
//{
//    GxWriteToIni(tr("pos/ptStarX"), QString::fromLocal8Bit("%1").arg(m_testRC.ptStar.x()));
//    GxWriteToIni(tr("pos/ptStarY"), QString::fromLocal8Bit("%1").arg(m_testRC.ptStar.y()));
//    GxWriteToIni(tr("pos/ptEndX"), QString::fromLocal8Bit("%1").arg(m_testRC.ptEnd.x()));
//    GxWriteToIni(tr("pos/ptEndY"), QString::fromLocal8Bit("%1").arg(m_testRC.ptEnd.y()));
//}

bool QGxImage::OpenV4l2(const char *szDevName)
{
    m_pV4l2 = new Qv4l2(this);
    m_pV4l2->open_device(szDevName);

    return 1;
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

    delete pMsgData;
    delete e;

    QPainter painter(m_pImage1);

    // 画矩形
    //    m_pPainter->setPen(Qt::SolidLine);
    // m_pPainter->setPen(QColor(Qt::blue));
    //    m_pPainter->setPen(QColor(255,0,0));
    painter.setPen(QPen(QBrush(Qt::blue), 4.0, Qt::SolidLine));
    //    m_pPainter->setBrush(QBrush(Qt::blue, Qt::Dense5Pattern));
//    painter.drawRect(m_testRC.rc);

    // 画顶点
    // 画2个顶点
//    int rad = m_testRC.rad;
//    int x = m_testRC.ptStar.x();
//    int y = m_testRC.ptStar.y();
//    painter.fillRect(x - rad, y - rad, rad * 2, rad * 2, Qt::green);
//    x = m_testRC.ptEnd.x();
//    y = m_testRC.ptEnd.y();
//    painter.fillRect(x - rad, y - rad, rad * 2, rad * 2, Qt::green);
//    x = m_testRC.ptStar.x() + m_testRC.rc.width();
//    y = m_testRC.ptStar.y();
//    painter.fillRect(x - rad, y - rad, rad * 2, rad * 2, Qt::green);
//    x = m_testRC.ptStar.x();
//    y = m_testRC.ptStar.y() + m_testRC.rc.height();
//    painter.fillRect(x - rad, y - rad, rad * 2, rad * 2, Qt::green);

    update();
}

int QGxImage::GetFps()
{
    int fps;
    if(m_pV4l2)
    {
        fps = m_totalFrames - m_lastTotalFrames;
        m_lastTotalFrames = m_totalFrames;
    }
    else
    {
        fps = 0;
    }
    return fps;
}
