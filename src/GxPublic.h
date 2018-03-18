#ifndef GXPUBLIC_H
#define GXPUBLIC_H

#include <QEvent>
#include <QObject>

const QEvent::Type GxMsgEvent = (QEvent::Type)5001;  //>1024
class QGxMsgData : public QObject
{
public:
    QGxMsgData(QString str = "", qint64 type = 0, qint64 val = 0)
    {
        nType = type;
        nVal = val;
        strMsg = str;
    }
    qint64 nType;
    qint64 nVal;
    QString strMsg;
};

extern QString g_strIniFile;
extern bool g_IsLineChanged; // 是否保存线图坐标


void GxInitPublic(QObject *hMainWind);
QString GxReadFromIni(QString strKey);
void GxWriteToIni(QString strKey, QString strVal);
void GxErrExit(const char* szErr);
void GxShowMsg(char* szMsg = NULL, qint64 type = 0, qint64 val = 0);











#endif // GXPUBLIC_H
