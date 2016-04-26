#include "sbox4x4handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int keycode = 0;
int keycode_pre = 0;

Sbox4x4Handler::Sbox4x4Handler(const QString &device, QObject *parent)
    :QObject(parent), QWSKeyboardHandler()
{
    setObjectName("SAPP 4x4 Keyboard Handler");
    this->kbdFd = ::open(device.toAscii().constData(), O_RDONLY);
    if(this->kbdFd < 0)
        return;
    this->notifier = new QSocketNotifier(this->kbdFd, QSocketNotifier::Read, this);
    connect(this->notifier, SIGNAL(activated(int)), this, SLOT(kbdReadyRead()));
}

Sbox4x4Handler::~Sbox4x4Handler()
{
    if(this->kbdFd >= 0)
        ::close(this->kbdFd);
}

void Sbox4x4Handler::kbdReadyRead()
{
    char key;

    // 读取设备文件
    if(read(this->kbdFd, &key, sizeof(key)) != sizeof(key))
        return;

    Qt::KeyboardModifiers modifiers = Qt::NoModifier;

    // unicode用于保存映射之后的字符
    int unicode = 0x0000;

    // keycode用于保存映射之后的Qt::Key类型的键值
//    int keycode = 0;

    switch(key)
    {
        case '1':
            keycode = Qt::Key_Enter;
            //printf("keycode = Qt::Key_Enter\n");
            break; //可以选择不同的按键
        case '2':
            keycode = Qt::Key_Up;
            break;
        case '3':
            keycode = Qt::Key_Down;
            break;
        case '4':
            keycode = Qt::Key_Tab;
            break;
        default:
            keycode = 0;
            //this->processKeyEvent(unicode, keycode, modifiers, 0, false);
 //           printf("keycode = %d\n", keycode);
            //return;
    }

    //this->processKeyEvent(unicode, keycode, modifiers, 1, false);

    if(keycode == 0 && keycode_pre != 0)
    {
        this->processKeyEvent(unicode, keycode_pre, modifiers, 0, false);
    }
    // else
    if(keycode != 0 && keycode_pre == 0)
    {
        this->processKeyEvent(unicode, keycode, modifiers, 1, false);
    }

//    printf("keycode = %d, keycode_pre = %d\n", keycode, keycode_pre);

    keycode_pre = keycode;
}
