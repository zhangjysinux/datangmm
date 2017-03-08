#include "transpondmgr.h"

TranspondMgr::TranspondMgr(QObject *parent)
    : QObject(parent)
{

}

void TranspondMgr::createServer(quint16 port)
{
    if(m_serverHash.contains(port))
        return;

    emit signalNewServer(port);
    Server *server = new Server(port, this);
    if(server->isListening())
        emit signalServerMsg(port, tr("Server begin listen port number ") + QString::number(port) + "......");
    else
        emit signalServerMsg(port, tr("Server listen ") + QString::number(port) + " error.");

    if(!server->isListening())
    {
        server->deleteLater();
        return;
    }

    m_serverHash.insert(port, server);
    connect(server, SIGNAL(newConnection(Connection*)), this, SLOT(onNewConnection(Connection*)));
}

void TranspondMgr::onNewConnection(Connection *connection)
{
    QTcpServer *server = qobject_cast<QTcpServer *>(sender());
    if(server != NULL)
    {
        quint16 port = server->serverPort();
        QHostAddress address = connection->peerAddress();
        QString ip = address.toString();
        emit signalServerMsg(port, ip + tr(" enter....."));
        connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onConnectionError(QAbstractSocket::SocketError)));
        connect(connection, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        connect(connection, SIGNAL(signalNewMsg(Message)), this, SLOT(onNewMsg(Message)));
        connect(connection, SIGNAL(signalNewPacket(PacketData)), this, SLOT(onNewPacket(PacketData)));
        connect(connection, SIGNAL(signalNewByteArray(QByteArray)), this, SLOT(onNewByteArray(QByteArray)));
        if(!m_clientHash.contains(port))
        {
            Connections connections;
            m_clientHash.insert(port, connections);
        }
        m_clientHash[port].append(connection);
    }
}

void TranspondMgr::onDisconnected()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection != NULL)
        removeConnection(connection);
}

void TranspondMgr::onConnectionError(QAbstractSocket::SocketError error)
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection != NULL)
    {
        quint16 port = findConnectionPort(connection);
        if(port == 0)
            return;

        QHostAddress address = connection->peerAddress();
        QString ip = address.toString();
        emit signalServerMsg(port, ip + tr(" error code ") + QString::number((int)error));
        removeConnection(connection);
    }
}

void TranspondMgr::onNewMsg(Message msg)
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection != NULL)
    {
        quint16 port = findConnectionPort(connection);
        if(port == 0)
            return;

        QHostAddress address = connection->peerAddress();
        QString ip = address.toString();
        emit signalServerMsg(port, ip + tr(": send msg type ")
                             + QString::number(msg.common.type));
        emit signalServerMsg(port, ip + tr(": send msg content ")
                             + QString(msg.content));

        QByteArray array;
        QDataStream stream(&array, QIODevice::WriteOnly);
        stream << msg;

        Connections connections = m_clientHash[port];
        for(int i = 0, iend = connections.count(); i < iend; i++)
        {
            if(connections[i] == connection)
                continue;

            quint64 count = connections[i]->write(array);
            QHostAddress clientAddress = connections[i]->peerAddress();
            QString clientIp = clientAddress.toString();
            emit signalServerMsg(port, tr("transpond msg to ") + clientIp + " " +
                                 QString::number(count) + tr(" Bytes"));
        }
    }
}

void TranspondMgr::onNewPacket(PacketData packet)
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection != NULL)
    {
        quint16 port = findConnectionPort(connection);
        if(port == 0)
            return;

        QHostAddress address = connection->peerAddress();
        QString ip = address.toString();
        emit signalServerMsg(port, ip + tr(": send pakcet size ")
                             + QString::number(packet.size));
        QByteArray array;
        QDataStream stream(&array, QIODevice::WriteOnly);
        stream << packet;

        Connections connections = m_clientHash[port];
        for(int i = 0, iend = connections.count(); i < iend; i++)
        {
            if(connections[i] == connection)
                continue;

            quint64 count = connections[i]->write(array);
            QHostAddress clientAddress = connections[i]->peerAddress();
            QString clientIp = clientAddress.toString();
            emit signalServerMsg(port, tr("transpond packet to ") + clientIp + " "
                                 + QString::number(count) + tr(" Bytes"));
        }
    }
}

void TranspondMgr::onNewByteArray(QByteArray array)
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection != NULL)
    {
        quint16 port = findConnectionPort(connection);
        if(port == 0)
            return;

        Connections connections = m_clientHash[port];
        for(int i = 0, iend = connections.count(); i < iend; i++)
        {
            if(connections[i] == connection)
                continue;

            quint64 count = connections[i]->write(array);
            QHostAddress clientAddress = connections[i]->peerAddress();
            QString clientIp = clientAddress.toString();
            emit signalServerMsg(port, tr("transpond packet to ") + clientIp + " "
                                 + QString::number(count) + tr(" Bytes"));
        }
    }
}

void TranspondMgr::onNewThread(QByteArray array)
{
    QtConcurrent::run(this, &TranspondMgr::onNewByteArray,array);
}

quint16 TranspondMgr::findConnectionPort(Connection *connection)
{
    quint16 port = 0;
    QList<quint16> ports = m_clientHash.keys();
    for(int i = 0, iend = ports.count(); i < iend; i++)
    {
        if(m_clientHash[ports[i]].contains(connection))
            port = ports[i];
    }

    return port;
}

void TranspondMgr::removeConnection(Connection *connection)
{
    quint16 port = findConnectionPort(connection);
    if(port == 0)
        return;

    QHostAddress address = connection->peerAddress();
    QString ip = address.toString();
    emit signalServerMsg(port, ip + tr(" leave....."));
    m_clientHash[port].removeOne(connection);
    connection->deleteLater();
}
