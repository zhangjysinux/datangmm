#include "checkonline.h"
#include <QHostAddress>

CheckOnline::CheckOnline(QObject *parent) :
    QObject(parent),m_areadlyStart(false)
{
}

void CheckOnline::setCheckIpPort(const int &port)
{
    m_port = port;
}

void CheckOnline::addIp(const QString &ip)
{
    m_ipList.push_back(ip);
}

void CheckOnline::delIp(const QString &ip)
{
    m_ipList.removeAll(ip);
}

void CheckOnline::start()
{
    if(!m_ipList.isEmpty() && !m_areadlyStart)
        checkIpList();
}

void CheckOnline::checkIpList()
{
    if(!m_ipList.isEmpty())
        m_areadlyStart = true;

    foreach(const QString &ip, m_ipList)
    {
        QTcpSocket *socket = new QTcpSocket(this);
        m_ipStatusMap.clear();
        m_wantConnectClientList.push_back(socket);
        QHostAddress addr(ip);
        socket->connectToHost(addr, m_port);
        qDebug() << "socket->connectToHost" << addr << m_port;
        connect(socket, SIGNAL(connected()), this, SLOT(onConnectServerFinished()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onConnectServerError(QAbstractSocket::SocketError)));
    }
}

void CheckOnline::onConnectServerFinished()
{
    qDebug() << "onConnectServerFinished";
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        m_ipStatusMap.insert(socket->peerName(), true);
        m_wantConnectClientList.removeAll(socket);
    }

    if(m_wantConnectClientList.isEmpty())
    {
        emit signalCheckFinished(m_ipStatusMap);
        m_areadlyStart = false;
        checkIpList();
    }
}

void CheckOnline::onConnectServerError(QAbstractSocket::SocketError error)
{
    qDebug() << "onConnectServerError" << error;
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        if( socket->error() == QAbstractSocket::HostNotFoundError ||
            socket->error() == QAbstractSocket::NetworkError )
        {
            m_ipStatusMap.insert(socket->peerName(), false);
        }
        else
        {
            m_ipStatusMap.insert(socket->peerName(), true);
        }

        qDebug() << "onConnectServerError ip: "
                 << socket->peerAddress()
                 << socket->peerName()
                 << socket->peerPort();
        socket->deleteLater();

        m_wantConnectClientList.removeAll(socket);
        if(m_wantConnectClientList.isEmpty())
        {
            emit signalCheckFinished(m_ipStatusMap);
            m_areadlyStart = false;
            checkIpList();
        }
    }
}
