#include "dlgoff.h"
#include "ui_dlgoff.h"

DlgOff::DlgOff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOff)
{
    ui->setupUi(this);
}

DlgOff::~DlgOff()
{
    delete ui;
}
