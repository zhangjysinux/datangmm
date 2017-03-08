#include "backgroundtransfer.h"
#include <QDBusReply>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QProcess>
#include <QTimer>
#include <QBluetoothLocalDevice>

BackgroundTransfer *BackgroundTransfer::instance = NULL;

BackgroundTransfer *BackgroundTransfer::getInstance()
{
    if(instance == NULL)
    {
        instance = new BackgroundTransfer();
    }

    return instance;
}

BackgroundTransfer::BackgroundTransfer(QObject *parent) :
    QObject(parent),
    m_interfaceServer("com.syberos.btService",
                "/BtServiceManager",
                "com.syberos.btService",
                QDBusConnection::sessionBus()),
    m_interfaceClient("com.syberos.btWatchService",
                      "/BtWatchManager",
                      "com.syberos.btWatchService",
                      QDBusConnection::sessionBus())
{
    if(m_interfaceServer.isValid())
    {
        qDebug() << "this is server";
        //cilent connected success bind for m+
        QDBusConnection::sessionBus().connect(QString(),
                                              QString(),
                                              "com.syberos.btService",
                                              "clientConnected",
                                              this,
                                              SLOT(onClientConnected(QString)));
        //service return message bind for watch and m+
        QDBusConnection::sessionBus().connect(QString(),
                                              QString(),
                                              "com.syberos.btService",
                                              "notifyMessage",
                                              this,
                                              SLOT(onMessageReceived(QString)));
    }
    if(m_interfaceClient.isValid())
    {
        qDebug() << "this is watch";
        //watch return message from service bind for watch
        QDBusConnection::sessionBus().connect(QString(),
                                              QString(),
                                              "com.syberos.btWatchService",
                                              "notifyDeviceInfo",//
                                              this,
                                              SLOT(onDiscovered(QString)));
        //service return message bind for watch and m+
        QDBusConnection::sessionBus().connect(QString(),
                                              QString(),
                                              "com.syberos.btWatchService",
                                              "notifyMessage",
                                              this,
                                              SLOT(onMessageReceived(QString)));
    }
//    onStartFindRemoteBluetooth();
}

int BackgroundTransfer::onGetServiceType()
{
    if(m_interfaceServer.isValid())
        return BLUETOOTHSERVICETYPE_SERVER;
    else if(m_interfaceClient.isValid())
        return BLUETOOTHSERVICETYPE_CLIENT;
    else
        return BLUETOOTHSERVICETYPE_NONE;
}

//BluetoothAddressData BackgroundTransfer::onGetLocalDeviceInfo()
//{
//    return BluetoothAddressData();
//}

//bool BackgroundTransfer::onOpenBluetooth()
//{
//    return true;
//}

//bool BackgroundTransfer::onCloseBluetooth()
//{
//    return true;
//}

qint64 BackgroundTransfer::onStartFindRemoteBluetooth()
{
    QBluetoothLocalDevice device;
    BTDeviceInfoList dList = device.pairedDevices();
    for(int i=0; i<dList.count(); i++)
    {
        BTDeviceInfo *d = dList.value(i);
        qDebug() << "connect paired devices: " << d->address();
        BluetoothAddressData addr;
        addr.address = d->address();
        onConnect(addr);
    }

    return 0;

//    if (m_interfaceClient.isValid()) {
//        // call remote checkIn，ret num_room
//        qint64 num_room = -1;
//        qDebug()<<"onStartFindRemoteBluetooth call discoveryBluetoothDevice" ;
//        QDBusReply<qint64> reply = m_interfaceClient.call(QLatin1String("discoveryBluetoothDevice"));
//         if (reply.isValid()) {
//            num_room = reply.value();
//            qDebug() << "get num_room: " << num_room ;
//        } else {
//             qDebug() <<"discoveryBluetoothDevice fail!" ;
//        }
//        return num_room;
//    }
//    else
//    {
//        qDebug() << "!m_interfaceClient.isValid()";
//    }
//    return -1;
}

//void BackgroundTransfer::onPairing(const BluetoothAddressData &address)
//{

//}

//void BackgroundTransfer::onPairingConfirmation(const bool &confirm)
//{

//}

