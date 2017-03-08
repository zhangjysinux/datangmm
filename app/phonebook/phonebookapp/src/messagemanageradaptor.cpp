#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>
#include "messagemanageradaptor.h"
#include "../../../commondata/datatype/commondata.h"


MessageManagerAdaptor *MessageManagerAdaptor::m_instance = NULL;
MessageManagerAdaptor *MessageManagerAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new MessageManagerAdaptor();

    return m_instance;
}

int MessageManagerAdaptor::removeMessageGroupData(const QString &groupId)
{
    int values = -1;
    QDBusReply<int> msgsReply = m_interface.call("onRemoveMessageGroupData", groupId);
    if(msgsReply.isValid())
        values = msgsReply.value();
    else
        qDebug() << "onRemoveMessageGroupData reply error:" << msgsReply.error();
    return values;
}

QStringList MessageManagerAdaptor::getMessageList(const QString &groupId)
{
    QStringList values;
    QDBusReply<QStringList> msgsReply = m_interface.call("onGetMessageList", groupId);
    if(msgsReply.isValid())
        values = msgsReply.value();
    else
        qDebug() << "onGetMessageList reply error:" << msgsReply.error();

    return values;
}

QStringList MessageManagerAdaptor::onGetMessageList(const QDateTime &startTime,
                                                    const QDateTime &endTime,
                                                    const QString &groupId)
{
    QStringList values;
    QDBusReply<QStringList> msgsReply = m_interface.call("onGetMessageList", startTime, endTime, groupId);
    if(msgsReply.isValid())
        values = msgsReply.value();
    else
        qDebug() << "onGetMessageList by time reply error:" << msgsReply.error();

    return values;
}

//startIndex from close start
QStringList MessageManagerAdaptor::onGetMessageList(const int &startIndex,
                                                    const int &endIndex,
                                                    const QString &groupId)
{
    QStringList values;
    QDBusReply<QStringList> msgsReply = m_interface.call("onGetMessageList", startIndex, endIndex, groupId);
    if(msgsReply.isValid())
        values = msgsReply.value();
    else
        qDebug() << "onGetMessageList by index reply error:" << msgsReply.error();

    return values;
}


quint64 MessageManagerAdaptor::sendMessage(const Message &msg)
{
    qDebug()<<"sendmessageAdaptor "<<msg.contentName;
    quint64 byteCount = 0;
    QVariant msgVar = QVariant::fromValue(msg);
    QDBusReply<quint64> sendReply = m_interface.call("onSendMessage", msgVar);
    if(sendReply.isValid())
        byteCount = sendReply.value();
    else
        qDebug() << "onSendMessage reply error:" << sendReply.error();

    return byteCount;
}

quint64 MessageManagerAdaptor::resendMessage(const Message &msg)
{
    qDebug() << "resendMessageAdaptor " << msg.contentName;
    quint64 byteCount = 0;
    QVariant msgVar = QVariant::fromValue(msg);
    QDBusReply<quint64> sendReply = m_interface.call("onResendMessage", msgVar);
    if(sendReply.isValid())
        byteCount = sendReply.value();
    else
        qDebug() << "onResendMessage reply error:" << sendReply.error();

    return byteCount;
}

quint64 MessageManagerAdaptor::sendTextMessage(const QString &groupId, const QString &text)
{
    quint64 byteCount = 0;
    QDBusReply<quint64> sendReply = m_interface.call("onSendTextMessage", groupId, text);
    if(sendReply.isValid())
        byteCount = sendReply.value();
    else
        qDebug() << "onSendTextMessage reply error:" << sendReply.error();

    return byteCount;
}

Message MessageManagerAdaptor::getMessage(const QString &id, const QString &groupId)
{
    Message msg;
    QDBusReply<QDBusVariant> msgReply = m_interface.call("onGetMessage", id, groupId);
    if(msgReply.isValid())
    {
        QDBusVariant msgDbusValue = msgReply.value();
        QDBusArgument argument = msgDbusValue.variant().value<QDBusArgument>();
        msg = qdbus_cast<Message>(argument);
    }
    else
    {
        qDebug() << "onGetMessage reply error:" << msgReply.error();
    }

    return msg;
}

QString MessageManagerAdaptor::getGroupNameByContacter(const QString &contactId)
{
    qDebug()<<"getGroupNameByContacter "<<contactId;
    QString ret = 0;
    QDBusReply<QString> sendReply = m_interface.call("onGetGroupNameByContacter", contactId);
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "onSendMessage reply error:" << sendReply.error();

    return ret;
}

