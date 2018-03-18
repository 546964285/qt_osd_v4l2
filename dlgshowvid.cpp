#include "dlgshowvid.h"
#include "ui_dlgshowvid.h"

DlgShowVid::DlgShowVid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgShowVid)
{
    ui->setupUi(this);
}

DlgShowVid::~DlgShowVid()
{
    delete ui;
}
