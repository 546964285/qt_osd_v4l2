#include <stdio.h>
#include <stdlib.h>         // exit
#include <fcntl.h>		// for open()
#include <unistd.h>     // for close()
#include <malloc.h>         // clloc
#include <getopt.h>		// getopt_long()
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <asm/types.h>		// for videodev2.h

#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <media/davinci/dm365_ccdc.h>
#include <media/davinci/vpfe_capture.h>
#include <media/davinci/imp_previewer.h>
#include <media/davinci/imp_resizer.h>
#include <media/davinci/dm365_ipipe.h>

#include <video/davincifb_ioctl.h>
#include <video/davinci_osd.h>

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>

#include <ti/sdo/dmai/ColorSpace.h>
#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Time.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/ce/Ienc1.h>

#include <iostream>
#include <QCoreApplication>
#include "qv4l2.h"

#include <QDebug>
//#include "qt.h"
extern "C" {
    #include "jpegenc.h"
}

using namespace std;

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
    this->dev_name_osd0 = "/dev/fb0";
    this->dev_name_osd1 = "/dev/fb2";
    this->dev_name_vid0 = "/dev/video2";
    this->capture_buffers = NULL;
    this->g_imgBufCount = 3;
    get_osd_nod();
    printf("in QV4l2 constructor\n");
}

QV4l2::~QV4l2()
{

}

int QV4l2::parse_yee_table(void)
{
        int ret = -1, val, i;
        FILE *fp;

        fp = fopen(YEE_TABLE_FILE, "r");
        if (fp == NULL) {
                printf("Error in opening file %s\n", YEE_TABLE_FILE);
                goto out;
        }

        for (i = 0; i < MAX_SIZE_YEE_LUT; i++) {
                fscanf(fp, "%d", &val);
                printf("%d,", val);
                yee_table[i] = val & 0x1FF;
        }
        printf("\n");
        if (i != MAX_SIZE_YEE_LUT)
                goto clean_file;
        ret = 0;
clean_file:
        fclose(fp);
out:
        return ret;
}

// 对指定设备进行特定操作, 返回0成功, -1失败;
int QV4l2::xioctl(int hDev, int nType, void *pData)
{
    int res;

    do
    {
       res = ioctl(hDev, nType, pData);
    }
    while(res == -1 && errno == EINTR); // errno == EINTR 被其它信号中断, 重新调用

    return res;
}

bool QV4l2::get_osd_nod()
{
    char d1[5];
    char d2[50];
    char * OSD1_DEVICE=NULL;
    FILE *fp;
    
    fp = fopen("/proc/fb","r");
    if(fp == NULL)
    {
        printf("Error in opening /proc/fb for reading\n");
        return false;
    }
    
    while( fscanf(fp, "%s",d1) != EOF)
    {
        fscanf(fp, "%s", d2);
        
        if(strcmp(d2, "dm_osd1_fb") == 0)
        {
            OSD1_DEVICE = (char *)malloc(10);
            strcpy(OSD1_DEVICE,"/dev/fb");
            strcat(OSD1_DEVICE,d1);
            this->dev_name_osd1=OSD1_DEVICE;
            break;
        }
    }
}

