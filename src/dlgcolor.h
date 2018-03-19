#ifndef DLGCOLOR_H
#define DLGCOLOR_H

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
    class DlgColor;
}

class DlgColor : public QDialog
{
    Q_OBJECT

public:
    explicit DlgColor(QWidget *parent = 0);
    ~DlgColor();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgColor *ui;

    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void LabelSet4_Show();
    void LabelChange();
    void LabelClear();
    void ControlInit();//控件初始化

    void WriteiniRemember(int n ,int Colornum);//存入ini文件
    void ReadiniRemember();

    QFont    g_Font;
    QPalette g_Palette;

    int m_ColumnNum;
    int m_Config1Num,m_Config2Num,m_Config3Num,m_Config4Num;

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();
    void SShowMainWindow();
    void SKeyPressed();

public slots:
    void SlotShowMainWindow();
    void SlotFreshThisWindow();
    void SlotTranslateThisWindow();
};

#endif // DLGCOLOR_H
