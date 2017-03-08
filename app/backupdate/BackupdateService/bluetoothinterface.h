#ifndef BLUETOOTHINTERFACE_H
#define BLUETOOTHINTERFACE_H

#include <QObject>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include "../../commondata/bluetoothdata.h"
#include <QDBusVariant>
#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QDBusInterface>
#include <QMap>
#include "bluetoothserver.h"
#include "bluetoothclient.h"
/*client to send message, server just receive message*/
class BluetoothInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.bluetooth")
public:
    static BluetoothInterface* getInstance();

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
    void signalMessagePercent(const double &percent);

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

    //receive messages
    void onMessageReceived(const QString &sender, const BluetoothTransData &message);

    //appid callback
    bool onAppidCallback(const BluetoothTransData &message);
private:
    static BluetoothInterface* m_instance;
    QBluetoothLocalDevice m_bluetoothLocalDevice;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceDiscoveryAgent *m_discoveryServiceAgent;
    int m_queryTimes;
    QString m_currentBluetoothAddress;

    BluetoothServer m_bluetoothServer;
    BluetoothClient m_bluetoothClient;

    QMap<QString,BluetoothAppidReceiveDataCallback> m_appidCallbackMap;
    QList<QString> m_bluetoothList;
    BluetoothAddressData m_connectingAddress;
    QTimer m_repeatConnectTimer;
private:
    explicit BluetoothInterface(QObject *parent = 0);
//    bool isFindRemoteBluetoothFinished();
    bool isDevicePaired(const BluetoothAddressData &address);
};

#endif // BLUETOOTHINTERFACE_H
