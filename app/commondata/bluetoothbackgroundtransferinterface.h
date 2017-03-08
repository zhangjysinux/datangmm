#ifndef BACKGROUNDTRANSFER_H
#define BACKGROUNDTRANSFER_H

#include <QObject>
#include "bluetoothdata.h"
#include <QDBusInterface>
#include <QDBusVariant>
#include <QFile>
#include <QDateTime>
#include <QJsonObject>

class BluetoothBackgroundTransferInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.backgroundtransfer")
public:
    static BluetoothBackgroundTransferInterface* getInstance();

public slots:
    qint64 onStartFindRemoteBluetooth();
    qint64 onConnect(const BluetoothAddressData &address);//ret 0 success else fail
    qint64 onSendMessage(const QByteArray &data);
//    qint64 onSendContacter(const Contacter &contacter, const int &handleType);
    int onGetServiceType();//get the service type (server or watch server)

signals:
    void signalDeviceConnected(const BluetoothAddressData &address);
    void signalDeviceDiscovered(const BluetoothAddressData &address);
    void signalMessageReceived(const BluetoothTransData &message);

private slots:
    //bluetooth device
    void onDiscovered(const QString &info);
    void onClientConnected(const QString &clientName);
    void onMessageReceived(const QByteArray &message);
    void delayToTest();

private:
    explicit BluetoothBackgroundTransferInterface(QObject *parent = 0);
    qint64 sendMessage(const QByteArray &data);
    qint64 sendMessages(const QByteArray &data);

private:
    static BluetoothBackgroundTransferInterface *instance;
    QDBusInterface m_interfaceServer;
    QDBusInterface m_interfaceClient;
    QByteArray m_packagesData;
    QString m_pairedAddress;
};

#endif // BACKGROUNDTRANSFER_H
