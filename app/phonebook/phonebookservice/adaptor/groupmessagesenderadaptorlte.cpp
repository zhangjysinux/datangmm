#include "groupmessagesenderadaptorlte.h"
#include <QDBusReply>
#include <QDebug>
#include <QDBusMetaType>
#include <QThread>
#include "../../commondata/datatype/groupdata.h"
#include "../groupinterface.h"
#include "connectiondatabase.h"

#define serviceName "com.dtt.GroupIntercomService"
#define servicePath "/GroupManager"
#define serviceInterfaceName "com.dtt.GroupIntercomService.GroupManager"

#define serviceNameMessage "com.dtt.GroupIntercomService"
#define servicePathMessage "/GroupCommunicationManager"
#define serviceInterfaceNameMessage "com.dtt.GroupIntercomService.GroupCommunicationManager"

const QString STEAM_OUT("/data/home/user/JJF/stream_out.pcm");
const QString STEAM_IN("/data/home/user/JJF/stream_in.pcm");

GroupMessageSenderAdaptorLte *GroupMessageSenderAdaptorLte::instance = NULL;

GroupMessageSenderAdaptorLte *GroupMessageSenderAdaptorLte::getInstance()
{
    if(instance == NULL)
        instance = new GroupMessageSenderAdaptorLte();

    return instance;
}

