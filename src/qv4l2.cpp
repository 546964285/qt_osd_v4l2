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
#include <GxPublic.h>
#include "qv4l2.h"
#include <QFile>
#include <QImage>
#include <QMessageBox>
#include <QCoreApplication>
#include "qgximage.h"

unsigned int g_imgBufCount = 4;
unsigned int g_imgBufSize = 0;
void** g_YUVBuf = NULL;
u_char** g_RGBBuf = NULL;
bool g_IsCaptrue = false;

AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;
AVFrame *frame_in;
AVFrame *frame_out;
unsigned char *frame_buffer_in;
unsigned char *frame_buffer_out;

int in_width;
int in_height;

const char *filters_descr = "crop=384:384:0:0";


void ShowErrMsg(const char *szErr)
{
    QMessageBox::information(NULL, QString("Qv4l2"), QString(szErr));
}

unsigned char clip(qint32 val)
{
    if(val > 255)
    {
        return 255;
    }
    else if(val > 0)
    {
        return val;
    }
    else
    {
        return 0;
    }
}

void process_image(QObject *hParentWnd, __u32 idx)
{
    short y1, y2, u, v;
    u_char *pYUV = (u_char*)g_YUVBuf[idx];
    u_char *p = g_RGBBuf[idx];

    for(unsigned int i = 0; i < g_imgBufSize / 4; i++)
    {
        y1 = *pYUV++ - 16;
        u = *pYUV++ - 128;
        y2 = *pYUV++ - 16;
        v = *pYUV++ - 128;

        *p++ = clip((298 * y1           + 409 * v + 128) >> 8);
        *p++ = clip((298 * y1 - 100 * u - 208 * v + 128) >> 8);
        *p++ = clip((298 * y1 + 516 * u           + 128) >> 8);
        *p++ = clip((298 * y2           + 409 * v + 128) >> 8);
        *p++ = clip((298 * y2 - 100 * u - 208 * v + 128) >> 8);
        *p++ = clip((298 * y2 + 516 * u           + 128) >> 8);
    }

    QCoreApplication::sendEvent(hParentWnd, new QChildEvent(V4l2Event, new QV4l2MsgData(g_RGBBuf[idx])));
}

// 对指定设备进行特定操作, 返回0成功, -1失败;
int xioctl(int hDev, int nType, void *pData)
{
    int res;

    do
    {
       res = ioctl(hDev, nType, pData);
    }
    while(res == -1 && errno == EINTR); // errno == EINTR 被其它信号中断, 重新调用

    return res;
}

//  取数据
int read_frame(QObject *hParentWnd, int hDev)
{
    int ret,i;
    void * src = NULL;
    struct v4l2_buffer v4l2Buf;
    memset(&v4l2Buf, 0, sizeof(v4l2Buf));
    v4l2Buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2Buf.memory = V4L2_MEMORY_MMAP;

    /*从视频缓冲区的输出队列中取得1个已经保存有一帧数据的buf*/
    if(xioctl(hDev, VIDIOC_DQBUF, &v4l2Buf) == -1)
    {
        ShowErrMsg("VIDIOC_DQBUF取视频数据时发生错误!");
        return false;
    }
    else
    {
        // sprintf(m_szMsg, "recv id =%d  length = %d", v4l2Buf.index, v4l2Buf.length);
        //GxShowMsg(m_szMsg);

        if(v4l2Buf.index >= g_imgBufCount)
        {
            ShowErrMsg("VIDIOC_DQBUF取视频数据时序号发生错误!");
            return false;
        }
    }

    src = (void *)g_YUVBuf[v4l2Buf.index];


    frame_in->data[0]=(uint8_t *)src;

    if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0) {
        printf( "Error while add frame.\n");
        return false;
    }

    /* pull filtered pictures from the filtergraph */
    ret = av_buffersink_get_frame(buffersink_ctx, frame_out);
    if (ret < 0)
    {
        printf( "Error while pull filtered pictures from the filtergraph.\n");
        return false;
    }

    //char* dst     = (char *)calloc(frame_out->width*frame_out->height*2, sizeof(char));
    memset(src, 0, v4l2Buf.length);
    if(frame_out->format==AV_PIX_FMT_YUYV422){
        for(i=0;i<frame_out->height;i++){
            //fwrite(frame_out->data[0]+frame_out->linesize[0]*i,1,frame_out->width,fp_out);
            memcpy(src,frame_out->data[0]+frame_out->linesize[0]*i,(frame_out->width)*2);
            src+=frame_out->width*2;
        }
    }
    //memset(src, 0, 640*480*2);
    //memcpy(src,dst,frame_out->width*frame_out->height*2);

    //printf("Process 1 frame!\n");
    av_frame_unref(frame_out);

    process_image(hParentWnd, v4l2Buf.index);

    if(xioctl(hDev, VIDIOC_QBUF, &v4l2Buf) == -1)
    {
        ShowErrMsg("VIDIOC_QBUF取完视频数据返回时发生错误!");
        return false;
    }

    return true;
}

