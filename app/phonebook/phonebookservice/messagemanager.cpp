#include <QDBusMetaType>
#include "messagemanager.h"
#include "contacterinterface.h"
#include "groupinterface.h"
#include "../../commondata/bluetoothbackgroundtransferinterface.h"
#include "adaptor/unreadmessageadaptor.h"
#include "adaptor/pointtopointadaptor.h"
#include "adaptor/p2pmessagesendertcpadaptor.h"

MessageManager *MessageManager::m_instance = NULL;
MessageManager *MessageManager::getInstance()
{
    if(m_instance == NULL)
        m_instance = new MessageManager();
    return m_instance;
}

int MessageManager::onRemoveMessageGroup(const QString &groupId)
{
    return removeGroupMessageManager(groupId);
}

int MessageManager::onRemoveMessageGroupData(const QString &groupId)
{
    return removeGroupMessageManagerData(groupId);
}

QStringList MessageManager::onGetMessageList(const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QStringList msgList = groupMsgManager->getMessageList();

    return msgList;
}

QStringList MessageManager::onGetMessageList(const QDateTime &startTime, const QDateTime &endTime, const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QStringList msgList = groupMsgManager->getMessageList(startTime, endTime);

    return msgList;
}
QStringList MessageManager::onGetMessageList(const int &startIndex, const int &endIndex, const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QStringList msgList = groupMsgManager->getMessageList(startIndex, endIndex);

    return msgList;
}


QDBusVariant MessageManager::onGetMessage(const QString &id, const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    Message msg = groupMsgManager->getMessage(id);

    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(msg);
    dbusValue.setVariant(dataVar);

    return dbusValue;
}

Message MessageManager::getMessage(const QString &id, const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    Message msg = groupMsgManager->getMessage(id);

    return msg;
}

void MessageManager::onGetMessageDownload(const QString &id, const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->getMessageDownload(id);
}

bool MessageManager::onSetMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->setMessageIsread(isReadFlag, id);
}

quint64 MessageManager::onSendMessage(const Message &msg)
{
    QString groupId = msg.common.groupId;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    quint64 bytesCount = groupMsgManager->sendMessage(msg);
    return bytesCount;
}

quint64 MessageManager::onResendMessage(const Message &msg)
{
    QString groupId = msg.common.groupId;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    quint64 bytesCount = groupMsgManager->resendMessage(msg.common.id);

    return bytesCount;
}

quint64 MessageManager::onSendTextMessage(const QString &groupId, const QString &text)
{
    Message msg;
    msg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;
    msg.common.groupId = groupId;
    msg.common.type = TEXT_MESSAGE;
    msg.content = text.toLocal8Bit();
    quint64 byteCount = onSendMessage(msg);
    return byteCount;
}

quint64 MessageManager::onSendOtherMessage(const QString &groupId, const QString &fileName, const int &messageType)
{
    Message msg;
    msg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;
    msg.common.groupId = groupId;
    msg.common.type = messageType;
    msg.contentName = fileName;
    quint64 byteCount = onSendMessage(msg);
    return byteCount;
}

QString MessageManager::onGetGroupNameByContacter(const QString &contactId)
{
    QString groupName = "group2_" + convertUuidToString(contactId);
    return groupName;
}


MessageManager::MessageManager(QObject *parent)
    : QObject(parent)
{
    qDBusRegisterMetaType<Message>();
    qDBusRegisterMetaType<MessageList>();
    PointToPointAdaptor::getInstance();
    connect(PointToPointAdaptor::getInstance(), SIGNAL(signalNewMessage(Message)),
            this, SLOT(onSignalPointToPointNewMessae(Message)));

    connect(P2PMessageSenderTcpAdaptor::getInstance(), SIGNAL(signalNewMessage(Message)),
            this, SLOT(onSignalPointToPointNewMessae(Message)));

    QStringList groupList = GroupInterface::getInstance()->onGetGroups(CONVERSION_GROUP);
    for(int i = 0, iend = groupList.count(); i < iend; i++)
    {
//        GroupMessageManager *manager = new GroupMessageManager(groupList[i], this);
//        connect(manager, SIGNAL(signalMessageChanged(int,Message)),
//                this, SLOT(onGroupMessageChanged(int,Message)));
//        connect(manager, SIGNAL(signalMessageDownload(Message)),
//                this, SLOT(onSignalMessageDownload(Message)));
//        connect(manager, SIGNAL(signalMessageChangedToChangeRemoteMessage(int,MessageList)),
//                this, SIGNAL(signalMessageChangedToChangeRemoteMessage(int,MessageList)));
//        connect(manager, SIGNAL(signalPttStateChanged(QString,int)),
//                this, SLOT(onSignalPttStateChanged(QString,int)));
//        connect(manager, SIGNAL(signalReceivePercent(QString,QString,int,int)),
//                this, SLOT(onSignalReceivePercent(QString,QString,int,int)));
//        connect(manager, SIGNAL(signalSendPercent(QString,QString,int,int)),
//                this, SLOT(onSignalSendPercent(QString,QString,int,int)));
//        connect(manager, SIGNAL(signalUpdateMessageCount(QString,int)),
//                this, SIGNAL(signalUpdateMessageCount(QString,int)));
//        m_msgManagerMap.insert(groupList[i], manager);
        getGroupMessageManager(groupList[i]);
    }
//    onGetMessageList("p2pgroup_11_networktype_16");
}

