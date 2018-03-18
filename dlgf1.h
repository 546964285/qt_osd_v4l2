#ifndef DLGF1_H
#define DLGF1_H

#include <QDialog>
#include <QLabel>

namespace Ui
{
    class DlgF1;
}

class DlgF1 : public QDialog
{
    Q_OBJECT

public:
    explicit DlgF1(QWidget *parent = 0);
    ~DlgF1();

private:
    Ui::DlgF1 *ui;
};

#endif // DLGF1_H
