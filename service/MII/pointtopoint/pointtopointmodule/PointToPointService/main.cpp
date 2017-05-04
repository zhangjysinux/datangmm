#include <SyberosServiceCache>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QDBusMetaType>
#include "PointToPointService_workspace.h"
#include "./interface/pointtopointinterface.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService("com.sinux.bus.pointtopointmodule"))
        qDebug()<<"bus error :"<<bus.lastError().message();

    qDBusRegisterMetaType<IdNetworkMap>();

    PointToPointInterface *pointToPointInterface = PointToPointInterface::getInstance();

    bool pointToPointInterfaceReg = bus.registerObject("/pointtopoint",pointToPointInterface,QDBusConnection::ExportAllSignals|QDBusConnection::ExportAllSlots);
    if(!pointToPointInterfaceReg)
        qDebug()<<" bus error :"<<"register pointToPointInterface failed";

    pointToPointInterface->onSetPort(33249);

    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new PointToPointService_Workspace());
}

