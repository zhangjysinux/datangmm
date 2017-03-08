#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>
#include "callhistoryinterfaceadaptor.h"

CallHistoryInterfaceAdaptor *CallHistoryInterfaceAdaptor::m_instance = NULL;
CallHistoryInterfaceAdaptor::CallHistoryInterfaceAdaptor(QObject *parent)
    : QObject(parent),
    m_interface(contacterServiceName, "/callhistory", "com.sinux.DBus.callhistory", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<CallHistory>();
    qDBusRegisterMetaType<CallHistoryList>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error myerr =:" << QDBusConnection::sessionBus().lastError().message();
    }
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(contacterServiceName, "/callhistory", "com.sinux.DBus.callhistory", "signalCallHistoryUpdate",
                       this, SLOT(onCallHistoryChanged(int, QDBusVariant)));
}

CallHistoryInterfaceAdaptor *CallHistoryInterfaceAdaptor::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CallHistoryInterfaceAdaptor();
    }

    return m_instance;
}

QStringList CallHistoryInterfaceAdaptor::getCallHistoryList()
{
    qDebug()<<"onGetCallHistoryList in app";
    QStringList values;
    QDBusReply<QStringList> historyReply = m_interface.call("onGetCallHistoryList");
    if(historyReply.isValid())
        values = historyReply.value();
    else
        qDebug() << "onGetCallHistoryList reply error:" << historyReply.error();

    return values;
}

CallHistory CallHistoryInterfaceAdaptor::getCallHistory(const QString &historyId)
{
    CallHistory value;
    QDBusReply<QDBusVariant> historyReply = m_interface.call("onGetCallHistory", historyId);
    if(historyReply.isValid())
    {
        QDBusVariant dbusVariant = historyReply.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        value = qdbus_cast<CallHistory>(argument);
    }
    else
    {
        qDebug() << "onGetCallHistory reply error:" << historyReply.error();
    }

    return value;
}

bool CallHistoryInterfaceAdaptor::addCallHistory(CallHistory &callHistory)
{
    bool flag = false;

    callHistory.dialHistoryId = QString("%1").arg(getCallHistoryCount() + 100);
    QVariant callHistoryVar = QVariant::fromValue(callHistory);
    QDBusReply<bool> addReply = m_interface.call("onAddCallHistory", callHistoryVar);
    if(addReply.isValid())
        flag = addReply.value();
    else
        qDebug() << "onAddCallHistory reply error:" << addReply.error();

    return flag;
}

int CallHistoryInterfaceAdaptor::getCallHistoryCount()
{
    int count = 0;
    QDBusReply<int> addReply = m_interface.call("onGetCallHistroyCount");
    if(addReply.isValid())
        count = addReply.value();
    else
        qDebug() << "onGetCallHistroyCount reply error:" << addReply.error();

    return count;
}

bool CallHistoryInterfaceAdaptor::removeCallHistory(const QString &historyId)
{
    bool flag = false;
    QDBusReply<bool> removeReply = m_interface.call("onRemoveCallHistory", historyId);
    if(removeReply.isValid())
        flag = removeReply.value();
    else
        qDebug() << "onRemoveCallHistory reply error:" << removeReply.error();

    return flag;
}

QStringList CallHistoryInterfaceAdaptor::getCallHistoryBytarget(QString target)
{
    qDebug()<<"getCallHistoryBytarget in app";
    QStringList values;
    QDBusReply<QStringList> historyReply = m_interface.call("onGetCallHistoryBytarget",target);
    if(historyReply.isValid())
        values = historyReply.value();
    else
        qDebug() << "getCallHistoryBytarget reply error:" << historyReply.error();

    return values;
}

bool CallHistoryInterfaceAdaptor::clearCallHistory()
{
    bool flag = false;
    QDBusReply<bool> removeReply = m_interface.call("onClearCallHistory");
    if(removeReply.isValid())
        flag = removeReply.value();
    else
        qDebug() << "onClearCallHistory reply error:" << removeReply.error();

    return flag;
}

void CallHistoryInterfaceAdaptor::onCallHistoryChanged(int type, QDBusVariant callHistory)
{
    qDebug() << "-----------------------receiveCallHistoryChanged------------------";
    QDBusArgument argument = callHistory.variant().value<QDBusArgument>();
    CallHistory value = qdbus_cast<CallHistory>(argument);
    emit signalCallHistoryChanged(value.dialHistoryId, type);
}

QStringList CallHistoryInterfaceAdaptor::searchCallHistory(const QString &searchText)
{
    QStringList values;
    QDBusReply<QStringList> historyReply = m_interface.call("onSearchCallHistory",searchText);
    if(historyReply.isValid())
        values = historyReply.value();
    else
        qDebug() << "onSearchCallHistory reply error:" << historyReply.error();

    return values;
}
