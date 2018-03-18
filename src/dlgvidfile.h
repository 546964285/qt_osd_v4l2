#ifndef DLGVIDFILE_H
#define DLGVIDFILE_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTranslator>
#include <QFont>
#include <QtGui>

#include "dlgshowvid.h"

namespace Ui
{
    class DlgVidFile;
}

class DlgVidFile : public QDialog
{
    Q_OBJECT

public:
    explicit DlgVidFile(QWidget *parent = 0);
    ~DlgVidFile();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgVidFile *ui;
    bool key_flg;

Q_SIGNALS:
    void SShowMainWindow();
    void SKeyPressed();
};

#endif // DLGVIDFILE_H
