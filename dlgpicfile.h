#ifndef DLGPICFILE_H
#define DLGPICFILE_H

#include <QDialog>

namespace Ui {
    class DlgPicFile;
}

class DlgPicFile : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPicFile(QWidget *parent = 0);
    ~DlgPicFile();

private:
    Ui::DlgPicFile *ui;
};

#endif // DLGPICFILE_H
