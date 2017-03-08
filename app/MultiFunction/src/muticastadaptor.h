#ifndef MUTI_CAST_ADAPTOR_H_
#define MUTI_CAST_ADAPTOR_H_

#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/groupips.h"
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
    bool init(int groupId,QStringList groupIps);
    bool start(int groupId);
    bool sendData(int groupId,const QString fileName,const QString path);
    void setRecivePath(int groupId,QString recPath);
    bool stop(int groupId);
    int GetGroupId(QString groupId);
public slots:
    void onRecvComplete(int groupId,int recvNum,int allNum, QString recFileName);
    void onSendComplete(int groupId,int sendNum,int allNum);
signals:
    void signalRecvComplete(int groupId,int recvNum,int allNum,QString recFileName);
    void signalSendComplete(int groupId,int sendNum,int allNum);
    private:
    QDBusInterface m_interface;
};

#endif // MUTI_CAST_ADAPTOR_H_