void MessageManager::onGroupMessageChanged(int type, Message msg)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(msg);
    dbusValue.setVariant(dataVar);
    emit signalMessageChanged(type, dbusValue);
}

void MessageManager::onSignalMessageDownload(const Message &msg)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(msg);
    dbusValue.setVariant(dataVar);
    emit signalMessageDownload(dbusValue);
}

GroupMessageManager *MessageManager::getGroupMessageManager(const QString &groupId)
{
    if(!m_msgManagerMap.keys().contains(groupId))
    {
        GroupMessageManager *manager = new GroupMessageManager(groupId, this);
        connect(manager, SIGNAL(signalMessageChanged(int,Message)),
                this, SLOT(onGroupMessageChanged(int,Message)));
        connect(manager, SIGNAL(signalMessageDownload(Message)),
                this, SLOT(onSignalMessageDownload(Message)));
        connect(manager, SIGNAL(signalMessageChangedToChangeRemoteMessage(int,MessageList)),
                this, SIGNAL(signalMessageChangedToChangeRemoteMessage(int,MessageList)));
        connect(manager, SIGNAL(signalPttStateChanged(int,QString,int)),
                this, SLOT(onSignalPttStateChanged(int,QString,int)));
        connect(manager, SIGNAL(signalReceivePercent(QString,QString,int,int)),
                this, SLOT(onSignalReceivePercent(QString,QString,int,int)));
        connect(manager, SIGNAL(signalSendPercent(QString,QString,int,int)),
                this, SLOT(onSignalSendPercent(QString,QString,int,int)));
        connect(manager, SIGNAL(signalUpdateMessageCount(QString,int)),
                this, SIGNAL(signalUpdateMessageCount(QString,int)));

        connect(manager, SIGNAL(signalAllowSetPttState(QString,int)),
                this, SIGNAL(signalAllowSetPttState(QString,int)));
        connect(manager, SIGNAL(signalCreateDeleteGroup(QString,int)),
                this, SIGNAL(signalCreateDeleteGroup(QString,int)));
        connect(manager, SIGNAL(signalJoinBtGroup(QString,int)),
                this, SIGNAL(signalJoinBtGroup(QString,int)));
        connect(manager, SIGNAL(signalSetBtPttState(QString,int)),
                this, SIGNAL(signalSetBtPttState(QString,int)));

        connect(manager, SIGNAL(signalCallP2PStateChanged(int,QString,int)),
                this, SIGNAL(signalCallP2PStateChanged(int,QString,int)));

        connect(manager, SIGNAL(signalP2PConnected(QString,int)),
                this, SIGNAL(signalP2PConnected(QString,int)));

        m_msgManagerMap.insert(groupId, manager);
    }

    GroupMessageManager *groupMsgManager = m_msgManagerMap.value(groupId);
    return groupMsgManager;
}

int MessageManager::removeGroupMessageManager(const QString &groupId)
{
    if(m_msgManagerMap.keys().contains(groupId))
    {
        GroupMessageManager *manager = m_msgManagerMap.value(groupId);
        manager->clearData();
        manager->deleteLater();

        m_msgManagerMap.remove(groupId);
    }
    return 0;
}

int MessageManager::removeGroupMessageManagerData(const QString &groupId)
{
    if(m_msgManagerMap.keys().contains(groupId))
    {
        GroupMessageManager *manager = m_msgManagerMap.value(groupId);
        manager->clearData();
    }
    return 0;
}

QString MessageManager::convertUuidToString(QString uuid)
{
    uuid.replace("{", "___");
    uuid.replace("}", "__");
    uuid.replace("-", "_");
    return uuid;
}

QString MessageManager::convertStringToUuid(QString string)
{
    string.replace("___", "{");
    string.replace("__", "}");
    string.replace("_", "-");
    return string;
}

void MessageManager::onMessageReceived(const int &handleType, const MessageList &msgs)
{
    if(msgs.size() > 0)
    {
        foreach(const Message &msg, msgs)
        {
//            Message msg = msgs.value(i);
            if(!msg.common.groupId.isEmpty())
            {
                GroupMessageManager *groupMsgManager = getGroupMessageManager(msg.common.groupId);
                MessageList sendMsgs;
                sendMsgs.append(msg);
                groupMsgManager->onMessageReceived(handleType, sendMsgs);
                if(handleType == MESSAGE_ADD_HANDLE)
                {
                    groupMsgManager->onAddSyncCount(1);
                }
            }
        }
        setSyncCountToAppIcon(getSyncCount());
    }
}

bool MessageManager::isWatch()
{
    return BluetoothBackgroundTransferInterface::getInstance()->onGetServiceType() == BLUETOOTHSERVICETYPE_CLIENT;
}

//sync count from phone
void MessageManager::onClearSyncCount(const QString &groupId)
{
    clearSyncCount(groupId);
}

