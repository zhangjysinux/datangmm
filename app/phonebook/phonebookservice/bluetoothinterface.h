#ifndef BLUETOOTHINTERFACE_H
#define BLUETOOTHINTERFACE_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusVariant>
#include <QtBluetooth/QBluetoothAddress>
#include "../../commondata/bluetoothdata.h"

class BluetoothInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.bluetoothadaptor")
public:
    static BluetoothInterface *getInstance();

public slots:
    BluetoothAddressData onGetLocalDeviceInfo();
    bool onIsBluetoothOpened();
    bool onOpenBluetooth();
    bool onCloseBluetooth();
    bool onStartFindRemoteBluetooth();
    void onPairing(const BluetoothAddressData &address);
    void onPairingConfirmation(const bool &confirm);
    bool onConnect(const BluetoothAddressData &address);
    bool onDisconnect();

    qint64 onSendMessage(const BluetoothTransData &data);
    qint64 onSendFile(const QString &fileName);
signals:
    void signalDeviceDiscovered(QDBusVariant address);
    void signalPairConfirmation(QDBusVariant address, QString pin);
    void signalPairFinished(QDBusVariant address, int pairing);//arg: the address
    void signalConnectServerFinished(QDBusVariant address);
    void signalConnectServerFailed(const int &error);

    void signalMessageReceived(QDBusVariant message);
    void signalMessageSended(const int &sendStatus);
    void signalFileSendPercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);
    void signalFileReceivePercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);

private:
    explicit BluetoothInterface(QObject *parent = 0);

private:
    static BluetoothInterface *m_instance;
    QDBusInterface m_interface;
};


//#include <QTcpServer>
//#include <QTcpSocket>
//#include "contacterdata.h"

////test qdatastream
//class TestServer : public QObject
//{
//    Q_OBJECT
//public:
//    QTcpServer *server;
//    QTcpSocket *newpeer;
//    TestServer()
//    {
//        server = new QTcpServer();
//        if( server->listen(QHostAddress::Any, 8888) )
//        {
//            qDebug() << "server listening ...";
//            QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
//        }
//    }
//public slots:
//    void onNewConnection()
//    {
//        newpeer = server->nextPendingConnection();
//        QObject::connect(newpeer, SIGNAL(readyRead()),
//                         this, SLOT(onReadData()));
//    }

//    void onReadData()
//    {
//        QByteArray by = newpeer->readAll();
//        BluetoothTransData data;
//        QDataStream stream(&by, QIODevice::ReadOnly);
//        stream >> data;

//        Contacter c;
//        QDataStream cstream(&data.transData, QIODevice::ReadOnly);
//        cstream >> c;

//        qDebug() << "data" << data.type << data.handleType << data.transData;
//    }

//};

#endif // BLUETOOTHINTERFACE_H
