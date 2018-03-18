#ifndef DLGF1_H
#define DLGF1_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>

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

    QPixmap  g_Pixmap;
    QString  g_PicturefileName;
    QFont    g_Font;
    QPalette g_Palette;

    int m_ColumnNum;
    int m_PageNum;

    //DlgPicOrVid dlgpicorvid;
    //DlgShowDev dlgshowdev;
    //DlgTakePicNum dlgtakepicnum;

Q_SIGNALS:
   void  SShowMainWindow();//显示主窗口信号
};

#endif // DLGF1_H
