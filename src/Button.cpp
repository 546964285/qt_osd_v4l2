#include "Button.h"
#include <QPainter>

Button::Button(QWidget *parent) : QPushButton(parent)
{
        //保存图片成员初始化
        buttonPicture   =   new QPixmap();
        pressPicture    =   new QPixmap();
        releasePicture  =   new QPixmap();
        enterPicture    =   new QPixmap();
        leavePicture    =   new QPixmap();

        //关闭按钮的默认显示
        this -> setFlat(true);
        this->setFocusPolicy(Qt::NoFocus);

        flag=false;//初始化flag
}

void Button::setButtonPicture(QPixmap pic)
{
        *buttonPicture = pic;
        this -> setIcon(QIcon(*buttonPicture));
}

void Button::setPressPicture(QPixmap pic)
{
        *pressPicture = pic;
}

void Button::setReleasePicture(QPixmap pic)
{
        *releasePicture = pic;
}

void Button::setEnterPicture(QPixmap pic)
{
        *enterPicture = pic;
}

void Button::setLeavePicture(QPixmap pic)
{
        *leavePicture = pic;
}

void Button::set_X_Y_width_height(int x, int y, int width, int height)
{
        this -> setIconSize(QSize(width, height));
        this -> setGeometry(x, y, width, height);
}

void Button::mouseDoubleClickEvent(QMouseEvent *event)
{
        //null
}

void Button::mousePressEvent (QMouseEvent *event)
{
        this -> setIcon (QIcon(*pressPicture));
}

void Button::mouseMoveEvent(QMouseEvent *event)
{
        //null
}

void Button::mouseReleaseEvent (QMouseEvent *event)
{
        this -> setIcon(QIcon(*releasePicture));
        emit clicked();
}

void Button::enterEvent(QEvent *)
{
        this->setIcon(QIcon(*enterPicture));
        flag=true;
        //  this->resizeit();

}

void Button::leaveEvent(QEvent *)
{
        this->setIcon(QIcon(*leavePicture));
        flag=false;
}

void Button::resizeit(int w , int h)
{
        this->raise();
        this->resize(w,h);
        this ->setIconSize(QSize(w, h));
}



