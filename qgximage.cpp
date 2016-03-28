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

}

QGxImage::~QGxImage()
{

}

//void QGxImage::paintEvent(QPaintEvent * )
void QGxImage::paintEvent()
{
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Clear);
    p.fillRect(40,40,400,400,Qt::SolidPattern);
}
