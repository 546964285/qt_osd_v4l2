#include "GxPublic.h"
#include <QSettings>
#include <QCoreApplication>
#include <QMessageBox>

QString g_strIniFile;
bool g_IsLineChanged; // 是否保存线图坐标
QObject *m_hMainWind;//向主窗体发消息


void GxErrExit(const char* szErr)
{
    QMessageBox::information(NULL, QString("err"), QString(szErr));
    exit(EXIT_FAILURE);
}




void GxShowMsg(char* szMsg, qint64 type, qint64 val)
{
    QCoreApplication::postEvent(m_hMainWind, new QChildEvent(GxMsgEvent, new QGxMsgData(QString(szMsg), type, val)));
}

QString GxReadFromIni(QString strKey)
{
    QSettings *ConfigIni = new QSettings(g_strIniFile, QSettings::IniFormat);
    ConfigIni->setIniCodec("UTF-8");
    QString str = ConfigIni->value(strKey, "???").toString();
    delete ConfigIni;

    if(str == QObject::tr("???"))
    {
        QMessageBox::information(NULL, "Caption", QString::fromUtf8("读取INI文件 [%1] 失败!").arg(strKey));
        return QObject::tr("");
    }

    return str;
}

void GxWriteToIni(QString strKey, QString strVal)
{
    QSettings *ConfigIni = new QSettings(g_strIniFile, QSettings::IniFormat);
    ConfigIni->setIniCodec("UTF-8");
    if(ConfigIni->contains(strKey))
    {
        ConfigIni->setValue(strKey, strVal);
    }
    else
    {
        QMessageBox::information(NULL, "Caption", QString::fromUtf8("写INI文件 [%1] 失败!").arg(strKey));
    }
    delete ConfigIni;
}


void GxInitPublic(QObject *hMainWind)
{
    m_hMainWind = hMainWind;

    g_IsLineChanged = false;
    g_strIniFile = QObject::tr("%1/GXSetup.ini").arg(QCoreApplication::applicationDirPath());

    //QMessageBox::information(NULL, "Caption", GxReadFromIni(QString("pos/test")));
}

















