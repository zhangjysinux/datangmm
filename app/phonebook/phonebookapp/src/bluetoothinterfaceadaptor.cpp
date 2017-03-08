#include "bluetoothinterfaceadaptor.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>

BluetoothInterfaceAdaptor *BluetoothInterfaceAdaptor::m_instance = NULL;

BluetoothInterfaceAdaptor *BluetoothInterfaceAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new BluetoothInterfaceAdaptor();

    return m_instance;
}

BluetoothInterfaceAdaptor::BluetoothInterfaceAdaptor(QObject *parent) :
    QObject(parent),
    m_interface(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<BluetoothAddressData>();
    qDBusRegisterMetaType<BluetoothTransData>();
    qDBusRegisterMetaType<BluetoothAppidReceiveDataCallback>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalDeviceDiscovered",
                       this, SLOT(onDbusSignalDeviceDiscovered(QDBusVariant)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalPairConfirmation",
                       this, SLOT(onDbusSignalPairConfirmation(QDBusVariant,QString)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalPairFinished",
                       this, SLOT(onDbusSignalPairFinished(QDBusVariant,int)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalMessageReceived",
                       this, SLOT(onDbusSignalMessageReceived(QDBusVariant)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalConnectServerFinished",
                       this, SLOT(onDbusSignalConnectServerFinished(QDBusVariant)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalMessageSended",
                       this, SIGNAL(signalMessageSended(int)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalConnectServerFailed",
                       this, SIGNAL(signalConnectServerFailed(int)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalFileSendPercent",
                       this, SIGNAL(signalFileSendPercent(QString,int,int,int)));
    sessionBus.connect(contacterServiceName, "/bluetoothadaptor", "com.sinux.DBus.bluetoothadaptor", "signalFileReceivePercent",
                       this, SIGNAL(signalFileReceivePercent(QString,int,int,int)));
//    isBluetoothOpened();
}

BluetoothAddressData BluetoothInterfaceAdaptor::getLocalDeviceInfo()
{
    BluetoothAddressData retVal;
    QDBusReply<BluetoothAddressData> retValDbus = m_interface.call("onGetLocalDeviceInfo");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onGetLocalDeviceInfo reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterfaceAdaptor::isBluetoothOpened()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onIsBluetoothOpened");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onIsBluetoothOpened reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterfaceAdaptor::openBluetooth()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onOpenBluetooth");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onOpenBluetooth reply error:" << retValDbus.error();

    return retVal;
}

//bool BluetoothInterfaceAdaptor::onCloseBluetooth()
//{
//    bool retVal;
//    QDBusReply<bool> retValDbus = m_interface.call("onCloseBluetooth");
//    if(retValDbus.isValid())
//        retVal = retValDbus.value();
//    else
//        qDebug() << "onCloseBluetooth reply error:" << retValDbus.error();

//    return retVal;
//}

bool BluetoothInterfaceAdaptor::startFindRemoteBluetooth()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onStartFindRemoteBluetooth");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onStartFindRemoteBluetooth reply error:" << retValDbus.error();

    return retVal;
}

void BluetoothInterfaceAdaptor::pairing(const BluetoothAddressData &address)
{
    m_interface.call("onPairing", QVariant::fromValue(address));
}

void BluetoothInterfaceAdaptor::pairingConfirmation(const bool &confirm)
{
    m_interface.call("onPairingConfirmation", confirm);
}

bool BluetoothInterfaceAdaptor::connect(const BluetoothAddressData &address)
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onConnect", QVariant::fromValue(address));
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onConnect reply error:" << retValDbus.error();

    return retVal;
}

bool BluetoothInterfaceAdaptor::disconnect()
{
    bool retVal;
    QDBusReply<bool> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
        retVal = retValDbus.value();
    else
        qDebug() << "onDisconnect reply error:" << retValDbus.error();

    return retVal;
}

qint64 BluetoothInterfaceAdaptor::sendMessage(const BluetoothTransData &data)
{
    qint64 retVal;
    QVariant dataVar = QVariant::fromValue(data);
    QDBusReply<qint64> retValReply = m_interface.call("onSendMessage", dataVar);
    if(retValReply.isValid())
    {
        qDebug() << "onSendMessage ahhahahahah------------";
        retVal = retValReply.value();
    }
    else
        qDebug() << "onSendMessage reply error:" << retValReply.error();

    return retVal;
}

qint64 BluetoothInterfaceAdaptor::sendFile(const QString &fileName)
{
    qint64 retVal;
    QDBusReply<qint64> retValReply = m_interface.call("onSendFile", fileName);
    if(retValReply.isValid())
    {
        qDebug() << "onSendFile ahhahahahah------------" << fileName;
        retVal = retValReply.value();
    }
    else
        qDebug() << "onSendFile reply error:" << retValReply.error();

    return retVal;
}

void BluetoothInterfaceAdaptor::onDbusSignalDeviceDiscovered(const QDBusVariant &address)
{
    QDBusArgument argument = address.variant().value<QDBusArgument>();
    BluetoothAddressData addr = qdbus_cast<BluetoothAddressData>(argument);
    emit signalDeviceDiscovered(addr);
    qDebug() << "onDbusSignalDeviceDiscovered";
}

void BluetoothInterfaceAdaptor::onDbusSignalPairConfirmation(const QDBusVariant &address, QString pin)
{
    QDBusArgument argument = address.variant().value<QDBusArgument>();
    BluetoothAddressData addr = qdbus_cast<BluetoothAddressData>(argument);
    emit signalPairConfirmation(addr, pin);
    qDebug() << "onDbusSignalPairConfirmation";
}

void BluetoothInterfaceAdaptor::onDbusSignalPairFinished(const QDBusVariant &address, int pairing)
{
    QDBusArgument argument = address.variant().value<QDBusArgument>();
    BluetoothAddressData addr = qdbus_cast<BluetoothAddressData>(argument);
    emit signalPairFinished(addr, pairing);
    qDebug() << "onDbusSignalPairFinished";
}

void BluetoothInterfaceAdaptor::onDbusSignalConnectServerFinished(const QDBusVariant &address)
{
    QDBusArgument argument = address.variant().value<QDBusArgument>();
    BluetoothAddressData addr = qdbus_cast<BluetoothAddressData>(argument);
    emit signalConnectServerFinished(addr);
    qDebug() << "onDbusSignalConnectServerFinished";
}

void BluetoothInterfaceAdaptor::onDbusSignalMessageReceived(const QDBusVariant &message)
{
    QDBusArgument argument = message.variant().value<QDBusArgument>();
    BluetoothTransData msg = qdbus_cast<BluetoothTransData>(argument);
    emit signalMessageReceived(msg);

    if(msg.type == BLUETOOTHTRANSDATATYPE_CONTACTSHARE)
    {
        Contacter contacter;
        QByteArray transData = msg.transData;;
        QDataStream stream(&transData, QIODevice::ReadOnly);
        stream >> contacter;
        emit signalContacterReceived(msg.handleType, contacter);
        qDebug() << "contacter" << contacter.surname << contacter.name << contacter.id;
    }
    qDebug() << "received " << msg.type << msg.transData;
}

qint64 BluetoothInterfaceAdaptor::sendContacter(const Contacter &contacter)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_CONTACTSHARE;
    data.handleType = CONTACTER_ADD_HANDLE;

    QByteArray transData;
    QDataStream stream(&transData, QIODevice::WriteOnly);
    stream << contacter;

    data.transData = transData;
    return sendMessage(data);
}

//void BluetoothInterfaceAdaptor::onDbusSignalDataReaded(const QDBusVariant &data)
//{
//    QDBusArgument argument = data.variant().value<QDBusArgument>();
//    BluetoothTransData value = qdbus_cast<BluetoothTransData>(argument);
//    emit signalDataReaded(value);
//}
