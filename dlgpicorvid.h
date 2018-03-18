#ifndef DLGPICORVID_H
#define DLGPICORVID_H

#include <QDialog>

namespace Ui {
    class DlgPicOrVid;
}

class DlgPicOrVid : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPicOrVid(QWidget *parent = 0);
    ~DlgPicOrVid();

private:
    Ui::DlgPicOrVid *ui;
};

#endif // DLGPICORVID_H
