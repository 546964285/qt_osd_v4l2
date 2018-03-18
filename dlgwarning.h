#ifndef DLGWARNING_H
#define DLGWARNING_H

#include <QDialog>

namespace Ui {
    class DlgWarning;
}

class DlgWarning : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWarning(QWidget *parent = 0);
    ~DlgWarning();

private:
    Ui::DlgWarning *ui;
};

#endif // DLGWARNING_H
