#include "bluetoothbackgroundtransferinterface.h"
#include <QDBusReply>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QProcess>
#include <QTimer>
#include <QBuffer>
#include <QThread>
#include <QJsonObject>
#include <QDateTime>
#include <QBluetoothLocalDevice>

BluetoothBackgroundTransferInterface *BluetoothBackgroundTransferInterface::instance = NULL;

BluetoothBackgroundTransferInterface *BluetoothBackgroundTransferInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new BluetoothBackgroundTransferInterface();
    }

    return instance;
}

BluetoothBackgroundTransferInterface::BluetoothBackgroundTransferInterface(QObject *parent) :
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
                                              SLOT(onMessageReceived(QByteArray)));
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
                                              SLOT(onMessageReceived(QByteArray)));
    }
}

int BluetoothBackgroundTransferInterface::onGetServiceType()
{
    if(m_interfaceServer.isValid())
        return BLUETOOTHSERVICETYPE_SERVER;
    else if(m_interfaceClient.isValid())
        return BLUETOOTHSERVICETYPE_CLIENT;
    else
        return BLUETOOTHSERVICETYPE_NONE;
}

qint64 BluetoothBackgroundTransferInterface::onStartFindRemoteBluetooth()
{
    QBluetoothLocalDevice device;
    BTDeviceInfoList dList = device.pairedDevices();
    for(int i=0; i<dList.count(); i++)
    {
        BTDeviceInfo *d = dList.value(i);
        qDebug() << "connect paired devices: " << d->address();
        m_pairedAddress = d->address();
    }

//    return 0;

    if (m_interfaceClient.isValid()) {
        // call remote checkIn，ret num_room
        qint64 num_room = -1;
        qDebug()<<"onStartFindRemoteBluetooth call discoveryBluetoothDevice" ;
        QDBusReply<qint64> reply = m_interfaceClient.call(QLatin1String("discoveryBluetoothDevice"));
         if (reply.isValid()) {
            num_room = reply.value();
            qDebug() << "get num_room: " << num_room ;
        } else {
             qDebug() <<"discoveryBluetoothDevice fail!" ;
        }
        return num_room;
    }
    else
    {
        qDebug() << "!m_interfaceClient.isValid()";
    }
    return -1;
}

