#include <SyberosServiceCache>
#include "TerminalcallService_workspace.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService("com.sinux.dial"))
        qDebug() << "bus error:" << bus.lastError().message();

    DialInterface *dial = DialInterface::getInstance();
    bool dialReg = bus.registerObject("/dial", dial, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!dialReg)
        qDebug() << "bus error:" << "register DialInterface false";

    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new TerminalcallService_Workspace());
}

