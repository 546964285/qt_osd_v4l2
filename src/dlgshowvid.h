#ifndef DLGSHOWVID_H
#define DLGSHOWVID_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTranslator>
#include <QFont>
#include <QtGui>

namespace Ui
{
    class DlgShowVid;
}

class DlgShowVid : public QDialog
{
    Q_OBJECT

public:
    explicit DlgShowVid(QWidget *parent = 0);
    ~DlgShowVid();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgShowVid *ui;

    bool key_flg;

Q_SIGNALS:
    void SShowMainWindow();
    void SKeyPressed();


};

#endif // DLGSHOWVID_H
