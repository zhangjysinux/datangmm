#include <SyberosServiceCache>
#include "DownloaderContactInterfaceModuleService_workspace.h"
#include "./interface/downloadcontactinterface.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusInterface>
#include <QDebug>
#include <QTimer>

Q_DECL_EXPORT int main(int argc, char *argv[])
{

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if(!dbus.registerService("com.sinux.downloadercontactinterfacemoduleservice"))
        qDebug()<<"bus error:"<<dbus.lastError().message();
    DownloadContactInterface *downloaderContactInterface = DownloadContactInterface::getInstance();
    bool downloaderContactInterfaceReg = dbus.registerObject("/downloadcontact",downloaderContactInterface,QDBusConnection::ExportAllSignals|QDBusConnection::ExportAllSlots);
        if(!downloaderContactInterfaceReg)
            qDebug()<<"bus error:" <<"register DownloadContactInterface false";
    QTimer::singleShot(3000,downloaderContactInterface,SLOT(onStartGet()));
    downloaderContactInterface->onStartGet();

    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new DownloaderContactInterfaceModuleService_Workspace());
}

