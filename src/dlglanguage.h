#ifndef DLGLANGUAGE_H
#define DLGLANGUAGE_H

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
    class DlgLanguage;
}

class DlgLanguage : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLanguage(QWidget *parent = 0);
    ~DlgLanguage();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgLanguage *ui;

    void LabelSet1_Show();
    void LabelSet2_Show();
    void LabelSet3_Show();
    void LabelSet4_Show();
    void LabelChange();
    void LabelClear();
    void ControlInit();//控件初始化
    void Translate(QString Language);

    void WriteiniRemember(QString  StrLanguage);//将语言设置存入ini文件
    void ReadiniRemember();

    void InitLanguage();

    QFont    g_Font;
    QPalette g_Palette;

    int m_ColumnNum;
    int m_PageNum;

    bool key_flg;

    QTranslator appTranslator;

Q_SIGNALS:
    void SShowdlgf1();
    void STranslatedlgf1();
    void SShowMainWindow();
    void SKeyPressed();

public slots:
    void SlotFreshThisWindow();

};

#endif // DLGLANGUAGE_H