void MessageManagerAdaptor::getMessageDownload(const QString &id, const QString &groupId)
{
    m_interface.call("onGetMessageDownload", id, groupId);
}

bool MessageManagerAdaptor::setMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId)
{
    bool handleRet = false;
    QDBusReply<bool> msgReply = m_interface.call("onSetMessageIsread", isReadFlag, id, groupId);
    if(msgReply.isValid())
    {
        handleRet = msgReply.value();
//        QDBusVariant msgDbusValue = msgReply.value();
//        QDBusArgument argument = msgDbusValue.variant().value<QDBusArgument>();
//        handleRet = qdbus_cast<bool>(argument);
    }
    else
    {
        qDebug() << "onSetMessageIsread reply error:" << msgReply.error();
    }

    return handleRet;
}

quint64 MessageManagerAdaptor::sendOtherMessage(const QString &groupId, const QString &fileName, const int &messageType)
{
    quint64 handleRet = 0;
    QDBusReply<QDBusVariant> msgReply = m_interface.call("onSendOtherMessage", groupId, fileName, messageType);
    if(msgReply.isValid())
    {
        QDBusVariant msgDbusValue = msgReply.value();
        QDBusArgument argument = msgDbusValue.variant().value<QDBusArgument>();
        handleRet = qdbus_cast<quint64>(argument);
    }
    else
    {
        qDebug() << "onSendOtherMessage reply error:" << msgReply.error();
    }

    return handleRet;
}

int MessageManagerAdaptor::getSpeakChance()
{
    int handleRet = 0;
    QDBusReply<QDBusVariant> msgReply = m_interface.call("onGetSpeakChance");
    if(msgReply.isValid())
    {
        QDBusVariant msgDbusValue = msgReply.value();
        QDBusArgument argument = msgDbusValue.variant().value<QDBusArgument>();
        handleRet = qdbus_cast<int>(argument);
    }
    else
    {
        qDebug() << "onGetSpeakChance reply error:" << msgReply.error();
    }

    return handleRet;
}

int MessageManagerAdaptor::startSpeak()
{
    int handleRet = 0;
    QDBusReply<QDBusVariant> msgReply = m_interface.call("onStartSpeak");
    if(msgReply.isValid())
    {
        QDBusVariant msgDbusValue = msgReply.value();
        QDBusArgument argument = msgDbusValue.variant().value<QDBusArgument>();
        handleRet = qdbus_cast<int>(argument);
    }
    else
    {
        qDebug() << "onStartSpeak reply error:" << msgReply.error();
    }

    return handleRet;
}

void MessageManagerAdaptor::onSetTts(QString groupId, bool on)
{
    QDBusReply<void> sendReply = m_interface.call("onSetTts", groupId, on);
    if(sendReply.isValid())
        qDebug() << "onSetTts groupid reply succ:";
    else
        qDebug() << "onSetTts groupid reply error:";
    return;
}

bool MessageManagerAdaptor::onGetTts(QString groupId)
{
    //return m_interface.call("onGetTts", groupId);
    bool value = false;
    QDBusReply<bool> sendReply = m_interface.call("onGetTts", groupId);
    if(sendReply.isValid()){
        value = sendReply.value();
        qDebug() << "onSetTts groupid reply succ:";
    }
    else {
        qDebug() << "onSetTts groupid reply error:";
    }
    return value;
}

void MessageManagerAdaptor::onGetPosition(int type, QString groupid)
{
    m_interface.call("onGetPosition",type,groupid);
}

void MessageManagerAdaptor::onDbusSignalMessageChanged(int type, QDBusVariant data)
{
    qDebug() << "-----------------------receiveMessageChange------------------";
    qDebug() << "message change type:" << type;
    QDBusArgument argument = data.variant().value<QDBusArgument>();
    Message value = qdbus_cast<Message>(argument);
    qDebug() << "message id:" << value.common.id;
    if(value.common.type == TEXT_MESSAGE)
    {
        QString text = parseTextMessage(value.content);
        qDebug() << "message text:" << text;
    }
    else if (value.common.type == IMAGE_MESSAGE)
    {

    }
    qDebug()<<value.common.type;
    qDebug() << "-----------------------end--------------------------------------";

    emit signalMessageChanged(type, value);
}

