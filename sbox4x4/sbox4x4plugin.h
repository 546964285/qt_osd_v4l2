#ifndef SBOX4X4PLUGIN_H
#define SBOX4X4PLUGIN_H

#include <QtGui/QKbdDriverPlugin>

class Sbox4x4Plugin : public QKbdDriverPlugin
{
    Q_OBJECT
public:
    Sbox4x4Plugin();
    QWSKeyboardHandler *create(const QString &driverName,
                               const QString &deviceName = QString("/dev/button"));

    QStringList keys() const;
};

#endif // SBOX4X4PLUGIN_H

