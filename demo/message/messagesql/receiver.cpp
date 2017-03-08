#include <QBuffer>
#include "receiver.h"

Receiver::Receiver(QObject *parent)
    : QObject(parent)
{
}

Receiver::Receiver(const QString &address, const int &port, QObject *parent)
    : QObject(parent), m_host(address), m_port(port)
{
    m_socket.bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress);
    m_socket.joinMulticastGroup(m_host);
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Receiver::processPendingDatagrams()
{
    while (m_socket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket.pendingDatagramSize());
        m_socket.readDatagram(datagram.data(), datagram.size());
        QBuffer buffer(&datagram);
        buffer.open(QIODevice::ReadOnly);

        QDataStream in(&buffer);
        Message message;
        in >> message;
        buffer.close();
        emit signalNewMessage(message);
    }
}

void Receiver::connectSocket(const QString &address, const int &port)
{
    m_host = QHostAddress(address);
    m_port = port;
    m_socket.bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress);
    m_socket.joinMulticastGroup(m_host);
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}
