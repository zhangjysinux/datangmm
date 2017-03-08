#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QMap>
#include "../../commondata/datatype/messagedata.h"

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);
    void setSocket(QUdpSocket *socket, const QString &address);

signals:
    void signalNewMessage(Message message);

private slots:
    void processPendingDatagrams();

private:
    QHostAddress m_host;
    int m_port;
    QUdpSocket *m_socket;
    QMap<int, QByteArray> m_packetMap;
};

#endif // RECEIVER_H
