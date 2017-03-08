#include "groupmessagesenderadaptorbt.h"
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

GroupMessageSenderAdaptorBt *GroupMessageSenderAdaptorBt::instance = NULL;

GroupMessageSenderAdaptorBt *GroupMessageSenderAdaptorBt::getInstance()
{
    if(instance == NULL)
        instance = new GroupMessageSenderAdaptorBt();

    return instance;
}

GroupMessageSenderAdaptorBt::GroupMessageSenderAdaptorBt(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_interfaceGroup(serviceName,
                     servicePath,
                     serviceInterfaceName,
                     QDBusConnection::sessionBus()),
    m_interfaceMessage(serviceNameMessage,
                       servicePathMessage,
                       serviceInterfaceNameMessage,
                       QDBusConnection::sessionBus()),
    m_pttSetAllow(true)
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
    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyStatusSignal",
                       this,
                       SLOT(onNotifyStatusSignal(int)));
    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyPTTGroupSignal",
                       this,
                       SLOT(onNotifyPTTGroupSignal(QStringList)));
    sessionBus.connect(serviceNameMessage,
                       servicePathMessage,
                       serviceInterfaceNameMessage,
                       "incommMessageSignal",
                       this,
                       SLOT(onIncommMessageSignal(QT_DTT::GroupTransMsg)));
    sessionBus.connect(serviceNameMessage,
                       servicePathMessage,
                       serviceInterfaceNameMessage,
                       "PttGroupMSGSignal",
                       this,
                       SLOT(onPttGroupMSGSignal(QString)));

    for(int i=0; i<10; i++)
        m_groupInfo.pttGroups.push_back(QString());

    setNetworkType(DIGITAL_WALKIE_TALKIE_NETWORK);

    m_groupInfo.network = DIGITAL_WALKIE_TALKIE_NETWORK;
}

