#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QDateTime>
//#include <QThread>
#include "qv4l2.h"
#include "Button.h"
#include "mainmenu.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QV4l2Thread v4l2thread;

    void msecSleep(int msec);

private:
    Ui::MainWindow *ui;

    QPoint oldPos;
    bool press;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    QDateTime current_time;
    QString current_time_str;
    QTimer* updateRTC_timer;
    QLabel * logoLabel;
    Button * capt_btn;
    Button * record_btn;

signals:

public slots:
    void call_testdialog();
    void rcdstarstop();
    void capture_ok();
    void capture_fail();
    void capture();

private Q_SLOTS:
    void UpdateRTC();

Q_SIGNALS:
    void call_dialog();
    void call_capture();
    void call_rcdstarstop();
};

class SleeperThread : public QThread
{
public:
    static void sleep(unsigned long secs)
    {
        QThread::sleep(secs);
    }
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
    static void usleep(unsigned long usecs)
    {
        QThread::usleep(usecs);
    }
};


#endif // MAINWINDOW_H
