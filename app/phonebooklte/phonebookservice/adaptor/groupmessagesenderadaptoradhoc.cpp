#include "groupmessagesenderadaptoradhoc.h"
#include <QDBusReply>
#include <QDebug>
#include <QDBusMetaType>
#include <QThread>
#include "../../commondata/datatype/groupdata.h"

#define serviceName "com.dtt.GroupIntercomService"
#define servicePath "/GroupManager"
#define serviceInterfaceName "com.dtt.GroupIntercomService.GroupManager"

#define serviceNameMessage "com.dtt.GroupIntercomService"
#define servicePathMessage "/GroupCommunicationManager"
#define serviceInterfaceNameMessage "com.dtt.GroupIntercomService.GroupCommunicationManager"

GroupMessageSenderAdaptorAdhoc *GroupMessageSenderAdaptorAdhoc::instance = NULL;

GroupMessageSenderAdaptorAdhoc *GroupMessageSenderAdaptorAdhoc::getInstance()
{
    if(instance == NULL)
        instance = new GroupMessageSenderAdaptorAdhoc();

    return instance;
}

GroupMessageSenderAdaptorAdhoc::GroupMessageSenderAdaptorAdhoc(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_interfaceGroup(serviceName,
                     servicePath,
                     serviceInterfaceName,
                     QDBusConnection::sessionBus()),
    m_interfaceMessage(serviceNameMessage,
                       servicePathMessage,
                       serviceInterfaceNameMessage,
                       QDBusConnection::sessionBus())
{
    qDebug() << "m_interfaceGroup.isValid()" << m_interfaceGroup.isValid();
    qDebug() << "m_interfaceMessage.isValid()" << m_interfaceMessage.isValid();

    qDBusRegisterMetaType<QT_DTT::UserInfo>();
    qRegisterMetaType<QT_DTT::UserInfo>("QT_DTT::UserInfo");
    qDBusRegisterMetaType<QT_DTT::GroupInfo>();
    qRegisterMetaType<QT_DTT::GroupInfo>("QT_DTT::GroupInfo");
    qDBusRegisterMetaType<QT_DTT::GroupTransMsg>();
    qRegisterMetaType<QT_DTT::GroupTransMsg>("QT_DTT::GroupTransMsg");

    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    setNetworkType(DIGITAL_WALKIE_TALKIE_NETWORK);
}

void GroupMessageSenderAdaptorAdhoc::createGroup(QT_DTT::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("createGroup", QVariant::fromValue<QT_DTT::GroupInfo>(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorAdhoc::createGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorAdhoc createGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorAdhoc::removeGroup(QT_DTT::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("removeGroup", QVariant::fromValue(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorAdhoc::removeGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorAdhoc removeGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorAdhoc::updateGroup(QT_DTT::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("updateGroup",
                                                        QVariant::fromValue(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorAdhoc::updateGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorAdhoc updateGroup reply error:" << retValDbus.error();
    }
    return;
}
