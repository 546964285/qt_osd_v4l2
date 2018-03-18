#ifndef DLGSHOWPIC_H
#define DLGSHOWPIC_H

#include <QDialog>

namespace Ui {
    class DlgShowPic;
}

class DlgShowPic : public QDialog
{
    Q_OBJECT

public:
    explicit DlgShowPic(QWidget *parent = 0);
    ~DlgShowPic();

private:
    Ui::DlgShowPic *ui;
};

#endif // DLGSHOWPIC_H
