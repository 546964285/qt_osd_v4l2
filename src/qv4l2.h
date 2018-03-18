#ifndef QV4L2_H
#define QV4L2_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <media/davinci/dm365_ccdc.h>
#include <media/davinci/vpfe_capture.h>
#include <media/davinci/imp_previewer.h>
#include <media/davinci/imp_resizer.h>
#include <media/davinci/dm365_ipipe.h>
#include <video/davincifb_ioctl.h>
#include <video/davinci_osd.h>

#include <QThread>
//#include <QEvent>

#define YEE_TABLE_FILE "EE_Table.txt"
#define CLEAR(x) memset(&(x), 0, sizeof(x))

class QV4l2
{
public:
    QV4l2();
    ~QV4l2();
    
    bool get_osd_nod();
    bool open_capture_device();
    bool init_capture_device();
    bool init_capture_mmap();
    bool open_display_device();
    bool init_display_device();
    bool init_display_mmap();
    bool open_osd1_device();
    bool init_osd1_device();
    bool init_osd1_mmap();
    bool blank_osd1();
    bool trans_osd1();
    bool open_osd0_device();
    bool init_osd0_device();
    bool start_loop();
    int stop_capture(int vid_win);
    void * get_display_buffer(int vid_win);
    int put_display_buffer(int vid_win, void *addr);

    int video0_capture();

private:

    struct buffer
    {
        void *start;
        size_t length;
    };
    buffer * capture_buffers;
    unsigned int g_imgBufCount;
    buffer *vid0Buf;
    //unsigned int index;
//    char *src_ptr;

    QString dev_name_capture;
    QString dev_name_rsz;
    QString dev_name_prev;
    QString dev_name_osd0;
    QString dev_name_osd1;
    QString dev_name_vid0;

    int capture_fd;
    int resizer_fd;
    int preview_fd;
    int osd0_fd;
    int osd1_fd;
    int vid0_fd;

    unsigned long oper_mode_1;
    unsigned long user_mode_1;

    rsz_channel_config rsz_chan_config;
    rsz_continuous_config rsz_ctn_config;
    prev_channel_config prev_chan_config;
    prev_continuous_config prev_ctn_config;

    v4l2_requestbuffers CapReqBufs;
    v4l2_requestbuffers VidReqBufs;

    fb_var_screeninfo osd0_varInfo;
    fb_var_screeninfo osd1_varInfo;

    fb_fix_screeninfo osd0_fixInfo;
    fb_fix_screeninfo osd1_fixInfo;

    short yee_table[MAX_SIZE_YEE_LUT];
    int parse_yee_table(void);
    int xioctl(int hDev, int nType, void *pData);

    char vbufferOut[20480];

    v4l2_buffer cap_buf;
};

class QV4l2Thread : public QThread
{
    Q_OBJECT

public:
    QV4l2Thread();
    ~QV4l2Thread();

protected:
    void run();

private:
    QV4l2 *pV4l2;

public slots:
    void blank_osd1();
    void trans_osd1();
    void video0_capture();

Q_SIGNALS:
    void capture_ok();
    void capture_fail();
};

#endif // QV4L2_H
