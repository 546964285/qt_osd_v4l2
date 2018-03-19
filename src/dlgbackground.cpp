#include "dlgbackground.h"
#include "ui_dlgbackground.h"

DlgBackGround::DlgBackGround(QWidget *parent) :QDialog(parent),ui(new Ui::DlgBackGround)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
}

DlgBackGround::~DlgBackGround()
{
    delete ui;
}



