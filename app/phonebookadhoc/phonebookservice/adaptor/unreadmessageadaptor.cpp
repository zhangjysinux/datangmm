#include "unreadmessageadaptor.h"
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusError>
#include <QDebug>

UnreadMessageAdaptor *UnreadMessageAdaptor::m_instance = NULL;
QMutex UnreadMessageAdaptor::m_lock;

UnreadMessageAdaptor::UnreadMessageAdaptor(QObject *parent) :
    QObject(parent)
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.launcher",
                       "/unreadMessageSetter",
                       "com.sinux.launcher.unreadMessageSetter",
                       "signalGroupMessageButtonClicked",
                       this, SLOT(onDbusSignalGroupMessageButtonClicked()));
    sessionBus.connect("com.sinux.launcher",
                       "/unreadMessageSetter",
                       "com.sinux.launcher.unreadMessageSetter",
                       "signalMessageButtonClicked",
                       this, SLOT(onDbusSignalMessageButtonClicked()));
}

UnreadMessageAdaptor* UnreadMessageAdaptor::getInstance()
{
    if (m_instance == NULL)
    {
        m_lock.lock();
        if (m_instance == NULL)
        {
            m_instance = new UnreadMessageAdaptor();
        }
        m_lock.unlock();
    }
    return m_instance;
}

bool UnreadMessageAdaptor::setUnreadGroupMessage(int count)
{
    // 创建QDBusInterface
    QDBusInterface iface( "com.sinux.launcher",
                          "/unreadMessageSetter",
                          "com.sinux.launcher.unreadMessageSetter",
                          QDBusConnection::sessionBus());
    if (!iface.isValid())
    {
        qDebug() << "UnreadMessageAdaptor::setUnreadGroupMessage" << qPrintable(QDBusConnection::sessionBus(). lastError().message());
        return false;
    }

    // 呼叫远程的setUnreadGroupMessageCount
    QDBusReply<bool> reply = iface.call("setUnreadGroupMessageCount", count);
    if (!reply.isValid())
        return false;

    return reply.value();
}

bool UnreadMessageAdaptor::setUnreadMessage(int count)
{
    // 创建QDBusInterface
    QDBusInterface iface( "com.sinux.launcher",
                          "/unreadMessageSetter",
                          "com.sinux.launcher.unreadMessageSetter",
                          QDBusConnection::sessionBus());
    if (!iface.isValid())
    {
        qDebug() << "UnreadMessageAdaptor::setUnreadGroupMessage" << qPrintable(QDBusConnection::sessionBus(). lastError().message());
        return false;
    }

    // 呼叫远程的setUnreadMessageCount
    QDBusReply<bool> reply = iface.call("setUnreadMessageCount", count);
    if (!reply.isValid())
        return false;

    return reply.value();
}

void UnreadMessageAdaptor::onDbusSignalGroupMessageButtonClicked()
{
    qDebug() << "UnreadMessageAdaptor::onDbusSignalGroupMessageButtonClicked()";
}

void UnreadMessageAdaptor::onDbusSignalMessageButtonClicked()
{
    qDebug() << "UnreadMessageAdaptor::onDbusSignalMessageButtonClickd()";
}
