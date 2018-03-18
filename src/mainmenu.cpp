#include "mainmenu.h"
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

MainMenu::MainMenu(QWidget *parent):QDialog(parent)
{
    setGeometry(QRect(0,0,640,480));

//    QDeclarativeView view;
//    view.setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/application.qml"));
//    view.show();

//    QDeclarativeView * view=new QDeclarativeView(this);
//    view->setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/application.qml"));
//    view->show();

    QDeclarativeView * view=new QDeclarativeView(this);
//    view->setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/highlight.qml"));
    view->setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/gridview.qml"));
    QObject * item = view->rootObject();
    if(item)
    {
        QObject::connect(item, SIGNAL(buttonClicked(int)),this,SLOT(cppSlot(int)));
    }

    view->show();
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

