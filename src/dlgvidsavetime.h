#ifndef DLGVIDSAVETIME_H
#define DLGVIDSAVETIME_H

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
    class DlgVidSaveTime;
}

class DlgVidSaveTime : public QDialog
{
    Q_OBJECT

public:
    explicit DlgVidSaveTime(QWidget *parent = 0);
    ~DlgVidSaveTime();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgVidSaveTime *ui;

    QFont    m_Font;
    QPalette m_Palette;

    int m_RowNum;

    void ControlInit();//控件初始化
    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void Label123Clear();
    void Label123Change();

    void WriteiniRemember(QString  Strvidsavetime);//将录像时间设置存入ini文件
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

#endif // DLGVIDSAVETIME_H
