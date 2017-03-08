#ifndef TRANSPONDMGR_H
#define TRANSPONDMGR_H

#include <QObject>
#include "connection.h"
#include "server.h"
#include "../../../commondata/datatype/messagedata.h"
#include "../sender.h"
#include <QtConcurrent/QtConcurrent>

typedef QList<Connection *> Connections;
class TranspondMgr : public QObject
{
    Q_OBJECT
public:
    explicit TranspondMgr(QObject *parent = 0);
    void createServer(quint16 port);

private slots:
    void onNewConnection(Connection *connection);
    void onDisconnected();
    void onConnectionError(QAbstractSocket::SocketError error);
    void onNewMsg(Message msg);
    void onNewPacket(PacketData packet);
    void onNewByteArray(QByteArray array);

    void onNewThread(QByteArray array);

signals:
    void signalNewServer(quint16 port);
    void signalServerMsg(quint16 port, QString msg);

private:
    quint16 findConnectionPort(Connection *connection);
    void removeConnection(Connection *connection);

    QHash<quint16, Connections> m_clientHash;
    QHash<quint16, Server*> m_serverHash;
};

#endif // TRANSPONDMGR_H
