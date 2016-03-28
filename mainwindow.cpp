#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    palette.setColor(QPalette::Background, QColor(253,253,253));
    setPalette(palette);

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->setupUi(this);

    ui->label->setText(tr(""));

    ui->label->move(40,40);
    //ui->label->resize(640, 480);
    ui->label->resize(400, 400);

    ui->label->paintEvent();
    ui->label->setMask(QRect(40,40,400,400));
    //this->setWindowOpacity(0);

}

MainWindow::~MainWindow()
{

}
