#include <SyberosServiceCache>
#include "MyService_workspace.h"
#include <QDBusConnection>
#include <QDBusError>
#include "dialinterface.h"
//#include "callhistoryinterface.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService("com.sinux.dial"))
        qDebug() << "bus error:" << bus.lastError().message();

    DialInterface *dial = DialInterface::getInstance();
    bool dialReg = bus.registerObject("/dial", dial, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!dialReg)
        qDebug() << "bus error:" << "register DialInterface false";

    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new MyService_Workspace());
}

