#include "dlgpicfile.h"
#include "ui_dlgpicfile.h"


DlgPicFile::DlgPicFile(QWidget *parent) :QDialog(parent),ui(new Ui::DlgPicFile)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);// 隐藏标题栏
    setAutoFillBackground(true);// 设置窗体背景
    QPalette  palette;
    palette.setColor(QPalette::Background, QColor(53,73,123));//设置背景颜色  QColor(0x00,0xff,0x00,0x00)透明
    setPalette(palette);
    g_RowNum=0;//行
    g_ColumnNum=0;//列
    g_PageNum=0;//页
    g_PictureNum=0;//照片总数
    g_PictureCurrentNo=0;//当前照片编号
    m_FirstScanFlag=0;
    m_FolderNo1=0;
    m_FolderNo2=0;
    dlgshowpic.setModal(false);
    dlgshowpic.hide();
    connect(this, SIGNAL(SSendPicPath(QString )), &dlgshowpic, SLOT(SlotPictureShow(QString )));
    connect(&dlgshowpic, SIGNAL(SNextOrPre(int )), this, SLOT(SlotNextOrPre(int )));
    connect(&dlgshowpic, SIGNAL(SShowMainWindow()), this, SLOT(SlotShowMainWindow()));
    ui->Label1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label5->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label6->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label7->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label8->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->Label9->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);////////设置label控件 水平居中，上下居中
    ui->LabelTextNum->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置label控件 水平居中，上下居中
    ui->LabelTextNo->setAlignment(Qt::AlignHCenter  | Qt::AlignVCenter);//设置label控件 水平居中，上下居中

    this->timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(SlotDelayToInit()));
    this->timer->start(3000);//定时3秒
    m_Init_f=false;
}

DlgPicFile::~DlgPicFile()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////线程
DlgPicFileThread::DlgPicFileThread()
{
    Thread_i=0;
    Lock_F=0;
    qDebug()<< "Thread Construct,Thread_i=0";
}
void DlgPicFileThread::run()
{
   QImage  Image;
   int     LabelN;
   QString FilePath;
   while(1)
   {
        //qDebug() << "Picture load thread running...";
        if(Lock_F==0)
        {
            if(Thread_i>0)
            {
                switch(Thread_i)
                {
                    case 1:LabelN=LabelN1;FilePath=FilePath1;break;
                    case 2:LabelN=LabelN2;FilePath=FilePath2;break;
                    case 3:LabelN=LabelN3;FilePath=FilePath3;break;
                    case 4:LabelN=LabelN4;FilePath=FilePath4;break;
                    case 5:LabelN=LabelN5;FilePath=FilePath5;break;
                    case 6:LabelN=LabelN6;FilePath=FilePath6;break;
                    case 7:LabelN=LabelN7;FilePath=FilePath7;break;
                    case 8:LabelN=LabelN8;FilePath=FilePath8;break;
                    case 9:LabelN=LabelN9;FilePath=FilePath9;break;
                    default:break;
                }
                qDebug()<< "while Thread_i is :" << Thread_i;
                Thread_i--;
                Image.load(FilePath);
                qDebug()<< "Thread Picture filePath is :" << FilePath;
                Image = Image.scaled(160, 110, Qt::IgnoreAspectRatio, Qt::FastTransformation);
                emit SLabelShowSmallImage(LabelN,Image);
            }
        }
        else
        {
            this->msecSleep(50);//等待发完
        }
   }
}
void DlgPicFileThread::SlotLoadImage(int LabelN,QString FilePath)
{
    Lock_F=1;
    Thread_i++;
    switch(Thread_i)
    {
        case 1:LabelN1=LabelN;FilePath1=FilePath;break;
        case 2:LabelN2=LabelN;FilePath2=FilePath;break;
        case 3:LabelN3=LabelN;FilePath3=FilePath;break;
        case 4:LabelN4=LabelN;FilePath4=FilePath;break;
        case 5:LabelN5=LabelN;FilePath5=FilePath;break;
        case 6:LabelN6=LabelN;FilePath6=FilePath;break;
        case 7:LabelN7=LabelN;FilePath7=FilePath;break;
        case 8:LabelN8=LabelN;FilePath8=FilePath;break;
        case 9:LabelN9=LabelN;FilePath9=FilePath;break;
        default:break;
    }
    Lock_F=0;
}
void DlgPicFileThread::SlotClearThread_i()
{
    Thread_i=0;
    qDebug()<< "SlotClearThread_i() got signal, Thread_i is :" << Thread_i;
}
void DlgPicFileThread::msecSleep(int msec)//非阻塞式延时
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() <dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
////////////////////////////////////////////////////////////////////////////////

