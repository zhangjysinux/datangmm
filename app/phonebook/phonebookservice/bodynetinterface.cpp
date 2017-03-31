#include "bodynetinterface.h"
#include <QDebug>
#include <QDBusInterface>
#include <QTimer>

#define BodyNetServiceName "com.coi208.bodynetservice"
#define BodyNetPath "/Service"
#define BodyNetDbus "local.BodyNetService"

BodyNetInterface *BodyNetInterface::m_instance = NULL;
BodyNetInterface::BodyNetInterface(QObject *parent) : QObject(parent)
//    m_interface("com.sinux.backupdate", "/bluetooth", "com.sinux.DBus.bluetooth", QDBusConnection::sessionBus())
{

    QTimer::singleShot(2000,this,SLOT(init()));
}

BodyNetInterface *BodyNetInterface::getInstance()
{
    if(m_instance == NULL)
        m_instance = new BodyNetInterface();

    return m_instance;
}

void BodyNetInterface::onSignalShortCutKey(int keyValue)
{
    emit signalShortCutKey(keyValue);
}

void BodyNetInterface::init()
{
    qDebug() << "BodyNetInterface::init()";
    m_interface = new QDBusInterface(BodyNetServiceName, BodyNetPath, BodyNetDbus, QDBusConnection::sessionBus(),this);
    if (!m_interface->isValid())
        qDebug() << "error in BodyNetInterface";
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect(BodyNetServiceName,
                       BodyNetPath,
                       BodyNetDbus,
                       "SignalShortCutKey",
                       this,
                       SLOT(onSignalShortCutKey(int)));
}
