
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

#include <sys/types.h>//以下几行是为了张冰冰调用驱动所用
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <QDebug>

#include "dlgf1.h"
#include "dlgoff.h"
#include "dlgbackground.h"

namespace Ui
{
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

    void OpenI2CDevice();
    void GetI2CValue();
    bool RgbAlgorithm();//算法
    void ClearRGBGain();

    void OpenFifo();
    void SendMessageFifo(char *bewrite);
    int  ReciveMessageFifo();

    QDateTime current_time;
    QString current_time_str;
    QTimer* updateRTC_timer;
    QTimer* SystemScanTimer;
    QLabel * logoLabel;
    Button * capt_btn;
    Button * record_btn;

    DlgF1          dlgf1;//定义第一级对话框对象，用于工作时候显示
    DlgBackGround  dlgbackground;

    bool key_flg;

    QString g_PowerOnTimeStr;

    int m_bq40z50;
    int m_Battery;
    int m_Charge;
    int m_OnOff;

    int  I2Cfd;
    bool m_Freezed_f;

public slots:
    void call_testdialog();
    void rcdstarstop();
    void capture_ok();
    void capture_fail();
    void capture();
    void SlotSystemScan();
    void SlotShowMainWindow();
    void SlotSendNewFolderPath(QString StrNewFolderPath);

private Q_SLOTS:
    void UpdateRTC();

Q_SIGNALS:
    void    call_dialog();
    void    call_capture();
    void    call_rcdstarstop();
    void    SHideOsd1();
    void    SShowOsd1();
    void    SSendNewFolderPath(QString StrNewFolderPath);
    void    SFreshDlgf1Window();
    void    SKeyPressed();

    void    SFreeze();
    void    SReFreeze();
    void    SWhiteBalance();
    void    SRunAgain();
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
