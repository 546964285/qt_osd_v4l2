#ifndef DLGCOLOR_H
#define DLGCOLOR_H

#include <QDialog>

namespace Ui {
    class DlgColor;
}

class DlgColor : public QDialog
{
    Q_OBJECT

public:
    explicit DlgColor(QWidget *parent = 0);
    ~DlgColor();

private:
    Ui::DlgColor *ui;
};

#endif // DLGCOLOR_H