// 1.打开设备
bool QV4l2::open_capture_device()
{
    struct stat st;

    oper_mode_1 = IMP_MODE_CONTINUOUS;
    resizer_fd = open(dev_name_rsz.toStdString().c_str(), O_RDWR);
    if(-1 == resizer_fd)
    {
        printf("open resizer failed\n");
        close(resizer_fd);
        return -1;
    }

    if(-1 == ioctl(resizer_fd, RSZ_S_OPER_MODE, &oper_mode_1))
    {
        printf("setting default configuration failed\n");
        return -1;
    }

    if(-1 == ioctl(resizer_fd, RSZ_G_OPER_MODE, &oper_mode_1))
    {
        printf("getting default configuration failed\n");
        return -1;
    }

    if (oper_mode_1 == user_mode_1)
    {
        printf("RESIZER: Operating mode changed successfully to Continuous");
    }

    rsz_chan_config.oper_mode = IMP_MODE_CONTINUOUS;
    rsz_chan_config.chain = 1;
    rsz_chan_config.len = 0;
    rsz_chan_config.config = NULL; // to set defaults in driver

    if (-1 == ioctl(resizer_fd, RSZ_S_CONFIG, &rsz_chan_config))
    {
        printf("setting default configuration for continuous mode failed\n");
        return -1;
    }

    CLEAR (rsz_ctn_config);
    rsz_chan_config.oper_mode = IMP_MODE_CONTINUOUS;
    rsz_chan_config.chain = 1;
    rsz_chan_config.len = sizeof(struct rsz_continuous_config);
    rsz_chan_config.config = &rsz_ctn_config;
    if (-1 == ioctl(resizer_fd, RSZ_G_CONFIG, &rsz_chan_config))
    {
        printf("getting default configuration for continuous mode failed\n");
        return -1;
    }

    rsz_ctn_config.output1.enable = 1;
    rsz_ctn_config.output2.enable = 0;
    rsz_chan_config.oper_mode = IMP_MODE_CONTINUOUS;
    rsz_chan_config.chain = 1;
    rsz_chan_config.len = sizeof(struct rsz_continuous_config);
    rsz_chan_config.config = &rsz_ctn_config;
    if (-1 == ioctl(resizer_fd, RSZ_S_CONFIG, &rsz_chan_config))
    {
        printf("setting default configuration for continuous mode failed\n");
        return -1;
    }

    oper_mode_1 = IMP_MODE_CONTINUOUS; // same as resizer
    preview_fd = open(dev_name_prev.toStdString().c_str(), O_RDWR);
    if(-1 == preview_fd)
    {
        printf("open previewer failed\n");
        close(preview_fd);
        return -1;
    }

    if(-1 == ioctl(preview_fd, PREV_S_OPER_MODE, &oper_mode_1))
    {
        printf("Can't set operation mode\n");
        return -1;
    }

    if(-1 == (ioctl(preview_fd, PREV_G_OPER_MODE, &user_mode_1)))
    {
        printf("Can't get operation mode\n");
        return -1;
    }

    if (oper_mode_1 == user_mode_1)
    {
        printf("Operating mode changed successfully to continuous in previewer\n");
    }

    prev_chan_config.oper_mode = IMP_MODE_CONTINUOUS;
    prev_chan_config.len = 0;
    prev_chan_config.config = NULL; // to set defaults in driver
    if(-1 == ioctl(preview_fd, PREV_S_CONFIG, &prev_chan_config))
    {
        printf("Error in setting default configuration\n");
        return -1;
    }

    CLEAR (prev_ctn_config);
    prev_chan_config.oper_mode = IMP_MODE_CONTINUOUS;
    prev_chan_config.len = sizeof(struct prev_continuous_config);
    prev_chan_config.config = &prev_ctn_config;
    if(-1 == ioctl(preview_fd, PREV_G_CONFIG, &prev_chan_config))
    {
        printf("Error in getting configuration from driver\n");
        return -1;
    }

    prev_chan_config.oper_mode = IMP_MODE_CONTINUOUS;
    prev_chan_config.len = sizeof(struct prev_continuous_config);
    prev_chan_config.config = &prev_ctn_config;
    prev_ctn_config.input.colp_elep= IPIPE_BLUE;
    prev_ctn_config.input.colp_elop= IPIPE_GREEN_BLUE;
    prev_ctn_config.input.colp_olep= IPIPE_GREEN_RED;
    prev_ctn_config.input.colp_olop= IPIPE_RED;
    if(-1 == ioctl(preview_fd, PREV_S_CONFIG, &prev_chan_config))
    {
        printf("Error in setting default configuration\n");
        return -1;
    }

    struct prev_cap cap;
    struct prev_module_param mod_param;
    struct prev_wb wb;
    struct prev_lum_adj lum_adj;
    struct prev_gamma gamma;
    struct prev_yee yee;

    int ret;

    cap.index=0;
    while (1)
    {
        ret = ioctl(preview_fd , PREV_ENUM_CAP, &cap);
        if (ret < 0)
        {
            break;
        }

        // find the defaults for this module

        strcpy(mod_param.version,cap.version);
        mod_param.module_id = cap.module_id;
        // try set parameter for this module
        if(cap.module_id == PREV_WB)
        {
            printf("cap.module_id == PREV_WB\n");
            bzero((void *)&wb, sizeof (struct prev_wb));
            wb.gain_r.integer = 1;
            wb.gain_r.decimal = 0;
            wb.gain_gr.integer = 1;
            wb.gain_gr.decimal = 0;
            wb.gain_gb.integer = 1;
            wb.gain_gb.decimal = 0;
            wb.gain_b.integer = 1;
            wb.gain_b.decimal = 0;
            wb.ofst_r = 0;
            wb.ofst_gb = 0;
            wb.ofst_b = 0;
            mod_param.len = sizeof(struct prev_wb);
            mod_param.param = &wb;
                }
        else if(cap.module_id == PREV_LUM_ADJ)
        {
            printf("cap.module_id == PREV_LUM_ADJ\n");
            bzero((void *)&lum_adj, sizeof (struct prev_lum_adj));
                        lum_adj.brightness = 0;
                        lum_adj.contrast = 20;
                        mod_param.len = sizeof (struct prev_lum_adj);
                        mod_param.param = &lum_adj;
        }
        else if (cap.module_id == PREV_GAMMA)
        {
            printf("Setting gamma for %s\n", cap.module_name);
            bzero((void *)&gamma, sizeof (struct prev_gamma));
            gamma.bypass_r = 1;
            gamma.bypass_b = 1;
            gamma.bypass_g = 1;
            gamma.tbl_sel = IPIPE_GAMMA_TBL_RAM;
            gamma.tbl_size = IPIPE_GAMMA_TBL_SZ_512;
            mod_param.len = sizeof (struct prev_gamma);
            mod_param.param = &gamma;
        }
        else if (cap.module_id == PREV_YEE)
        {
            printf("Setting Edge Enhancement for %s\n", cap.module_name);
            bzero((void *)&yee, sizeof (struct prev_yee));
            bzero((void *)&yee_table, sizeof (struct prev_yee));
            yee.en = 1;
            //yee.en_halo_red = 1;
            yee.en_halo_red = 0;
            //yee.merge_meth = IPIPE_YEE_ABS_MAX;
            yee.merge_meth = IPIPE_YEE_EE_ES;
            yee.hpf_shft = 6; // 5, 10
            //yee.hpf_coef_00 = 8;
            //yee.hpf_coef_01 = 2;
            //yee.hpf_coef_02 = -2;
            //yee.hpf_coef_10 = 2;
            //yee.hpf_coef_11 = 0;
            //yee.hpf_coef_12 = -1;
            //yee.hpf_coef_20 = -2;
            //yee.hpf_coef_21 = -1;
            //yee.hpf_coef_22 = 0;
            yee.hpf_coef_00 = 84,
            yee.hpf_coef_01 = (-8 & 0x3FF),
            yee.hpf_coef_02 = (-4 & 0x3FF),
            yee.hpf_coef_10 = (-8 & 0x3FF),
            yee.hpf_coef_11 = (-4 & 0x3FF),
            yee.hpf_coef_12 = (-2 & 0x3FF),
            yee.hpf_coef_20 = (-4 & 0x3FF),
            yee.hpf_coef_21 = (-2 & 0x3FF),
            yee.hpf_coef_22 = (-1 & 0x3FF),
            yee.yee_thr = 20; //12
            yee.es_gain = 128;
            yee.es_thr1 = 768;
            yee.es_thr2 = 32;
            yee.es_gain_grad = 32;
            yee.es_ofst_grad = 0;
            if(parse_yee_table() <0)
            {
                printf("read yee table error.\n");
            }
            yee.table = yee_table;

            mod_param.len = sizeof (struct prev_yee);
            mod_param.param = &yee;
        }
        else
        {
            // using defaults
            printf("Setting default for %s\n", cap.module_name);
            mod_param.param = NULL;
        }

        if (ioctl(preview_fd, PREV_S_PARAM, &mod_param) < 0)
        {
            printf("Error in Setting %s params from driver\n", cap.module_name);
            close(preview_fd);
            //exit (EXIT_FAILURE);
        }


        cap.index++;
    }

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

    return true;
}

