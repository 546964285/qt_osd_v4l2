#ifndef DLGBACKGROUND_H
#define DLGBACKGROUND_H

#include <QDialog>

namespace Ui
{
    class DlgBackGround;
}

class DlgBackGround : public QDialog
{
    Q_OBJECT

public:
    explicit DlgBackGround(QWidget *parent = 0);
    ~DlgBackGround();

private:
    Ui::DlgBackGround *ui;
};

#endif // DLGBACKGROUND_H
