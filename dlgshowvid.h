#ifndef DLGSHOWVID_H
#define DLGSHOWVID_H

#include <QDialog>

namespace Ui {
    class DlgShowVid;
}

class DlgShowVid : public QDialog
{
    Q_OBJECT

public:
    explicit DlgShowVid(QWidget *parent = 0);
    ~DlgShowVid();

private:
    Ui::DlgShowVid *ui;
};

#endif // DLGSHOWVID_H
