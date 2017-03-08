#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include "messagedata.h"

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);
    explicit Receiver(const QString &address, const int &port, QObject *parent = 0);
    void connectSocket(const QString &address, const int &port);

signals:
    void signalNewMessage(Message message);

private slots:
    void processPendingDatagrams();

private:
    QHostAddress m_host;
    int m_port;
    QUdpSocket m_socket;
};

#endif // RECEIVER_H