//  4.接收循环
void recv_loop(QObject *hParentWnd, int hDev)
{
    while(g_IsCaptrue)
    {
        for(;;)
        {
            fd_set fds;
            struct timeval tv;
            FD_ZERO(&fds);
            FD_SET(hDev, &fds);
            tv.tv_sec = 2;
            tv.tv_usec = 0;
            int r = select(hDev + 1, &fds, NULL, NULL, &tv);
            if(r == -1)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    ShowErrMsg("select接收视频时发生错误");
                    return;
                }
            }
            else if(r == 0)
            {
                ShowErrMsg("select接收视频时超时");
                return;
            }
            else
            {
                if(read_frame(hParentWnd, hDev))
                    break;
            }
        }
    }
}


QV4l2Thread::QV4l2Thread(QObject *hParentWnd, int hDev)
{
    m_hDev = hDev;
    m_hParentWnd = hParentWnd;
}

void QV4l2Thread::run()
{
    recv_loop(m_hParentWnd,m_hDev);    
}

// 0.构造函数
Qv4l2::Qv4l2(QObject *hParentWnd)
{
    int ret,i;
    // 视频采集的I/O方式
    // READWRITE V4L2_CAP_READWRITE  直接使用read和write函数进行读写,用户空间和内核空间不断copy数据,效率最低
    // MMAP      V4L2_CAP_ASYNCIO    把内核空间内存映射到用户空间内存地址上操作,一直占用内核空间
    // USERPTR   V4L2_CAP_STREAMING  程序员在用户空间分配内存，由v4l2驱动直接把数据填充到指定内存中,这个最好

    //m_ioMethod = V4L2_CAP_STREAMING;

    m_hParentWnd = hParentWnd;

    in_width=640;
    in_height=480;

    avfilter_register_all();

    char args[512];

    AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    AVFilter *buffersink = avfilter_get_by_name("buffersink");
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();

    enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUYV422, AV_PIX_FMT_NONE };

    filter_graph = avfilter_graph_alloc();
    filter_graph = avfilter_graph_alloc();
    if (!outputs || !inputs || !filter_graph)
    {
        //ret = AVERROR(ENOMEM);
        //goto end;
        //ShowErrMsg("avfilter_init发生错误");
        printf("avfilter_init发生错误\n");
    }

    snprintf(args, sizeof(args),
            "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
            in_width,in_height,AV_PIX_FMT_YUYV422,
            1, 25,1,1);
    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                           args, NULL, filter_graph);
    if (ret < 0) {
        //av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
        //goto end;
        printf("Cannot create buffer source\n");
    }
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                           NULL, NULL, filter_graph);
    if (ret < 0) {
        //av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
        //goto end;
        printf("Cannot create buffer sink\n");
    }

    ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
                                  AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        //av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
        //goto end;
        printf("Cannot set output pixel format\n");
    }

    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
                                        &inputs, &outputs, NULL)) < 0)
    {
        printf("avfilter_graph_parse_ptr inputs fail\n");
    }

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
    {
        printf("avfilter_graph_parse_ptr 2 fail\n");
    }

    frame_in=av_frame_alloc();
    frame_buffer_in=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUYV422, in_width,in_height,1));
    av_image_fill_arrays(frame_in->data, frame_in->linesize,frame_buffer_in,
        AV_PIX_FMT_YUYV422,in_width, in_height,1);

    frame_out=av_frame_alloc();
    frame_buffer_out=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUYV422, in_width,in_height,1));
    av_image_fill_arrays(frame_out->data, frame_out->linesize,frame_buffer_out,
        AV_PIX_FMT_YUYV422,in_width, in_height,1);

    frame_in->width=in_width;
    frame_in->height=in_height;
    frame_in->format=AV_PIX_FMT_YUYV422;
}