void DlgPicFile::SlotDelayToInit()
{
    this->timer->stop();
    LabelHideBorder();//隐藏框
    LabelWhiteBorder();
    LoadPictureFile();
    InitLabelText();//照片编号的文字初始化
    qDebug()<< "LoadPictureFile()............................";
    m_Init_f=true;
    QThread* dlgpicfilethread = new DlgPicFileThread();
    connect(this,SIGNAL(SToThreadFilePath(int ,QString )),dlgpicfilethread,SLOT(SlotLoadImage(int ,QString )));
    connect(dlgpicfilethread,SIGNAL(SLabelShowSmallImage(int ,QImage )),this,SLOT(SlotLabelShowOne(int ,QImage )));
    connect(this,SIGNAL(SClearThread_i()),dlgpicfilethread,SLOT(SlotClearThread_i()));
    dlgpicfilethread->start();
}

void DlgPicFile::SlotNextOrPre(int msg)
{
    if(msg==1)
    {
        KeyPressDown();//向下按钮子函数
    }
    else if(msg==2)
    {
        KeyPressUp();//向上按钮子函数
    }
    emit SSendPicPath(PictureDetailStu[g_PictureCurrentNo].PicturePath);
}

void DlgPicFile::SendShow()//发信号到下一级显示照片的类中，传递照片路径过去
{
    emit SSendPicPath(PictureDetailStu[g_PictureCurrentNo].PicturePath);
    dlgshowpic.show();
}

void DlgPicFile::SlotShowMainWindow()//先传递到上一级，再传递到主窗口
{
    emit SShowMainWindow();
}

void DlgPicFile::SlotFreshThisWindow()
{
    if(m_Init_f==true)
    {
        LabelHideBorder();//隐藏框
        LabelWhiteBorder();
        LoadPictureFile();
        InitLabelText();//照片编号的文字初始化
        qDebug()<< "LoadPictureFile()............................";
    }
}

void DlgPicFile::KeyPressDown()//向下 按钮子函数
{
    int f1,f2;
    f1=g_PictureCurrentNo/9;
    if(g_PictureCurrentNo<(g_PictureNum-1))
    {
        g_PictureCurrentNo++;
    }
    else
    {
        g_PictureCurrentNo=0;
    }
    f2=g_PictureCurrentNo/9;
    if(f1!=f2)
    {
        m_PageFirstPicNum=(g_PictureCurrentNo/9)*9;
       LabelPictureShowPage(m_PageFirstPicNum);
    }
    g_RowNum=g_PictureCurrentNo%9/3;
    g_ColumnNum=g_PictureCurrentNo%9%3;
    LabelHideBorder();//隐藏框
    LabelWhiteBorder();//显示白框
}

void DlgPicFile::KeyPressUp()//向上 按钮子函数
{
    int f1,f2;
    f1=g_PictureCurrentNo/9;
    if(g_PictureCurrentNo>0)
    {
        g_PictureCurrentNo--;
    }
    else
    {
        g_PictureCurrentNo=g_PictureNum-1;
    }
    f2=g_PictureCurrentNo/9;
    if(f1!=f2)
    {
        m_PageFirstPicNum=(g_PictureCurrentNo/9)*9;
       LabelPictureShowPage(m_PageFirstPicNum);
    }
    g_RowNum=g_PictureCurrentNo%9/3;
    g_ColumnNum=g_PictureCurrentNo%9%3;
    LabelHideBorder();//隐藏框
    LabelWhiteBorder();//显示白框
}

