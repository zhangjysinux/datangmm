#include <QCoreApplication>
#include <SyberosServiceCache>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QDBusMetaType>
#include "./interface/pointtopointinterface.h"

#define serviceName "com.sinux.bus.pointtopointmodule"
#define serviceInterface "/pointtopoint"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService(serviceName))
        qDebug()<<"bus error :"<<bus.lastError().message();

    qDBusRegisterMetaType<IdNetworkMap>();

    PointToPointInterface *pointToPointInterface = PointToPointInterface::getInstance();

    bool pointToPointInterfaceReg = bus.registerObject(serviceInterface,pointToPointInterface,QDBusConnection::ExportAllSignals|QDBusConnection::ExportAllSlots);
    if(!pointToPointInterfaceReg)
        qDebug()<<" bus error :"<<"register pointToPointInterface failed";

    pointToPointInterface->onSetPort(33249);

    return a.exec();
}

