#include <QDebug>
#include <QDBusReply>
#include "adhocadaptor.h"

AdHocAdaptor* AdHocAdaptor::m_instance = NULL;
AdHocAdaptor::AdHocAdaptor(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_groupInterface("com.dtt.GroupIntercomService", "/GroupManager",
                     "com.dtt.GroupIntercomService.GroupManager",
                     QDBusConnection::sessionBus()),
    m_groupCommuInterface("com.dtt.GroupIntercomService",
                          "/GroupCommunicationManager",
                          "com.dtt.GroupIntercomService.GroupCommunicationManager",
                          QDBusConnection::sessionBus())

{
    qDBusRegisterMetaType<QT_DTT_ADHOC::GroupInfo>();
    qDBusRegisterMetaType<QT_DTT_ADHOC::UserInfo>();
    qDBusRegisterMetaType<QT_DTT_ADHOC::GroupTransMsg>();
    qRegisterMetaType<QT_DTT_ADHOC::GroupInfo>("QT_DTT_ADHOC::GroupInfo");
    qRegisterMetaType<QT_DTT_ADHOC::UserInfo>("QT_DTT_ADHOC::UserInfo");
    qRegisterMetaType<QT_DTT_ADHOC::GroupTransMsg>("QT_DTT_ADHOC::GroupTransMsg");


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

    qDebug() << "AdHocAdaptor" << m_groupInterface.isValid() << m_groupCommuInterface.isValid();
    setNetworkType(AD_HOC_NETWORK);
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

void AdHocAdaptor::createGroupInfo(const QT_DTT_ADHOC::GroupInfo &info)
{
    QVariant infoVar = QVariant::fromValue(info);
    QDBusReply<void> createReply = m_groupInterface.call("createGroup", infoVar);
    if(createReply.isValid())
        CTS_PRINT << "adhoc create Group call success";
    else
        CTS_PRINT << "adhoc create Group Info error:" << createReply.error();
}

void AdHocAdaptor::removeGroupInfo(const QT_DTT_ADHOC::GroupInfo &info)
{
    QVariant infoVar = QVariant::fromValue(info);
    QDBusReply<void> removeReply = m_groupInterface.call("removeGroup", infoVar);
    if(removeReply.isValid())
        CTS_PRINT << "adhoc remove Group Info success";
    else
        CTS_PRINT << "adhoc remove Group Info error:" << removeReply.error();
    sleep(3);
}

void AdHocAdaptor::updateGroupInfo(const QT_DTT_ADHOC::GroupInfo &info)
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
    QT_DTT_ADHOC::GroupInfo mInfo = qdbus_cast<QT_DTT_ADHOC::GroupInfo>(argument);
    emit signalGroupUpdate(type, mInfo);

    Group group;

    group.id = mInfo.id;
    group.address = mInfo.addr;
    group.networkType = getNetowrkType();
    group.creator = mInfo.creator;
    group.port = mInfo.reserve.toInt();

    foreach(const QT_DTT_ADHOC::UserInfo &userInfo, mInfo.baseMembers)
        qDebug() << "userInfo.userId: " << userInfo.userId << userInfo.lte_pocId;

    QStringList members;
    foreach(const QT_DTT_ADHOC::UserInfo &userInfo, mInfo.baseMembers)
    {
        if(group.creator.toInt() != userInfo.userId)
            members.push_back(QString::number(userInfo.userId));
    }
    group.members = members;//give poc id to members
    group.name = mInfo.name;
    group.type = CONVERSION_GROUP;

    emit signalRecvGroupFromRemote(group, GROUP_ADD_HANDLE);
}

void AdHocAdaptor::onGroupTransSignalToGroupIntercomApp(QDBusVariant data)
{
    qDebug() << ".....................onGroupTransSignalToGroupIntercomApp................";
    QDBusArgument argument = data.variant().value<QDBusArgument>();
    QT_DTT_ADHOC::GroupTransMsg info = qdbus_cast<QT_DTT_ADHOC::GroupTransMsg>(argument);
    qDebug() << ".....................info text:" << info.text;
    qDebug() << ".....................info size:" << info.text.size();
    emit signalMessageChanged(info);
    emit signalNewMessage(converTransMessageToLocalMessage(info));
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

void AdHocAdaptor::adHocCreateMsg(QString muticastAddr)
{
    qDebug() << "AdHocAdaptor::adHocCreateMsg" << muticastAddr;

    QDBusInterface interface(
                "com.dtt.GroupIntercomService",
                "/GroupIntercom",
                "com.dtt.GroupIntercomService.GroupIntercom",
                QDBusConnection::sessionBus());

    QDBusReply<void> reply = m_groupCommuInterface.call("groupInfoRecv", muticastAddr);
    if(reply.isValid())
        CTS_PRINT << "call AdHocAdaptor::adHocCreateMsg success";
    else
        CTS_PRINT << "call AdHocAdaptor::adHocCreateMsg error:" << reply.error();
}

void AdHocAdaptor::setGroupIpsAndPath(const QString &groupId,
                                      const QString &address,
                                      const int &port,
                                      const QStringList &ips,
                                      const QString &path)
{
    GroupMessageController2 *adhocMessageSender = new GroupMessageController2(address, port, port+100, groupId);
    connect(adhocMessageSender, SIGNAL(signalNewMessage(Message)),
            this, SIGNAL(signalNewMessage(Message)));
    connect(adhocMessageSender, SIGNAL(signalReceivePercent(QString,QString,int,int)),
            this, SIGNAL(signalReceivePercent(QString,QString,int,int)));
    connect(adhocMessageSender, SIGNAL(signalSendPercent(QString,QString,int,int)),
            this, SIGNAL(signalSendPercent(QString,QString,int,int)));
    adhocMessageSender->setGroupIps(ips);
    adhocMessageSender->setReceivePath(path);
    adhocMessageSender->start();

    m_groupIdMessageSenderMap.insert(groupId, adhocMessageSender);
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

void AdHocAdaptor::sendMessage(const QT_DTT_ADHOC::GroupTransMsg &msg)
{
    QVariant msgVar = QVariant::fromValue(msg);
    QDBusReply<void> sendReply = m_groupCommuInterface.call("sendMessage", msgVar);
    if(sendReply.isValid())
        CTS_PRINT << "adhoc send message call success";
    else
        CTS_PRINT << "adhoc send message error:" << sendReply.error();
//    sleep(3);

}

qint64 AdHocAdaptor::sendMessage(const Message &message)
{
    if(message.common.type == TEXT_MESSAGE)
    {
        QT_DTT_ADHOC::GroupTransMsg transMessage = converLocalMessageToTransMessage(message);
        qDebug() << "................adhoc send text:" << transMessage.text;
        qDebug() << "................adhoc send size:" << transMessage.text.size();
        transMessage.network = AD_HOC_NETWORK;
        transMessage.optId = QT_DTT_ADHOC::GROUP_CTRL_MSG_UNRELIABLE_TRANS;
        sendMessage(transMessage);
    }
    else
    {
        GroupMessageController2 *adhocMessageSender =
                m_groupIdMessageSenderMap.value(message.common.groupId);
        if(adhocMessageSender != NULL)
        {
            adhocMessageSender->sendMessage(message);
        }
    }

    return 0;
}

//1--succ  0--fail
void AdHocAdaptor::onAdHocCreateGroupResult(QDBusVariant infoVar, int reason)
{
    CTS_PRINT << "onAdHocCreateGroupResult" << reason;
    QDBusArgument argument = infoVar.variant().value<QDBusArgument>();
    QT_DTT_ADHOC::GroupInfo info = qdbus_cast<QT_DTT_ADHOC::GroupInfo>(argument);
    emit signalAdHocCreateGroupResult(reason, info);
}

AdHocAdaptor *AdHocAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new AdHocAdaptor();

    return m_instance;
}

QT_DTT_ADHOC::GroupTransMsg AdHocAdaptor::converLocalMessageToTransMessage(const Message &message)
{
    QT_DTT_ADHOC::GroupTransMsg msg;
    msg.id = message.common.id;
    msg.groupId = message.common.groupId;
    msg.network = message.networkType;
    msg.addr = message.addr;
    msg.description = message.common.userId;
    switch (message.common.type)
    {
    case TEXT_MESSAGE:
    {
        msg.type = QT_DTT_ADHOC::MSG_TYPE_TEXT;
        msg.text = message.content;
        break;
    }
    }
    msg.optId = message.optId;
    msg.starttime = convertDateTimeToString(message.common.date);
    msg.direction = 0;//send or receive
    msg.status = message.common.state;
    msg.threadId = reinterpret_cast<int>(thread()->currentThreadId());

    return msg;
}

Message AdHocAdaptor::converTransMessageToLocalMessage(const QT_DTT_ADHOC::GroupTransMsg &transMessage)
{
    Message msg;
    msg.common.id = transMessage.id;
    msg.common.groupId = transMessage.groupId;
    msg.networkType = transMessage.network;
    msg.addr = transMessage.addr;
    msg.common.userId = transMessage.description;//description as userid
    switch (transMessage.type)
    {
    case QT_DTT_ADHOC::MSG_TYPE_TEXT:
    {
        msg.common.type = TEXT_MESSAGE;
        msg.content = transMessage.text;
        msg.common.state = MESSAGE_DOWNLOAD_SUCCESS;
        break;
    }
    default:
        break;
    }
    msg.optId = transMessage.optId;
    msg.common.date = convertStringToDateTime(transMessage.starttime);

    return msg;
}
