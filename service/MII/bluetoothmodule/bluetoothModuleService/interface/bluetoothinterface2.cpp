#include "bluetoothinterface2.h"
#include <QDebug>
#include <QDBusError>

static const QLatin1String serviceUuid("e8e10f96-1a70-4b27-9ccf-02010264e9c8");

BluetoothInterface2 *BluetoothInterface2::m_instance = NULL;

BluetoothInterface2 *BluetoothInterface2::getInstance()
{
    if(m_instance == NULL)
        m_instance = new BluetoothInterface2();

    return m_instance;
}

BluetoothInterface2::BluetoothInterface2(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType<BluetoothAddressData>();
    qDBusRegisterMetaType<BluetoothTransData>();
    qDBusRegisterMetaType<BluetoothAppidReceiveDataCallback>();

    qDebug() << "m_bluetoothLocalDevice: "<< m_bluetoothLocalDevice.address().toString();

    connect(&m_bluetoothLocalDevice, SIGNAL(pairingNeedInputPinCode(QString)),
                     this, SLOT(onPairingNeedInputPinCode(QString)));
    connect(&m_bluetoothLocalDevice, SIGNAL(pairingDisplayConfirmation(QBluetoothAddress,QString)),
                     this, SLOT(onPairingDisplayConfirmation(QBluetoothAddress,QString)));
    connect(&m_bluetoothLocalDevice, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)),
                     this, SLOT(onPairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));

    //discover agent
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(onDiscovered(QBluetoothDeviceInfo)));
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(onDiscoveryFinished()));
    connect(m_discoveryAgent, SIGNAL(canceled()), this, SLOT(onDiscoveryFinished()));

    //server and client message control
    connect(&m_bluetoothClient, SIGNAL(connectFinished(BluetoothAddressData)),
            this, SLOT(onConnectServerFinished(BluetoothAddressData)));
    connect(&m_bluetoothClient, SIGNAL(fileSendPercent(QString,int,qint64,qint64)),
            this, SLOT(onFileSendPercent(QString,int,qint64,qint64)));

//    connect(&m_bluetoothServer, SIGNAL(messageReceived(QString,BluetoothTransData)),
//            this, SLOT(onMessageReceived(QString,BluetoothTransData)));
    connect(&m_bluetoothServer, SIGNAL(clientConnected(BluetoothAddressData)),
            this, SLOT(onClientConnected(BluetoothAddressData)));
    connect(&m_bluetoothServer, SIGNAL(fileReceivedPercent(QString,int,qint64,qint64)),
            this, SLOT(onFileReceivedPercent(QString,int,qint64,qint64)));

    connect(&m_repeatConnectTimer, SIGNAL(timeout()), this, SLOT(onRepeatConnect()));
}

