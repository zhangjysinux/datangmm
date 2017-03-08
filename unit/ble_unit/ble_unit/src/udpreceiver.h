#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    static UdpReceiver *getInstance();

signals:
    void signalReceive(QVariant text);

private slots:
    void onReadPendingDatagrams();

private:
    explicit UdpReceiver(QObject *parent = 0);

    QUdpSocket *m_socket;
    static UdpReceiver *m_instance;
};

#endif // UDPRECEIVER_H
