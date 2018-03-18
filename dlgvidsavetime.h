#ifndef DLGVIDSAVETIME_H
#define DLGVIDSAVETIME_H

#include <QDialog>

namespace Ui {
    class DlgVidSaveTime;
}

class DlgVidSaveTime : public QDialog
{
    Q_OBJECT

public:
    explicit DlgVidSaveTime(QWidget *parent = 0);
    ~DlgVidSaveTime();

private:
    Ui::DlgVidSaveTime *ui;
};

#endif // DLGVIDSAVETIME_H
