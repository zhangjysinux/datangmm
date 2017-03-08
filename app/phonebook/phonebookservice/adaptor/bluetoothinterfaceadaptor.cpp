#include "bluetoothinterfaceadaptor.h"

BluetoothInterfaceAdaptor *BluetoothInterfaceAdaptor::m_instance = NULL;

BluetoothInterfaceAdaptor *BluetoothInterfaceAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new BluetoothInterfaceAdaptor();

    return m_instance;
}

BluetoothInterfaceAdaptor::BluetoothInterfaceAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", QDBusConnection::sessionBus())
{
    m_interface.call("remoteFunction");
}

