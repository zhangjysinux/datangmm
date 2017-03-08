#include "sinuxtest.h"

SinuxTest::SinuxTest(QObject *parent) :
    QObject(parent)
{

}

int SinuxTest::onTest1()
{
    return 1;
}

QString SinuxTest::onTest2()
{
    return "hello world";
}

int SinuxTest::onTest4()
{
    emit signalTest1(100200);
    emit signalTest2("signal hello world");

    QList<StructTest> structs;
    StructTest struct1;
    struct1.test1 = "signal strcut1 test1 string";
    struct1.test2 = 123;
    structs.append(struct1);
    StructTest struct2;
    struct2.test1 = "signal struct2 test1 string";
    struct2.test2 = 321;
    structs.append(struct2);
    QVariant value;
    value = QVariant::fromValue(structs);
    QDBusVariant dbusValue;
    dbusValue.setVariant(value);

    emit signalTest3(dbusValue);

    return 0;
}

QDBusVariant SinuxTest::onTest3()
{
    QList<StructTest> structs;

    StructTest struct1;
    struct1.test1 = "strcut1 test1 string";
    struct1.test2 = 123;
    structs.append(struct1);

    StructTest struct2;
    struct2.test1 = "struct2 test1 string";
    struct2.test2 = 321;
    structs.append(struct2);

    QVariant value;
    value = QVariant::fromValue(structs);

    QDBusVariant dbusValue;
    dbusValue.setVariant(value);

    return dbusValue;
}

