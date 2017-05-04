#include <QCoreApplication>
#include <SyberosServiceCache>
#include "./interface/downloadcontactinterface.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusInterface>
#include <QDebug>
#include <QTimer>

#define serviceName "com.sinux.downloadercontactinterfacemoduleservice"
#define serviceInterface "/downloadcontact"
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if(!dbus.registerService(serviceName))
        qDebug()<<"bus error:"<<dbus.lastError().message();
    DownloadContactInterface *downloaderContactInterface = DownloadContactInterface::getInstance();
    bool downloaderContactInterfaceReg = dbus.registerObject(serviceInterface,downloaderContactInterface,QDBusConnection::ExportAllSignals|QDBusConnection::ExportAllSlots);
        if(!downloaderContactInterfaceReg)
            qDebug()<<"bus error:" <<"register DownloadContactInterface false";
    QTimer::singleShot(3000,downloaderContactInterface,SLOT(onStartGet()));
    downloaderContactInterface->onStartGet();

    return a.exec();
}