void MessageManager::onClearSyncCount()
{
    clearSyncCount();
}

int MessageManager::onGetSyncCount(const QString &groupId)
{
    return getSyncCount(groupId);
}

int MessageManager::onGetSyncCount()
{
    return getSyncCount();
}


void MessageManager::clearSyncCount(const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    groupMsgManager->onClearSyncCount();
    return;
}

void MessageManager::clearSyncCount()
{
    QList<QString> keys = m_msgManagerMap.keys();
    foreach(const QString &groupId, keys)
    {
        GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
        groupMsgManager->onClearSyncCount();
    }
    return;
}

int MessageManager::getSyncCount(const QString &groupId)
{
    if(groupId.isEmpty())
        return 0;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onGetSyncCount();
}

int MessageManager::getSyncCount()
{
    int count = 0;
    QList<QString> keys = m_msgManagerMap.keys();
    foreach(const QString &groupId, keys)
    {
        GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
        count += groupMsgManager->onGetSyncCount();
    }
    return count;
}

void MessageManager::setSyncCountToAppIcon(const int &count)
{
    qDebug() << "MessageManager::setSyncCountToAppIcon" << count;
    UnreadMessageAdaptor::getInstance()->setUnreadGroupMessage(count);
}

unsigned MessageManager::onJoinGroup(int network, const QString &groupId)
{
    qDebug() << "onJoinGroup" << network << groupId;
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onJoinGroup(network);
}

int MessageManager::onLeaveGroup(int network, const QString &groupId)
{
    qDebug() << "onLeaveGroup" << network << groupId;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onLeaveGroup(network);
}

int MessageManager::onGetPttState(int network, const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onGetPttState(network);
}

int MessageManager::onSetPttState(int network, const QString &groupId, int state)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onSetPttState(network, state);
}

void MessageManager::onSetBtPttState(const QString &groupId, int state)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onSetBtPttState(state);
}

bool MessageManager::onGetBtPttAllow(const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onGetBtPttAllow();
}

void MessageManager::onInGroupUi(const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    groupMsgManager->onInGroupUi();
    return;
}

void MessageManager::onOutGroupUi(const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    groupMsgManager->onOutGroupUi();
    return;
}

void MessageManager::onGetPosition(int type, QString groupid)
{
    qDebug() << "signalGetPositon";
    emit signalGetPositon(type, groupid);
}

int MessageManager::onCallP2P(int network, const QString &groupId)
{
    qDebug() << "MessageManager::onCallP2P" << network << groupId;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QString id = groupId.section("_", 1, 1);
    Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);
    QString ip = getPhoneNumberType(contacter.numbers, network).number;

    return groupMsgManager->onCallP2P(network, ip);
}

int MessageManager::onHangupP2P(int network, const QString &groupId)
{
    qDebug() << "MessageManager::onHangupP2P" << network << groupId;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QString id = groupId.section("_", 1, 1);
    Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);
    QString ip = getPhoneNumberType(contacter.numbers, network).number;

    return groupMsgManager->onHangupP2P(network, ip);
}

int MessageManager::onGetP2PState(int network, const QString &groupId)
{
    qDebug() << "MessageManager::onGetP2PState" << network << groupId;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QString id = groupId.section("_", 1, 1);
    Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);
    QString ip = getPhoneNumberType(contacter.numbers, network).number;

    return groupMsgManager->onGetP2PState(network, ip);
}

int MessageManager::onSetP2PState(int network, const QString &groupId, int state)
{
    qDebug() << "MessageManager::onSetP2PState" << network << groupId << state;

    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    QString id = groupId.section("_", 1, 1);
    Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);
    QString ip = getPhoneNumberType(contacter.numbers, network).number;

    return groupMsgManager->onSetP2PState(network, ip, state);
}

void MessageManager::onSetTts(QString groupId, bool on)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    groupMsgManager->onSetTts(groupId, on);
    return;
}

bool MessageManager::onGetTts(QString groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onGetTts();
}

void MessageManager::onJoinBtGroup(const QString &groupId)
{
    GroupMessageManager *groupMsgManager = getGroupMessageManager(groupId);
    return groupMsgManager->onJoinBtGroup();
}


void MessageManager::onSignalPttStateChanged(int network, const QString &groupId, int state)
{
    emit signalPttStateChanged(network, groupId, state);
}

void MessageManager::onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "MessageManager::onSignalSendPercent";
    emit signalSendPercent(groupId, id, recvNum, allNum);
}

void MessageManager::onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "MessageManager::onSignalReceivePercent";
    emit signalReceivePercent(groupId, id, recvNum, allNum);
}

void MessageManager::onSignalPointToPointNewMessae(const Message &message)
{
    Message msg = message;
    QString localContacterId = msg.common.groupId.section("_", 1,1);
    QString remoteContacterId = msg.common.userId;
    msg.common.groupId.replace("_"+localContacterId+"_", "_"+remoteContacterId+"_");
    GroupMessageManager *groupMsgManager = getGroupMessageManager(msg.common.groupId);
    return groupMsgManager->onNewMessage(message);
}
