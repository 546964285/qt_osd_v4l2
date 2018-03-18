#include "dlgrecovery.h"
#include "ui_dlgrecovery.h"

DlgRecovery::DlgRecovery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRecovery)
{
    ui->setupUi(this);
}

DlgRecovery::~DlgRecovery()
{
    delete ui;
}
