#include "qbattery.h"

QBattery::QBattery(QWidget *parent) :QWidget(parent)
{
    //m_bReverse=false;
    m_value=0;
    //updateTimer=new QTimer(this);
    //updateTimer->setInterval(5);
    //connect(updateTimer,SIGNAL(timeout()),this,SLOT(UpdateGraph()));
}

void QBattery::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawFrame(&painter);
    drawBattery(&painter);
    drawNum(&painter);
    painter.end();
}
void QBattery::drawFrame(QPainter *painter)
{
    painter->save();

    int side = qMin(width(), height());
    m_headWidth=width()/10; // 20
    qreal batteryWidth=width()-m_headWidth; //180
    //qDebug() <<"batteryWidth="<<batteryWidth<< endl;
    //qDebug() <<"m_headWidth="<<m_headWidth<< endl;
    //qDebug() <<"height()="<<height()<< endl;

    // 矩形框
    QPointF topLeftPot(0,0);
    QPointF bottomRightPot(batteryWidth,height());//180,120
    QRectF batteryRect(topLeftPot,bottomRightPot);
    m_batteryRect=batteryRect;
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::white,side/10));//矩形框的颜色线宽
    painter->drawRect(batteryRect);

    QPointF headRectTopLeft(batteryRect.topRight().x(),height()/4);//180,30
    //qDebug() <<"batteryRect.topRight().x()="<<batteryRect.topRight().x()<< endl;

    QPointF headRectBottomRight(width(),height()-height()/4);//200,90
    QRectF headRect(headRectTopLeft,headRectBottomRight);

//    //渐变填充
//    QLinearGradient headRectGradient(headRect.topLeft(),headRect.bottomLeft());
//    headRectGradient.setColorAt(0.0,QColor(150,150,150));
//    headRectGradient.setColorAt(0.5,QColor(220,220,220));
//    headRectGradient.setColorAt(1.0,QColor(150,150,150));
//    painter->setBrush(headRectGradient);
//    painter->drawRect(headRect);

    painter->setPen(QPen(Qt::white,side/10));
    //painter->setBrush(Qt::NoBrush);
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawRect(headRect);

//    numSpin-setGeometry(QRect(batteryWidth/2,height()/2));
    painter->restore();
}

void QBattery::drawBattery(QPainter *painter)
{
    painter->save();

    int side = qMin(width(), height());
    QLinearGradient batteryGradient(QPointF(0,0),QPointF(0,height()));

    if(m_value<20)
    {
        batteryGradient.setColorAt(0.0,QColor(220,220,220));
        batteryGradient.setColorAt(0.7,QColor(180,50,50));
        batteryGradient.setColorAt(1.0,QColor(120,50,50));
    }
    else if(m_value<40)
    {
        batteryGradient.setColorAt(0.0,QColor(220,220,220));
        batteryGradient.setColorAt(0.7,QColor(200,200,0));
        batteryGradient.setColorAt(0.9,QColor(160,160,0));
    }
    else
    {
        batteryGradient.setColorAt(0.0,QColor(220,220,220));
        //batteryGradient.setColorAt(0.3,QColor(74,180,0));
        //batteryGradient.setColorAt(0.5,QColor(60,230,0));
        batteryGradient.setColorAt(0.7,QColor(0,180,0));
        //batteryGradient.setColorAt(0.9,QColor(0,200,0));
        batteryGradient.setColorAt(1.0,QColor(0,120,0));
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(batteryGradient);

    // 表示电池电量的进度条矩形框
    QPointF topLeftPot(m_batteryRect.topLeft().x()+side/10,m_batteryRect.topLeft().y()+side/10);
    qreal unit=(m_batteryRect.width()-side/5)/100;
    qreal wid=side/10+m_value*unit;
    QPointF bottomRightPot(wid,m_batteryRect.bottomRight().y()-side/10);
    painter->drawRect(QRectF(topLeftPot,bottomRightPot));

    painter->restore();

}

void QBattery::drawNum(QPainter *painter)
{
    painter->save();
    QFont font("Arial",10,QFont::Bold,false);
    int side = qMin(width(), height());
    painter->setPen(QPen(Qt::white,side/10));
    painter->setFont(font);
    painter->drawText(m_batteryRect, Qt::AlignHCenter|Qt::AlignVCenter, tr("%1%").arg(m_value));
    //painter->drawText(QPoint(width()*7/20, height()/2+5),tr("%1%").arg(m_value));
    //QLabel *label = new QLabel("<h2><i>Hello</i> ""<font color=red>Qt!</font></h2>");
    painter->restore();
}

//void QBattery::UpdateGraph()
//{
//    if(m_bReverse)
//    {
//        m_value-=0.1;

//        if(m_value<=m_currentValue)
//        {
//            updateTimer->stop();
//        }
//    }else{
//        m_value+=0.1;
//        if(m_value>=m_currentValue)
//        {
//            updateTimer->stop();
//        }
//    }
//    emit valueChanged(m_value);

//    update();
//}

void QBattery::setValue(int value)
{
    if(value<0 || value>100)
    {
        qDebug()<< "The value must be between 0 and 100 !";
        return ;
    }
    m_value = value;
    emit valueChanged(m_value);
    update();
}

void QBattery::addValue()
{
    qreal add = 1;

    qDebug() <<"addValue()"<< endl;
    if((m_value+add)>100)
    {
        qWarning() << "The value reaches max!";
        m_value = 100;
    }
    else
    {
        m_value += add;
    }

    emit valueChanged(m_value);
    update();
}

void QBattery::subValue()
{
    qreal sub=1;

    qDebug() <<"subValue()"<< endl;
    if((m_value-sub)<0)
    {
        qWarning() << "The value reaches min!";
        m_value = 0;
    }
    else
    {
        m_value -= sub;
    }

    emit valueChanged(m_value);
    update();
}
