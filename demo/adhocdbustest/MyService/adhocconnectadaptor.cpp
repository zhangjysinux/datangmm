#include "adhocconnectadaptor.h"
#include <QDBusMetaType>
#include <QDBusReply>
#include <string>
#include <QDebug>
#include <QDBusError>

using std::string;


AdhocConnectAdaptor *AdhocConnectAdaptor::instance = NULL;

AdhocConnectAdaptor *AdhocConnectAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new AdhocConnectAdaptor();
    }

    return instance;
}

AdhocConnectAdaptor::AdhocConnectAdaptor(QObject *parent) :
    QObject(parent),
    m_interface(serviceName,
                servicePath,
                serviceInterfaceName,
                QDBusConnection::systemBus())
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    qDebug() << "bus create-----";
    if(!bus.registerService("com.sinux"))
        qDebug() << "bus error:" << bus.lastError().message();

    bool regflag = bus.registerObject("/adhoc", this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!regflag)
        qDebug() << "bus error:" << "register adhoc false";

    qDBusRegisterMetaType<stru_mcmwnu_group_info_cfg_req>();
    qDBusRegisterMetaType<Stru_mcmwnu_link_setup_req>();

    if(m_interface.isValid())
    {
        qDebug() << "ofnod is valid";
    }
    else
    {
        qDebug() << "m_interface error" << m_interface.lastError().message();
    }
    onGetDevIp();
    Stru_mcmwnu_link_setup_req link_setup;
    onLinkSetup(link_setup);
}

QString AdhocConnectAdaptor::onGetDevIp()
{
    qDebug() << "AdhocConnectAdaptor::onGetDevIp";

    QString value;

    QDBusReply<QString> reply = m_interface.call("GetDevIp");
    if(reply.isValid())
    {
        value = reply.value();
        qDebug() << "GetDevIp reply success:" << reply.error();
    }
    else
    {
        qDebug() << "GetDevIp reply error:" << reply.error();
    }

    return value;
}

void AdhocConnectAdaptor::onLinkSetup(const Stru_mcmwnu_link_setup_req &link_setup)
{
    qDebug() << "AdhocConnectAdaptor::onLinkSetup";

    QDBusReply<void> reply = m_interface.call("LinkSetup", QVariant::fromValue(link_setup));
    if(reply.isValid())
    {
        qDebug() << "LinkSetup reply success:" << reply.error();
    }
    else
    {
        qDebug() << "LinkSetup reply error:" << reply.error();
    }

    return;
}



