#include "sbox4x4plugin.h"
#include "sbox4x4handler.h"

Sbox4x4Plugin::Sbox4x4Plugin()
{

}

QWSKeyboardHandler *Sbox4x4Plugin::create(const QString &driverName,
                                          const QString &deviceName)
{
    if(driverName.toLower() == "sbox4x4")
        return new Sbox4x4Handler(deviceName);

    return NULL;
}

QStringList Sbox4x4Plugin::keys() const
{
    return QStringList() << "sbox4x4";
}

Q_EXPORT_PLUGIN2(sbox4x4, Sbox4x4Plugin);
