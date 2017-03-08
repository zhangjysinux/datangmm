#ifndef SIGNALTEST_H
#define SIGNALTEST_H

#include <QObject>
#include <QDBusVariant>
#include <QDBusArgument>

class SignalTest : public QObject
{
    Q_OBJECT
public:
    SignalTest(QObject *parent = 0);

public slots:
    void onNumberTest(int number);
    void onStringTest(QString str);
    void onStructTest(QDBusVariant value);
};


#endif // SIGNALTEST_H
