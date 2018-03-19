#ifndef DLGSHOWDEV_H
#define DLGSHOWDEV_H

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
    class DlgShowDev;
}

class DlgShowDev : public QDialog
{
    Q_OBJECT

public:
    explicit DlgShowDev(QWidget *parent = 0);
    ~DlgShowDev();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void SlotFreshThisWindow();
    void SlotTranslateThisWindow();

private:
    Ui::DlgShowDev *ui;
    void ControlInit();//控件初始化

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();
    void SShowMainWindow();
    void SKeyPressed();
};

#endif // DLGSHOWDEV_H
