#ifndef DLGPICORVID_H
#define DLGPICORVID_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTranslator>
#include <QFont>
#include <QtGui>

#include "dlgpicfile.h"
#include "dlgvidfile.h"

namespace Ui
{
    class DlgPicOrVid;
}

class DlgPicOrVid : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPicOrVid(QWidget *parent = 0);
    ~DlgPicOrVid();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgPicOrVid *ui;

    DlgPicFile dlgpicfile;
    DlgVidFile dlgvidfile;

    QFont    g_Font;
    QPalette g_Palette;

    int m_RowNum;

    void ControlInit();//控件初始化
    void LabelSet1_Show();
    void LabelSet2_Show();
    void Label12Clear();
    void Label12Change();

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();//不需要实现
    void SShowMainWindow();
    void SSendNewFolderPath(QString StrNewFolderPath);
    void SFreshOtherWindow();
    void SKeyPressed();

public slots:
    void SlotFreshThisWindow();
    void SlotTranslateThisWindow();
    void SlotShowDlgF1();
    void SlotShowMainWindow();
    void SlotSendNewFolderPath(QString StrNewFolderPath);


};

#endif // DLGPICORVID_H
