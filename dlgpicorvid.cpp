#include "dlgpicorvid.h"
#include "ui_dlgpicorvid.h"

DlgPicOrVid::DlgPicOrVid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPicOrVid)
{
    ui->setupUi(this);
}

DlgPicOrVid::~DlgPicOrVid()
{
    delete ui;
}
