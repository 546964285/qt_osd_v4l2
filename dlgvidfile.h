#ifndef DLGVIDFILE_H
#define DLGVIDFILE_H

#include <QDialog>

namespace Ui {
    class DlgVidFile;
}

class DlgVidFile : public QDialog
{
    Q_OBJECT

public:
    explicit DlgVidFile(QWidget *parent = 0);
    ~DlgVidFile();

private:
    Ui::DlgVidFile *ui;
};

#endif // DLGVIDFILE_H
