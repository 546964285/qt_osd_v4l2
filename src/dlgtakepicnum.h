#ifndef DLGTAKEPICNUM_H
#define DLGTAKEPICNUM_H

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
    class DlgTakePicNum;
}

class DlgTakePicNum : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTakePicNum(QWidget *parent = 0);
    ~DlgTakePicNum();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void SlotFreshThisWindow();
    void SlotTranslateThisWindow();

private:
    Ui::DlgTakePicNum *ui;

    QFont    m_Font;
    QPalette m_Palette;

    int m_RowNum;

    void ControlInit();//控件初始化
    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void Label123Clear();
    void Label123Change();

    void WriteiniRemember(QString  Str);//存入ini文件
    void ReadiniRemember();//读取ini文件

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();
    void SShowMainWindow();
    void SKeyPressed();
};

#endif // DLGTAKEPICNUM_H
