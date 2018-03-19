#ifndef DLGF1_H
#define DLGF1_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QTranslator>
#include <QtGui>

#include "dlgshowdev.h"
#include "dlglanguage.h"
#include "dlgtakepicnum.h"
#include "dlgpicorvid.h"
#include "dlgstandby.h"
#include "dlgvidsavetime.h"
#include "dlgtvmode.h"
#include "dlgrecovery.h"
#include "dlgoff.h"
#include "dlgcolor.h"
#include "dlgtime.h"

namespace Ui
{
    class DlgF1;
}

class DlgF1 : public QDialog
{
    Q_OBJECT

public:
    explicit DlgF1(QWidget *parent = 0);
    ~DlgF1();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);



private:
    Ui::DlgF1 *ui;

    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void LabelSet4_Show();
    void LabelChange();
    void LabelClear();
    void WindowStyleSet();
    void ControlInit();//控件初始化

    QFont     g_Font;
    QPalette  g_Palette;

    int m_ColumnNum;
    int m_PageNum;

    DlgShowDev     dlgshowdev;
    DlgTakePicNum  dlgtakepicnum;
    DlgLanguage    dlglanguage;
    DlgPicOrVid    dlgpicorvid;
    DlgStandby     dlgstandby;
    DlgVidSaveTime dlgvidsavetime;
    DlgTVMode      dlgtvmode;
    DlgRecovery    dlgrecovery;
    DlgOff         dlgoff;
    DlgColor       dlgcolor;
    DlgTime        dlgtime;

    bool key_flg;

Q_SIGNALS:
    void  SShowMainWindow();//显示主窗口信号
    void  STranslateOtherWindow();//刷新其它窗口
    void  SSendNewFolderPath(QString StrNewFolderPath);
    void  SKeyPressed();

    void  SFreshDlgShowDevWindow();
    void  SFreshDlgTakePicNumWindow();
    void  SFreshDlgPicOrVidWindow();
    void  SFreshDlgStandbyWindow();
    void  SFreshDlgVidSaveTimeWindow();
    void  SFreshDlgTVModeWindow();
    void  SFreshDlgRecoveryWindow();
    void  SFreshDlgOffWindow();
    void  SFreshDlgColorWindow();
    void  SFreshDlgTimeWindow();

public slots:
    void  SlotShowThisWindow();
    void  SlotFreshThisWindow();
    void  SlotTranslateThisWindow();
    void  SlotShowMainWindow();
    void  SlotSendNewFolderPath(QString StrNewFolderPath);
};

#endif // DLGF1_H