bool QV4l2::init_capture_device()
{
    struct v4l2_capability v4l2Cap;
    // 获取并分析v4l2设备属性
    if(xioctl(capture_fd, VIDIOC_QUERYCAP, &v4l2Cap) == -1)
    {
        printf("VIDIOC_QUERYCAP failed. %s is no a V4L2 device\n", dev_name_capture.toStdString().c_str());
        return false;
    }
    else
    {
        printf("VIDIOC_QUERYCAP ok. %s is a V4L2 device\n", dev_name_capture.toStdString().c_str());
        if(!(v4l2Cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        {
            printf("%s can't be captured.\n", dev_name_capture.toStdString().c_str());
            return false;
        }

//        if(!(v4l2Cap.capabilities & V4L2_CAP_READWRITE))
//        {
//            printf("%s can't be read.\n", dev_name_capture.toStdString().c_str());
//            return false;
//        }

        // 是否支持mmap 或 userptr
        if(!(v4l2Cap.capabilities & V4L2_CAP_STREAMING))
        {
            printf("%s can't streaming.\n", dev_name_capture.toStdString().c_str());
            return false;
        }
    }

    // 枚举支持的视频格式
    struct v4l2_fmtdesc fmtdest;
    fmtdest.index = 0;
    fmtdest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(xioctl(capture_fd, VIDIOC_ENUM_FMT, &fmtdest) == 0)
    {
        fmtdest.index++;
        char szFmt[5] = {0};
        memcpy(szFmt, &fmtdest.pixelformat, 4);
        printf("VIDIOC_ENUM_FMT = %s ", szFmt);

        // 列举视频大小
        struct v4l2_frmsizeenum frmSize;
        frmSize.index = 0;
        frmSize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        frmSize.pixel_format = fmtdest.pixelformat;
        while(xioctl(capture_fd, VIDIOC_ENUM_FRAMESIZES, &frmSize) == 0)
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
//    pFmt->pixelformat = V4L2_PIX_FMT_UYVY;
    pFmt->pixelformat = V4L2_PIX_FMT_NV12;
    pFmt->width = 384;
    pFmt->height = 384;
    if(xioctl(capture_fd, VIDIOC_S_FMT, &v4l2Fmt) == -1)
    {
        printf("VIDIOC_S_FMT failed.\n");
    }
    else
    {
        printf("VIDIOC_S_FMT ok!\n");
    }

    // 获取当前视频格式
    //struct v4l2_format v4l2Fmt;
    memset(&v4l2Fmt, 0, sizeof(v4l2Fmt));
    v4l2Fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(capture_fd, VIDIOC_G_FMT, &v4l2Fmt) == -1)
    {
        printf("VIDIOC_G_FMT failed.\n");
        return false;
    }
    else
    {
        char szFmt[5] = {0};
        memcpy(szFmt, &v4l2Fmt.fmt.pix.pixelformat, 4);
        struct v4l2_pix_format *pFmt = &v4l2Fmt.fmt.pix;
        printf("VIDIOC_G_FMT: %s, %d x %d, tytesperline = %d, size = %d, field = %d\n",
               szFmt, pFmt->width, pFmt->height, pFmt->bytesperline, pFmt->sizeimage, pFmt->field);
    }

//    // set frame-rate
//    struct v4l2_streamparm parm;
//    CLEAR (parm);
//    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    parm.parm.capture.timeperframe.numerator = 1;
//    parm.parm.capture.timeperframe.denominator = 60;
//    parm.parm.capture.capturemode = 0;
//    if (-1 == ioctl(capture_fd, VIDIOC_S_PARM, &parm))
//    {
//        printf("VIDIOC_S_PARM failed ");
//    }

//    if (-1 == ioctl(capture_fd, VIDIOC_G_PARM, &parm))
//    {
//        printf("VIDIOC_G_PARM failed\n");
//    }
//    else
//    {
//        printf("streamparm:\n\tnumerator =%d\n\tdenominator=%d\n\tcapturemode=%d\n\n",
//        parm.parm.capture.timeperframe.numerator,
//        parm.parm.capture.timeperframe.denominator,
//        parm.parm.capture.capturemode);
//    }

    return true;
}

bool QV4l2::init_capture_mmap()
{
    // 建立1个空的视频缓冲区队列,返回的v4l2ReqBufs.count是实际队列长度
    CLEAR (CapReqBufs);
    CapReqBufs.count = g_imgBufCount;
    CapReqBufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    CapReqBufs.memory = V4L2_MEMORY_MMAP;
    if(xioctl(capture_fd, VIDIOC_REQBUFS, &CapReqBufs) == -1)
    {
        printf("VIDIOC_REQBUFS failed.\n");
        return false;
    }
    else
    {
        g_imgBufCount = CapReqBufs.count;//取得实际队列长度
        printf("success: request %d buffers for capture\n", g_imgBufCount);
    }

    // 2 is minimum buffers
    if (CapReqBufs.count < 2)
    {
        printf("Insufficient buffer memory on %s\n", dev_name_capture.toStdString().c_str());
        return false;
    }

    capture_buffers = (buffer *) calloc (CapReqBufs.count, sizeof (*capture_buffers));

    if (!capture_buffers)
    {
        printf("fail: Out of memory for capture\n");
        return false;
    }

    // 在内核空间申请内存，并映射到用户空间
    for(unsigned int i = 0; i < CapReqBufs.count; i++)
    {
        struct v4l2_buffer buf;
        CLEAR (buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if(xioctl(capture_fd, VIDIOC_QUERYBUF, &buf) == -1)
        {
            printf("fail: in init_capture_buffers: VIDIOC_QUERYBUF\n");
            return false;
        }
        else
        {
            printf("success: query buffers for capture\n");
            printf("\tbuffer.length = %d\n",buf.length);
            printf("\tbuffer.bytesused = %d\n",buf.bytesused);
        }

        capture_buffers[i].length = buf.length;
        capture_buffers[i].start =
                mmap(NULL, // start anywhere
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     capture_fd, buf.m.offset);

        if(MAP_FAILED == capture_buffers[i].start)
        {
            printf("fail: mmap for capture buffers\n");
            return false;
        }
    }

    for(unsigned int i = 0; i < CapReqBufs.count; i++)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory =V4L2_MEMORY_MMAP;
        buf.index = i;

        if(-1 == ioctl(capture_fd, VIDIOC_QBUF, &buf))
        {
            printf("fail: test capture stream queue buffer %d\n", i);
            return false;
        }
        else
        {
            printf("success: test capture stream queue buffer %d\n", i);
        }
    }

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(-1 == ioctl(capture_fd, VIDIOC_STREAMON, &type))
    {
        printf("fail: capture stream on\n");
        return false;
    }
    else
    {
        printf("success: capture stream on\n\n");
    }

    return true;
}

bool QV4l2::open_display_device()
{
    vid0_fd = open(dev_name_vid0.toStdString().c_str(), O_RDWR);

    if (-1 == vid0_fd)
    {
        printf("fail: open VID0 display device\n");
        return false;
    }
    else
    {
        printf("seccess: open VID0 display device\n");
    }

    return true;
}

bool QV4l2::init_display_device()
{
    int ret = 0;
    v4l2_capability capability;

    ret = ioctl(vid0_fd, VIDIOC_QUERYCAP, &capability);
    if (ret < 0)
    {
        printf("fail: video out query capability\n");
        return false;
    }
    else
    {
        printf("seccess: video out query capability\n");
        if (capability.capabilities & V4L2_CAP_VIDEO_OUTPUT)
        {
            printf("\tDisplay capability is supported\n");
        }
        if (capability.capabilities & V4L2_CAP_STREAMING)
        {
            printf("\tStreaming is supported\n");
        }
    }

    // set display format
    v4l2_format setfmt;
    CLEAR(setfmt);
    setfmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
//    setfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
    setfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;
    setfmt.fmt.pix.width = 384;
    setfmt.fmt.pix.height = 384;
    setfmt.fmt.pix.field = V4L2_FIELD_NONE;

    ret = ioctl(vid0_fd, VIDIOC_S_FMT, &setfmt);
    if (ret < 0)
    {
        printf("fail: set format for display\n");
    }
    else
    {
        printf("success: set format for display\n");
    }

    // set display crop
    v4l2_crop crop;
    CLEAR(crop);

    crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    crop.c.height=384;
    crop.c.width=384;
    crop.c.top=48;
    crop.c.left=48;

    ret = ioctl(vid0_fd, VIDIOC_S_CROP, &crop);
    if (ret < 0)
    {
        printf("fail: set crop for display\n");
    }
    else
    {
        printf("success: set crop for display\n");
    }

    int disppitch, dispheight, dispwidth;
    v4l2_format fmt;
    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    ret = ioctl(vid0_fd, VIDIOC_G_FMT, &fmt);

    if (ret < 0)
    {
        printf("fail: get format for display\n");
    }
    else
    {
        printf("success: get format for display\n");
        dispheight = fmt.fmt.pix.height;
        disppitch = fmt.fmt.pix.bytesperline;
        dispwidth = fmt.fmt.pix.width;
        printf("\tdispheight = %d\n\tdisppitch = %d\n\tdispwidth = %d\n", dispheight, disppitch, dispwidth);
        printf("\timagesize = %d\n", fmt.fmt.pix.sizeimage);
    }

    return true;
}

bool QV4l2::init_display_mmap()
{
    int ret = 0;

    VidReqBufs.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    VidReqBufs.count = g_imgBufCount;
    VidReqBufs.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(vid0_fd, VIDIOC_REQBUFS, &VidReqBufs);
    if (ret < 0)
    {
        printf("fail: request buffers for display\n");
        return false;
    }
        else
    {
        printf("success: request buffers for display\n");
    }

    v4l2_buffer buf;

    vid0Buf = (buffer *)calloc(VidReqBufs.count, sizeof(struct buffer));
    if (!vid0Buf)
    {
        printf("fail: Out of memory for display\n");
        return false;
    }

    for(unsigned int i = 0; i < VidReqBufs.count; i++)
    {
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(vid0_fd, VIDIOC_QUERYBUF, &buf);
        if (ret < 0)
        {
            printf("fail: query buffer for display\n");
            return false;
        }
        else
        {
            printf("success: query buffer for display\n");
        }

        vid0Buf[i].length = buf.length;
        vid0Buf[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, vid0_fd, buf.m.offset);
        printf("buffer:%d phy:%x mmap:%p length:%d\n",
            buf.index,
            buf.m.offset,
            vid0Buf[i].start,
            buf.length);

        if (MAP_FAILED == vid0Buf[i].start)
        {
            printf("fail: mmap buffer for display\n");
        }
        else
        {
            printf("success: mmap buffer for display\n");
        }

        //memset(vid0Buf[i].start, 0x80, buf.length);
        memset(vid0Buf[i].start, 0x00, buf.length);
    }

    v4l2_buf_type type;
    for(unsigned int i = 0; i < VidReqBufs.count; i++)
    {
        v4l2_buffer buf;
        CLEAR (buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        ret = ioctl(vid0_fd, VIDIOC_QBUF, &buf);
        if(ret < 0)
        {
            printf("fail: test display stream queue buffer %d\n", i);
        }
        else
        {
            printf("success: test display stream queue buffer %d\n", i);
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    ret = ioctl(vid0_fd, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
        printf("fail: display stream on\n");
        return false;
    }
    else
    {
        printf("success: display stream on\n");
    }

    return true;
}

bool QV4l2::open_osd0_device()
{
    osd0_fd = open(dev_name_osd0.toStdString().c_str(), O_RDWR);
    if (-1 == osd0_fd)
    {
        printf("fail: open osd0 device\n");
        return false;
    }
    else
    {
        printf("seccess: open osd0 device\n");
    }

    return true;
}

bool QV4l2::init_osd0_device()
{
    vpbe_window_position_t pos;

    if (ioctl(osd0_fd, FBIOGET_FSCREENINFO, &osd0_fixInfo) < 0)
    {
        printf("\nFailed FBIOGET_FSCREENINFO osd0");
        return false;
    }
    else
    {
        printf("get default info:\n");
        printf("osd0_fixInfo.id=%s\n", osd0_fixInfo.id);
        printf("osd0_fixInfo.accel=%d\n", osd0_fixInfo.accel);
        printf("osd0_fixInfo.line_length=%d\n", osd0_fixInfo.line_length);
        printf("osd0_fixInfo.mmio_len=%d\n", osd0_fixInfo.mmio_len);
        printf("osd0_fixInfo.smem_len=%d\n", osd0_fixInfo.smem_len);
        printf("osd0_fixInfo.smem_start=0x%lx\n", osd0_fixInfo.smem_start);
        printf("osd0_fixInfo.type=%d\n", osd0_fixInfo.type);
        printf("osd0_fixInfo.type_aux=%d\n", osd0_fixInfo.type_aux);
        printf("osd0_fixInfo.visual=%d\n", osd0_fixInfo.visual);
        printf("osd0_fixInfo.xpanstep=%d\n", osd0_fixInfo.xpanstep);
        printf("osd0_fixInfo.ypanstep=%d\n", osd0_fixInfo.ypanstep);
        printf("osd0_fixInfo.ywrapstep=%d\n\n", osd0_fixInfo.ywrapstep);
    }

    // Get Existing var_screeninfo for osd0 window
    if (ioctl(osd0_fd, FBIOGET_VSCREENINFO, &osd0_varInfo) < 0)
    {
        printf("\nFailed FBIOGET_VSCREENINFO");
        return false;
    }
    else
    {
        printf("osd0_varInfo.xres=%d\n", osd0_varInfo.xres);
        printf("osd0_varInfo.yres=%d\n", osd0_varInfo.yres);
        printf("osd0_varInfo.xres_virtual=%d\n", osd0_varInfo.xres_virtual);
        printf("osd0_varInfo.yres_virtual=%d\n", osd0_varInfo.yres_virtual);
        printf("osd0_varInfo.xoffset=%d\n", osd0_varInfo.xoffset);
        printf("osd0_varInfo.yoffset=%d\n", osd0_varInfo.yoffset);
        printf("osd0_varInfo.accel_flags=%d\n", osd0_varInfo.accel_flags);
        printf("osd0_varInfo.activate=%d\n", osd0_varInfo.activate);
        printf("osd0_varInfo.bits_per_pixel=%d\n", osd0_varInfo.bits_per_pixel);
        printf("osd0_varInfo.grayscale=%d\n", osd0_varInfo.grayscale);
        printf("osd0_varInfo.height=%d\n", osd0_varInfo.height);
        printf("osd0_varInfo.hsync_len=%d\n", osd0_varInfo.hsync_len);
        printf("osd0_varInfo.left_margin=%d\n", osd0_varInfo.left_margin);
        printf("osd0_varInfo.lower_margin=%d\n", osd0_varInfo.lower_margin);
        printf("osd0_varInfo.nonstd=%d\n", osd0_varInfo.nonstd);
        printf("osd0_varInfo.pixclock=%d\n", osd0_varInfo.pixclock);
        printf("osd0_varInfo.right_margin=%d\n", osd0_varInfo.right_margin);
        printf("osd0_varInfo.rotate=%d\n", osd0_varInfo.rotate);
        printf("osd0_varInfo.sync=%d\n", osd0_varInfo.sync);
        printf("osd0_varInfo.upper_margin=%d\n", osd0_varInfo.upper_margin);
        printf("osd0_varInfo.vmode=%d\n", osd0_varInfo.vmode);
        printf("osd0_varInfo.vsync_len=%d\n", osd0_varInfo.vsync_len);
        printf("osd0_varInfo.width=%d\n", osd0_varInfo.width);
        printf("osd0_varInfo.xoffset=%d\n", osd0_varInfo.xoffset);
        printf("osd0_varInfo.xres=%d\n", osd0_varInfo.xres);
        printf("osd0_varInfo.xres_virtual=%d\n", osd0_varInfo.xres_virtual);
        printf("osd0_varInfo.yoffset=%d\n", osd0_varInfo.yoffset);
        printf("osd0_varInfo.yres=%d\n", osd0_varInfo.yres);
        printf("osd0_varInfo.yres_virtual=%d\n\n", osd0_varInfo.yres_virtual);
    }

//    // Modify the resolution and bpp as required
//    osd0_varInfo.xres = 480;
//    osd0_varInfo.yres = 640;
//    osd0_varInfo.bits_per_pixel = 16;
//    osd0_varInfo.vmode = FB_VMODE_NONINTERLACED;
//    osd0_varInfo.xres_virtual = 480;
//    // Change the virtual Y-resolution for buffer flipping (2 buffers)
//    osd0_varInfo.yres_virtual = osd0_varInfo.yres * 2;
//    osd0_varInfo.nonstd = 0;
//
//    // Set osd1 window format
//    if (ioctl(osd0_fd, FBIOPUT_VSCREENINFO, &osd0_varInfo) < 0)
//    {
//        printf("\nFailed FBIOPUT_VSCREENINFO for osd1");
//        return false;
//    };

    // Set window position
    pos.xpos = 0;
    pos.ypos = 0;

    if (ioctl(osd0_fd, FBIO_SETPOSX, &pos.xpos) < 0)
    {
        printf("\nFailed osd0 FBIO_SETPOSX\n\n");
        return false;
    }
    if (ioctl(osd0_fd, FBIO_SETPOSY, &pos.ypos) < 0)
    {
        printf("\nFailed osd0 FBIO_SETPOSY\n\n");
        return false;
    }

    // Enable the window
    if (ioctl(osd0_fd, FBIOBLANK, 0))
    {
        printf("Error enabling OSD0\n");
        return false;
    }

    return true;
}

// blend window control
bool QV4l2::open_osd1_device()
{
    osd1_fd = open(dev_name_osd1.toStdString().c_str(), O_RDWR);
    if (-1 == osd1_fd)
    {
        printf("fail: open osd1 device\n");
        return false;
    }
    else
    {
        printf("seccess: open osd1 device\n");
    }

    return true;
}

bool QV4l2::init_osd1_device()
{
    vpbe_window_position_t pos;

    if (ioctl(osd1_fd, FBIOGET_FSCREENINFO, &osd1_fixInfo) < 0)
    {
        printf("\nFailed FBIOGET_FSCREENINFO osd1");
        return false;
    }
    else
    {
        printf("get default info:\n");
        printf("osd1_fixInfo.id=%s\n", osd1_fixInfo.id);
        printf("osd1_fixInfo.accel=%d\n", osd1_fixInfo.accel);
        printf("osd1_fixInfo.line_length=%d\n", osd1_fixInfo.line_length);
        printf("osd1_fixInfo.mmio_len=%d\n", osd1_fixInfo.mmio_len);
        printf("osd1_fixInfo.smem_len=%d\n", osd1_fixInfo.smem_len);
        printf("osd1_fixInfo.smem_start=0x%lx\n", osd1_fixInfo.smem_start);
        printf("osd1_fixInfo.type=%d\n", osd1_fixInfo.type);
        printf("osd1_fixInfo.type_aux=%d\n", osd1_fixInfo.type_aux);
        printf("osd1_fixInfo.visual=%d\n", osd1_fixInfo.visual);
        printf("osd1_fixInfo.xpanstep=%d\n", osd1_fixInfo.xpanstep);
        printf("osd1_fixInfo.ypanstep=%d\n", osd1_fixInfo.ypanstep);
        printf("osd1_fixInfo.ywrapstep=%d\n\n", osd1_fixInfo.ywrapstep);
    }

    // Get Existing var_screeninfo for osd0 window
    if (ioctl(osd1_fd, FBIOGET_VSCREENINFO, &osd1_varInfo) < 0)
    {
        printf("\nFailed FBIOGET_VSCREENINFO");
        return false;
    }
    else
    {
        printf("osd1_varInfo.xres=%d\n", osd1_varInfo.xres);
        printf("osd1_varInfo.yres=%d\n", osd1_varInfo.yres);
        printf("osd1_varInfo.xres_virtual=%d\n", osd1_varInfo.xres_virtual);
        printf("osd1_varInfo.yres_virtual=%d\n", osd1_varInfo.yres_virtual);
        printf("osd1_varInfo.xoffset=%d\n", osd1_varInfo.xoffset);
        printf("osd1_varInfo.yoffset=%d\n", osd1_varInfo.yoffset);
        printf("osd1_varInfo.accel_flags=%d\n", osd1_varInfo.accel_flags);
        printf("osd1_varInfo.activate=%d\n", osd1_varInfo.activate);
        printf("osd1_varInfo.bits_per_pixel=%d\n", osd1_varInfo.bits_per_pixel);
        printf("osd1_varInfo.grayscale=%d\n", osd1_varInfo.grayscale);
        printf("osd1_varInfo.height=%d\n", osd1_varInfo.height);
        printf("osd1_varInfo.hsync_len=%d\n", osd1_varInfo.hsync_len);
        printf("osd1_varInfo.left_margin=%d\n", osd1_varInfo.left_margin);
        printf("osd1_varInfo.lower_margin=%d\n", osd1_varInfo.lower_margin);
        printf("osd1_varInfo.nonstd=%d\n", osd1_varInfo.nonstd);
        printf("osd1_varInfo.pixclock=%d\n", osd1_varInfo.pixclock);
        printf("osd1_varInfo.right_margin=%d\n", osd1_varInfo.right_margin);
        printf("osd1_varInfo.rotate=%d\n", osd1_varInfo.rotate);
        printf("osd1_varInfo.sync=%d\n", osd1_varInfo.sync);
        printf("osd1_varInfo.upper_margin=%d\n", osd1_varInfo.upper_margin);
        printf("osd1_varInfo.vmode=%d\n", osd1_varInfo.vmode);
        printf("osd1_varInfo.vsync_len=%d\n", osd1_varInfo.vsync_len);
        printf("osd1_varInfo.width=%d\n", osd1_varInfo.width);
        printf("osd1_varInfo.xoffset=%d\n", osd1_varInfo.xoffset);
        printf("osd1_varInfo.xres=%d\n", osd1_varInfo.xres);
        printf("osd1_varInfo.xres_virtual=%d\n", osd1_varInfo.xres_virtual);
        printf("osd1_varInfo.yoffset=%d\n", osd1_varInfo.yoffset);
        printf("osd1_varInfo.yres=%d\n", osd1_varInfo.yres);
        printf("osd1_varInfo.yres_virtual=%d\n\n", osd1_varInfo.yres_virtual);
    }

//    test_data.osd1_bpp = 4;
//    test_data.osd1_xpos = 0;
//    test_data.osd1_xpos = 0;
//    test_data.osd1_width = 480;
//    test_data.osd1_height = 640;
//    test_data.osd1_vmode = FB_VMODE_NONINTERLACED;
//    test_data.osd1_coloridx = BLUE_COLOR;
//
//    // Modify the resolution and bpp as required
//    osd1_varInfo.xres = 480;
//    osd1_varInfo.yres = 640;
//    osd1_varInfo.bits_per_pixel = 4;
//    osd1_varInfo.vmode = FB_VMODE_NONINTERLACED;
//    osd1_varInfo.xres_virtual = 480;
//    // Change the virtual Y-resolution for buffer flipping (2 buffers)
//    osd1_varInfo.yres_virtual = osd1_varInfo.yres * 2;
//    osd1_varInfo.nonstd = 1;
//
//    // Set osd1 window format
//    if (ioctl(osd1_fd, FBIOPUT_VSCREENINFO, &osd1_varInfo) < 0)
//    {
//        printf("\nFailed FBIOPUT_VSCREENINFO for osd1");
//        return false;
//    }

    // Set window position
    pos.xpos = 0;
    pos.ypos = 0;

    if (ioctl(osd1_fd, FBIO_SETPOSX, &pos.xpos) < 0)
    {
        printf("\nFailed osd1 FBIO_SETPOSX\n\n");
        return false;
    }
    if (ioctl(osd1_fd, FBIO_SETPOSY, &pos.ypos) < 0)
    {
        printf("\nFailed osd1 FBIO_SETPOSY\n\n");
        return false;
    }

    // Enable the window
    if (ioctl(osd1_fd, FBIOBLANK, 0))
    {
        printf("Error enabling OSD1\n");
        return false;
    }

    return true;
}

bool QV4l2::init_osd1_mmap()
{
    char *osd1buff = NULL;
    int memsize;

    memsize = ((osd1_fixInfo.line_length*osd1_varInfo.yres));
    osd1buff = (char *) mmap(NULL,memsize,PROT_READ|PROT_WRITE,MAP_SHARED,osd1_fd,0);

    if (osd1buff == MAP_FAILED)
    {
        printf("mmap failed\n");
        return false;
    }

    memset(osd1buff,0x00,memsize);

    munmap(osd1buff,memsize);

    return true;
}

bool QV4l2::blank_osd1()
{
    char *osd1buff = NULL;
    int memsize;
    printf("QV4l2::blank_osd1()");

    memsize = ((osd1_fixInfo.line_length*osd1_varInfo.yres));
    osd1buff = (char *) mmap(NULL,memsize,PROT_READ|PROT_WRITE,MAP_SHARED,osd1_fd,0);

    if (osd1buff == MAP_FAILED)
    {
        printf("mmap failed\n");
        return false;
    }

    memset(osd1buff,0xff,memsize);

    munmap(osd1buff,memsize);

    return true;   
}

bool QV4l2::trans_osd1()
{
    char *osd1buff = NULL;
    int memsize;
    printf("QV4l2::blank_osd1()");

    memsize = ((osd1_fixInfo.line_length*osd1_varInfo.yres));
    osd1buff = (char *) mmap(NULL,memsize,PROT_READ|PROT_WRITE,MAP_SHARED,osd1_fd,0);

    if (osd1buff == MAP_FAILED)
    {
        printf("mmap failed\n");
        return false;
    }

    memset(osd1buff,0x00,memsize);

    munmap(osd1buff,memsize);

    return true;   
}

int QV4l2::video0_capture()
{
    qDebug()<<"video0_capture="<<endl;
    IMGENC1_Params          params      = Ienc1_Params_DEFAULT;
    IMGENC1_DynamicParams   dynParams   = Ienc1_DynamicParams_DEFAULT;
    BufferGfx_Attrs         gfxAttrs    = BufferGfx_Attrs_DEFAULT;
    Buffer_Attrs            bAttrs      = Buffer_Attrs_DEFAULT;
    Time_Attrs              tAttrs      = Time_Attrs_DEFAULT;
    Ienc1_Handle            hIe         = NULL;
    Engine_Handle           hEngine     = NULL;
    Time_Handle             hTime       = NULL;
    Buffer_Handle           hOutBuf     = NULL;
    Buffer_Handle           hInBuf      = NULL;
    int                     inBufSize,outBufSize;
    FILE                   *outFile     = NULL;
    int                     ret         = Dmai_EOK;

    qDebug()<<"Starting capture"<<endl;

    CERuntime_init();
    Dmai_init();

    QDateTime time = QDateTime::currentDateTime();
    QString out_file=QString("DICM"+time.toString("yyMMddhhmmss")+".jpg");

    outFile = fopen(out_file.toStdString().c_str(),"wb");

    if(outFile == NULL)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to open output file"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"open output file ok"<<endl;
    }

    if(setvbuf(outFile,vbufferOut,_IOFBF,sizeof(vbufferOut))!=0)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to setvbuf on output file descriptor"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"setvbuf on output file descriptor ok"<<endl;
    }

    hEngine = Engine_open("encode", NULL, NULL);

    if(hEngine == NULL)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to open codec engine"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"open codec engine ok"<<endl;
    }

    qDebug()<<"Using output color format YUV420P"<<endl;
    params.maxWidth=384;
    params.maxHeight=384;
    params.forceChromaFormat=XDM_YUV_420P;

    dynParams.inputWidth=params.maxWidth;
    dynParams.inputHeight=params.maxHeight;
    dynParams.captureWidth=params.maxWidth;
    dynParams.qValue=75;
    qDebug()<<"Using input color format YUV420SP"<<endl;
    dynParams.inputChromaFormat=XDM_YUV_420SP;

    hIe = Ienc1_create(hEngine,"jpegenc",&params,&dynParams);

    if(hIe == NULL)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to create image encoder"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"create image encoder ok"<<endl;
    }

    gfxAttrs.bAttrs.memParams.align = bAttrs.memParams.align = 128;
    gfxAttrs.dim.width = 384;
    gfxAttrs.dim.height = 384;
    gfxAttrs.dim.lineLength = BufferGfx_calcLineLength(384,ColorSpace_YUV420PSEMI);
    gfxAttrs.colorSpace = ColorSpace_YUV420PSEMI;


    inBufSize = Ienc1_getInBufSize(hIe);

    hInBuf = Buffer_create(Dmai_roundUp(inBufSize,128),BufferGfx_getBufferAttrs(&gfxAttrs));

    if(hInBuf == NULL)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to create contiguous input buffer"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"create contiguous input buffer ok"<<endl;
    }

    outBufSize = Ienc1_getOutBufSize(hIe);

    hOutBuf = Buffer_create(Dmai_roundUp(outBufSize,128),&bAttrs);

    if(outBufSize == NULL)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to create contiguous input buffer"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"create contiguous input buffer ok"<<endl;
    }

    qDebug()<<"Reading input..."<<endl;

    if((readFrame(hInBuf,capture_buffers[cap_buf.index].start))==-1)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to read frame data to input buffer"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"Read frame data to input buffer ok"<<endl;
    }

    csayhello("what's up?");

    qDebug()<<"Encoding image..."<<endl;
    if(Ienc1_process(hIe,hInBuf,hOutBuf)<0)
    {
        ret = Dmai_EFAIL;
        qDebug()<<"Failed to encode image buffer"<<endl;
        // TODO: handle cleanup
    }
    else
    {
        qDebug()<<"Encode image buffer OK"<<endl;
    }

    if(Buffer_getNumBytesUsed(hOutBuf))
    {
        if(fwrite(Buffer_getUserPtr(hOutBuf),Buffer_getNumBytesUsed(hOutBuf),1,outFile)!=1)
        {
            ret = Dmai_EFAIL;
            qDebug()<<"Failed to write encoded image data to file"<<endl;
            // TODO: handle cleanup
        }
        else
        {
            qDebug()<<"Write encoded image data to file ok"<<endl;
        }
    }

    Buffer_delete(hOutBuf);
    Buffer_delete(hInBuf);
    Ienc1_delete(hIe);
    Engine_close(hEngine);
    fclose(outFile);

    qDebug()<<"Capture done!"<<endl;
    return 0;
}

