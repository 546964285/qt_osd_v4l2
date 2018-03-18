#include "dlgshowpic.h"
#include "ui_dlgshowpic.h"

DlgShowPic::DlgShowPic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgShowPic)
{
    ui->setupUi(this);
}

DlgShowPic::~DlgShowPic()
{
    delete ui;
}
