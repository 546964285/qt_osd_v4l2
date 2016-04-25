#ifndef SBOX4X4HANDLER_H
#define SBOX4X4HANDLER_H

#include "sbox4x4handler_global.h"
#include <QObject>
#include <QtGui/QWSKeyboardHandler>
#include <QSocketNotifier>

class SBOX4X4SHARED_EXPORT Sbox4x4Handler
    :public QObject, public QWSKeyboardHandler
{
    Q_OBJECT
public:
    Sbox4x4Handler(const QString &device =QString("/dev/button"),
                   QObject * parent = 0);
    ~Sbox4x4Handler();
private:
    int kbdFd;
    QSocketNotifier *notifier;
private slots:
    void kbdReadyRead();
};

#endif // SBOX4X4HANDLER_H