qint64 BluetoothBackgroundTransferInterface::onConnect(const BluetoothAddressData &address)//ret 0 success else fail
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

        if(num_room < 0)
        {
            QTimer::singleShot(5000, this, SLOT(onStartFindRemoteBluetooth()));
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

qint64 BluetoothBackgroundTransferInterface::onSendMessage(const QByteArray &data)
{
    return sendMessage(data);
    //parse json //{key:contacts,count:count,state:ok,msg:””,Array:[]}
//    QJsonObject jsonObj;
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
//    if(jsonDoc.isObject())
//    {
//        jsonObj = jsonDoc.object();
//        QString key = jsonObj.value("key").toString();
//        QString count = jsonObj.value("count").toString();
//        QString state = jsonObj.value("state").toString();
//        QString msg = jsonObj.value("msg").toString();
//        QJsonArray array = jsonObj.value("Array").toArray();
//        qDebug() << array.count();
//        QJsonValue jsonValue = array.at(0);

//        QString transData = jsonValue.toString();
//        QByteArray transDataByte = transData.toLatin1();

//        BluetoothTransData tData;
//        QDataStream stream(&transDataByte, QIODevice::ReadOnly);
//        stream >> tData;

//        Contacter contacter;
//        QByteArray contacterByte = tData.transData;
//        QDataStream cStream(&contacterByte, QIODevice::ReadOnly);
//        cStream >> contacter;
//        qDebug() << tData.type;
//    }

//    QByteArray dataArray;
//    QDataStream out(&dataArray, QIODevice::WriteOnly);
//    out << data;
//    qDebug() << "onSendMessage message" << data.type << data.handleType << data.transData;

//    Contacter contacter;
//    QByteArray byteArray = data.transData;
//    QDataStream stream(&byteArray, QIODevice::ReadOnly);
//    stream >> contacter;

//    QString postMsg = "orange nihao";
//    if (m_interfaceClient.isValid())
//    {
//        // call remote checkIn， ret num_room
//        qint64 num_room = -1;
//        QDBusReply<qint64> reply = m_interfaceClient.call("postMessage", data);
//        if (reply.isValid()) {
//            num_room = reply.value();
//            qDebug() << "interfaceClient postMessage reply " << num_room << "rooms";
//        } else {
//            qDebug() << "interfaceClient call postMessage fail!\n";
//        }
//        return num_room;
//    }
//    else if(m_interfaceServer.isValid())
//    {
//        // call remote checkIn， ret num_room
//        qint64 num_room = -1;
//        QDBusReply<qint64> reply = m_interfaceServer.call("postMessage", data);
//        if (reply.isValid()) {
//            num_room = reply.value();
//            qDebug() << "interfaceServer postMessage reply " << num_room << "rooms";
//        } else {
//            qDebug() << "interfaceServer call postMessage fail!\n";
//        }
//        return num_room;
//    }
//    else
//    {
//        qDebug() << "!m_interfaceClient.isValid()";
//    }
//    return -1;
}

void BluetoothBackgroundTransferInterface::onMessageReceived(const QByteArray &message)
{
    qDebug() << "BluetoothBackgroundTransferInterface onMessageReceived----------------------------------";

//    PackageData pData;
//    QByteArray msg = message;
//    QDataStream in(&msg, QIODevice::ReadOnly);
//    in >> pData;
//    if(pData.index == 0)
//    {
//        m_packagesData.clear();
//        m_packagesData.append(pData.data);
//    }
//    else
//    {
//        m_packagesData.append(pData.data);
//    }
//    qDebug() << "pData index" << pData.index << pData.count << pData.data.size();

//    if(pData.index != (pData.count-1))
//        return;

    qDebug() << "onMessageReceived count: " << message.count();
    QJsonObject jsonObj;
//    QByteArray data = message.toLatin1();
    QJsonDocument jsonDoc = QJsonDocument::fromBinaryData(message);
    if(jsonDoc.isObject())
    {
        qDebug() << "jsonDoc.isObject() true-------------------";
        jsonObj = jsonDoc.object();
        QString key = jsonObj.value("key").toString();
        QString count = jsonObj.value("count").toString();
        QString state = jsonObj.value("state").toString();
        QString msg = jsonObj.value("msg").toString();
        QJsonArray array = jsonObj.value("Array").toArray();
//        qDebug() << array.count();
        QJsonValue jsonValue = array.at(0);

        QString transData = jsonValue.toString();
        QByteArray transDataByte = transData.toLatin1();

        BluetoothTransData tData;
        QDataStream stream(&transDataByte, QIODevice::ReadOnly);
        stream >> tData;

        emit signalMessageReceived(tData);
        qDebug() << "count:" <<array.count() << "type" << tData.type;
    }
    else
    {
        qDebug() << "jsonDoc.isObject() error-------------------";
    }
}

void BluetoothBackgroundTransferInterface::delayToTest()
{
    onMessageReceived("");
}

void BluetoothBackgroundTransferInterface::onDiscovered(const QString &deviceaddress)
{
    qDebug() << " onDiscovered  Address =" << deviceaddress << endl;

    QStringList foundDevices;
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

                emit signalDeviceDiscovered(data);

                foundDevices.append(data.address);
            }
        }
    }
    else
    {
        qDebug() << "onDiscovered json parse array error" << json_error.errorString();
    }

    if(foundDevices.contains(m_pairedAddress))
    {
        BluetoothAddressData data;
        data.address = m_pairedAddress;
        onConnect(data);
    }
    else
    {
        QTimer::singleShot(5000, this, SLOT(onStartFindRemoteBluetooth()));
    }
}

void BluetoothBackgroundTransferInterface::onClientConnected(const QString &clientName)
{
    qDebug() << "clientName: " << clientName;
    QFile file("clientConneted.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(QDateTime::currentDateTime().toString().toLatin1());
        file.close();
    }

    BluetoothAddressData data;
    data.address = clientName;

    emit signalDeviceConnected(data);
}

qint64 BluetoothBackgroundTransferInterface::sendMessage(const QByteArray &data)
{
    qDebug() << "BluetoothBackgroundTransferInterface::sendMessage count: " << data.count();
    if (m_interfaceClient.isValid())
    {
        // call remote checkIn， ret num_room
        qint64 num_room = -1;
        QDBusReply<qint64> reply = m_interfaceClient.call("postMessage", data);
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
        QDBusReply<qint64> reply = m_interfaceServer.call("postMessage", data);
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

qint64 BluetoothBackgroundTransferInterface::sendMessages(const QByteArray &data)
{
    int singleSize = 128;
    qint64 maxSize = 0;
    quint64 size = data.size();
    int count = size / singleSize;
    int leftSize = size % singleSize;
    if(leftSize > 0)
        count ++;

//    PackageData pData;
//    pData.count = count;

//    for(int i=0; i<pData.count; i++)
//    {
//        pData.index = i;
//        if(i != pData.count-1)
//            pData.data = data.mid(i*singleSize, singleSize);
//        else
//            pData.data = data.mid(i*singleSize, leftSize);

//        QByteArray sendPackageData;
//        QDataStream out(&sendPackageData, QIODevice::WriteOnly);
//        out << pData;

//        maxSize += sendMessage(sendPackageData);
//        QThread::msleep(10);
//    }

    return maxSize;
}
