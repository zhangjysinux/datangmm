#ifndef P2PTRANSFERINTERFACE_H
#define P2PTRANSFERINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include "fileshare.h"
#include <QMutex>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusVariant>

#define UDP_PORT 77755
#define SEND_SIZE 512

class P2pTransferInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.p2pTransfer")

public:
    static P2pTransferInterface *getInstance();

signals:
    void signalFileReceivedPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalFileSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalConnectStateChanged(const QString &addr, int state);

private slots:
    void readPendingDatagrams();
    int sendMessage(QString msg);
    void onSendPacketData();
    void onReadSocket();
    void onReceivedDataRespond(PackageData pdata);

public slots:
    void onConnectToServer(QString ip);
    void onConnectToServer(QString ip, int port);
    void onSetPort(int port);
    int onSendFile(const QString &fileName);
    void initSocket(QString localIp, int port);

private:
    static P2pTransferInterface *instance;
    explicit P2pTransferInterface(QObject *parent = 0);
    //QDBusConnection m_dbusConnection;

    int m_port;
    QUdpSocket *m_udpSocketRead;
    QUdpSocket *m_udpSocketWrite;
    QHostAddress localAddress;
    QHostAddress remoteAddress;

    QMutex m_sendMutex;
    QFile m_sendingFile;
    QFile m_receivingfile;
    PackageData m_currentPackageData;
    int m_sentDataType; //0 data  1--message
    QTimer m_sendTimer;
    QTimer m_receivedTimer;
    QMap<QString,PackageData> m_mapReceivedDataList;
};

#endif // P2PTRANSFERINTERFACE_H
