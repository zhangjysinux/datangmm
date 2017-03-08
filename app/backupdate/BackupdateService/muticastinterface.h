#ifndef _MUTICAST_INTERFACE_H_
#define _MUTICAST_INTERFACE_H_
#include <QObject>
#include<vector>
#include <string>
#include "groupips.h"
#include <QDBusConnection>
#include <QDBusVariant>
#include <map>
class MuticastInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.muticast")
public:
    static MuticastInterface* getInstance();
private:
    explicit MuticastInterface(QObject *parent = 0);
public slots:
    int onCreateMuticast(const QString &mcIp,int mcPort,int udpPort);
    int onCreateMuticast(int netType,const QString &mcIp,int mcPort,int udpPort,const QString &groupId);
    bool onInit(int groupId,QStringList groupIps);
    bool onStart(int groupId);
    bool onSendData(int msgtype,int fileid,int groupId,const QString fileName,const QString path);
    void onSetRecivePath(int groupId,QString recPath);
    bool onStop(int groupId);
    int onGetGroupId(QString groupId);
    void onSendFinished(bool bIsSuccesd);
public slots:
    void onRecvComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,std::string recvFileName, int bIsSuccesd);
    void onSendComplete(int msgtype,int fileid,int groupId,int sendNum,int allNum,std::string sendFileName, int bIsSuccesd);
signals:
    void signalRecvComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,QString recvFileName, int bIsSuccesd);
    void signalSendComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,QString sendFileName, int bIsSuccesd);
    void singalSendFinished(bool bIsSuccesd);
private:
    static MuticastInterface* instance;
    QDBusConnection m_dbusConnection;
    std::map<int,QString> m_mapGroupIds;
};
#endif
