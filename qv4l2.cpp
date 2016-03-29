#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>         // exit
#include <sys/ioctl.h>
#include <fcntl.h>          // io底层操作  打开设备 open
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <unistd.h>         // getpagesize
#include <malloc.h>         // clloc
#include <stdio.h>

#include <iostream>
#include <QCoreApplication>
#include "qv4l2.h"

// 0.构造函数
QV4l2::QV4l2()
{
    // 视频采集的I/O方式
    // READWRITE V4L2_CAP_READWRITE  直接使用read和write函数进行读写,用户空间和内核空间不断copy数据,效率最低
    // MMAP      V4L2_CAP_ASYNCIO    把内核空间内存映射到用户空间内存地址上操作,一直占用内核空间
    // USERPTR   V4L2_CAP_STREAMING  程序员在用户空间分配内存，由v4l2驱动直接把数据填充到指定内存中,这个最好

    this->dev_name_capture = "/dev/video0";
    this->dev_name_rsz = "/dev/davinci_resizer";
    this->dev_name_prev = "/dev/davinci_previewer";

    printf("in QV4l2 constructor\n");
}

QV4l2::~QV4l2()
{

}

// 1.打开设备
bool QV4l2::open_device()
{
    struct stat st;
    if(stat(dev_name_capture.toStdString().c_str(), &st) == -1)
    {
        printf("can't get capture device status!\n");
        return false;
    }
    else
    {
        printf("success getting capture device status!\n");
    }

    // 分析设备状态
    if(!S_ISCHR(st.st_mode))
    {
        printf("capture device status err!\n");
        return false;
    }
    else
    {
        printf("capture device status ok!\n");
    }

    // 如果非阻塞模式打开, 即使尚未捕获到信息, 驱动依然会把缓冲区(DQBUFF)里的东西返回给应用程序
    // 打开设备
    capture_fd = open(dev_name_capture.toStdString().c_str(), O_RDWR | O_NONBLOCK);
    if(capture_fd == -1)
    {
        printf("can't open capture device!\n");
        return false;
    }
    else
    {
        printf("success opening capture device!\n");
    }
}

QV4l2Thread::QV4l2Thread()
{

}

QV4l2Thread::~QV4l2Thread()
{

}

void QV4l2Thread::run()
{
    std::cout << "thread 2 running" << std::endl;
    pV4l2 = new QV4l2();
    pV4l2->open_device();


}

