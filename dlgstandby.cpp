#include "dlgstandby.h"
#include "ui_dlgstandby.h"

DlgStandby::DlgStandby(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgStandby)
{
    ui->setupUi(this);
}

DlgStandby::~DlgStandby()
{
    delete ui;
}
