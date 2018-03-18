#ifndef QV4L2_H
#define QV4L2_H

#include <QThread>
#include <QEvent>

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#ifdef __cplusplus
};
#endif

const QEvent::Type V4l2Event = (QEvent::Type)5123;  //>1024
class QV4l2MsgData : public QObject
{
public:
    QV4l2MsgData(uchar *p)
    {
        pBuf = p;
    }

    uchar* pBuf;
};

class QV4l2Thread : public QThread
{
    Q_OBJECT

public:
    QV4l2Thread(QObject *hParentWnd, int hDev);

protected:
    void run();

private:
    int m_hDev;
    QObject *m_hParentWnd;
};

class Qv4l2
{
public:
    Qv4l2(QObject *hParentWnd);
    ~Qv4l2();
    bool open_device(const char *szDevName);
    bool init_device();
    void init_userptr();    
    bool init_mmap();

private:
    int m_hDev;
    QV4l2Thread *m_thread;
    QObject *m_hParentWnd;
    char m_szMsg[512];    
};

#endif // QV4L2_H
