#include <QDebug>
#include <QDBusReply>
#include "adhocadaptor.h"


AdHocAdaptor* AdHocAdaptor::m_instance = NULL;
AdHocAdaptor::AdHocAdaptor(QObject *parent) :
    QObject(parent),
    m_groupInterface("com.dtt.GroupIntercomService", "/GroupManager",
                     "com.dtt.GroupIntercomService.GroupManager",
                     QDBusConnection::sessionBus()),
    m_groupCommuInterface("com.dtt.GroupIntercomService",
                          "/GroupCommunicationManager",
                          "com.dtt.GroupIntercomService.GroupCommunicationManager",
                          QDBusConnection::sessionBus())

{
    qDBusRegisterMetaType<GroupInfo>();
    qDBusRegisterMetaType<UserInfo>();
    qDBusRegisterMetaType<GroupTransMsg>();

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.dtt.GroupIntercomService", "/AdHocGroupManager",
                       "com.dtt.GroupIntercomService.GroupManager.AdHocGroupManager",
                       "updateAdHocGroup",
                       this, SLOT(onGroupUpdate(QDBusVariant,int)));
    sessionBus.connect("com.dtt.GroupIntercomService", "/AdHocGroup",
                       "com.dtt.GroupIntercomService.GroupManager.AdHocGroup",
                       "adHocCreateGroupResult",
                       this, SLOT(onAdHocCreateGroupResult(QDBusVariant,int)));
    sessionBus.connect(NULL, NULL,
                       "com.dtt.GroupIntercomService.GroupManager.Transaction",
                       "groupTransSignalToGroupIntercomApp",
                       this, SLOT(onGroupTransSignalToGroupIntercomApp(QDBusVariant)));
    sessionBus.connect(NULL, "/AdHocPtt",
                       "com.dtt.GroupIntercomService.AdHocPtt",
                       "adhocPTTPushOnResultToApp",
                       this, SLOT(onPttStateChanged(int,QString)));
    sessionBus.connect(NULL, "/AdHocPtt",
                       "com.dtt.GroupIntercomService.AdHocPtt",
                       "adhocPTTStatusToApp",
                       this, SLOT(onPttAllowChanged(int,QString)));
    //to do add message receive
    //to do add message send

    qDebug() << "AdHocAdaptor";
}

AdHocAdaptor::~AdHocAdaptor()
{
}

void AdHocAdaptor::testGroup()
{
    QDBusReply<void> createReply = m_groupInterface.call("testGroup");
    if(createReply.isValid())
        CTS_PRINT << "adhoc testGroup call success";
    else
        CTS_PRINT << "adhoc testGroup Info error:" << createReply.error();
}

void AdHocAdaptor::createGroupInfo(const GroupInfo &info)
{
    QVariant infoVar = QVariant::fromValue(info);
    QDBusReply<void> createReply = m_groupInterface.call("createGroup", infoVar);
    if(createReply.isValid())
        CTS_PRINT << "adhoc create Group call success";
    else
        CTS_PRINT << "adhoc create Group Info error:" << createReply.error();
}

void AdHocAdaptor::removeGroupInfo(const GroupInfo &info)
{
    QVariant infoVar = QVariant::fromValue(info);
    QDBusReply<void> removeReply = m_groupInterface.call("removeGroup", infoVar);
    if(removeReply.isValid())
        CTS_PRINT << "adhoc remove Group Info success";
    else
        CTS_PRINT << "adhoc remove Group Info error:" << removeReply.error();
    sleep(3);
}

void AdHocAdaptor::updateGroupInfo(const GroupInfo &info)
{
    QVariant infoVar = QVariant::fromValue(info);
    QDBusReply<void> updateReply = m_groupInterface.call("updateGroup", infoVar,1);
    if(updateReply.isValid())
        CTS_PRINT << "adhoc update Group Info success";
    else
        CTS_PRINT << "adhoc update Group Info error:" << updateReply.error();

}


//0--creat  1--delete  2--update
void AdHocAdaptor::onGroupUpdate(QDBusVariant infoVar,int type)
{
    CTS_PRINT << type;
    QDBusArgument argument = infoVar.variant().value<QDBusArgument>();
    GroupInfo info = qdbus_cast<GroupInfo>(argument);
    emit signalGroupUpdate(type, info);
}

void AdHocAdaptor::onGroupTransSignalToGroupIntercomApp(QDBusVariant data)
{
    qDebug() << "onGroupTransSignalToGroupIntercomApp";
    QDBusArgument argument = data.variant().value<QDBusArgument>();
    GroupTransMsg info = qdbus_cast<GroupTransMsg>(argument);
    emit signalMessageChanged(info);
}

void AdHocAdaptor::onSetPtt(QString ip, bool on)
{
    if (on)
    {
        QDBusReply<void> removeReply = m_groupCommuInterface.call("groupAdHocPTTTalkRequest", ip);
        if(removeReply.isValid())
            CTS_PRINT << "adhoc groupAdHocPTTTalkRequest success";
        else
            CTS_PRINT << "adhoc groupAdHocPTTTalkRequest error:" << removeReply.error();
    }
    else
    {
        QDBusReply<void> removeReply = m_groupCommuInterface.call("groupAdHocPTTTalkRelease", ip);
        if(removeReply.isValid())
            CTS_PRINT << "adhoc groupAdHocPTTTalkRelease success";
        else
            CTS_PRINT << "adhoc groupAdHocPTTTalkRelease error:" << removeReply.error();
    }
}

void AdHocAdaptor::onPttStateChanged(int status, QString ip)
{
    CTS_PRINT << status << ip;
    emit signalPttStateChanged(status, ip);
}

void AdHocAdaptor::onPttAllowChanged(int status, QString ip)
{
    CTS_PRINT << status << ip;
    emit signalPttAllowChanged(status, ip);
}

void AdHocAdaptor::sendMessage(const GroupTransMsg &msg)
{
    QVariant msgVar = QVariant::fromValue(msg);
    QDBusReply<void> sendReply = m_groupCommuInterface.call("sendMessage", msgVar);
    if(sendReply.isValid())
        CTS_PRINT << "adhoc send message call success";
    else
        CTS_PRINT << "adhoc send message error:" << sendReply.error();
    sleep(3);

}

//1--succ  0--fail
void AdHocAdaptor::onAdHocCreateGroupResult(QDBusVariant infoVar, int reason)
{
    CTS_PRINT << "onAdHocCreateGroupResult" << reason;
    QDBusArgument argument = infoVar.variant().value<QDBusArgument>();
    GroupInfo info = qdbus_cast<GroupInfo>(argument);
    emit signalAdHocCreateGroupResult(reason, info);
}

AdHocAdaptor *AdHocAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new AdHocAdaptor();

    return m_instance;
}
