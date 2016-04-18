#include <iostream>
#include <sys/ioctl.h>
#include <QCoreApplication>
#include "qhbutton.h"

QHButton::QHButton()
{
    this->dev_name_button = "/dev/leo368_leds"; // just use this name temporarily until demo had been finished

}

QHButton::~QHButton()
{

}

bool QHButton::open_gpio()
{
    button_fd = open(dev_name_button.toStdString().c_str(), O_RDWR);

    if(-1 == button_fd)
    {
        printf("open button failed\n");
        close(button_fd);
        return false;
    }

    return true;
}

bool QHButton::init_gpio()
{
    ioctl(button_fd, DEF_GPIO_SET_DATA, GPIO_BUTTON);
    ioctl(button_fd, DEF_GPIO_DIR_IN, GPIO_BUTTON);

    return true;
}

bool QHButton::poll_gpio()
{
    int ret;
    ret = ioctl(button_fd, DEF_GPIO_GET_DATA, GPIO_BUTTON);

    if(ret)
    {
//        std::cout << "oops" << std::endl;
    }
    else
    {
        std::cout << "read button pressed!" << std::endl;
    }

    return true;
}

bool QHButton::close_gpio()
{
    close(button_fd);
}


QHButtonThread::QHButtonThread()
{

}

QHButtonThread::~QHButtonThread()
{

}

void QHButtonThread::run()
{
    std::cout << "thread 3 running" << std::endl;
    pHButton = new QHButton();
    pHButton->open_gpio();
    while(1)
    {
        msleep(10);
        pHButton->poll_gpio();


    }
}