Qv4l2::~Qv4l2()
{
    g_IsCaptrue = false;
    m_thread->wait();
    usleep(100);

    // 关闭视频采集 (v4l2驱动开始获取相机数据添加到缓冲区队列)
    enum v4l2_buf_type v4l2BufType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(m_hDev, VIDIOC_STREAMOFF, &v4l2BufType) == -1)
    {
        ShowErrMsg("mmp模式下关闭视频采集发生错误!");
        printf("mmp模式下关闭视频采集发生错误!");
        return;
    }

    for(u_int i = 0; i < g_imgBufCount; i++)
    {
        if(munmap(g_YUVBuf[i], g_imgBufSize) == -1)
        {
            printf("mmp模式下释放内存发生错误!");
            return;
        }
    }

    close(m_hDev);

    printf("mmp模式下释放内存ok!\n");
}

// 1.打开设备
bool Qv4l2::open_device(const char *szDevName)
{
    struct stat st;

    // 获取设备状态stat
    if(stat(szDevName, &st) == -1)
    {
        ShowErrMsg("获取设备状态stat失败!");
        return false;
    }

    // 分析设备状态
    if(!S_ISCHR(st.st_mode))
    {
        ShowErrMsg("设备不存在!");
        return false;
    }

    /*如果非阻塞模式打开, 即使尚未捕获到信息, 驱动依然会把缓冲区(DQBUFF)里的东西返回给应用程序*/
    // 打开设备
    m_hDev = open(szDevName, O_RDWR/*读写*/ | O_NONBLOCK/*非阻塞*/);/*Linux系统API*/
    if(m_hDev == -1)
    {
        ShowErrMsg("打开设备失败!");
        return false;
    }

    if(!init_device())
    {
        return false;
    }

    if(!init_mmap())
    {
        return false;
    }

    g_IsCaptrue = true;
    m_thread = new QV4l2Thread(m_hParentWnd ,m_hDev);
    m_thread->start();

    return true;
}

