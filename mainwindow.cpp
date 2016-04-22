#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // pc版本运行位置
    //setGeometry(QRect(100,100,640,480));

    // 隐藏标题栏
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置窗体背景
    setAutoFillBackground(true);
    QPalette  palette;
    //palette.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    palette.setColor(QPalette::Background, QColor(53,73,123));
    setPalette(palette);

    //this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->setupUi(this);

    ui->label->setText(tr(""));

    ui->pushButton->setShortcut(Qt::Key_Enter);
    ui->pushButton->setShortcut(Qt::Key_Return);

//    ui->label->move(48,48);
    //ui->label->resize(640, 480);
    //ui->label->resize(400, 400);
//    ui->label->resize(384, 384);

//    ui->label->setMask(QRect(0,0,384,384));
    //this->setWindowOpacity(0);

    v4l2thread.start();
    std::cout << "thread 1 running" << std::endl;
}

MainWindow::~MainWindow()
{

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        oldPos = event->globalPos();
        press = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (press)
    {
        this->move(this->pos() + event->globalPos() - oldPos);
        oldPos = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    press = false;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        printf("opps\n");
        ui->pushButton->click();
    }
    printf("I am here!\n");
}

//void MainWindow::raising_value()
//{
//    ui->progressBar->setValue(90);
//}