void MessageManagerAdaptor::onDbusSignalMessageDownload(QDBusVariant data)
{
    qDebug() << "-----------------------onDbusSignalMessageDownload------------------";
    QDBusArgument argument = data.variant().value<QDBusArgument>();
    Message value = qdbus_cast<Message>(argument);
    emit signalMessageDownload(value);
}

void MessageManagerAdaptor::clearSyncCount(const QString &groupId)
{
    m_interface.call("onClearSyncCount", groupId);
}

void MessageManagerAdaptor::clearSyncCount()
{
    m_interface.call("onClearSyncCount");
}

int MessageManagerAdaptor::getSyncCount(const QString &groupId)
{
    int ret = 0;
    QDBusReply<int> sendReply = m_interface.call("onGetSyncCount", groupId);
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "getSyncCount groupid reply error:" << sendReply.error();

    return ret;
}

int MessageManagerAdaptor::getSyncCount()
{
    int ret = 0;
    QDBusReply<int> sendReply = m_interface.call("onGetSyncCount");
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "getSyncCount reply error:" << sendReply.error();

    return ret;
}

unsigned MessageManagerAdaptor::joinGroup(int network, const QString &groupId)
{
    unsigned ret = 0;
    QDBusReply<unsigned> sendReply = m_interface.call("onJoinGroup", network, groupId);
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "onJoinGroup reply error:" << sendReply.error();

    return ret;
}

int MessageManagerAdaptor::leaveGroup(int network, const QString &groupId)
{
    int ret = 0;
    QDBusReply<int> sendReply = m_interface.call("onLeaveGroup", network, groupId);
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "onLeaveGroup reply error:" << sendReply.error();

    return ret;
}

int MessageManagerAdaptor::getPttState(int network, const QString &groupId)
{
    int ret = 0;
    QDBusReply<int> sendReply = m_interface.call("onGetPttState", network, groupId);
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "onGetPttState reply error:" << sendReply.error();

    return ret;
}

int MessageManagerAdaptor::setPttState(int network, const QString &groupId, int state)
{
    qDebug()<<"setPttState"<<network<<groupId<<state;
    int ret = 0;
    QDBusReply<int> sendReply = m_interface.call("onSetPttState", network, groupId, state);
    if(sendReply.isValid())
        ret = sendReply.value();
    else
        qDebug() << "onSetPttState reply error:" << sendReply.error();

    return ret;
}

void MessageManagerAdaptor::joinBtGroup(const QString &groupId)
{
    QDBusReply<void> sendReply = m_interface.call("onJoinBtGroup", groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onJoinBtGroup reply success";
    }
    else
    {
        qDebug() << "onJoinBtGroup reply error:" << sendReply.error();
    }
    return;
}

void MessageManagerAdaptor::setBtPttState(const QString &groupId, int state)
{
    QDBusReply<void> sendReply = m_interface.call("onSetBtPttState", groupId, state);
    if(sendReply.isValid())
    {
        qDebug() << "onSetBtPttState reply success";
    }
    else
    {
        qDebug() << "onSetBtPttState reply error:" << sendReply.error();
    }
    return;
}

bool MessageManagerAdaptor::getBtPttAllow(const QString &groupId)
{
    QDBusReply<bool> sendReply = m_interface.call("onGetBtPttAllow", groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onGetBtPttAllow reply success" << sendReply.value();
    }
    else
    {
        qDebug() << "onGetBtPttAllow reply error:" << sendReply.error();
    }
    return sendReply.value();
}

void MessageManagerAdaptor::onInGroupUi(const QString &groupId)
{
    QDBusReply<void> sendReply = m_interface.call("onInGroupUi", groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onInGroupUi reply success";
    }
    else
    {
        qDebug() << "onInGroupUi reply error:" << sendReply.error();
    }
    return;
}

void MessageManagerAdaptor::onOutGroupUi(const QString &groupId)
{
    QDBusReply<void> sendReply = m_interface.call("onOutGroupUi", groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onOutGroupUi reply success";
    }
    else
    {
        qDebug() << "onOutGroupUi reply error:" << sendReply.error();
    }
    return;
}

int MessageManagerAdaptor::onCallP2P(int network, const QString &groupId)
{
    QDBusReply<int> sendReply = m_interface.call("onCallP2P", network, groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onCallP2P reply success" << sendReply.value();
    }
    else
    {
        qDebug() << "onCallP2P reply error:" << sendReply.error();
    }
    return sendReply.value();
}

