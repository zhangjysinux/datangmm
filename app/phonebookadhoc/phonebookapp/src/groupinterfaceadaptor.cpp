#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>
#include "groupinterfaceadaptor.h"
#include "../../../commondata/datatype/commondata.h"


GroupInterfaceAdaptor *GroupInterfaceAdaptor::m_instance = NULL;
GroupInterfaceAdaptor *GroupInterfaceAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new GroupInterfaceAdaptor();

    return m_instance;
}

bool GroupInterfaceAdaptor::addGroup(const Group &group)
{
    bool flag = false;
    QVariant groupVar = QVariant::fromValue(group);
    QDBusReply<bool> addReply = m_interface.call("onAddGroup", groupVar);
    if(addReply.isValid())
        flag = addReply.value();
    else
        qDebug() << "onAddGroup reply error:" << addReply.error();

    return flag;
}

bool GroupInterfaceAdaptor::removeGroup(const QString &id)
{
    bool flag = false;
    QDBusReply<bool> removeReply = m_interface.call("onRemoveGroup", id);
    if(removeReply.isValid())
        flag = removeReply.value();
    else
        qDebug() << "onRemoveGroup reply error:" << removeReply.error();

    return flag;
}

bool GroupInterfaceAdaptor::updateGroup(const Group &group)
{
    qDebug() << "updateGroup: " << group.id;
    bool flag = false;
    QVariant groupVar = QVariant::fromValue(group);
    QDBusReply<bool> updateReply = m_interface.call("onUpdateGroup", groupVar);
    if(updateReply.isValid())
        flag = updateReply.value();
    else
        qDebug() << "onUpdateGroup reply error:" << updateReply.error();

    return flag;
}

Group GroupInterfaceAdaptor::getGroup(const QString &id)
{
    Group value;
    QDBusReply<QDBusVariant> groupReply = m_interface.call("onGetGroup", id);
    if(groupReply.isValid())
    {
        QDBusVariant dbusVariant = groupReply.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        value = qdbus_cast<Group>(argument);
    }
    else
    {
        qDebug() << "onGetGroup reply error:" << groupReply.error();
    }

    return value;
}

QStringList GroupInterfaceAdaptor::getGroups(const GroupType &type)
{
    QStringList values;
    QDBusReply<QStringList> groupsReply = m_interface.call("onGetGroups", (int)type);
    if(groupsReply.isValid())
        values = groupsReply.value();
    else
        qDebug() << "onGetGroups reply error:" << groupsReply.error();

    return values;
}

QStringList GroupInterfaceAdaptor::getSelfCreatedGroups(const GroupType &type)
{
    QStringList values;
    QDBusReply<QStringList> groupsReply = m_interface.call("onGetSelfCreatedGroups", (int)type);
    if(groupsReply.isValid())
        values = groupsReply.value();
    else
        qDebug() << "onGetSelfCreatedGroups reply error:" << groupsReply.error();

    return values;
}

void GroupInterfaceAdaptor::onDbusSignalGroupChanged(int type, QDBusVariant data)
{
    qDebug() << "-----------------------receiveGroupChange------------------";
    qDebug() << "group change type:" << type;
    QDBusArgument argument = data.variant().value<QDBusArgument>();
    Group value = qdbus_cast<Group>(argument);
    qDebug() << "change group id:" << value.id;
    qDebug() << "change group name:" << value.name;
    qDebug() << "-----------------------end--------------------------------------";

    emit signalGroupChanged(type, value);
}

GroupInterfaceAdaptor::GroupInterfaceAdaptor(QObject *parent)
    : QObject(parent),
      m_interface(contacterServiceName, "/group", "com.sinux.DBus.group", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<Group>();
    qDBusRegisterMetaType<GroupList>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(contacterServiceName, "/group", "com.sinux.DBus.group", "signalGroupChanged",
                       this, SLOT(onDbusSignalGroupChanged(int, QDBusVariant)));
    sessionBus.connect(contacterServiceName, "/group", "com.sinux.DBus.group", "signalGroupExchange",
                       this, SIGNAL(signalGroupExchange()));
}

