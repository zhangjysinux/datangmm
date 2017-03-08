#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include "connection.h"
#include "contacterdata.h"
#include "contacterinterface.h"
#include "groupinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!createConnection())
        return 1;

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService("com.sinux.contact"))
        qDebug() << "bus error:" << bus.lastError().message();

    ContacterInterface contacter;
    bool contacterReg = bus.registerObject("/contacter", &contacter, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterReg)
        qDebug() << "bus error:" << "register ContacterInterface false";

    GroupInterface group;
    bool groupReg = bus.registerObject("/group", &group, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!groupReg)
        qDebug() << "bus error:" << "register GroupInterface false";

    qDebug() << "registry contact interface finished.";
    return a.exec();
}

