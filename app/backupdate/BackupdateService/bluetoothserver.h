#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QTimer>
#include <QMap>
#include <QFile>
#include "../../commondata/bluetoothdata.h"

class BluetoothServer : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothServer(QObject *parent = 0);
    ~BluetoothServer();

    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopServer();

public slots:
    qint64 onSendMessage(const BluetoothTransData &message);

signals:
    void messageReceived(const QString &sender, const BluetoothTransData &message);
    void clientConnected(const BluetoothAddressData &address);
    void clientDisconnected(const QString &name);
    void cancelTransmit();

private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onReadSocket();
    void onReceivedDataRespond();

private:
    QBluetoothServer *m_rfcommServer;
    QBluetoothServiceInfo m_serviceInfo;
    QList<QBluetoothSocket *> m_clientSockets;
    QMap<QBluetoothSocket*,IntPackageDataMap> m_mapReceivedDataList;
    QTimer m_receivedTimer;
    QFile m_receivingfile;

};

#endif // BLUETOOTHSERVER_H
