#include "messagemanager.h"

MessageManager *MessageManager::m_instance = NULL;
MessageManager *MessageManager::getInstance()
{
    if(m_instance == NULL)
        m_instance = new MessageManager();

    return m_instance;
}

QStringList MessageManager::getMessageList(const QString &groupId)
{
    //to do check group exist in contact data
    if(!m_msgManagerMap.keys().contains(groupId))
    {
        GroupMessageManager *manager = new GroupMessageManager(groupId, this);
        connect(manager, SIGNAL(signalMessageChanged(int,Message)),
                this, SIGNAL(signalMessageChanged(int,Message)));
        connect(manager, SIGNAL(signalMessageChanged(int,Message)),
                this, SLOT(onSignalMessageChanged(int,Message)));
        m_msgManagerMap.insert(groupId, new GroupMessageManager(groupId, this));
    }

    GroupMessageManager *groupMsgManager = m_msgManagerMap.value(groupId);
    QStringList msgList = groupMsgManager->getMessageList();

    return msgList;
}

quint64 MessageManager::sendMessage(const Message &msg)
{
    QString groupId = msg.common.groupId;
    //to do check group exist in contact data
    if(!m_msgManagerMap.keys().contains(groupId))
    {
        GroupMessageManager *manager = new GroupMessageManager(groupId, this);
        connect(manager, SIGNAL(signalMessageChanged(int,Message)),
                this, SIGNAL(signalMessageChanged(int,Message)));
        connect(manager, SIGNAL(signalMessageChanged(int,Message)),
                this, SLOT(onSignalMessageChanged(int,Message)));
        m_msgManagerMap.insert(groupId, new GroupMessageManager(groupId, this));
    }

    GroupMessageManager *groupMsgManager = m_msgManagerMap.value(groupId);
    quint64 bytesCount = groupMsgManager->sendMessage(msg);

    return bytesCount;
}

quint64 MessageManager::sendTextMessage(const QString &groupId, const QString &userId, const QString &text)
{
    Message msg;
    msg.common.userId = userId;
    msg.common.groupId = groupId;
    msg.content = wrapTextMessage(text);
    quint64 byteCount = sendMessage(msg);
    return byteCount;
}

void MessageManager::setQMLObject(QObject *qmlObject)
{
    m_qmlObject = qmlObject;
}

void MessageManager::onSignalMessageChanged(int type, Message msg)
{
    if(m_qmlObject == NULL && type == MESSAGE_ADD_HANDLE)
        return;

    QMetaObject::invokeMethod(m_qmlObject, "insertMainList",
                              Q_ARG(QVariant, msg.common.date.toString("HH:mm:ss")),Q_ARG(QVariant, msg.common.userId),
                              Q_ARG(QVariant, parseTextMessage(msg.content)));
}

MessageManager::MessageManager(QObject *parent)
    : QObject(parent), m_qmlObject(NULL)
{
    GroupMessageManager *manager = new GroupMessageManager("1001", this);
    m_msgManagerMap.insert("1001", manager);
    connect(manager, SIGNAL(signalMessageChanged(int,Message)),
            this, SIGNAL(signalMessageChanged(int,Message)));
    connect(manager, SIGNAL(signalMessageChanged(int,Message)),
            this, SLOT(onSignalMessageChanged(int,Message)));
}