qint64 BackgroundTransfer::onConnect(const BluetoothAddressData &address)//ret 0 success else fail
{
    if (m_interfaceClient.isValid())
    {
        // call remote checkIn，ret num_room
        qint64 num_room = -1;
        QDBusReply<qint64> reply = m_interfaceClient.call("connectBTservice", address.address);

        if (reply.isValid()) {
            num_room = reply.value();
            if (num_room == 0){
//                BluetoothTransData data;
//                data.type = 100;
//                data.handleType = 200;
//                data.transData = "hi, this is watch";
//                onSendMessage(data);
            }
            qDebug() <<"connectBTservice reply " << num_room <<  "rooms";
        } else{
            qDebug() <<"call connectBTservice fail!";
        }
        return num_room;
    }
    else
    {
        qDebug() << "!m_interfaceClient.isValid()";
    }
    return -1;
}

//bool BackgroundTransfer::onDisconnect()
//{

//}

qint64 BackgroundTransfer::onSendMessage(const BluetoothTransData &data)
{
    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out << data;
    qDebug() << "onSendMessage message" << data.type << data.handleType << data.transData;

//    Contacter contacter;
//    QByteArray byteArray = data.transData;
//    QDataStream stream(&byteArray, QIODevice::ReadOnly);
//    stream >> contacter;

    QString postMsg = "orange nihao";
    if (m_interfaceClient.isValid())
    {
        // call remote checkIn， ret num_room
        qint64 num_room = -1;
        QDBusReply<qint64> reply = m_interfaceClient.call("postMessage", postMsg);
        if (reply.isValid()) {
            num_room = reply.value();
            qDebug() << "interfaceClient postMessage reply " << num_room << "rooms";
        } else {
            qDebug() << "interfaceClient call postMessage fail!\n";
        }
        return num_room;
    }
    else if(m_interfaceServer.isValid())
    {
        // call remote checkIn， ret num_room
        qint64 num_room = -1;
        QDBusReply<qint64> reply = m_interfaceServer.call("postMessage", postMsg);
        if (reply.isValid()) {
            num_room = reply.value();
            qDebug() << "interfaceServer postMessage reply " << num_room << "rooms";
        } else {
            qDebug() << "interfaceServer call postMessage fail!\n";
        }
        return num_room;
    }
    else
    {
        qDebug() << "!m_interfaceClient.isValid()";
    }
    return -1;
}

void BackgroundTransfer::onMessageReceived(const QString &message)
{
    BluetoothTransData userData;

    Contacter contacter;
    contacter.id = "110";
    contacter.name = "orange";
    contacter.surname = "li";
    QByteArray contacterByteArray;
    QDataStream stream(&contacterByteArray, QIODevice::WriteOnly);
    stream << contacter;

    userData.type = 0;
    userData.handleType = 1;
    userData.transData = contacterByteArray;

    QByteArray msg;
    QDataStream transDataStream(&msg, QIODevice::WriteOnly);
    transDataStream << userData;


    BluetoothTransData transData;
    QByteArray data = msg;
    QDataStream in(&data, QIODevice::ReadOnly);
    in >> transData;

    qDebug() << "onMessageReceived message" << transData.type << transData.handleType << transData.transData;

    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(transData);
    dbusValue.setVariant(dataVar);
    emit signalMessageReceived(dbusValue);
}

void BackgroundTransfer::delayToTest()
{
    onMessageReceived("");
}

void BackgroundTransfer::onDiscovered(const QString &deviceaddress)
{
    qDebug() << " onDiscovered  Address =" << deviceaddress << endl;

    QJsonParseError json_error;
    QByteArray msg_array = deviceaddress.toLatin1();
    QJsonDocument document = QJsonDocument::fromJson(msg_array,&json_error);
    if (json_error.error == QJsonParseError::NoError)
    {
        if (document.isArray())
        {
            QJsonArray array = document.array();
            int size = array.size();
            qDebug() << "size = " << size << endl;
            QJsonValue value;
            for (int i = 0; i < size; i++)
            {
                value = array.at(i);
                qDebug() << "i = " <<i<< ";address = " << value.toString()<< endl;

                BluetoothAddressData data;
//                data.name = info.name();
                data.address = value.toString();
                if(data.address.contains("74:51"))
                    m_remoteServerAddr = data;

                QDBusVariant dbusValue;
                QVariant dataVar = QVariant::fromValue(data);
                dbusValue.setVariant(dataVar);

                emit signalDeviceDiscovered(dbusValue);
            }
        }
    }
    else
    {
        qDebug() << "onDiscovered json parse array error" << json_error.errorString();
    }
//    onConnect(m_remoteServerAddr);
}

void BackgroundTransfer::onClientConnected(const QString &clientName)
{
    qDebug() << "clientName: " << clientName;

    BluetoothAddressData data;
    data.address = clientName;

    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(data);
    dbusValue.setVariant(dataVar);

    emit signalDeviceConnected(dbusValue);
}