void DlgPicFile::PictureNoShowFresh()//当前照片编号/总照片数  显示
{
    ui->LabelTextNum->setText(QString::number(g_PictureCurrentNo+1)+"/"+QString::number(g_PictureNum));
    ui->LabelTextNo->setText(PictureDetailStu[g_PictureCurrentNo].StringNo);//显示照片编号（文件夹高3位-照片低4位）
}

void DlgPicFile::LoadPictureFile()
{
    FilePictureScan("/media/mmcblk0p1/DCIM/");
}

void DlgPicFile::InitLabelText()
{
    QFont Font1;
    Font1.setPointSize(25);//大小
    ui->LabelTextNum->setFont(Font1);
    QPalette Palette1;
    Palette1.setColor(QPalette::WindowText,Qt::white);//颜色
    ui->LabelTextNum->setPalette(Palette1);    
    ui->LabelTextNum->setText(QString::number(g_PictureCurrentNo+1)+"/"+QString::number(g_PictureNum));//当前照片编号/照片总数
    ui->LabelTextNo->setFont(Font1);
    ui->LabelTextNo->setPalette(Palette1);   
    ui->LabelTextNo->setText(PictureDetailStu[0].StringNo);//显示照片编号（文件夹高3位-照片低4位）
}

bool DlgPicFile::DelDir(const QString &path)
{
    if (path.isEmpty())//path 没有值
    {
        return false;
    }
    QDir dir(path);
    if(!dir.exists())//如果路径不存在
    {
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);///设置过滤
    QFileInfoList fileList = dir.entryInfoList();/////////////获取所有的文件信息
    foreach (QFileInfo file, fileList)////////////////////////遍历文件信息
    {
        if(file.isFile())// 是文件，删除
        {
            file.dir().remove(file.fileName());
        }else// 递归删除
        {
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

void DlgPicFile::FilePictureScan(QString path)//文件扫描
{
    QDir dir(path);
    QDir dir2;
    QString str1;
    int i=0;
    bool File_Flag=false;
    QStringList filter;//产生过滤器
    filter<<"*.jpg";//过滤出jpg文件
    dir2.setNameFilters(filter);//过滤
    foreach(QFileInfo mfi,dir.entryInfoList())
    {
        if(mfi.isFile())//如果是文件
        {
            qDebug()<< "It is File :" << mfi.fileName();
        }
        else//如果是文件夹
        {
            if((mfi.fileName()=="." )|| (mfi.fileName() == "..")||(mfi.fileName().length()!=8))continue;
            qDebug() << "It is FileFolder,the Path is" << mfi.absoluteFilePath();
            dir2.setPath(mfi.absoluteFilePath());
            File_Flag=false;
            foreach(QFileInfo mfi2,dir2.entryInfoList())
            {
                if(mfi2.isFile())//如果是文件
                {
                    //qDebug()<< "Picture File :" << mfi2.fileName();
                    PictureDetailStu[i].FolderPath=mfi.absoluteFilePath();
                    PictureDetailStu[i].FolderName=mfi.fileName();
                    PictureDetailStu[i].PicturePath=mfi2.absoluteFilePath();
                    PictureDetailStu[i].PictureName=mfi2.fileName();
                    str1="";
                    str1.append(PictureDetailStu[i].FolderName[0]);
                    str1.append(PictureDetailStu[i].FolderName[1]);
                    str1.append(PictureDetailStu[i].FolderName[2]);

                    m_FolderNo1=str1.toInt();
                    if(m_FolderNo1>m_FolderNo2)
                    {
                        m_FolderNo2=m_FolderNo1;
                        m_StrLastOldFolderName=PictureDetailStu[i].FolderName;
                        qDebug()<< "m_StrLastOldFolderName is :" << m_StrLastOldFolderName;
                    }

                    str1+="-";
                    str1.append(PictureDetailStu[i].PictureName[4]);
                    str1.append(PictureDetailStu[i].PictureName[5]);
                    str1.append(PictureDetailStu[i].PictureName[6]);
                    str1.append(PictureDetailStu[i].PictureName[7]);
                    PictureDetailStu[i].StringNo=str1;
                    //qDebug()<< "Picture StringNo :" << str1;
                    if(i<1000)
                    i++;
                    File_Flag=true;
                }else//如果是文件夹
                {
                    if((mfi2.fileName()==".") || (mfi2.fileName() == "..")||(mfi.fileName().length()!=8))continue;
                    qDebug() << "Entry Dir" << mfi2.absoluteFilePath();
                }
            }
            if(File_Flag==false)//如果没有文件
            {
                DelDir(mfi.absoluteFilePath());//删除该路径
            }
        }
    }
    g_PictureNum=i;

    g_PictureCurrentNo=g_PictureNum-1;
    g_RowNum=g_PictureCurrentNo%9/3;
    g_ColumnNum=g_PictureCurrentNo%9%3;

    m_PageFirstPicNum=(g_PictureCurrentNo/9)*9;
    LabelPictureShowPage(m_PageFirstPicNum);

    LabelHideBorder();//隐藏框
    LabelWhiteBorder();//显示白框

    qDebug()<< "PictureNum" << g_PictureNum;

    if(m_FirstScanFlag!=1)//第一次扫描以后创建文件夹
    {
        m_FirstScanFlag=1;

        QDateTime current_time = QDateTime::currentDateTime();//获取当前时间
        QString PowerOnTimeStr = current_time.toString("yy");//转成字符串
        PowerOnTimeStr = PowerOnTimeStr[1];
        PowerOnTimeStr = PowerOnTimeStr + current_time.toString("MMdd");//转成字符串 yMMdd
        qDebug()<< "CreatFolder PowerOnTimeStr.... :" << PowerOnTimeStr;

        str1="";
        str1.append(m_StrLastOldFolderName[3]);
        str1.append(m_StrLastOldFolderName[4]);
        str1.append(m_StrLastOldFolderName[5]);
        str1.append(m_StrLastOldFolderName[6]);
        str1.append(m_StrLastOldFolderName[7]);

        qDebug()<< "str1 is.... :" << str1;
        qDebug()<< "PowerOnTimeStr is.... :" << str1;

        if(str1!=PowerOnTimeStr)
        {
            if(m_FolderNo2>99)//DCIM下有文件夹
            {
                m_FolderNo2=m_FolderNo2+1;
            }
            else//DCIM下还完全没有文件夹
            {
                m_FolderNo2=100;
            }
            m_StrNewFolderName= QString::number(m_FolderNo2, 10); //数字转字符串
            m_StrNewFolderName=m_StrNewFolderName+PowerOnTimeStr; //三位编号+yMMdd
            m_StrNewFolderPath = "/media/mmcblk0p1/DCIM/"+m_StrNewFolderName+"/";
            CreatFolder();//创建文件夹
            emit SSendNewFolderPath(m_StrNewFolderPath);
            qDebug()<< "emit m_StrNewFolderPath  in dlgpicfile ";
            qDebug()<< "CreatFolder m_StrNewFolderPath is....:" << m_StrNewFolderPath;
        }
        else//文件夹已经存在
        {
            m_StrNewFolderPath = "/media/mmcblk0p1/DCIM/"+m_StrLastOldFolderName+"/";
            qDebug() << "New Folder already exist and the path is @dlgpicfile" << m_StrNewFolderPath;
            emit SSendNewFolderPath(m_StrNewFolderPath);
        }
    }
}

void DlgPicFile::CreatFolder()//第一次扫描文件后，创建文件夹
{
    QDir dir;
    if (!dir.exists(m_StrNewFolderPath))//如果目录不存在，再选择新建
    {
        bool res = dir.mkpath(m_StrNewFolderPath);
        qDebug() << "New Folder Make in dlgpicfile " << res;
        qDebug() << "New Folder Make in dlgpicfile name is " << m_StrNewFolderPath;       
    }else//如果目录存在
    {
        qDebug() << "New Folder already exist and the path is @dlgpicfile" << m_StrNewFolderPath;
    }
}

void DlgPicFile::SlotLabelShowOne(int LabelN,QImage SmallImage)
{
    QPixmap Pixmap = QPixmap::fromImage(SmallImage);
    switch(LabelN)
    {
        case 1:ui->Label1->clear();if((m_PageFirstPicNum+0) < g_PictureNum)ui->Label1->setPixmap(Pixmap);ui->Label1->show();break;
        case 2:ui->Label2->clear();if((m_PageFirstPicNum+1) < g_PictureNum)ui->Label2->setPixmap(Pixmap);ui->Label2->show();break;
        case 3:ui->Label3->clear();if((m_PageFirstPicNum+2) < g_PictureNum)ui->Label3->setPixmap(Pixmap);ui->Label3->show();break;
        case 4:ui->Label4->clear();if((m_PageFirstPicNum+3) < g_PictureNum)ui->Label4->setPixmap(Pixmap);ui->Label4->show();break;
        case 5:ui->Label5->clear();if((m_PageFirstPicNum+4) < g_PictureNum)ui->Label5->setPixmap(Pixmap);ui->Label5->show();break;
        case 6:ui->Label6->clear();if((m_PageFirstPicNum+5) < g_PictureNum)ui->Label6->setPixmap(Pixmap);ui->Label6->show();break;
        case 7:ui->Label7->clear();if((m_PageFirstPicNum+6) < g_PictureNum)ui->Label7->setPixmap(Pixmap);ui->Label7->show();break;
        case 8:ui->Label8->clear();if((m_PageFirstPicNum+7) < g_PictureNum)ui->Label8->setPixmap(Pixmap);ui->Label8->show();break;
        case 9:ui->Label9->clear();if((m_PageFirstPicNum+8) < g_PictureNum)ui->Label9->setPixmap(Pixmap);ui->Label9->show();break;
        default:break;
    }
}

void DlgPicFile::LabelShowBlackFirst(int LabelN)//在图片还没加载出来的时候先给控件显示一个图
{
    switch(LabelN)
    {
        case 1:ui->Label1->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label1->show();break;
        case 2:ui->Label2->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label2->show();break;
        case 3:ui->Label3->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label3->show();break;
        case 4:ui->Label4->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label4->show();break;
        case 5:ui->Label5->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label5->show();break;
        case 6:ui->Label6->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label6->show();break;
        case 7:ui->Label7->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label7->show();break;
        case 8:ui->Label8->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label8->show();break;
        case 9:ui->Label9->setPixmap(QPixmap(":/images/VideoPlayer_48.png"));ui->Label9->show();break;
        default:break;
    }
}

void DlgPicFile::LabelPictureShowOne(int LabelN, int PictureNo)//指定一个控件显示缩略图
{
    QString filePath;
    filePath=PictureDetailStu[PictureNo].PicturePath;
    qDebug()<< "Picture filePath is :" << filePath;
    if(filePath.isEmpty())
    {
        qDebug()<< "Picture filePath is Empty ";//为空不发送
    }
    else
    {
        LabelShowBlackFirst(LabelN);
        emit SToThreadFilePath(LabelN,filePath);
    }
}

void DlgPicFile::LabelClearAll(int No)//清空不需要显示的label
{
    if(g_PictureNum<(No+9))
        ui->Label9->clear();//清空
    if(g_PictureNum<(No+8))
        ui->Label8->clear();//清空
    if(g_PictureNum<(No+7))
        ui->Label7->clear();//清空
    if(g_PictureNum<(No+6))
        ui->Label6->clear();//清空
    if(g_PictureNum<(No+5))
        ui->Label5->clear();//清空
    if(g_PictureNum<(No+4))
        ui->Label4->clear();//清空
    if(g_PictureNum<(No+3))
        ui->Label3->clear();//清空
    if(g_PictureNum<(No+2))
        ui->Label2->clear();//清空
    if(g_PictureNum<(No+1))
        ui->Label1->clear();//清空
}

void DlgPicFile::LabelPictureShowPage(int No)//显示一页缩略图
{
    qDebug()<< "PictureShowPage Starting .......";
    emit SClearThread_i();
    if((No+8)<=g_PictureNum)
        LabelPictureShowOne(9, No+8);
    if((No+7)<=g_PictureNum)
        LabelPictureShowOne(8, No+7);
    if((No+6)<=g_PictureNum)
        LabelPictureShowOne(7, No+6);
    if((No+5)<=g_PictureNum)
        LabelPictureShowOne(6, No+5);
    if((No+4)<=g_PictureNum)
        LabelPictureShowOne(5, No+4);
    if((No+3)<=g_PictureNum)
        LabelPictureShowOne(4, No+3);
    if((No+2)<=g_PictureNum)
        LabelPictureShowOne(3, No+2);
    if((No+1)<=g_PictureNum)
        LabelPictureShowOne(2, No+1);
    if((No+0)<=g_PictureNum)
        LabelPictureShowOne(1, No+0);
    LabelClearAll(No);//清空不需要显示的label
}


void DlgPicFile::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "DlgPicFile pressed key is" << QKeySequence(event->modifiers() + event->key());
    emit SKeyPressed();
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            if(key_flg==true)
            {
                hide();
                SendShow();//根据路径显示图片;
            }
        }
        break;
        case Qt::Key_M:
        {
            if(key_flg==true)
            {
                hide();
                emit SShowMainWindow();
            }
        }
        break;
        case Qt::Key_Left:
        {
            if(key_flg==true)
            {
                KeyPressUp();//向上按钮子函数
            }
        }
        break;
        case Qt::Key_Right:
        {
            if(key_flg==true)
            {
                KeyPressDown();//向下按钮子函数
            }
        }
        break;
        case Qt::Key_T:
        {
        }
        break;
        case Qt::Key_P:  //录像
        {
        }
        break;
        case Qt::Key_C:  //Captrue
        {
                if(key_flg==true)
                {
                    hide();
                    emit SShowMainWindow();
                }
        }
        break;
        default:QWidget::keyReleaseEvent(event);break;
      }
      key_flg=false;
      PictureNoShowFresh();//编号刷新显示
}

