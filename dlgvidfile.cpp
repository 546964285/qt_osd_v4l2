#include "dlgvidfile.h"
#include "ui_dlgvidfile.h"

DlgVidFile::DlgVidFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgVidFile)
{
    ui->setupUi(this);
}

DlgVidFile::~DlgVidFile()
{
    delete ui;
}
