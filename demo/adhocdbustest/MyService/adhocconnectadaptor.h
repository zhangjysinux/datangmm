#ifndef ADHOCCONNECTADAPTOR_H
#define ADHOCCONNECTADAPTOR_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusError>
#include <QDebug>
#include <QDBusArgument>
#include <adhocdata.h>

#define serviceName "org.ofono"
#define servicePath "/manet"
#define serviceInterfaceName "org.ofono.Manet"


class AdhocConnectAdaptor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.adhoc")
public:
    explicit AdhocConnectAdaptor(QObject *parent = 0);
    static AdhocConnectAdaptor *getInstance();
signals:

public slots:
    QString onGetDevIp();
    void onLinkSetup(const Stru_mcmwnu_link_setup_req &link_setup);

private:
    static AdhocConnectAdaptor *instance;
    QDBusInterface m_interface;
};

#endif // ADHOCCONNECTADAPTOR_H
