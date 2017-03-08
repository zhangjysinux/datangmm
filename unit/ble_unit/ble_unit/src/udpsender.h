#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>
#include <QUdpSocket>

class UdpSender : public QObject
{
    Q_OBJECT
public:
    static UdpSender *getInstance();

    Q_INVOKABLE void sendText(QString text, QString address);
private:
    explicit UdpSender(QObject *parent = 0);
    QUdpSocket *m_socket;
    static UdpSender *m_instance;
};

#endif // UDPSENDER_H
