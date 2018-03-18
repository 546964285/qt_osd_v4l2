#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "qbattery.h"

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
    //palette.setColor(QPalette::Background, QColor(53,73,123));
    palette.setColor(QPalette::Background, Qt::darkGray);
    setPalette(palette);

    // 设置窗体透明，需要注释掉setAutoFillBackground(true);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);

    // 窗体透明控件不透明，写在窗体构造函数里
//    QPalette pal = palette();
//    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
//    setPalette(pal);

    ui->setupUi(this);

    ui->label->setText(tr(""));

//    ui->pushButton->setShortcut(Qt::Key_Enter);
//    ui->pushButton->setFocusPolicy(Qt::NoFocus);
//    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);

    connect(ui->battery, SIGNAL(valueChanged(int)), ui->progressBar, SLOT(setValue(int)));

    ui->battery->setValue(50);


//    ui->label->move(48,48);
    //ui->label->resize(640, 480);
    //ui->label->resize(400, 400);
//    ui->label->resize(384, 384);

//    ui->label->setMask(QRect(0,0,384,384));
    //this->setWindowOpacity(0);

    //v4l2thread.start();
    std::cout << "thread 1 running" << std::endl;
    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(call_testdialog()));
//    connect(ui->pushButton, SIGNAL(clicked()), ui->battery, SLOT(addValue()));
//    connect(ui->pushButton_2, SIGNAL(clicked()), ui->battery, SLOT(subValue()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::call_testdialog()
{
    TestDialog dialog(this);
    dialog.exec();
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
    int keyValue;
    keyValue=event->key();

    qDebug() <<"pressed keyValue= "<< keyValue << endl;

    switch (keyValue) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        {
            //printf("opps 1\n");
//            ui->pushButton->click();
//            ui->pushButton->setDown(true);
            ui->battery->addValue();
        }
        break;
    case Qt::Key_A:
        {
            //printf("opps 1\n");
//            ui->pushButton_2->click();
//            ui->pushButton_2->setDown(true);
            ui->battery->subValue();
        }
        break;

    default:
        QWidget::keyPressEvent(event);
        break;
    }

    //printf("I am here 1!\n");
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int keyValue;
    keyValue=event->key();

    qDebug() <<"released keyValue= "<< keyValue << endl;

    switch (keyValue) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        {
//            ui->pushButton->setDown(false);
        }
        break;
    case Qt::Key_A:
        {
//            ui->pushButton_2->setDown(false);
        }
        break;

    default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}


//void MainWindow::raising_value()
//{
//    ui->progressBar->setValue(90);
//}

TestDialog::TestDialog(QWidget *parent)
    : QDialog(parent)
{
    setGeometry(QRect(400,100,480,180));

    button = new QPushButton("Quit");
    button->setEnabled(true);
    button->show();

    QVBoxLayout *VLayout = new QVBoxLayout;
    VLayout->addWidget(button);

    setLayout(VLayout);

    connect(button, SIGNAL(clicked()), this, SLOT(close()));
}