bool BluetoothInterface2::onIsBluetoothOpened()
{
    // Check if Bluetooth is available on this device
    if (m_bluetoothLocalDevice.isValid())
    {
        if(m_bluetoothLocalDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        {
            return false;
        }
        else if(m_bluetoothLocalDevice.hostMode() == QBluetoothLocalDevice::HostConnectable)
        {
            for(int i=0; i<3; i++)
            {
                m_bluetoothLocalDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
                //sleep 1s
                QThread::msleep(1000);
                if(m_bluetoothLocalDevice.hostMode() == QBluetoothLocalDevice::HostDiscoverable)
                {
                    break;
                }
            }
        }
        //start the bluetooth server
        m_bluetoothServer.startServer(m_bluetoothLocalDevice.address());

        return true;
    }
    qDebug() << "bluetooth device is not valid";
    return false;
}


BluetoothAddressData BluetoothInterface2::onGetLocalDeviceInfo()
{
    BluetoothAddressData data;
    data.name = m_bluetoothLocalDevice.name();
    data.address = m_bluetoothLocalDevice.address().toString();
    return data;
}


bool BluetoothInterface2::onOpenBluetooth()
{
    QString localDeviceName;

    // Check if Bluetooth is available on this device
    if (m_bluetoothLocalDevice.isValid()) {

        // Turn Bluetooth on
        m_bluetoothLocalDevice.powerOn();

        // Read local device name
        localDeviceName = m_bluetoothLocalDevice.name();

        for(int i=0; i<3; i++)
        {
            m_bluetoothLocalDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
            //sleep 1s
            QThread::msleep(1000);
            if(m_bluetoothLocalDevice.hostMode() == QBluetoothLocalDevice::HostDiscoverable)
            {
                break;
            }
        }

        //start the bluetooth server
        m_bluetoothServer.startServer(m_bluetoothLocalDevice.address());
    }

//    QThread::msleep(1000);
//    onStartFindRemoteBluetooth();
//    QThread::msleep(1000);
//    BluetoothAddressData data;
//    data.address = "00:02:2C:60:93:5C";
//    onPairing(data);
//    onConnect(data);

    return false;
}

bool BluetoothInterface2::onStartFindRemoteBluetooth()
{
    m_queryTimes = 0;
    return startFindRemoteBluetooth();
}


bool BluetoothInterface2::startFindRemoteBluetooth()
{
    // Start a discovery
    m_discoveryAgent->start();
    m_bluetoothList.clear();
    if(m_discoveryAgent->error() == QBluetoothDeviceDiscoveryAgent::NoError)
        return true;
    qDebug() << m_discoveryAgent->errorString();

    return false;
}

void BluetoothInterface2::onDiscovered(const QBluetoothDeviceInfo &info)
{
    qDebug() << "QBluetoothDeviceInfo" << info.name() << info.address().toString();
    BluetoothAddressData data;
    data.name = info.name();
    data.address = info.address().toString();
    if(data.name.isEmpty())
        data.name = data.address;

    if(!m_bluetoothList.contains(data.address))
    {
        m_bluetoothList.append(data.address);

        QDBusVariant dbusValue;
        QVariant dataVar = QVariant::fromValue(data);
        dbusValue.setVariant(dataVar);

        emit signalDeviceDiscovered(dbusValue);
    }
}

void BluetoothInterface2::onDiscoveryFinished()
{
    qDebug() << "discovery finished";
//    BluetoothAddressData data;
//    data.address = "00:02:2C:60:93:5C";
//    onPairing(data);
}

bool BluetoothInterface2::onCloseBluetooth()
{
    m_bluetoothLocalDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    if(m_bluetoothLocalDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
        qDebug() << "close bluetooth ok";
        return true;
    }
    else
    {
        qDebug() << "close bluetooth fail";
        return false;
    }
}

void BluetoothInterface2::onPairing(const BluetoothAddressData &address)
{
    qDebug() << "onPairing";
//    if(!isDevicePaired(address))
//    {
        QBluetoothAddress addr(address.address);
//        m_bluetoothLocalDevice.pairingSetPinCode(true, "123456");
        m_bluetoothLocalDevice.requestPairing(addr, QBluetoothLocalDevice::Paired);
//    }
    return;
}

void BluetoothInterface2::onPairingConfirmation(const bool &confirm)
{
    qDebug() << "onPairingConfirmation" << confirm;
    m_bluetoothLocalDevice.pairingConfirmation(confirm);
}

void BluetoothInterface2::onPairingNeedInputPinCode(const QString &pin)
{
    qDebug() << "onPairingNeedInputPinCode" << pin;
}

void BluetoothInterface2::onPairingDisplayConfirmation(const QBluetoothAddress & address, const QString &pin)
{
    qDebug() << "onPairingDisplayConfirmation" << address.toString() << pin;
    BluetoothAddressData addr;
    addr.address = address.toString();
    addr.name = m_bluetoothLocalDevice.getNameByAddress(addr.address);

    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(addr);
    dbusValue.setVariant(dataVar);
    emit signalPairConfirmation(dbusValue, pin);

//    m_bluetoothLocalDevice.pairingConfirmation(true);
}

void BluetoothInterface2::onPairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing)
{
    BluetoothAddressData data;
    data.name = m_bluetoothLocalDevice.getNameByAddress(address.toString());
    data.address = address.toString();

    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(data);
    dbusValue.setVariant(dataVar);
    onDisconnect();
    emit signalPairFinished(dbusValue, pairing);
    qDebug() << "paring finished" << pairing;

//    BluetoothAddressData addr;
//    addr.address = "00:02:2C:60:93:5C";

//    onConnect(addr);
}

bool BluetoothInterface2::onConnect(const BluetoothAddressData &address)
{
    m_repeatConnectTimer.stop();

    m_connectingAddress = address;
    onRepeatConnect();
    m_repeatConnectTimer.start(5000);

//    qDebug() << "start new connect";
//    m_bluetoothClient.stopClient();

//    // Start a discovery
//    m_discoveryAgent->stop();
//    QBluetoothAddress addr(m_connectingAddress.address);
//    if(m_bluetoothClient.startClient(addr, QBluetoothUuid(serviceUuid)) == 0)
//    {
//        return true;
//    }
    return false;
}

void BluetoothInterface2::onRepeatConnect()
{
    qDebug() << "onRepeatConnect";

        if(!m_bluetoothClient.isConnected())
        {
            qDebug() << "start new connect";
            m_bluetoothClient.stopClient();

            // Start a discovery
            m_discoveryAgent->stop();
            QBluetoothAddress addr(m_connectingAddress.address);
            if(m_bluetoothClient.startClient(addr, QBluetoothUuid(serviceUuid)) == 0)
            {
                return;
            }
            return;
        }
        else
        {
            m_repeatConnectTimer.stop();
        }
}

