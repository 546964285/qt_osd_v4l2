#ifndef QGXIMAGE_H
#define QGXIMAGE_H

#include <QLabel>

class QGxImage : public QLabel
{
    Q_OBJECT
public:    
    explicit QGxImage(QWidget *parent=0, Qt::WindowFlags f=0);
    ~QGxImage();    

public:
    int GetFps();

private:
    QImage * m_pImage1=NULL;
    u_int32_t m_totalFrames, m_lastTotalFrames;

protected:
//    void mousePressEvent(QMouseEvent *ev);
//    void mouseMoveEvent(QMouseEvent *ev);
//    void mouseReleaseEvent(QMouseEvent *ev);
    void customEvent(QEvent *e);
    void paintEvent(QPaintEvent*);

};

#endif // QGXIMAGE_H