int MessageManagerAdaptor::onHangupP2P(int network, const QString &groupId)
{
    QDBusReply<int> sendReply = m_interface.call("onHangupP2P", network, groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onHangupP2P reply success" << sendReply.value();
    }
    else
    {
        qDebug() << "onHangupP2P reply error:" << sendReply.error();
    }
    return sendReply.value();
}

int MessageManagerAdaptor::onGetP2PState(int network, const QString &groupId)
{
    QDBusReply<int> sendReply = m_interface.call("onGetP2PState", network, groupId);
    if(sendReply.isValid())
    {
        qDebug() << "onGetP2PState reply success" << sendReply.value();
    }
    else
    {
        qDebug() << "onGetP2PState reply error:" << sendReply.error();
    }
    return sendReply.value();
}

int MessageManagerAdaptor::onSetP2PState(int network, const QString &groupId, int state)
{
    QDBusReply<int> sendReply = m_interface.call("onSetP2PState", network, groupId, state);
    if(sendReply.isValid())
    {
        qDebug() << "onSetP2PState reply success" << sendReply.value();
    }
    else
    {
        qDebug() << "onSetP2PState reply error:" << sendReply.error();
    }
    return sendReply.value();
}


MessageManagerAdaptor::MessageManagerAdaptor(QObject *parent) :
    QObject(parent),
    m_interface(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<Message>();
    qDBusRegisterMetaType<MessageList>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalMessageChanged",
                       this, SLOT(onDbusSignalMessageChanged(int, QDBusVariant)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalMessageDownload",
                       this, SLOT(onDbusSignalMessageDownload(QDBusVariant)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalPttStateChanged",
                       this, SIGNAL(signalPttStateChanged(int,QString,int)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalPttStateChanged",
                       this, SIGNAL(signalPttGroupStateChanged(int,QString,int)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalReceivePercent",
                       this, SLOT(onSignalReceivePercent(QString,QString,int,int)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalSendPercent",
                       this, SLOT(onSignalSendPercent(QString,QString,int,int)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalUpdateMessageCount",
                       this, SLOT(signalUpdateMessageCount(QString,int)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalGetPositon",
                       this, SIGNAL(signalGetPosition(int,QString)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalCallP2PFinished",
                       this, SLOT(onSignalCallP2PStateChanged(int,QString,int)));
    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalP2PConnected",
                       this, SIGNAL(signalP2PConnected(QString,int)));

    sessionBus.connect(contacterServiceName, "/msgmgr", "com.sinux.DBus.msgmgr", "signalUpdateMessageCount",
                       this, SLOT(signalUpdateMessageCount(QString,int)));

}

void MessageManagerAdaptor::onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "MessageManagerAdaptor::onSignalSendPercent";
    emit signalSendPercent(groupId, id, recvNum, allNum);
}

void MessageManagerAdaptor::onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "MessageManagerAdaptor::onSignalReceivePercent";
    emit signalReceivePercent(groupId, id, recvNum, allNum);
}

void MessageManagerAdaptor::onSignalCreateDeleteGroup(const QString &groupId, int success)
{
    qDebug() << "MessageManagerAdaptor::onSignalCreateDeleteGroup" << groupId << success;
    emit signalCreateDeleteGroup(groupId, success);
}

void MessageManagerAdaptor::onSignalJoinBtGroup(const QString &groupId, int success)
{
    qDebug() << "MessageManagerAdaptor::onSignalJoinBtGroup" << groupId << success;
    emit signalJoinBtGroup(groupId, success);
}

void MessageManagerAdaptor::onSignalSetBtPttState(const QString &groupId, int success)
{
    qDebug() << "MessageManagerAdaptor::onSignalSetBtPttState" << groupId << success;
    emit signalSetBtPttState(groupId, success);
}

void MessageManagerAdaptor::onSignalAllowSetPttState(const QString &groupId, int allow)
{
    qDebug() << "MessageManagerAdaptor::onSignalAllowSetPttState" << groupId << allow;
    emit signalAllowSetPttState(groupId, allow);
}

void MessageManagerAdaptor::onSignalCallP2PStateChanged(int network,
                                                    const QString &groupId,
                                                    int state)
{
    qDebug() << "MessageManagerAdaptor::onSignalCallP2PFinished"
             << network
             << groupId
             << state;
    emit signalCallP2PStateChanged(network, groupId, state);
}

