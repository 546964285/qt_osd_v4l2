#ifndef DLGOFF_H
#define DLGOFF_H

#include <QDialog>
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
    class DlgOff;
}

class DlgOff : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOff(QWidget *parent = 0);
    ~DlgOff();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgOff *ui;

    QFont    m_Font;
    QPalette m_Palette;

    int m_RowNum;

    void ControlInit();//控件初始化
    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void Label123Clear();
    void Label123Change();

    void WriteiniRemember(QString  Strautooff);//将自动关机时间设置存入ini文件
    void ReadiniRemember();//读取ini文件

    bool key_flg;

Q_SIGNALS:
    void SShowdlgf1();
    void SShowMainWindow();
    void SKeyPressed();

public slots:
    void SlotFreshThisWindow();
    void SlotTranslateThisWindow();

};

#endif // DLGOFF_H