void GroupMessageSenderAdaptorBt::createGroup(QT_DTT::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("createGroup", QVariant::fromValue<QT_DTT::GroupInfo>(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::createGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt createGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::removeGroup(QT_DTT::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("removeGroup", QVariant::fromValue(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::removeGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt removeGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::updateGroup(QT_DTT::GroupInfo info, int groupOpt)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("updateGroup",
                                                        QVariant::fromValue(info),
                                                        groupOpt);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::updateGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt updateGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::getPTTGroup(int workMode)
{
    QDBusReply<void> retValDbus = m_interfaceGroup.call("getPTTGroup",
                                                        workMode);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::getPTTGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt getPTTGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::setWorkMode(const int &workMode)
{
    qDebug() << "GroupMessageSenderAdaptorBt::setWorkMode" << workMode;
    m_groupInfo.pttWorkMode = workMode;
}

void GroupMessageSenderAdaptorBt::addGroup(const int &index, const QString &pttId)
{
    qDebug() << "GroupMessageSenderAdaptorBt::addGroup" << index << pttId;
    m_groupInfo.pttGroups.replace(index, pttId);
    createGroup(m_groupInfo);
}

void GroupMessageSenderAdaptorBt::addGroupMap(const QString &pttGroupId, const QString &uiGroupId)
{
    m_pttGroupUiGroupMap.insert(pttGroupId, uiGroupId);
    m_uiGroupPttGroupMap.insert(uiGroupId, pttGroupId);
}

void GroupMessageSenderAdaptorBt::removeGroup(const int &index)
{
    qDebug() << "GroupMessageSenderAdaptorBt::removeGroup" << index;
    QString pttGroupId = m_groupInfo.pttGroups.value(index);
    QString uiGroupId = m_pttGroupUiGroupMap.value(pttGroupId);
    m_pttGroupUiGroupMap.remove(pttGroupId);
    m_uiGroupPttGroupMap.remove(uiGroupId);

    m_groupInfo.pttGroups.replace(index, QString());
    removeGroup(m_groupInfo);
}

void GroupMessageSenderAdaptorBt::updateGroup(const int &index, const QString &pttId)
{
    m_groupInfo.pttGroups.replace(index, pttId);
    updateGroup(m_groupInfo, 0);
}

int GroupMessageSenderAdaptorBt::findPttGroupIndex(const QString &pttId)
{
    int index = -1;
    for(int i=0; i<m_groupInfo.pttGroups.count(); i++)
    {
        const QString &pId = m_groupInfo.pttGroups.value(i);
        if(pId == pttId)
        {
            index = i;
            break;
        }
    }
    return index;
}

int GroupMessageSenderAdaptorBt::getUseableGroupIndex()
{
    int index = -1;
    for(int i=0; i<m_groupInfo.pttGroups.count(); i++)
    {
        const QString &pId = m_groupInfo.pttGroups.value(i);
        if(pId.isEmpty())
        {
            index = i;
            break;
        }
    }
    return index;
}

qint64 GroupMessageSenderAdaptorBt::sendMessage(const Message &message)
{
    QT_DTT::GroupTransMsg transMsg = converLocalMessageToTransMessage(message);
    sendMessage(transMsg);
}

void GroupMessageSenderAdaptorBt::GroupPTTDial(QString groupId, int mode)
{
    QDBusReply<void> retValDbus = m_interfaceMessage.call("GroupPTTDial",
                                                        groupId,
                                                        mode);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::GroupPTTDial call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt GroupPTTDial reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::GroupPTTAnswer()
{
    QDBusReply<void> retValDbus = m_interfaceMessage.call("GroupPTTAnswer");
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::GroupPTTAnswer call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt GroupPTTAnswer reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::GroupPTTHangup()
{
    QDBusReply<void> retValDbus = m_interfaceMessage.call("GroupPTTHangup");
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::GroupPTTHangup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt GroupPTTHangup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorBt::GroupPTTSet(bool set)
{
    QDBusReply<void> retValDbus = m_interfaceMessage.call("GroupPTTSet", set);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::GroupPTTSet call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt GroupPTTSet reply error:" << retValDbus.error();
    }
    return;
}

bool GroupMessageSenderAdaptorBt::getPttSetAllow()
{
    return m_pttSetAllow;
}

QString GroupMessageSenderAdaptorBt::sendMessage(QT_DTT::GroupTransMsg msgData)
{
    QDBusReply<QString> retValDbus = m_interfaceMessage.call("sendMessage",
                                                          QVariant::fromValue<QT_DTT::GroupTransMsg>(msgData));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorBt::sendMessage call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorBt sendMessage reply error:" << retValDbus.error();
    }
    return retValDbus.value();
}

void GroupMessageSenderAdaptorBt::onNotifyStatusSignal(int status)
{
    qDebug() << "GroupMessageSenderAdaptorBt::onNotifyStatusSignal" << status;
    switch (status) {
    case QT_DTT::GSTATUS_CREATE_SUCCESS:
    {
        signalCreateDeleteGroup(0);
        break;
    }
    case QT_DTT::GSTATUS_CREATE_FAIL:
    {
        signalCreateDeleteGroup(1);
        break;
    }
    case QT_DTT::GSTATUS_DELETE_SUCCESS:
    {
        signalCreateDeleteGroup(2);
        break;
    }
    case QT_DTT::GSTATUS_DELETE_FAIL:
    {
        signalCreateDeleteGroup(3);
        break;
    }
    default:
        break;
    }

    emit notifyStatusSignal(status);
}

void GroupMessageSenderAdaptorBt::onNotifyPTTGroupSignal(QStringList groupList)
{
    qDebug() << "GroupMessageSenderAdaptorBt::onNotifyPTTGroupSignal" << groupList;
    emit notifyPTTGroupSignal(groupList);
}

void GroupMessageSenderAdaptorBt::onIncommMessageSignal(QT_DTT::GroupTransMsg msg)
{
    qDebug() << "GroupMessageSenderAdaptorBt::onIncommMessageSignal"
             << msg.id
             << msg.addr
             << msg.text;
    Message message = converTransMessageToLocalMessage(msg);
    emit signalNewMessage(message);
}

void GroupMessageSenderAdaptorBt::onPttGroupMSGSignal(QString msgState)
{
    qDebug() << "GroupMessageSenderAdaptorBt::onPttGroupMSGSignal"
             << msgState;
    if(msgState == "PttAllow:0")
    {
        m_pttSetAllow = false;
        emit signalAllowSetPttState(0);
    }
    else if(msgState == "PttAllow:1")
    {
        m_pttSetAllow = true;
        emit signalAllowSetPttState(1);
    }
    else if(msgState == "PttGetted:0")
    {
        emit signalSetBtPttState(0);
    }
    else if(msgState == "PttGetted:1")
    {
        emit signalSetBtPttState(1);
    }
    else if(msgState == "CallState:active")
    {
        emit signalJoinBtGroup(1);
        GroupPTTSet(false);
    }
    else if(msgState.contains("DialResult"))
    {
        emit signalJoinBtGroup(0);
    }
}

QT_DTT::GroupTransMsg GroupMessageSenderAdaptorBt::converLocalMessageToTransMessage(const Message &message)
{
    QT_DTT::GroupTransMsg msg;
    msg.id = message.common.id;
    msg.groupId = m_uiGroupPttGroupMap.value(message.common.groupId);
    msg.network = getNetowrkType();
    msg.addr = message.addr;
    if(message.common.type == TEXT_MESSAGE)
        msg.type = QT_DTT_JJF::MSG_TYPE_TEXT;

    msg.optId = message.optId;
    msg.starttime = convertDateTimeToString(message.common.date);
    msg.direction = 123456;//send or receive
    msg.status = message.common.state;
    msg.text = message.content;
//    msg.userCount = message.usersSendTo.count();
//    msg.userPocId = message.usersSendTo;
    msg.threadId = reinterpret_cast<int>(thread()->currentThreadId());

    return msg;
}

Message GroupMessageSenderAdaptorBt::converTransMessageToLocalMessage(const QT_DTT::GroupTransMsg &transMessage)
{
    if(!m_pttGroupUiGroupMap.contains(transMessage.groupId))
    {
        qDebug() << m_pttGroupUiGroupMap.keys() << transMessage.groupId;
        return Message();
    }

    Message msg;
    msg.common.id = QUuid::createUuid().toString();//transMessage.id;
    msg.common.groupId = m_pttGroupUiGroupMap.value(transMessage.groupId);
    msg.networkType = transMessage.network;
    msg.addr = transMessage.addr;
    switch (transMessage.type)
    {
    case QT_DTT_JJF::MSG_TYPE_TEXT:
    {
        msg.common.type = TEXT_MESSAGE;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_FILE:
    {
        msg.common.type = FILE_MESSAGE;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_PICTURE:
    {
        msg.common.type = IMAGE_MESSAGE;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_RECORD:
    {
        msg.common.type = AUDIO_MESSAGE;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_VIDEO:
    {
        msg.common.type = VIDEO_MESSAGE;
        break;
    }
    default:
        break;
    }

//    msg.common.type = transMessage.type;
    msg.optId = transMessage.optId;
    msg.common.date = QDateTime::currentDateTime();//convertStringToDateTime(transMessage.starttime);
//    msg.direction = 1;//send or receive
    msg.common.state = transMessage.status;
    msg.common.userId = QString::number(transMessage.direction);
    msg.content = transMessage.text;
//    msg.userCount = message.usersSendTo.count();
//    msg.usersSendTo = transMessage.userPocId;
//    msg.threadId = thread()->currentThreadId();

    return msg;
}
