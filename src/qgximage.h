#ifndef QGXIMAGE_H
#define QGXIMAGE_H

#include <QLabel>
#include "qv4l2.h"

#pragma pack(push, bfhead)
#pragma pack(1)
struct BITMAPFILEHEADER
{
        quint16    bfType;
        quint32    bfSize;
        quint16    bfReserved1;
        quint16    bfReserved2;
        quint32    bfOffBits;
};
struct BITMAPINFOHEADER
{
        quint32      biSize;
        qint32       biWidth;
        qint32       biHeight;
        quint16      biPlanes;
        quint16      biBitCount;
        quint32      biCompression;
        quint32      biSizeImage;
        qint32       biXPelsPerMeter;
        qint32       biYPelsPerMeter;
        quint32      biClrUsed;
        quint32      biClrImportant;
};
#pragma pack(pop, bfhead)

struct TEST_RECT
{
    QPoint ptStar, ptEnd;				// 本检测区域左上角和右下角2个顶点
    QRect  rc;
    int rad;
    int MoveID;
//	int bufSize;						// 本检测区域24位图数据大小, 注意4字节对齐
//	Rect gdiRect;						// 本检测区域的RECT(gdi+)
//	CvRect cvRect;						// 本检测区域的RECT(OpenCV)
//	IplImage *pImgTmp;					// 本检测区域24位图
//	IplImage *pCurImg;					// 本检测区域 8位图
//	BYTE *pLastData;					// 保存上次本检测区域8位图
//	int nDirection;						// 本检测区域锁定抓拍方向=雷达返回的方向ID
//	WCHAR RoadDirection[64];			// 路口方向
//	BOOL IsTouched;						// 抓拍判定是否有效
//	int MoveID;							// 当前移动的点 -1表示没有 0表示ptStar  1表示ptEnd
//	int idx;							// 数字本检测区域序号
//	WCHAR szIdx[16];					// 字串本检测区域序号
//	WCHAR szType[16];					// 抓拍类型; 闯红灯 压黄线 超速 逆向行驶 卡口....
//	int idxFontSize;					// 线圈ID字体大小
//	int LineWidth;						// 线圈线的宽度
//	int rad;							// 线圈结点半径

//	// =========== 截图 ===========
//	Point PartLeftPt;					// 截图的左上点
//	int nPartWidth;						// 截图宽
//	int nPartHeight;					// 截图高
//	Bitmap *pBmpPart;					// 截图本身
//	Graphics *pGDIBmpPart;				// 截图DC
};

class QGxImage : public QLabel
{
    Q_OBJECT
public:    
    explicit QGxImage(QWidget *parent=0, Qt::WindowFlags f=0);
    ~QGxImage();    

public:
    void InitImage(u_int32_t nWidth, u_int32_t nHeight);
    void SaveRect();
    bool OpenV4l2(const char *szDevName);    
    int GetFps();
    Qv4l2 *m_pV4l2;

private:
    QImage *m_pImage1;
    uchar *m_bufTmp;
    uchar *m_aryBuf[5];
    QString m_strIniFile;
    int m_nBmpWidth, m_nBmpHeight, m_npBytesPerLine;
    int m_PanelWidth, m_PanelHeight;//窗口大小随时改变
    u_int32_t m_totalFrames, m_lastTotalFrames;
    //TEST_RECT m_testRC;
    //void UpdatePolyRect();

protected:
//    void mousePressEvent(QMouseEvent *ev);
//    void mouseMoveEvent(QMouseEvent *ev);
//    void mouseReleaseEvent(QMouseEvent *ev);
    void customEvent(QEvent *e);
    void paintEvent(QPaintEvent*);

};

#endif // QGXIMAGE_H
