#ifndef DLGSTANDBY_H
#define DLGSTANDBY_H

#include <QDialog>

namespace Ui {
    class DlgStandby;
}

class DlgStandby : public QDialog
{
    Q_OBJECT

public:
    explicit DlgStandby(QWidget *parent = 0);
    ~DlgStandby();

private:
    Ui::DlgStandby *ui;
};

#endif // DLGSTANDBY_H
