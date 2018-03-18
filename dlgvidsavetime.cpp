#include "dlgvidsavetime.h"
#include "ui_dlgvidsavetime.h"

DlgVidSaveTime::DlgVidSaveTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgVidSaveTime)
{
    ui->setupUi(this);
}

DlgVidSaveTime::~DlgVidSaveTime()
{
    delete ui;
}
