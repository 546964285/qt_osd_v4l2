#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QDateTime>
//#include "qv4l2.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
//    QV4l2Thread v4l2thread;
    QLabel * logoLabel;

signals:

public slots:
//    void raising_value();

    void call_testdialog();

private Q_SLOTS:
    void UpdateRTC();

Q_SIGNALS:
    void call_dialog();
};

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    TestDialog(QWidget *parent = 0);

private:

    QPushButton *button;
};


#endif // MAINWINDOW_H