// 2.初始化设备
bool Qv4l2::init_device()
{
    struct v4l2_capability v4l2Cap;

    // 获取并分析v4l2设备属性
    if(xioctl(m_hDev, VIDIOC_QUERYCAP, &v4l2Cap) == -1)
    {
        if(errno == EINVAL)
        {
            ShowErrMsg("此视频设备不支持v4l2驱动!");
        }
        else
        {
            ShowErrMsg("未知原因无法与视频设备取得联系!");
        }
        return false;
    }
    else
    {
        if(!(v4l2Cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        {            
            ShowErrMsg("此视频设备没有提供采集功能!");
            return false;
        }

        //sprintf(m_szMsg, "此视频设备全部功能 = 0x%08X\n", v4l2Cap.capabilities);
        //GxShowMsg(m_szMsg);

        if(!(v4l2Cap.capabilities & V4L2_CAP_READWRITE))
        {
            printf("It can not be read\n");
        }
        else
        {
            printf("It can be read\n");
        }

        // 是否支持mmap 或 userptr
        if(!(v4l2Cap.capabilities & V4L2_CAP_STREAMING))
        {
            ShowErrMsg("此视频设备不支持mmap或userptr!");
            return false;
        }
    }

    // 图像的裁剪移位
    struct v4l2_cropcap cropcap;
    memset(&cropcap, 0, sizeof(cropcap));
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(m_hDev, VIDIOC_CROPCAP, &cropcap) == -1)
    {
        ShowErrMsg("此视频设备不支持cropcap!");
    }
    else
    {
        printf("查询cropcap成功!\n");
        printf("cropcap.type = %d!\n",cropcap.type);
        printf("cropcap.bounds.height = %d!\n",cropcap.bounds.height);
        printf("cropcap.bounds.width = %d!\n",cropcap.bounds.width);
        printf("cropcap.bounds.left = %d!\n",cropcap.bounds.left);
        printf("cropcap.bounds.top = %d!\n",cropcap.bounds.top);
        printf("cropcap.defrect.height = %d!\n",cropcap.defrect.height);
        printf("cropcap.defrect.width = %d!\n",cropcap.defrect.width);
        printf("cropcap.defrect.left = %d!\n",cropcap.defrect.left);
        printf("cropcap.defrect.top = %d!\n",cropcap.defrect.top);
    }


    // 枚举支持的视频格式
    struct v4l2_fmtdesc fmtdest;
    fmtdest.index = 0;
    fmtdest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(xioctl(m_hDev, VIDIOC_ENUM_FMT, &fmtdest) == 0)
    {
        fmtdest.index++;
        char szFmt[5] = {0};
        memcpy(szFmt, &fmtdest.pixelformat, 4);
        printf("获得视频格式列表 = %s ", szFmt);

        // 列举视频大小
        struct v4l2_frmsizeenum frmSize;
        frmSize.index = 0;
        frmSize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        frmSize.pixel_format = fmtdest.pixelformat;
        while(xioctl(m_hDev, VIDIOC_ENUM_FRAMESIZES, &frmSize) == 0)
        {
            frmSize.index++;
            printf("[%d x %d], ", frmSize.discrete.width, frmSize.discrete.height);
        }
        printf("\n");
    }

    // 设置视频格式
    struct v4l2_format v4l2Fmt;
    memset(&v4l2Fmt, 0, sizeof(v4l2Fmt));
    v4l2Fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    struct v4l2_pix_format *pFmt = &v4l2Fmt.fmt.pix;
    pFmt->pixelformat = V4L2_PIX_FMT_YUYV;
    pFmt->width = 640;
    pFmt->height = 480;
    if(xioctl(m_hDev, VIDIOC_S_FMT, &v4l2Fmt) == -1)
    {
        GxErrExit("设置视频格式失败!\n");
    }
    else
    {
        printf("设置视频格式成功!\n");
    }


    // 获取当前视频格式
    //struct v4l2_format v4l2Fmt;
    memset(&v4l2Fmt, 0, sizeof(v4l2Fmt));
    v4l2Fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(m_hDev, VIDIOC_G_FMT, &v4l2Fmt) == -1)
    {        
        ShowErrMsg("获取当前视频格式失败!");
        return false;
    }
    else
    {
        char szFmt[5] = {0};
        memcpy(szFmt, &v4l2Fmt.fmt.pix.pixelformat, 4);
        struct v4l2_pix_format *pFmt = &v4l2Fmt.fmt.pix;
        //sprintf(m_szMsg,"当前视频格式: %s, %d x %d, tytesperline = %d, size = %d, field = %d\n",
        //       szFmt, pFmt->width, pFmt->height, pFmt->bytesperline, pFmt->sizeimage, pFmt->field);
        //ShowErrMsg(m_szMsg);
        printf("当前视频格式: %s, %d x %d, tytesperline = %d, size = %d, field = %d\n",
                              szFmt, pFmt->width, pFmt->height, pFmt->bytesperline, pFmt->sizeimage, pFmt->field);
        ((QGxImage*)m_hParentWnd)->InitImage(pFmt->width, pFmt->height);
    }

    struct v4l2_streamparm parm;
    memset(&parm, 0, sizeof(parm));
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(m_hDev, VIDIOC_G_PARM, &parm) == -1)
    {
        ShowErrMsg("获取当前timeperframe失败!");
        return false;
    }
    else
    {
        printf("parm.parm.capture.timeperframe.numerator=%d\n",parm.parm.capture.timeperframe.numerator);
        printf("parm.parm.capture.timeperframe.denominator=%d\n",parm.parm.capture.timeperframe.denominator);
        printf("parm.parm.capture.capturemode=%d\n",parm.parm.capture.capturemode);
    }

//    memset(&parm, 0, sizeof(parm));
//    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    parm.parm.capture.timeperframe.numerator = 1;
//    parm.parm.capture.timeperframe.denominator = 60;
//    parm.parm.capture.capturemode = 0;
//    if(xioctl(m_hDev, VIDIOC_S_PARM, &parm) == -1)
//    {
//        ShowErrMsg("设置当前timeperframe失败!");
//        return false;
//    }
//    memset(&parm, 0, sizeof(parm));
//    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    if(xioctl(m_hDev, VIDIOC_G_PARM, &parm) == -1)
//    {
//        ShowErrMsg("获取当前timeperframe失败!");
//        return false;
//    }
//    else
//    {
//        printf("parm.parm.capture.timeperframe.numerator=%d\n",parm.parm.capture.timeperframe.numerator);
//        printf("parm.parm.capture.timeperframe.denominator=%d\n",parm.parm.capture.timeperframe.denominator);
//        printf("parm.parm.capture.capturemode=%d\n",parm.parm.capture.capturemode);
//    }

