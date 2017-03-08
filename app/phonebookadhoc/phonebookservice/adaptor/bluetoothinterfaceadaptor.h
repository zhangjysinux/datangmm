#ifndef BLUETOOTHINTERFACEADAPTOR_H
#define BLUETOOTHINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>

class BluetoothInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    static BluetoothInterfaceAdaptor *getInstance();

signals:

public slots:

private:
    explicit BluetoothInterfaceAdaptor(QObject *parent = 0);
private:
    static BluetoothInterfaceAdaptor *m_instance;
    QDBusInterface m_interface;
};

#endif // BLUETOOTHINTERFACEADAPTOR_H
