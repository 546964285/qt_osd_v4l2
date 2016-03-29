#ifndef QV4L2_H
#define QV4L2_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


#include <sys/ioctl.h>
#include <sys/mman.h>

#include <asm/types.h>
#include <linux/videodev2.h>
#include <media/davinci/dm365_ccdc.h>
#include <media/davinci/vpfe_capture.h>
#include <media/davinci/imp_previewer.h>
#include <media/davinci/imp_resizer.h>
#include <media/davinci/dm365_ipipe.h>

#include <QThread>
//#include <QEvent>

#define YEE_TABLE_FILE "EE_Table.txt"
#define CLEAR(x) memset(&(x), 0, sizeof(x))

class QV4l2
{
public:
    QV4l2();
    ~QV4l2();
    //bool open_device(const char *szDevName);
    bool open_device();
    bool init_device();
    bool init_mmap();

private:
    QString dev_name_capture;
    QString dev_name_rsz;
    QString dev_name_prev;
    int capture_fd;
    int resizer_fd;
    int preview_fd;
    unsigned long oper_mode_1;
    unsigned long user_mode_1;

    rsz_channel_config rsz_chan_config;
    rsz_continuous_config rsz_ctn_config;
    prev_channel_config prev_chan_config;
    prev_continuous_config prev_ctn_config;

    short yee_table[MAX_SIZE_YEE_LUT];
    int parse_yee_table(void);
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
};

#endif // QV4L2_H
