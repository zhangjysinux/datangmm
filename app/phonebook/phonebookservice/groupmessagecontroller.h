#ifndef GROUPMESSAGECONTROLLER_H
#define GROUPMESSAGECONTROLLER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QMap>
#include <QDataStream>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "../../commondata/datatype/messagedata.h"

class GroupMessageController : public QObject
{
    Q_OBJECT
public:
    explicit GroupMessageController(QObject *parent = 0);
    qint64 sendMessage(const Message& message);
    void setPort(const int &port);

public slots:
    void onConnectServer();

signals:
    void signalNewMessage(const Message& message);

private slots:
    void onSocketReceived();
    void onSocketReceivedControll();
    void onSocketConnected();
    void onSocketDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onConnect();

    qint64 onSendMessage(const Message& message);

private:
    bool connect(const QString &ip, const int &port);

private:
    QTcpSocket *m_tcpSocket;
    QList<QTcpSocket*> m_tcpSocketList;
    QMap<int, QByteArray> m_packetMap;
    QQueue<QByteArray> m_queue;
    QMutex m_mutex;
    int m_port;
    bool m_isReceiving;
    bool m_isSending;
    QByteArray m_data;
};

#endif // GROUPMESSAGECONTROLLER_H