//    struct v4l2_crop crop;
//    memset(&crop, 0, sizeof(crop));
//    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    crop.c.width = 480;
//    crop.c.height = 480;
//    crop.c.top = 0;
//    crop.c.left = 0;
//    if(xioctl(m_hDev, VIDIOC_S_CROP, &crop) == -1)
//    {
//        ShowErrMsg("此视频设备设置crop失败!");
//    }
//    else
//    {
//        printf("此视频设备设置crop成功!\n");
//    }
//    if(xioctl(m_hDev, VIDIOC_G_CROP, &crop) == -1)
//    {
//        ShowErrMsg("此视频设备获取crop失败!");
//    }
//    else
//    {
//        printf("此视频设备获取crop成功!\n");
//    }

    return true;
}


// 3.1初始化mmap
bool Qv4l2::init_mmap()
{
    // 建立1个空的视频缓冲区队列,返回的v4l2ReqBufs.count是实际队列长度
    struct v4l2_requestbuffers v4l2ReqBufs;
    memset(&v4l2ReqBufs, 0, sizeof(v4l2ReqBufs));
    v4l2ReqBufs.count = g_imgBufCount;
    v4l2ReqBufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2ReqBufs.memory = V4L2_MEMORY_MMAP;
    if(xioctl(m_hDev, VIDIOC_REQBUFS, &v4l2ReqBufs) == -1)
    {
        ShowErrMsg("mmap模式下建立内存缓冲队列发生错误!");
        return false;
    }
    else
    {
        g_imgBufCount = v4l2ReqBufs.count;//取得实际队列长度
        //sprintf(m_szMsg, "成功分配%d块内核内存\n", g_imgBufCount);
        //GxShowMsg(m_szMsg);
    }

    // calloc分配的内存会自动清零
    g_YUVBuf = (void**)calloc(g_imgBufCount, sizeof(void*));

    // 在内核空间申请内存，并映射到用户空间
    for(unsigned int i = 0; i < g_imgBufCount; i++)
    {
        struct v4l2_buffer v4l2Buf;
        memset(&v4l2Buf, 0, sizeof(v4l2Buf));
        v4l2Buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2Buf.memory = V4L2_MEMORY_MMAP;
        v4l2Buf.index = i;

        if(xioctl(m_hDev, VIDIOC_QUERYBUF, &v4l2Buf) == -1)
        {            
            ShowErrMsg("mmap模式下在系统空间分配内存发生错误!");
            return false;
        }

        // 把申请完的内存映射到用户空间
        g_imgBufSize = v4l2Buf.length;
        g_YUVBuf[i] = mmap(NULL, v4l2Buf.length, PROT_READ | PROT_WRITE,
                                  MAP_SHARED, m_hDev, v4l2Buf.m.offset);

//        printf("g_imgBufSize=%d\n",g_imgBufSize);
        if(g_YUVBuf[i] == MAP_FAILED)
        {
            ShowErrMsg("mmap模式下系统空间内存映射到用户空间发生错误!");
            return false;
        }

        if(xioctl(m_hDev, VIDIOC_QBUF, &v4l2Buf) == -1)
        {
            ShowErrMsg("mmp模式下向队列添加内存发生错误!");
            return false;
        }
    }

    // 申请rgb内存
    g_RGBBuf = (u_char**)calloc(g_imgBufCount, sizeof(void*));
    u_int page_size = getpagesize();//4096
    u_int buf_size = g_imgBufSize * 3 / 2;//921600 = 614400 * 3 / 2;
    buf_size = (buf_size + page_size - 1) &~ (page_size - 1);
    for(u_int i = 0; i < g_imgBufCount; i++)
    {
        /*memalign 分配1个bufsize大小，pagesize倍数的内存块，pagesize必须是2的幂,多用来分配大内存*/
        /*malloc返回内存地址为8的倍数,64位系统则为16的倍数,memalign返回指定粒度 */
        g_RGBBuf[i] = (u_char*)memalign(page_size, buf_size);
    }

    // 启动视频采集 (v4l2驱动开始获取相机数据添加到缓冲区队列)
    enum v4l2_buf_type v4l2BufType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(m_hDev, VIDIOC_STREAMON, &v4l2BufType) == -1)
    {        
        ShowErrMsg("mmp模式下启动视频采集发生错误!");
        return false;
    }
    return true;
}

