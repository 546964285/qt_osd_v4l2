#ifndef DLGPICFILE_H
#define DLGPICFILE_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTranslator>
#include <QFont>
#include <QtGui>
#include <QDir>
#include <QPixmap>
#include <QFile>
#include <QTimer>
#include <QThread>
#include <QEventLoop>

#include "dlgshowpic.h"

namespace Ui
{
    class DlgPicFile;
}

class DlgPicFile : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPicFile(QWidget *parent = 0);
    ~DlgPicFile();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::DlgPicFile *ui;

    DlgShowPic dlgshowpic;//显示图片的对象

        int g_RowNum;
        int g_ColumnNum;
        int g_PageNum;
        int g_PictureNum;//照片总数
        int g_PictureCurrentNo;//当前照片编号

        int m_PageFirstPicNum;//翻页时候页面的第一张，主要用于判断哪个label可以显示

        struct PictureDetail
        {
              QString FolderPath;
              QString FolderName;
              QString PicturePath;
              QString PictureName;
              QString StringNo;
        };
        struct PictureDetail  PictureDetailStu[1000];

        void LabelWhiteBorder();
        void LabelHideBorder();//隐藏框
        void LabelPictureShowPage(int No);//显示一页缩略图
        void LoadPictureFile();
        void LabelPictureShowOne(int LabelN, int PictureNo);//指定一个控件  显示缩略图
        void FilePictureScan(QString path);
        void PictureNoShowFresh();//当前照片编号/总照片数  显示
        void KeyPressDown();//向下按钮子函数
        void KeyPressUp();//向上按钮子函数
        void InitLabelText();//照片编号的文字初始化
        void SendShow();
        void CreatFolder();//第一次扫描文件后，创建文件夹
        bool DelDir(const QString &path);

        void LabelClearAll(int No);//清空所有label

        int m_FirstScanFlag;
        int m_FolderNo1;
        int m_FolderNo2;

        void LabelShowBlackFirst(int LabelN);//在图片还没加载出来的时候先给控件显示一个背景色

        QString m_StrNewFolderPath;
        QString m_StrNewFolderName;
        QString m_StrLastOldFolderName;

         bool key_flg;

        QTimer *timer;
        bool m_Init_f;


    Q_SIGNALS:
        void SSendPicPath(QString Path);
        void SShowMainWindow();
        void SSendNewFolderPath(QString StrNewFolderPath);
        void SKeyPressed();

        void SToThreadFilePath(int LabelN,QString FilePath);///////////////
        void SClearThread_i();

    public slots:
        void SlotNextOrPre(int msg);
        void SlotShowMainWindow();
        void SlotDelayToInit();
        void SlotFreshThisWindow();

        void SlotLabelShowOne(int LabelN,QImage SmallImage);///////////////
};

class DlgPicFileThread : public QThread
{
    Q_OBJECT

    public:
        DlgPicFileThread();
        void msecSleep(int msec);//非阻塞式延时
        void run();

    private:
        int Thread_i;

        int LabelN1,LabelN2,LabelN3,LabelN4,LabelN5,LabelN6,LabelN7,LabelN8,LabelN9;
        QString FilePath1,FilePath2,FilePath3,FilePath4,FilePath5,FilePath6,FilePath7,FilePath8,FilePath9;

        int Lock_F;


    Q_SIGNALS:
        void SLabelShowSmallImage(int LabelN,QImage Image);/////////////////////

    public slots:
        void SlotLoadImage(int LabelN,QString FilePath);////////////////////////
        void SlotClearThread_i();


};

#endif // DLGPICFILE_H
