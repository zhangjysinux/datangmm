#ifndef BLUETOOTHINTERFACE2_H
#define BLUETOOTHINTERFACE2_H

#include <QObject>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include "./datatype/bluetoothdata.h"
#include <QDBusVariant>
#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QDBusInterface>
#include <QMap>
#include "bluetoothserver2.h"
#include "bluetoothclient2.h"
/*client to send message, server just receive message*/
class BluetoothInterface2 : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.bluetooth")
public:
    static BluetoothInterface2* getInstance();

public slots:
    BluetoothAddressData onGetLocalDeviceInfo();
    bool onIsBluetoothOpened();
    bool onOpenBluetooth();
    bool onCloseBluetooth();
    bool onStartFindRemoteBluetooth();
    void onPairing(const BluetoothAddressData &address);
    void onPairingConfirmation(const bool &confirm);
    bool onConnect(const BluetoothAddressData &address);//ret 0 success else fail
    bool onDisconnect();

    qint64 onSendMessage(const BluetoothTransData &data);
    qint64 onSendFile(const QString &fileName);
    bool onCancelSend();

    bool onRegisterAppid(const QString &appid, const BluetoothAppidReceiveDataCallback &callback);
    bool onUnregisterAppid(const QString &appid);

signals:
    void signalDeviceDiscovered(QDBusVariant address);
    void signalPairConfirmation(QDBusVariant address, QString pin);
    void signalPairFinished(QDBusVariant address, int pairing);//arg: the address
    void signalConnectServerFinished(QDBusVariant address);
    void signalConnectServerFailed(const int &error);
    void signalClientConnected(QDBusVariant address);

    void signalMessageReceived(QDBusVariant message);
    void signalMessageSended(const int &sendStatus);
    void signalFileSendPercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);
    void signalFileReceivePercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);

private slots:
    //bluetooth device
    bool startFindRemoteBluetooth();
    void onDiscovered(const QBluetoothDeviceInfo &info);
    void onDiscoveryFinished();
    void onRepeatConnect();

    //pairing
    void onPairingNeedInputPinCode(const QString &pin);
    void onPairingDisplayConfirmation(const QBluetoothAddress & address, const QString &pin);
    void onPairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing);

    //connected
    void onConnectServerFinished(const BluetoothAddressData &address);
    void onConnectServerError(QBluetoothSocket::SocketError error);
    void onClientConnected(const BluetoothAddressData &address);

    //appid callback
    bool onAppidCallback(const BluetoothTransData &message);

    //
    void onFileSendPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize);
    void onFileReceivedPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize);


private:
    static BluetoothInterface2* m_instance;
    QBluetoothLocalDevice m_bluetoothLocalDevice;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceDiscoveryAgent *m_discoveryServiceAgent;
    int m_queryTimes;
    QString m_currentBluetoothAddress;

    BluetoothServer2 m_bluetoothServer;
    BluetoothClient2 m_bluetoothClient;

    QMap<QString,BluetoothAppidReceiveDataCallback> m_appidCallbackMap;
    QList<QString> m_bluetoothList;
    BluetoothAddressData m_connectingAddress;
    QTimer m_repeatConnectTimer;
private:
    explicit BluetoothInterface2(QObject *parent = 0);
//    bool isFindRemoteBluetoothFinished();
    bool isDevicePaired(const BluetoothAddressData &address);
    //receive messages
    void emitMessageReceived(const QString &sender, const BluetoothTransData &message);
};

#endif // BLUETOOTHINTERFACE2_H