bool QV4l2::start_loop()
{
//    v4l2_buffer buf;
    char *displaybuffer = NULL;
    char *src = NULL;
    char *dest = NULL;
//    char *dst = NULL;
    int i;
    int ret;

    while(1)
    {
        CLEAR(cap_buf);
        cap_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        cap_buf.memory = V4L2_MEMORY_MMAP;

        // determine ready buffer
        if (-1 == ioctl(capture_fd, VIDIOC_DQBUF, &cap_buf))
        {
            if (EAGAIN == errno)
                continue;
            printf("StartCameraCaputre:ioctl:VIDIOC_DQBUF\n");
            return false;
        }

        displaybuffer = (char*)get_display_buffer(vid0_fd);
        if (NULL == displaybuffer)
        {
            printf("Error in getting the  display buffer:VID1\n");
            return false;
        }

        src = (char *)capture_buffers[cap_buf.index].start;
        //src_ptr = (char *)capture_buffers[buf.index].start;
        //dst = (char *)calloc(294912, sizeof(char));
        //printf("Opps!\n");
        //printf("displaybuffer=%p\n",displaybuffer);
        dest = displaybuffer;

        for(i=0 ; i < 576; i++)
        {
            memcpy(dest, src, 384);
            src += 384;
            dest += 384;
        }

        //free(dst);
        //printf("displaybuffer=%p\n",displaybuffer);
        ret = put_display_buffer(vid0_fd, displaybuffer);
        //printf("Opps!!!\n");
        //display_bitmap_osd1();

        if (-1 == ioctl(capture_fd, VIDIOC_QBUF, &cap_buf))
        {
            printf("StartCameraCaputre:ioctl:VIDIOC_QBUF\n");
        }

//        printf("Opps!!\n");
    }
    ret = stop_capture(capture_fd);
    if (ret < 0)
        printf("Error in VIDIOC_STREAMOFF:capture\n");
}

