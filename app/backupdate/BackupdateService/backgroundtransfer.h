#ifndef BACKGROUNDTRANSFER_H
#define BACKGROUNDTRANSFER_H

#include <QObject>
#include "../../commondata/bluetoothdata.h"
#include <QDBusInterface>
#include <QDBusVariant>
#include "../../commondata/datatype/contacterdata.h"
//#include "CallBTService.h"

class BackgroundTransfer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.backgroundtransfer")
public:
    static BackgroundTransfer* getInstance();
public slots:
//    BluetoothAddressData onGetLocalDeviceInfo();
//    bool onOpenBluetooth();
//    bool onCloseBluetooth();
    qint64 onStartFindRemoteBluetooth();
//    void onPairing(const BluetoothAddressData &address);
//    void onPairingConfirmation(const bool &confirm);
    qint64 onConnect(const BluetoothAddressData &address);//ret 0 success else fail
//    bool onDisconnect();
    qint64 onSendMessage(const BluetoothTransData &data);
    int onGetServiceType();//get the service type (server or watch server)

signals:
    void signalDeviceDiscovered(QDBusVariant address);
    void signalDeviceConnected(QDBusVariant address);
//    void signalPairConfirmation(QDBusVariant address, QString pin);
//    void signalPairFinished(QDBusVariant address, int pairing);//arg: the address
//    void signalConnectServerFinished(QDBusVariant address);

    void signalMessageReceived(QDBusVariant message);

private slots:
    //bluetooth device
    void onDiscovered(const QString &info);
    void onClientConnected(const QString &clientName);
//    bool startFindRemoteBluetooth();
//    void onDiscovered(const QBluetoothDeviceInfo &info);
//    void onDiscoveryFinished();

    //pairing
//    void onPairingNeedInputPinCode(const QString &pin);
//    void onPairingDisplayConfirmation(const QBluetoothAddress & address, const QString &pin);
//    void onPairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing);

    //receive messages

    void onMessageReceived(const QString &message);
    void delayToTest();
private:
    explicit BackgroundTransfer(QObject *parent = 0);
private:
    static BackgroundTransfer *instance;
    QDBusInterface m_interfaceServer;
    QDBusInterface m_interfaceClient;
    BluetoothAddressData m_remoteServerAddr;
};

#endif // BACKGROUNDTRANSFER_H
