#ifndef HBUTTON_H
#define HBUTTON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <QThread>


#define DEF_GPIO_DIR_OUT    0x01
#define DEF_GPIO_DIR_IN     0x06
#define DEF_GPIO_SET_DATA   0x03
#define DEF_GPIO_CLR_DATA   0x04
#define DEF_GPIO_GET_DATA   0x05

#define GPIO_BUTTON 26  // GIO26 is a non-interrupt GPIO pin

class QHButton
{
public:
    QHButton();
    ~QHButton();

    bool open_gpio();
    bool init_gpio();
    bool poll_gpio();
    bool close_gpio();

private:

    QString dev_name_button;

    int button_fd;

};

class QHButtonThread : public QThread
{
    Q_OBJECT

public:
    QHButtonThread();
    ~QHButtonThread();

protected:
    void run();

private:
    QHButton *pHButton;

    int button_key_pre;
    int button_key;
};

#endif // HBUTTON_H
