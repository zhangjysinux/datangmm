#ifndef P2PMESSAGESENDERTCPADAPTOR_H
#define P2PMESSAGESENDERTCPADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/messagedata.h"
#include "groupmessagesenderadaptor.h"

class P2PMessageSenderTcpAdaptor : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    static P2PMessageSenderTcpAdaptor *getInstance();
    ~P2PMessageSenderTcpAdaptor();
    qint64 sendMessage(const Message &message);

signals:
    void signalConnectFinished(const QString &id, const int &status);
    void signalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void signalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

//    void signalGroupMessageReceived(const Message &message);

public slots:
    void onSetPort(const int &port);

    int onConnect(const QString &id,
                  const QString &address);
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &id,
                   const QString &msgId,
                   const QString &fileName,
                   const int &msgType);

    int onSendMessage(const Message &message);

private slots:
    void onSignalConnectFinished(const QString &id, const int &status);
    void onSignalReceivePercent(const QString &id,
                                const QString &msgId,
                                const int &msgType,
                                const QString &fileName,
                                const qint64 &size,
                                const qint64 &maxSize);
    void onSignalSendPercent(const QString &id,
                             const QString &msgId,

                             const int &msgType,
                             const QString &fileName,
                             const qint64 &size,
                             const qint64 &maxSize);
private:
    explicit P2PMessageSenderTcpAdaptor(QObject *parent = 0);

private:
    static P2PMessageSenderTcpAdaptor *instance;
    QDBusInterface m_interface;
    QSet<QString> m_fileNameSet;
};

#endif // P2PMESSAGESENDERTCPADAPTOR_H
