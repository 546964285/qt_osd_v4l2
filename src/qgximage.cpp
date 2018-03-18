#include "qgximage.h"
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QSettings>
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
//void QGxImage::paintEvent()
{
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Clear);
    p.fillRect(0,0,384,384,Qt::SolidPattern);
    //p.fillRect(40,40,400,400,Qt::green);
}
