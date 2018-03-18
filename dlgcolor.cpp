#include "dlgcolor.h"
#include "ui_dlgcolor.h"

DlgColor::DlgColor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgColor)
{
    ui->setupUi(this);
}

DlgColor::~DlgColor()
{
    delete ui;
}
