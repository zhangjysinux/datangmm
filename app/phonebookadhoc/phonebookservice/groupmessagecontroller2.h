#ifndef GROUPMESSAGECONTROLLER2_H
#define GROUPMESSAGECONTROLLER2_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QMap>
#include <QDataStream>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "../../commondata/datatype/messagedata.h"
//#include "../../commondata/groupmessagetrans/MutiCast.h"
#include "adaptor/muticastadaptor.h"
#include "adaptor/groupmessagesenderadaptor.h"

class GroupMessageController2 : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    explicit GroupMessageController2(const QString &mcAddr, int mcPort, int udpPort, const QString &groupId, QObject *parent = 0);
    ~GroupMessageController2();
    void setGroupIps(const QStringList &ips);
    void setReceivePath(const QString &path);
    void start();
    qint64 sendMessage(const Message& message);

signals:
//    void signalNewMessage(const Message& message);
    void signalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void signalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

private slots:
    //bIsSuccesd 0: fail    bIsSuccesd 1:sending recving   bIsSuccesd 2: success    bIsSuccesd -1: start
    void onSignalRecvComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,QString recFileName, int bIsSuccesd);
    void onSignalSendComplete(int msgtype,int fileid,int groupId,int sendNum,int allNum,QString sendFileName, int bIsSuccesd);

private:
    QString m_groupAddr;
    int m_groupPort;
    int m_udpPort;
    QStringList m_ips;
    QString m_sendFileName;
    QString m_sendFilePath;
    MutiCastAdaptor *m_mutiCastAdaptor;
    QMap<QString, QString> m_fileNameGroupId;
    QMap<QString, QString> m_fileNameMessageId;
    int m_groupId;
    QString m_groupIdStr;
};

#endif // GROUPMESSAGECONTROLLER2_H
