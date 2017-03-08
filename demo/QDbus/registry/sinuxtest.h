#ifndef SINUXTEST_H
#define SINUXTEST_H

#include <QObject>
#include <QDBusVariant>
#include "structtest.h"

class SinuxTest : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.sinuxtest")
public:
    explicit SinuxTest(QObject *parent = 0);

signals:
    void signalTest1(int number);

    void signalTest2(QString str);

    void signalTest3(QDBusVariant);

public slots:
    int onTest1();

    QString onTest2();

    QDBusVariant onTest3();

    int onTest4();//emit signal
};

#endif // SINUXTEST_H
