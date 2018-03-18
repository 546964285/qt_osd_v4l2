#ifndef DLGTIME_H
#define DLGTIME_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTranslator>
#include <QFont>
#include <QtGui>

namespace Ui {
    class DlgTime;
}

class DlgTime : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTime(QWidget *parent = 0);
    ~DlgTime();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgTime *ui;

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();
    void SShowMainWindow();
    void SKeyPressed();

public slots:
    void SlotShowMainWindow();
    void SlotTranslateThisWindow();

};

#endif // DLGTIME_H
