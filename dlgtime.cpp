#include "dlgtime.h"
#include "ui_dlgtime.h"

DlgTime::DlgTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTime)
{
    ui->setupUi(this);
}

DlgTime::~DlgTime()
{
    delete ui;
}
