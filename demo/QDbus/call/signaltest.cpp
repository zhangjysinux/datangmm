#include <QDebug>
#include "signaltest.h"
#include "structtest.h"

SignalTest::SignalTest(QObject *parent)
    :QObject(parent)
{

}

void SignalTest::onNumberTest(int number)
{
    qDebug() << "signal test number:" << number;
}

void SignalTest::onStringTest(QString str)
{
    qDebug() << "signal test string:" << str;
}

void SignalTest::onStructTest(QDBusVariant value)
{
    QDBusArgument argument = value.variant().value<QDBusArgument>();
    QList<StructTest> values = qdbus_cast<QList<StructTest> >(argument);
    qDebug() << "signal test struct count:" << values.count();
    qDebug() << "signal test struct index 0 test1:" << values[0].test1;
    qDebug() << "signal test struct index 0 test2:" << values[0].test2;
    qDebug() << "signal test struct index 1 test1:" << values[1].test1;
    qDebug() << "signal test struct index 1 test2:" << values[1].test2;
}

