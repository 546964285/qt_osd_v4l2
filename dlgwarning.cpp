#include "dlgwarning.h"
#include "ui_dlgwarning.h"

DlgWarning::DlgWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWarning)
{
    ui->setupUi(this);
}

DlgWarning::~DlgWarning()
{
    delete ui;
}
