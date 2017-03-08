#ifndef STRUCTTEST
#define STRUCTTEST

#include <QDBusArgument>

struct StructTest
{
    QString test1;
    int test2;
};

typedef QList<StructTest> StructTestList;
Q_DECLARE_METATYPE(StructTest)

// Marshall the MyStructure data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const StructTest &structTest)
{
    argument.beginStructure();
    QString t1 = structTest.test1;
    int t2 = structTest.test2;
    argument << t1;
    argument << t2;
    argument.endStructure();
    return argument;
}

// Retrieve the structTest data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, StructTest &structTest)
{
    QString t1;
    int t2;
    argument.beginStructure();
    argument >> t1;
    argument >> t2;
    argument.endStructure();

    structTest.test1 = t1;
    structTest.test2 = t2;
    return argument;
}

#endif // STRUCTTEST

