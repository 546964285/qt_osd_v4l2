#include "mainmenu.h"
#include <QDeclarativeView>

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
    view->setSource(QUrl::fromLocalFile("../qt_osd_X11_beta/src/highlight.qml"));
    view->show();
}
