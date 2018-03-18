#ifndef DLGTVMODE_H
#define DLGTVMODE_H

#include <QDialog>

namespace Ui {
    class DlgTVMode;
}

class DlgTVMode : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTVMode(QWidget *parent = 0);
    ~DlgTVMode();

private:
    Ui::DlgTVMode *ui;
};

#endif // DLGTVMODE_H
