#ifndef P2PTRANSFERADAPTOR_H
#define P2PTRANSFERADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "groupmessagesenderadaptor.h"

class P2pTransferAdaptor : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    static P2pTransferAdaptor *getInstance();

    ~P2pTransferAdaptor();
    static P2pTransferAdaptor *instance;
    QDBusInterface m_interface;

    void setPort(const int &port);
    void initSocket(QString ip, int port);
    void connectToServer(QString ip);
    int sendFile(const QString &fileName);
    qint64 sendMessage(const Message &message);

private:
    explicit P2pTransferAdaptor(QObject *parent = 0);

signals:
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalConnectStateChanged(const QString &addr, int state);


public slots:

};

#endif // P2PTRANSFERADAPTOR_H
