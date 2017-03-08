#ifndef BLUETOOTHINTERFACEADAPTOR_H
#define BLUETOOTHINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusVariant>
#include "../../../commondata/bluetoothdata.h"
#include "../../../commondata/datatype/contacterdata.h"

class BluetoothInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    static BluetoothInterfaceAdaptor *getInstance();

public slots:
//    bool openBluetooth();
//    bool onCloseBluetooth();
//    bool startFindRemoteBluetooth();
//    quint64 writeData(const BluetoothTransData &data);

public slots:
    BluetoothAddressData getLocalDeviceInfo();
    bool isBluetoothOpened();
    bool openBluetooth();
    bool startFindRemoteBluetooth();
    void pairing(const BluetoothAddressData &address);
    void pairingConfirmation(const bool &confirm);
    bool connect(const BluetoothAddressData &address);
    bool disconnect();

    qint64 sendMessage(const BluetoothTransData &data);
    qint64 sendContacter(const Contacter &contacter);
    qint64 sendFile(const QString &fileName);

signals:
    void signalDeviceDiscovered(const BluetoothAddressData &address);
    void signalPairConfirmation(const BluetoothAddressData &address, QString pin);
    void signalPairFinished(const BluetoothAddressData &address, int pairing);//arg: the address
    void signalConnectServerFinished(const BluetoothAddressData &address);
    void signalConnectServerFailed(const int &error);

    void signalMessageReceived(const BluetoothTransData &message);
    void signalContacterReceived(const int &handleType, const Contacter &contacter);
    void signalMessageSended(const int &sendStatus);

    void signalFileSendPercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);
    void signalFileReceivePercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);


private slots:
    void onDbusSignalDeviceDiscovered(const QDBusVariant &address);
    void onDbusSignalPairConfirmation(const QDBusVariant &address, QString pin);
    void onDbusSignalPairFinished(const QDBusVariant &address, int pairing);//arg: the address

    void onDbusSignalConnectServerFinished(const QDBusVariant &address);
    void onDbusSignalMessageReceived(const QDBusVariant &message);

//    void onDbusSignalContacterReceived(const Contacter &contacter);
//    void onDbusSignalDataReaded(const QDBusVariant &data);

private:
    explicit BluetoothInterfaceAdaptor(QObject *parent = 0);

private:
    static BluetoothInterfaceAdaptor *m_instance;
    QDBusInterface m_interface;
};


//#include <QTcpServer>
//#include <QTcpSocket>

//class Client : public QObject
//{
//    Q_OBJECT
//public:
//    QTcpSocket *newpeer;
//    Client()
//    {
//        newpeer = new QTcpSocket();
//        newpeer->connectToHost(QHostAddress::LocalHost, 8888);
//        BluetoothTransData data;
//        Contacter c;
//        c.surname = "li";
//        c.name = "orange";
//        c.id = "102";
//        c.title = 123;
//        QDataStream cstream(&data.transData, QIODevice::WriteOnly);
//        cstream << c;
//        data.type = 100;
//        data.handleType = 200;
////        data.transData = "nihao";
//        QByteArray by;
//        QDataStream stream(&by, QIODevice::WriteOnly);
//        stream << data;
//        newpeer->write(by);
//    }
//public slots:
//    void onReadData()
//    {
//        QByteArray by = newpeer->readAll();
//        BluetoothTransData data;
//        QDataStream stream(&by, QIODevice::ReadOnly);
//        stream >> data;
//    }
//};



#endif // BLUETOOTHINTERFACEADAPTOR_H
