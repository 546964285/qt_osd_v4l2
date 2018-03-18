#include "mainmenu.h"
#include "mainwindow.h"
#include "backplay.h"
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QDebug>
#include <iostream>

#include <QDir>
#include <QTranslator>
#include <QAbstractProxyModel>
#include <QTextCodec>

MainMenu::MainMenu(QWidget *parent):QDialog(parent)
{
    setGeometry(QRect(0,0,640,480));

//    QDeclarativeView view;
//    view.setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/application.qml"));
//    view.show();

//    QDeclarativeView * view=new QDeclarativeView(this);
//    view->setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/application.qml"));
//    view->show();

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QDeclarativeView * view=new QDeclarativeView(this);
//    view->setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/highlight.qml"));
    view->setSource(QUrl::fromLocalFile("./qml/gridview.qml"));
//    view->setSource(QString("qrc:/gridview.qml"));
    QObject * item = view->rootObject();
    if(item)
    {
        QObject::connect(item, SIGNAL(buttonClicked(int)),this,SLOT(cppSlot(int)));
    }

    view->show();

    MainWindow * ptr = (MainWindow *)parentWidget();
    connect(this,SIGNAL(destroyed()),&(ptr->v4l2thread),SLOT(trans_osd1()));
}

void MainMenu::cppSlot(int index)
{
    qDebug()<<"c++ get "<<index;

    if(index==0)
    {
        BackPlay backplay(this);
        backplay.exec();
    }

}