void * QV4l2::get_display_buffer(int vid_win)
{
        int ret;
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        ret = ioctl(vid_win, VIDIOC_DQBUF, &buf);
        if (ret < 0) {
                perror("VIDIOC_DQBUF\n");
                return NULL;
        }
        //printf("index = %d, addr=%p\n", buf.index, vid0Buf[buf.index].start);
        return vid0Buf[buf.index].start;
}

int QV4l2::put_display_buffer(int vid_win, void *addr)
{
        struct v4l2_buffer buf;
        int i, index = 0;
        int ret;
        if (addr == NULL)
                return -1;
        memset(&buf, 0, sizeof(buf));

        for (i = 0; i < 3; i++)
        {
            if (addr == vid0Buf[i].start)
            {
                index = i;
                //printf("found out same index\n");
                //printf("index = %d, addr=%p, start=%p\n", i, addr, vid0Buf[i].start);
                break;
            }
            else
            {
                //printf("could not find same index\n");
                //printf("index = %d, addr=%p, start=%p\n", i, addr, vid0Buf[i].start);
            }
        }
        //printf("index=%d\n",index);
        //printf("Opps!!\n");
        buf.m.offset = (unsigned long)addr;
        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = index;
//printf("in put_display_buffer();\n");
        ret = ioctl(vid_win, VIDIOC_QBUF, &buf);
        return ret;
}

int QV4l2::stop_capture(int vid_win)
{
        int ret;
        enum v4l2_buf_type type;
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ret = ioctl(vid_win, VIDIOC_STREAMOFF, &type);
        return ret;
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
    pV4l2->open_capture_device();
    pV4l2->init_capture_device();
    pV4l2->init_capture_mmap();
    pV4l2->open_osd0_device();
    pV4l2->init_osd0_device();
    pV4l2->open_osd1_device();
    pV4l2->init_osd1_device();
    pV4l2->init_osd1_mmap();
    pV4l2->open_display_device();
    pV4l2->init_display_device();
    pV4l2->init_display_mmap();
    pV4l2->start_loop();
}

void QV4l2Thread::blank_osd1()
{
    pV4l2->blank_osd1();
}

void QV4l2Thread::trans_osd1()
{
    pV4l2->trans_osd1();
}

void QV4l2Thread::video0_capture()
{
    int ret;
    ret = pV4l2->video0_capture();
    if(ret == 0)
    {
        emit capture_ok();
    }
    else
    {
        emit capture_fail();
    }
}