bool BluetoothInterface2::onDisconnect()
{
    m_repeatConnectTimer.stop();
    m_bluetoothClient.stopClient();
    return true;
}

void BluetoothInterface2::onConnectServerFinished(const BluetoothAddressData &address)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(address);
    dbusValue.setVariant(dataVar);
    emit signalConnectServerFinished(dbusValue);
    qDebug() << "onConnectServerFinished" << address.name << address.address;
    m_repeatConnectTimer.stop();
//    BluetoothTransData data;
//    data.type = 100;
//    data.transData = "i am client";
//    onSendMessage(data);

//    QThread::msleep(1000);
//    onDisconnect();
}

void BluetoothInterface2::onConnectServerError(QBluetoothSocket::SocketError error)
{
    if(error != QBluetoothSocket::NoSocketError)
    {
        qDebug() << "onConnectError: -----------" << error;
        emit signalConnectServerFailed(int(error));
    }
}

void BluetoothInterface2::onClientConnected(const BluetoothAddressData &address)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(address);
    dbusValue.setVariant(dataVar);
    emit signalClientConnected(dbusValue);
    qDebug() << "onClientConnected" << address.name << address.address;
}

qint64 BluetoothInterface2::onSendMessage(const BluetoothTransData &data)
{
    return m_bluetoothClient.onSendMessage(data);
}

qint64 BluetoothInterface2::onSendFile(const QString &fileName)
{
    return m_bluetoothClient.onSendFile(fileName);
}

bool BluetoothInterface2::onCancelSend()
{
    return m_bluetoothClient.onCancelTransmit();
}

void BluetoothInterface2::emitMessageReceived(const QString &sender, const BluetoothTransData &message)
{
    qDebug() << "onReceiveMessage:" << sender << message.type << message.transData;
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(message);
    dbusValue.setVariant(dataVar);
    signalMessageReceived(dbusValue);
}



bool BluetoothInterface2::isDevicePaired(const BluetoothAddressData &address)
{
    BTDeviceInfoList bluetoothList = m_bluetoothLocalDevice.pairedDevices();
    for(int i=0; i<bluetoothList.count(); i++)
    {
        BTDeviceInfo *bluetooth = bluetoothList.at(i);
        if(bluetooth->address() == address.address)
        {
            return true;
        }
    }
    return false;
}

bool BluetoothInterface2::onRegisterAppid(const QString &appid, const BluetoothAppidReceiveDataCallback &callback)
{
    if(!m_appidCallbackMap.contains(appid))
    {
        m_appidCallbackMap.insert(appid, callback);
        return true;
    }
    return false;
}

bool BluetoothInterface2::onUnregisterAppid(const QString &appid)
{
    if(m_appidCallbackMap.contains(appid))
    {
        m_appidCallbackMap.remove(appid);
    }
    return true;
}

bool BluetoothInterface2::onAppidCallback(const BluetoothTransData &message)
{
    QStringList appids = m_appidCallbackMap.keys();
    foreach(const QString &appid, appids)
    {
        BluetoothAppidReceiveDataCallback callback = m_appidCallbackMap.value(appid);
        QDBusInterface iface(callback.dbusService,
                             callback.dbusPath,
                             callback.dbusInterface);
        if(iface.isValid())
            iface.call(callback.function, QVariant::fromValue(message));
        else
            qDebug() << "callback interface error: " << iface.lastError();
    }
}

void BluetoothInterface2::onFileSendPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize)
{
    qDebug() << "BluetoothInterface2::onMessageSendPercent" << fileName << dataType << size << maxSize;
    emit signalFileSendPercent(fileName, dataType, size, maxSize);
    if(size == maxSize)
    {
        emit signalMessageSended(0);
    }
}

void BluetoothInterface2::onFileReceivedPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize)
{
    qDebug() << "BluetoothInterface2::onMessageReceivedPercent" << fileName << dataType << size << maxSize;
    emit signalFileReceivePercent(fileName, dataType, size, maxSize);

    if(size == maxSize)
    {
        if(dataType == 0)
        {
            QFile file(fileName);
            if(file.open(QIODevice::ReadOnly))
            {
                qDebug() << "BluetoothServer2::onFileReceivedPercent file open success: " << file.fileName();
                QByteArray data = file.readAll();
                BluetoothTransData message;
                QDataStream in(&data, QIODevice::ReadOnly);
                in >> message;
                emitMessageReceived("", message);

                file.close();
            }
            else
            {
                qDebug() << "BluetoothServer2::onFileReceivedPercent file open fail: " << file.errorString() << file.fileName();
            }
        }
    }
}


