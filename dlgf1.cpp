#include "dlgf1.h"
#include "ui_dlgf1.h"

DlgF1::DlgF1(QWidget *parent) :QDialog(parent),ui(new Ui::DlgF1)
{
    ui->setupUi(this);
}

DlgF1::~DlgF1()
{
    delete ui;
}
