#include <SyberosServiceCache>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>
#include <QDBusMetaType>
#include "./interface/bluetoothinterface2.h"
#include "bluetoothMoudleService_workspace.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();

    if(!bus.registerService("com.sinux.bluetoothmodule"))
        qDebug()<<"bus error :"<<bus.lastError().message();

    qDBusRegisterMetaType<BluetoothAddressData>();
    qDBusRegisterMetaType<BluetoothTransData>();
    qDBusRegisterMetaType<BluetoothAppidReceiveDataCallback>();


    BluetoothInterface2 *bluetoothInterface2 = BluetoothInterface2::getInstance();

    bool bluetoothInterface2Reg2 = bus.registerObject("/bluetoothmodule",bluetoothInterface2,QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);

    if(!bluetoothInterface2Reg2)
        qDebug()<<"bus error :"<<"register bluetoothInterface2 false" ;

    bluetoothInterface2->onOpenBluetooth();

    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new bluetoothMoudleService_Workspace());
}

