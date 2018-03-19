#ifndef DLGTVMODE_H
#define DLGTVMODE_H

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
    class DlgTVMode;
}

class DlgTVMode : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTVMode(QWidget *parent = 0);
    ~DlgTVMode();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgTVMode *ui;

    QFont    m_Font;
    QPalette m_Palette;

    int m_RowNum;

    void ControlInit();//控件初始化
    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void Label123Clear();
    void Label123Change();

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();
    void SShowMainWindow();
    void SKeyPressed();

public slots:
    void SlotFreshThisWindow();
    void SlotTranslateThisWindow();

};

#endif // DLGTVMODE_H
