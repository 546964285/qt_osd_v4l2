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

BackPlay::BackPlay(QWidget *parent):QDialog(parent)
{
    setGeometry(QRect(0,0,640,480));
    QDir dir("./photos");
    QString path = dir.filePath("a1.jpg");
    QString absolutePath = dir.absoluteFilePath("a1.jpg");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //dir.setSorting(QDir::Size | QDir::Reversed);
    dir.setSorting(QDir::Name);
    QStringList filters;
    filters << "*.jpg" << "*.cpp" << "*.cxx" << "*.cc";
    dir.setNameFilters(filters);
    QDeclarativeView * view =new QDeclarativeView(this);
    QStringList dataList;
    QFileInfoList list = dir.entryInfoList();
    std::cout << "     Bytes Filename" << std::endl;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                                                .arg(fileInfo.fileName()));
        std::cout << std::endl;
        qDebug() << fileInfo.baseName();
        dataList.append(fileInfo.baseName());
    }
    std::cout << qPrintable(dir.absolutePath()) << std::endl;
    std::cout << qPrintable(dir.dirName()) << std::endl;
    std::cout << qPrintable(path) << std::endl;
    std::cout << qPrintable(absolutePath) << std::endl;

    QDeclarativeContext *ctxt = view->rootContext();
    ctxt->setContextProperty("myModel", QVariant::fromValue(dataList));
    view->setSource(QString("./qml/playback.qml"));
    view->show();
}
