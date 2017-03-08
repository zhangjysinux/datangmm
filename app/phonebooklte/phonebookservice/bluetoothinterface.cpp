#include "bluetoothinterface.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>

BluetoothInterface *BluetoothInterface::m_instance = NULL;

BluetoothInterface *BluetoothInterface::getInstance()
{
    if(m_instance == NULL)
        m_instance = new BluetoothInterface();

    return m_instance;
}

BluetoothInterface::BluetoothInterface(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<BluetoothAddressData>();
    qDBusRegisterMetaType<BluetoothTransData>();
    qDBusRegisterMetaType<BluetoothAppidReceiveDataCallback>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalDeviceDiscovered",
                       this, SIGNAL(signalDeviceDiscovered(QDBusVariant)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalPairConfirmation",
                       this, SIGNAL(signalPairConfirmation(QDBusVariant,QString)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalPairFinished",
                       this, SIGNAL(signalPairFinished(QDBusVariant,int)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalConnectServerFinished",
                       this, SIGNAL(signalConnectServerFinished(QDBusVariant)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalMessageReceived",
                       this, SIGNAL(signalMessageReceived(QDBusVariant)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalMessageSended",
                       this, SIGNAL(signalMessageSended(int)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalConnectServerFailed",
                       this, SIGNAL(signalConnectServerFailed(int)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalFileSendPercent",
                       this, SIGNAL(signalFileSendPercent(QString,int,int,int)));
    sessionBus.connect("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", "signalFileReceivePercent",
                       this, SIGNAL(signalFileReceivePercent(QString,int,int,int)));
}

BluetoothAddressData BluetoothInterface::onGetLocalDeviceInfo()
{
    BluetoothAddressData retVal;
    QDBusReply<BluetoothAddressData> retValDbus = m_interface.call("onGetLocalDeviceInfo");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onGetLocalDeviceInfo reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterface::onIsBluetoothOpened()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onIsBluetoothOpened");
    if(retValDbus.isValid())
    {
        retVal = retValDbus.value();
        qDebug() << "BluetoothInterface::onIsBluetoothOpened" << retVal ;
    }
    else
        qDebug() << "onIsBluetoothOpened reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterface::onOpenBluetooth()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onOpenBluetooth");
    if(retValDbus.isValid())
    {
        retVal = retValDbus.value();
        qDebug() << "BluetoothInterface::onOpenBluetooth" << retVal ;
    }
    else
        qDebug() << "onOpenBluetooth reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterface::onCloseBluetooth()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onCloseBluetooth");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onCloseBluetooth reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterface::onStartFindRemoteBluetooth()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onStartFindRemoteBluetooth");
    if(retValDbus.isValid())
    {
        retVal = retValDbus.value();
        qDebug() << "BluetoothInterface::onOpenBluetooth" << retVal ;
    }
    else
        qDebug() << "onStartFindRemoteBluetooth reply error:" << retValDbus.error();

    return retVal;
}

void BluetoothInterface::onPairing(const BluetoothAddressData &address)
{
    m_interface.call("onPairing", QVariant::fromValue(address));
    qDebug() << "BluetoothInterface::onPairing" << address.name << address.address ;
}

void BluetoothInterface::onPairingConfirmation(const bool &confirm)
{
    m_interface.call("onPairingConfirmation", confirm);
    qDebug() << "BluetoothInterface::onPairingConfirmation" << confirm ;
}

bool BluetoothInterface::onConnect(const BluetoothAddressData &address)
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onConnect", QVariant::fromValue(address));
    if(retValDbus.isValid())
    {
        retVal = retValDbus.value();
        qDebug() << "BluetoothInterface::onConnect" << retVal ;
    }
    else
        qDebug() << "onConnect reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterface::onDisconnect()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        retVal = retValDbus.value();
        qDebug() << "BluetoothInterface::onDisconnect" << retVal ;
    }
    else
        qDebug() << "onDisconnect reply error:" << retValDbus.error();

    return retVal;
}



qint64 BluetoothInterface::onSendMessage(const BluetoothTransData &data)
{
    qint64 retVal;
    QVariant dataVar = QVariant::fromValue(data);
    QDBusReply<qint64> retValReply = m_interface.call("onSendMessage", dataVar);
    if(retValReply.isValid())
    {
        retVal = retValReply.value();
        qDebug() << "BluetoothInterface::onSendMessage" << retVal ;
    }
    else
        qDebug() << "onSendMessage reply error:" << retValReply.error();

    return retVal;
}

qint64 BluetoothInterface::onSendFile(const QString &fileName)
{
    qint64 retVal;
    QDBusReply<qint64> retValReply = m_interface.call("onSendFile", fileName);
    if(retValReply.isValid())
    {
        retVal = retValReply.value();
        qDebug() << "BluetoothInterface::onSendFile" << retVal ;
    }
    else
        qDebug() << "BluetoothInterface::onSendFile reply error:" << retValReply.error();

    return retVal;
}

