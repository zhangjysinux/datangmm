#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include "messagedata.h"

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(const QString &address, const int &port, QObject *parent = 0);
    qint64 sendMessage(const Message &message);
    int ttl() const;
    void setTtl(int ttl);

private:
    QHostAddress m_host;
    QUdpSocket m_socket;
    int m_port;
    int m_ttl;
};

#endif // SENDER_H
