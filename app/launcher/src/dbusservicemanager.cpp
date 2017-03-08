#include "dbusservicemanager.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QDebug>
#include "unreadmessagesetter.h"

DbusServiceManager::DbusServiceManager(QObject *parent) :
    QObject(parent), m_unreadMessageSetter(NULL)
{
    m_unreadMessageSetter = new UnreadMessageSetter(this);
    connect(this, SIGNAL(signalGroupMessageButtonClicked()),
            m_unreadMessageSetter, SIGNAL(signalGroupMessageButtonClicked()));
    connect(this, SIGNAL(signalMessageButtonClicked()),
            m_unreadMessageSetter, SIGNAL(signalMessageButtonClicked()));
}

DbusServiceManager::~DbusServiceManager()
{
    if (m_unreadMessageSetter)
        delete m_unreadMessageSetter;
}

void DbusServiceManager::createService()
{
    // 用于建立到session bus的连接
    QDBusConnection bus = QDBusConnection::sessionBus();

    // 在session bus上注册名为"com.sinux.launcher"的service
    if (!bus.registerService("com.sinux.launcher"))
    {
        qDebug() << "DbusServiceManager::createService()" << bus.lastError().message();
        return;
    }
    // 注册名为"/unreadMessageSetter/registry"的object。
    // "QDBusConnection::ExportAllSlots"

    // 表示把类Hotel的所有Slot都导出为这个Object的method
    bus.registerObject("/unreadMessageSetter", m_unreadMessageSetter,
                       QDBusConnection::ExportAllSlots);
}
