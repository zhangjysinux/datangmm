#include <QCoreApplication>
#include <QDebug>
#include <SyberosServiceCache>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>
#include <QDBusMetaType>
#include "./interface/bluetoothinterface2.h"

#define serviceName "com.sinux.bluetoothmodule"
#define serviceInterface "/bluetoothmodule"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();

    if(!bus.registerService(serviceName))
        qDebug()<<"bus error :"<<bus.lastError().message();

    qDBusRegisterMetaType<BluetoothAddressData>();
    qDBusRegisterMetaType<BluetoothTransData>();
    qDBusRegisterMetaType<BluetoothAppidReceiveDataCallback>();


    BluetoothInterface2 *bluetoothInterface2 = BluetoothInterface2::getInstance();

    bool bluetoothInterface2Reg2 = bus.registerObject(serviceInterface,bluetoothInterface2,QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);

    if(!bluetoothInterface2Reg2)
        qDebug()<<"bus error :"<<"register bluetoothInterface2 false" ;

    bluetoothInterface2->onOpenBluetooth();

    return a.exec();
}