void DlgPicFile::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "DlgPicFile Pressed Key is" << QKeySequence(event->modifiers() + event->key());
    key_flg=false;
    switch (event->key())
    {
        case Qt::Key_Return:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_M:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_Left:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_Right:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_T:
        {
            key_flg=true;
        }
        break;
        case Qt::Key_P://录像
        {
            key_flg=true;
        }
        break;
        case Qt::Key_C:  //Captrue
        {
            key_flg=true;
        }
        break;
        default:QWidget::keyPressEvent(event);break;
    }
}

void DlgPicFile::LabelHideBorder()//隐藏框
{
    ui->Label1->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label2->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label3->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label4->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label5->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label6->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label7->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label8->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
    ui->Label9->setStyleSheet("QLabel{border:5px solid rgb (0x00,0xff,0x00,0x00);}");
}

void DlgPicFile::LabelWhiteBorder()//根据行列，让该显示白框的显示白框
{
    int a[3][3]={0,1,2,3,4,5,6,7,8};
    int b=0;
    b=a[g_RowNum][g_ColumnNum];
    switch(b)
    {
        case 0:ui->Label1->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 1:ui->Label2->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 2:ui->Label3->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 3:ui->Label4->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 4:ui->Label5->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 5:ui->Label6->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 6:ui->Label7->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 7:ui->Label8->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        case 8:ui->Label9->setStyleSheet("QLabel{border:4px solid rgb(255, 255, 255);}");break;//Label显示周框 白色
        default:break;
    }
}







