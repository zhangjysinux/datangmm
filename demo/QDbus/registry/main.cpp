#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QDebug>
#include "sinuxtest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDBusRegisterMetaType<StructTest>();
    qDBusRegisterMetaType<StructTestList>();

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService("com.sinux.test"))
    {
        qDebug() << "bus error:" << bus.lastError().message();
    }

    SinuxTest test;
    bool reg = bus.registerObject("/", &test, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!reg)
    {
        qDebug() << "bus error:" << "register sinuxtest false";
    }

    qDebug() << "registry sinux test success.";

    return a.exec();
}

