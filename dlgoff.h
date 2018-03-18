#ifndef DLGOFF_H
#define DLGOFF_H

#include <QDialog>

namespace Ui {
    class DlgOff;
}

class DlgOff : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOff(QWidget *parent = 0);
    ~DlgOff();

private:
    Ui::DlgOff *ui;
};

#endif // DLGOFF_H
