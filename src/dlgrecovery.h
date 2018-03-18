#ifndef DLGRECOVERY_H
#define DLGRECOVERY_H

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
    class DlgRecovery;
}

class DlgRecovery : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRecovery(QWidget *parent = 0);
    ~DlgRecovery();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgRecovery *ui;

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

#endif // DLGRECOVERY_H
