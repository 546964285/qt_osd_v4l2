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
    void paintEvent(QPaintEvent *);
//    void paintEvent();

private:

protected:

};

#endif // QGXIMAGE_H
