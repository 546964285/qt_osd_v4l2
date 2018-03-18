#ifndef DLGTIME_H
#define DLGTIME_H

#include <QDialog>

namespace Ui {
    class DlgTime;
}

class DlgTime : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTime(QWidget *parent = 0);
    ~DlgTime();

private:
    Ui::DlgTime *ui;
};

#endif // DLGTIME_H
