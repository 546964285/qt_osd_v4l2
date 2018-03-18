#include "dlgpicfile.h"
#include "ui_dlgpicfile.h"

DlgPicFile::DlgPicFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPicFile)
{
    ui->setupUi(this);
}

DlgPicFile::~DlgPicFile()
{
    delete ui;
}
