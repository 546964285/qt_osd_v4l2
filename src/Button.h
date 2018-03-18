#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT
public:
    Button(QWidget *parent);
    void setButtonPicture(QPixmap pic);
    void setPressPicture(QPixmap pic);
    void setReleasePicture(QPixmap pic);
    void setEnterPicture(QPixmap pic);
    void setLeavePicture(QPixmap pic);
    void set_X_Y_width_height(int x, int y, int width, int height);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent (QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);
//    void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void resizeit(int w , int h);

    QPixmap * buttonPicture;
    QPixmap * pressPicture;
    QPixmap * releasePicture;

    QPixmap * enterPicture;
    QPixmap * leavePicture;

    bool flag;
};

#endif
