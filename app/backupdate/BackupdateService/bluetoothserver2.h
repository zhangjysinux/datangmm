#ifndef BLUETOOTHSERVER2_H
#define BLUETOOTHSERVER2_H

#include <QObject>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QTimer>
#include <QMap>
#include <QFile>
#include "../../commondata/bluetoothdata.h"

class BluetoothServer2 : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothServer2(QObject *parent = 0);
    ~BluetoothServer2();

    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopServer();

public slots:
    qint64 onSendMessage(const BluetoothTransData &message);

signals:
    void messageReceived(const QString &sender, const BluetoothTransData &message);
    void clientConnected(const BluetoothAddressData &address);
    void clientDisconnected(const QString &name);
    void cancelTransmit();
    void fileReceivedPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize);

private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onReadSocket();
    void onReceivedDataRespond();

    //
//    void onFileReceivedPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize);

private:
    QBluetoothServer *m_rfcommServer;
    QBluetoothServiceInfo m_serviceInfo;
    QList<QBluetoothSocket *> m_clientSockets;
    QMap<QBluetoothSocket*,PackageData> m_mapReceivedDataList;
    QTimer m_receivedTimer;
    QFile m_receivingfile;
    QString m_lastUuid;
    int m_sendLastUuidCount;
//    PackageData m_currentPackageData;
};

#endif // BLUETOOTHSERVER2_H