GroupMessageSenderAdaptorLte::GroupMessageSenderAdaptorLte(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_interface(serviceName,
                servicePath,
                serviceInterfaceName,
                QDBusConnection::sessionBus()),
    m_communInterface(serviceNameMessage,
                      servicePathMessage,
                      serviceInterfaceNameMessage,
                      QDBusConnection::sessionBus()),
    m_isLogin(false)
{
    if(!m_interface.isValid())
    {
        qDebug() << "can not connect : " << serviceInterfaceName;
    }
    else
    {
        qDebug() << "connect dbus success...";
    }
    setNetworkType(LTE_NETWORK);

//    qDebug() << "testinfo id:" << qDBusRegisterMetaType<testInfo>();
//    qRegisterMetaType<testInfo>("testInfo");
    qDBusRegisterMetaType<QT_DTT_JJF::UserInfo>();
    qRegisterMetaType<QT_DTT_JJF::UserInfo>("QT_DTT_JJF::UserInfo");
    qDBusRegisterMetaType<QT_DTT_JJF::GroupInfo>();
    qRegisterMetaType<QT_DTT_JJF::GroupInfo>("QT_DTT_JJF::GroupInfo");
    qDBusRegisterMetaType<QT_DTT_JJF::GroupTransMsg>();
    qRegisterMetaType<QT_DTT_JJF::GroupTransMsg>("QT_DTT_JJF::GroupTransMsg");

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "sendLoginStatus",
                       this,
                       SLOT(onSendLoginStatus(int)));
    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "sendPocStatusToApp",
                       this,
                       SLOT(onSendPocStatusToApp(QT_DTT_JJF::GroupTransMsg, int)));
    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "sentMessageInfoToApp",
                       this,
                       SLOT(onSentMessageInfoToApp(QT_DTT_JJF::GroupTransMsg,int)));
    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "recvMessageInfoToApp",
                       this,
                       SLOT(onRecvMessageInfoToApp(QT_DTT_JJF::GroupTransMsg,int)));


    sessionBus.connect(serviceNameMessage,
                       servicePathMessage,
                       serviceInterfaceNameMessage,
                       "sendLoginStatusToApp",
                       this,
                       SLOT(onSendLoginStatusToApp(int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyTalkBegin",
                       this,
                       SLOT(onNotifyTalkBegin(int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyTalkEnd",
                       this,
                       SLOT(onNotifyTalkEnd(int,int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyChannelConnect",
                       this,
                       SLOT(onNotifyChannelConnect(int,int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyChannelDisconnect",
                       this,
                       SLOT(onNotifyChannelDisconnect(int,int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyListenBegin",
                       this,
                       SLOT(onNotifyListenBegin(int,QString,QString)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyListenEnd",
                       this,
                       SLOT(onNotifyListenEnd(int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyRecordStop",
                       this,
                       SLOT(onNotifyRecordStop(int)));

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterfaceName,
                       "notifyPlayerStop",
                       this,
                       SLOT(onNotifyPlayerStop(int)));

    m_timer.setInterval(10000);
    m_timer.setSingleShot(false);
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(onLogin()));
    m_timer.start();

//    login("userid", "password", 0);
//    logout(0);
//    GroupInfo groupInfo;
//    groupInfo.id = "1";
//    createGroup(groupInfo);
//    removeGroup(0, "roomid");
//    updateGroup(GroupInfo());
//    GroupTransMsg groupMsg;
//    groupMsg.id = "";
//    sendMessage(groupMsg);
//    testInfo tt;
    //    dbustest(tt);
}

GroupMessageSenderAdaptorLte::~GroupMessageSenderAdaptorLte()
{
    logout(m_networkType);
}

void GroupMessageSenderAdaptorLte::login(QString userId,
                                         QString userPwd,
                                         int networkType,
                                         QString serverIp,
                                         int serverPort)
{
    Contacter localContacter = ContacterInterface::getInstance()->getLocalInfo();
    m_userId = localContacter.pocId;
    m_userPwd = userPwd;
    m_networkType = networkType;
    m_serverIp = serverIp;
    m_serverPort = serverPort;

    QDBusReply<void> retValDbus = m_interface.asyncCall("login", m_userId, m_userPwd, m_networkType, m_serverIp, m_serverPort);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::login call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte login reply error:" << retValDbus.error();
    }

    qDebug() << "+++++++++++++++++++++GroupMessageSenderAdaptorLte::onLogin()"
             << m_userId
             << m_userPwd
             << m_networkType
             << m_serverIp
             << m_serverPort;

    return;
}

void GroupMessageSenderAdaptorLte::onLogin()
{
    login(m_userId, m_userPwd, m_networkType,m_serverIp,m_serverPort);
}

void GroupMessageSenderAdaptorLte::logout(int network)
{
    QDBusReply<void> retValDbus = m_interface.call("logout", network);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::logout call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte logout reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorLte::logout()
{
    logout(m_networkType);
}

void GroupMessageSenderAdaptorLte::createGroup(QT_DTT_JJF::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interface.call("createGroup", QVariant::fromValue<QT_DTT_JJF::GroupInfo>(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::createGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte createGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorLte::removeGroup(QT_DTT_JJF::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interface.call("removeGroup", QVariant::fromValue(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::removeGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte removeGroup reply error:" << retValDbus.error();
    }
    return;
}

void GroupMessageSenderAdaptorLte::updateGroup(QT_DTT_JJF::GroupInfo info)
{
    QDBusReply<void> retValDbus = m_interface.call("updateGroup", QVariant::fromValue(info));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::updateGroup call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte updateGroup reply error:" << retValDbus.error();
    }
    return;
}

QString GroupMessageSenderAdaptorLte::sendMessage(QT_DTT_JJF::GroupTransMsg msgData)
{
    qDebug() << "interface.isValid()" << m_communInterface.isValid();
    QDBusReply<QString> retValDbus = m_communInterface.call("sendMessage", QVariant::fromValue(msgData));
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::sendMessage call success" << retValDbus.value();
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte sendMessage reply error:" << retValDbus.error();
    }
    return retValDbus.value();
}

QT_DTT_JJF::GroupTransMsg GroupMessageSenderAdaptorLte::converLocalMessageToTransMessage(const Message &message)
{
    QT_DTT_JJF::GroupTransMsg msg;
    msg.id = message.common.id;
    msg.groupId = message.common.groupId;
    msg.network = message.networkType;
    msg.addr = message.addr;
//    msg.type = message.common.type;
    QString fileName = message.contentName;
    switch (message.common.type)
    {
    case TEXT_MESSAGE:
    {
        msg.type = QT_DTT_JJF::MSG_TYPE_TEXT;
        msg.text = message.content;
        break;
    }
    case FILE_MESSAGE:
    {
        msg.type = QT_DTT_JJF::MSG_TYPE_FILE;
        msg.text = message.contentName.toLocal8Bit();
        break;
    }
    case IMAGE_MESSAGE:
    {
        msg.type = QT_DTT_JJF::MSG_TYPE_PICTURE;
        msg.text = message.contentName.toLocal8Bit();
        break;
    }
    case AUDIO_MESSAGE:
    {
        msg.type = QT_DTT_JJF::MSG_TYPE_RECORD;
        msg.text = message.contentName.toLocal8Bit();
        break;
    }
    case VIDEO_MESSAGE:
    {
        msg.type = QT_DTT_JJF::MSG_TYPE_VIDEO;
        msg.text = message.contentName.toLocal8Bit();
        break;
    }
    }
    msg.optId = message.optId;
    msg.starttime = convertDateTimeToString(message.common.date);
    msg.direction = 0;//send or receive
    msg.status = message.common.state;

//    msg.userCount = message.usersSendTo.count();
//    msg.userPocId = message.usersSendTo;
//    for
//    msg.baseMembers
    msg.threadId = reinterpret_cast<int>(thread()->currentThreadId());

    return msg;
}

Message GroupMessageSenderAdaptorLte::converTransMessageToLocalMessage(const QT_DTT_JJF::GroupTransMsg &transMessage)
{
    Message msg;
    msg.common.id = transMessage.id;
    msg.common.groupId = transMessage.groupId;
    msg.networkType = transMessage.network;
    msg.addr = transMessage.addr;
    switch (transMessage.type)
    {
    case QT_DTT_JJF::MSG_TYPE_TEXT:
    {
        msg.common.type = TEXT_MESSAGE;
        msg.content = transMessage.text;
        msg.common.state = MESSAGE_DOWNLOAD_SUCCESS;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_FILE:
    {
        msg.common.type = FILE_MESSAGE;
        msg.contentName = transMessage.extraMsg.toLocal8Bit();
        msg.common.state = MESSAGE_DOWNLOAD_READY;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_PICTURE:
    {
        msg.common.type = IMAGE_MESSAGE;
        msg.contentName = transMessage.extraMsg.toLocal8Bit();
        msg.common.state = MESSAGE_DOWNLOAD_READY;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_RECORD:
    {
        msg.common.type = AUDIO_MESSAGE;
        msg.contentName = transMessage.extraMsg.toLocal8Bit();
        msg.common.state = MESSAGE_DOWNLOAD_READY;
        break;
    }
    case QT_DTT_JJF::MSG_TYPE_VIDEO:
    {
        msg.common.type = VIDEO_MESSAGE;
        msg.contentName = transMessage.extraMsg.toLocal8Bit();
        msg.common.state = MESSAGE_DOWNLOAD_READY;
        break;
    }
    default:
        break;
    }

//    msg.common.type = transMessage.type;
    msg.optId = transMessage.optId;
    msg.common.date = convertStringToDateTime(transMessage.starttime);
//    msg.direction = 1;//send or receive
//    msg.common.state = transMessage.status;
//    msg.userCount = message.usersSendTo.count();
//    msg.usersSendTo = transMessage.userPocId;
//    msg.threadId = thread()->currentThreadId();

    return msg;
}

void GroupMessageSenderAdaptorLte::onSendLoginStatus(int loginStatus)
{
    qDebug() << "###################################GroupMessageSenderAdaptorLte::onSendLoginStatus" << loginStatus;
    if(loginStatus)
    {
        m_isLogin = true;
        m_timer.stop();
    }
    else
    {
        m_isLogin = false;
        m_timer.start();
    }
    emit signalSendLoginStatus(loginStatus);
}

void GroupMessageSenderAdaptorLte::onSendLoginStatusToApp(int status)
{
    qDebug() << "################################################GroupMessageSenderAdaptorLte::onSendLoginStatusToApp" << status;
    emit signalSendLoginStatusToApp(status);
}

//void GroupMessageSenderAdaptorLte::onReceivedMessage(GroupTransMsg msgData)
//{
//    Message message = converTransMessageToLocalMessage(msgData);
//    emit signalNewMessage(message);
//}

void GroupMessageSenderAdaptorLte::onSendPocStatusToApp(int status)
{
    qDebug() << "GroupMessageSenderAdaptorLte::onSendPocStatusToApp: " << status;

}

void GroupMessageSenderAdaptorLte::onSentMessageInfoToApp(QT_DTT_JJF::GroupTransMsg mInfo, int operateType)
{
    if(operateType == QT_DTT_JJF::OPERATE_CREATE ||
       operateType == QT_DTT_JJF::OPERATE_DELETE ||
       operateType == QT_DTT_JJF::OPERATE_UPDATE)
    {
        qDebug() << "GroupMessageSenderAdaptorLte::onSentMessageInfoToApp: " << operateType;
        qDebug() << "mInfo.id" << mInfo.id
                 << "mInfo.name " << mInfo.name
                 << "mInfo.addr" << mInfo.addr
                 << "mInfo.groupId" << mInfo.groupId
                 << "mInfo.description " << mInfo.description
                 << "mInfo.priority" << mInfo.priority
                 << "mInfo.type" << mInfo.type;

        for(int i=0; i<mInfo.baseMembers.count(); i++)
        {
            QT_DTT_JJF::UserInfo userInfo;
            qDebug() << "created user: "
                     << userInfo.userId
                     << userInfo.lte_pocId;
        }

        GroupInterface *gInterface = GroupInterface::getInstance();
        Group addedGroup = gInterface->findGroupByName(mInfo.name);
        gInterface->removeGroupById(addedGroup.id);

        addedGroup.id = mInfo.groupId;
        addedGroup.address = mInfo.groupId;
        addedGroup.handle = GROUP_ADD_HANDLE;
        emit signalGroupChangedFinished(addedGroup);
        m_groupTransMsgGroup = mInfo;

        emit sentMessageInfoToApp(mInfo, operateType);
        getGroupMember(LTE_NETWORK, mInfo.groupId);
    }
    else if(operateType == QT_DTT_JJF::OPERATE_SENT ||
            operateType == QT_DTT_JJF::OPERATE_RECV)
    {
        qDebug() << "send success------------";
//        Message message = converTransMessageToLocalMessage(mInfo);
//        emit signalNewMessage(message);
    }
    else if(operateType == QT_DTT_JJF::OPERATE_UPLOAD)
    {
        qDebug() << "GroupMessageSenderAdaptorLte::onSentMessageInfoToApp operateType == QT_DTT_JJF::OPERATE_UPLOAD";
    }
}

void GroupMessageSenderAdaptorLte::onRecvMessageInfoToApp(QT_DTT_JJF::GroupTransMsg mInfo, int operateType)
{
    qDebug() << "GroupMessageSenderAdaptorLte::onRecvMessageInfoToApp: " << operateType;
    if(operateType == QT_DTT_JJF::OPERATE_CREATE ||
       operateType == QT_DTT_JJF::OPERATE_DELETE ||
       operateType == QT_DTT_JJF::OPERATE_UPDATE)
    {
        Group group;
        group.id = mInfo.groupId;
        group.address = mInfo.groupId;
        group.networkType = getNetowrkType();
        group.creator = QString::number(mInfo.baseMembers.value(0).userId);

        foreach(const QT_DTT_JJF::UserInfo &userInfo, mInfo.baseMembers)
            qDebug() << "userInfo.userId: " << userInfo.userId << userInfo.lte_pocId;

        if(!mInfo.baseMembers.isEmpty())
            mInfo.baseMembers.removeAt(0);
        QStringList members;
        foreach(const QT_DTT_JJF::UserInfo &userInfo, mInfo.baseMembers)
            members.push_back(QString::number(userInfo.userId));
        group.members = members;//give poc id to members
        group.name = mInfo.name;
        group.type = CONVERSION_GROUP;

        if(operateType == QT_DTT_JJF::OPERATE_CREATE)
            operateType = GROUP_ADD_HANDLE;
        else if(operateType == QT_DTT_JJF::OPERATE_DELETE)
            operateType = GROUP_DELETE_HANDLE;
        else if(operateType == QT_DTT_JJF::OPERATE_UPDATE)
            operateType = GROUP_UPDATE_HANDLE;

        emit recvGroupFromRemote(group, operateType);
        getGroupMember(LTE_NETWORK, group.address);
    }
    else if(operateType == QT_DTT_JJF::OPERATE_SENT ||
            operateType == QT_DTT_JJF::OPERATE_RECV)
    {
        qDebug() << "mInfo poc id" <<  mInfo.ipocId
                 << "type: " << mInfo.type
                 << "state: " << mInfo.status;
        Contacter contacter = ContacterInterface::getInstance()->getContacterByPocId(mInfo.ipocId);
        Message message = converTransMessageToLocalMessage(mInfo);
        message.common.userId = contacter.id;
        emit signalNewMessage(message);
    }
    else if(operateType == QT_DTT_JJF::OPERATE_GET_MEMBERS)//get members of group
    {
        qDebug() << "operateType == QT_DTT_JJF::OPERATE_GET_MEMBERS";
        GroupInterface *gInterface = GroupInterface::getInstance();
        Group group = gInterface->getGroup(mInfo.groupId);
        QStringList members;
        int i = 0;
        foreach(const QT_DTT_JJF::UserInfo &userInfo, mInfo.baseMembers)
        {
            QString pocId = userInfo.lte_pocId;
            Contacter contacter = ContacterInterface::getInstance()->getContacterByPocId(pocId);
            if (i == 0)
                group.creator = contacter.id;
            else
                members.push_back(contacter.id);
            i++;
            qDebug() << "OPERATE_GET_MEMBERS " << userInfo.userId << contacter.id;
        }
        group.members = members;
        qDebug() << "xxxxxxxxxxxxxxxxxxxxxxx" << group.creator;
        qDebug() << "xxxxxxxxxxxxxxxxxxxxxxx" << group.members;
//        group.members.removeOne(group.creator);
        emit recvGroupFromRemote(group, GROUP_UPDATE_HANDLE);
    }
}

qint64 GroupMessageSenderAdaptorLte::sendMessage(const Message &message)
{
    GroupInterface *gInterface = GroupInterface::getInstance();
    Group addedGroup = gInterface->getGroup(message.common.groupId);

    Message sendMsg = message;
    QString lte_server_ip = ConnectionDatabase::getSetting("SETTING/LTE_SERVER_IP");
    sendMsg.addr = lte_server_ip;
//    sendMsg.common.type = 0;
    sendMsg.common.groupId = addedGroup.address;//address lte groupid
    sendMsg.networkType = LTE_NETWORK;
    sendMsg.optId = 1;
    sendMsg.common.date = QDateTime::currentDateTime();
    sendMsg.common.state = QT_DTT_JJF::MSG_STATUS_SENDING;
//    sendMsg.usersSendTo << "100110003" << "100110002";

    QT_DTT_JJF::GroupTransMsg msg = converLocalMessageToTransMessage(sendMsg);
    sendMessage(msg);
    return sizeof(QT_DTT_JJF::GroupTransMsg);
}

void GroupMessageSenderAdaptorLte::getGroupMember(int network, QString groupId)
{
    QDBusReply<void> retValDbus = m_interface.call("getGroupMember",
                                                   network,
                                                   groupId);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte::getGroupMember call success" ;
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte getGroupMember reply error:" << retValDbus.error();
    }
    return;
}

bool GroupMessageSenderAdaptorLte::isLogin()
{
    return m_isLogin;
}

void GroupMessageSenderAdaptorLte::enterChart(const QString &groupId)
{
    qDebug() << "enter room id:" << groupId;
    QDBusReply<int> retValDBus = m_interface.call("lteServiceSessionEnterChat",
                                                  groupId);
    if(retValDBus.isValid())
    {
        int sessionIndex = retValDBus.value();
        m_sessionIndexs.insert(groupId, sessionIndex);
        qDebug() << "GroupMessageSenderAdaptorLte lteServiceSessionEnterChat session index:"
                 << sessionIndex;
        qDebug() << "GroupMessageSenderAdaptorLte lteServiceSessionEnterChat call success";
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte lteServiceSessionEnterChat reply error:"
                 << retValDBus.error();
    }
}

void GroupMessageSenderAdaptorLte::exitChart(const QString &groupId)
{
    QDBusReply<void> retValDBus = m_interface.call("lteServiceSessionExitChat",
                                                   groupId);
    if(retValDBus.isValid())
    {
        int sessionIndex = m_sessionIndexs.value(groupId);
        m_sessionIndexs.remove(groupId);
        qDebug() << "GroupMessageSenderAdaptorLte lteServiceSessionExitChat session index:"
                 << sessionIndex;
        qDebug() << "GroupMessageSenderAdaptorLte lteServiceSessionExitChat call success";
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte lteServiceSessionExitChat reply error:"
                 << retValDBus.error();
    }
}

void GroupMessageSenderAdaptorLte::talkRequest(QString groupId)
{
    if(!m_sessionIndexs.contains(groupId))
        return;

    int sessionIndex = m_sessionIndexs.value(groupId);
    QDBusReply<void> retValDBus = m_communInterface.call("groupLteTalkRequest",
                                                         sessionIndex);
    if(retValDBus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte groupLteTalkRequest session index:"
                 << sessionIndex;
        qDebug() << "GroupMessageSenderAdaptorLte groupLteTalkRequest call success";
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte groupLteTalkRequest reply error:"
                 << retValDBus.error();
    }
}

void GroupMessageSenderAdaptorLte::talkRelease()
{
    QDBusReply<void> retValDBus = m_communInterface.call("groupLteTalkRelease");

    if(retValDBus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte groupLteTalkRelease call success";
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte groupLteTalkRelease reply error:"
                 << retValDBus.error();
    }
}

void GroupMessageSenderAdaptorLte::groupLteResetTalkFile()
{
    QDBusReply<void> retValDBus = m_communInterface.call("groupLteResetTalkFile");

    if(retValDBus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte groupLteResetTalkFile call success";
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte groupLteResetTalkFile reply error:"
                 << retValDBus.error();
    }
}

void GroupMessageSenderAdaptorLte::setGroupForeground(const QString &groupId, bool foreground)
{
    QDBusReply<void> retValDBus = m_interface.call("setGroupForeground",
                                                   groupId);
                                                   //foreground);
    if(retValDBus.isValid())
    {
        qDebug() << "GroupMessageSenderAdaptorLte setGroupForeground groupid:" << groupId;
        qDebug() << "GroupMessageSenderAdaptorLte setGroupForeground foreground flag:" << foreground;
        qDebug() << "GroupMessageSenderAdaptorLte setGroupForeground call success";
    }
    else
    {
        qDebug() << "GroupMessageSenderAdaptorLte setGroupForeground reply error:"
                 << retValDBus.error();
    }
}

void GroupMessageSenderAdaptorLte::onNotifyTalkBegin(int sessionIndex)
{
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyTalkBegin receive";
    QString groupId = getGroupId(sessionIndex);
    if(groupId.isEmpty())
        return;
    else
        emit signalNotifyTalkBegin(groupId);
}

void GroupMessageSenderAdaptorLte::onNotifyTalkEnd(int sessionIndex, int errCode)
{
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyTalkEnd receive";
    QString groupId = getGroupId(sessionIndex);
    bool exist = QFile::exists(STEAM_IN);
    qDebug() << "steam in file:" << exist;
    if(groupId.isEmpty())
        return;
    else
        emit signalNotifyTalkEnd(groupId, errCode);
}

void GroupMessageSenderAdaptorLte::onNotifyChannelConnect(int sessionIndex,
                                                          int connectStatus)
{
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyChannelConnect receive";
    QString groupId = getGroupId(sessionIndex);
    if(groupId.isEmpty())
        return;
    else
        emit signalNotifyChannelConnect(groupId, connectStatus);
}

void GroupMessageSenderAdaptorLte::onNotifyChannelDisconnect(int sessionIndex, int connectStatus)
{
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyChannelDisconnect receive";
    QString groupId = getGroupId(sessionIndex);
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyChannelDisconnect group:" << groupId;
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyChannelDisconnect status:" << connectStatus;
    if(groupId.isEmpty())
        return;
    else
        emit signalNotifyChannelDisconnnect(groupId);
}

QString GroupMessageSenderAdaptorLte::getGroupId(int sessionIndex)
{
    QStringList keys = m_sessionIndexs.keys();
    for(int i = 0, iend = keys.count(); i < iend; i++)
    {
        int sessiontmp = m_sessionIndexs[keys[i]];
        if(sessiontmp == sessionIndex)
            return keys[i];
    }
    qDebug() << "GroupMessageSenderAdaptorLte getGroupId session id:" << sessionIndex << " false";
    return "";
}

void GroupMessageSenderAdaptorLte::onNotifyListenBegin(int sessionIndex, QString pocId,
                                                       QString userName)
{
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyListenBegin receive";
    QString groupId = getGroupId(sessionIndex);
    Q_UNUSED(pocId);
    Q_UNUSED(userName);
    if(groupId.isEmpty())
        return;
    else
        emit signalNotifyListenBegin(groupId);
}

void GroupMessageSenderAdaptorLte::onNotifyListenEnd(int sessionIndex)
{
    qDebug() << "GroupMessageSenderAdaptorLte onNotifyListenEnd receive";
    QString groupId = getGroupId(sessionIndex);
    bool exist = QFile::exists(STEAM_OUT);
    qDebug() << "steam out file:" << exist;
    if(groupId.isEmpty())
        return;
    else
        emit signalNotifyListenEnd(groupId);
}

void GroupMessageSenderAdaptorLte::onNotifyRecordStop(int mode)
{
    Q_UNUSED(mode)
//    qDebug() << "GroupMessageSenderAdaptorLte onNotifyRecordStop receive";
//    bool exist = QFile::exists(STEAM_OUT);
//    qDebug() << "record out file exist:" << exist;
//    if(exist)
//        emit signalReceiveRecord(STEAM_OUT);
//    else
//        qDebug() << "GroupMessageSenderAdaptorLte STEAM OUT no exist";
}

void GroupMessageSenderAdaptorLte::onNotifyPlayerStop(int mode)
{
    Q_UNUSED(mode)
//    qDebug() << "GroupMessageSenderAdaptorLte onNotifyPlayerStop receive";
//    bool exist = QFile::exists(STEAM_IN);
//    qDebug() << "record in file exist:" << exist;
//    if(exist)
//        emit signalSendRecord(STEAM_IN);
//    else
//        qDebug() << "GroupMessageSenderAdaptorLte STEAM IN no exist";
}
