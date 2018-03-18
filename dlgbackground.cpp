#include "dlgbackground.h"
#include "ui_dlgbackground.h"

DlgBackGround::DlgBackGround(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgBackGround)
{
    ui->setupUi(this);
}

DlgBackGround::~DlgBackGround()
{
    delete ui;
}
