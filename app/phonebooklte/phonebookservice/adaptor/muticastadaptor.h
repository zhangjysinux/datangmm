#ifndef MUTI_CAST_ADAPTOR_H_
#define MUTI_CAST_ADAPTOR_H_

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/groupips.h"
class MutiCastAdaptor : public QObject
{
    Q_OBJECT
public:
    static MutiCastAdaptor *getInstance();

private:
    explicit MutiCastAdaptor(QObject *parent = 0);
    static MutiCastAdaptor *instance;

public:
    int createMuticast(const QString &mcIp,int mcPort,int udpPort);
    int onCreateMuticast(int netType,const QString &mcIp,int mcPort,int udpPort,const QString &groupId);
    bool init(int groupId,QStringList groupIps);
    bool start(int groupId);
    bool sendData(int msgtype,int fileid,int groupId,const QString fileName,const QString path);
    void setRecivePath(int groupId,QString recPath);
    bool stop(int groupId);
    int GetGroupId(QString groupId);
public slots:
    void onRecvComplete(int msgtype, int fileid, int groupId, int recvNum, int allNum, QString recFileName, int bIsSuccesd);
    void onSendComplete(int msgtype,int fileid,int groupId,int sendNum,int allNum,QString sendFileName, int bIsSuccesd);
    void onSendFinished(int bIsSuccesd);
    void onRecvFinished(int bIsSuccesd);
signals:
    void signalRecvComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,QString recFileName, int bIsSuccesd);
    void signalSendComplete(int msgtype,int fileid,int groupId,int sendNum,int allNum,QString sendFileName, int bIsSuccesd);
    void singalSendFinished(int bIsSuccesd);
    void signalRecvFinished(int bIsSuccesd);
private:
    QDBusInterface m_interface;
};

#endif // MUTI_CAST_ADAPTOR_H_
