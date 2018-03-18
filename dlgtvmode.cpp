#include "dlgtvmode.h"
#include "ui_dlgtvmode.h"

DlgTVMode::DlgTVMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTVMode)
{
    ui->setupUi(this);
}

DlgTVMode::~DlgTVMode()
{
    delete ui;
}
