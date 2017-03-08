#include <QBuffer>
#include "sender.h"

Sender::Sender(QObject *parent)
    : QObject(parent)
{
}

Sender::Sender(const QString &address, const int &port, QObject *parent)
    : QObject(parent),
      m_host(address), m_port(port), m_ttl(6)
{
    m_socket.setSocketOption(QAbstractSocket::MulticastTtlOption, m_ttl);
}

qint64 Sender::sendMessage(const Message &message)
{
    QByteArray data;
    data.resize(sizeof(Message));

    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);

    QDataStream out(&buffer);
    out << message;
    buffer.close();

    return m_socket.writeDatagram(buffer.data(), m_host, m_port);
}

void Sender::setSocket(const QString &address, const int &port)
{
    m_host = QHostAddress(address);
    m_port = port;
}

int Sender::ttl() const
{
    return m_ttl;
}

void Sender::setTtl(int ttl)
{
    m_ttl = ttl;
    m_socket.setSocketOption(QAbstractSocket::MulticastTtlOption, m_ttl);
}
