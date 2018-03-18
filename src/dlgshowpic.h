#ifndef DLGSHOWPIC_H
#define DLGSHOWPIC_H

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
    class DlgShowPic;
}

class DlgShowPic : public QDialog
{
    Q_OBJECT

public:
    explicit DlgShowPic(QWidget *parent = 0);
    ~DlgShowPic();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void msecSleep(int msec);//非阻塞式延时

private:
    Ui::DlgShowPic *ui;

    bool key_flg;
    bool m_keylock;

public slots:
    void SlotPictureShow(QString PicturePath);//根据路径显示图片

Q_SIGNALS:
    void SNextOrPre(int msg);//不需要实现
    void SShowMainWindow();
    void SKeyPressed();
};

#endif // DLGSHOWPIC_H
