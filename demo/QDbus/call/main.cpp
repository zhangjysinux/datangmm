#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QDebug>
#include "structtest.h"
#include "signaltest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDBusRegisterMetaType<StructTest>();
    qDBusRegisterMetaType<StructTestList>();

    //run interface call
    QDBusInterface testInterface("com.sinux.test", "/", "com.sinux.DBus.sinuxtest", QDBusConnection::sessionBus());
    if(!testInterface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    SignalTest signalTest;
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.test", "/", "com.sinux.DBus.sinuxtest", "signalTest1", &signalTest, SLOT(onNumberTest(int)));
    sessionBus.connect("com.sinux.test", "/", "com.sinux.DBus.sinuxtest", "signalTest2", &signalTest, SLOT(onStringTest(QString)));
    sessionBus.connect("com.sinux.test", "/", "com.sinux.DBus.sinuxtest", "signalTest3", &signalTest, SLOT(onStructTest(QDBusVariant)));

    //test1 call
    QDBusReply<int> reply1 = testInterface.call("onTest1");
    if(reply1.isValid())
    {
        int result = reply1.value();
        qDebug() << "sinux test1 result:" << result;
    }
    else
    {
        qDebug() << "sinux test1 reply error:" << reply1.error();
    }

    //test2 call
    QDBusReply<QString> reply2 = testInterface.call("onTest2");
    if(reply2.isValid())
    {
        QString result = reply2.value();
        qDebug() << "sinux test2 result:" << result;
    }
    else
    {
        qDebug() << "sinux test2 reply error:" << reply2.error();
    }

    //test3 call
    QDBusReply<QDBusVariant> reply3 = testInterface.call("onTest3");
    if(reply3.isValid())
    {
        QDBusVariant dbusVariant = reply3.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        QList<StructTest> values = qdbus_cast<QList<StructTest> >(argument);
        qDebug() << "sinux test3 values count:" << values.count();
        qDebug() << "sinxu test3 values index 0 test1:" << values[0].test1;
        qDebug() << "sinux test3 values index 0 test2:" << values[0].test2;
        qDebug() << "sinux test3 values index 1 test1:" << values[1].test1;
        qDebug() << "sinux test3 values index 1 test2:" << values[1].test2;
    }
    else
    {
        qDebug() << "sinux test3 reply error:" << reply3.error();
    }

    //test signal call
    QDBusReply<int> reply4 = testInterface.call("onTest4");
    if(!reply4.isValid())
    {
        qDebug() << "sinux test4 reply error:" << reply4.error();
    }
    return a.exec();
}
