#ifndef DLGRECOVERY_H
#define DLGRECOVERY_H

#include <QDialog>

namespace Ui {
    class DlgRecovery;
}

class DlgRecovery : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRecovery(QWidget *parent = 0);
    ~DlgRecovery();

private:
    Ui::DlgRecovery *ui;
};

#endif // DLGRECOVERY_H
