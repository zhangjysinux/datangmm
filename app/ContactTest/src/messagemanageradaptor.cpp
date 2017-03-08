#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>
#include "messagemanageradaptor.h"

MessageManagerAdaptor *MessageManagerAdaptor::m_instance = NULL;
MessageManagerAdaptor *MessageManagerAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new MessageManagerAdaptor();

    return m_instance;
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

void MessageManagerAdaptor::getMessageDownload(const QString &id, const QString &groupId)
{
    m_interface.call("onGetMessageDownload", id, groupId);
}

bool MessageManagerAdaptor::setMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId)
{
    bool handleRet = false;
    QDBusReply<QDBusVariant> msgReply = m_interface.call("onSetMessageIsread", isReadFlag, id, groupId);
    if(msgReply.isValid())
    {
        QDBusVariant msgDbusValue = msgReply.value();
        QDBusArgument argument = msgDbusValue.variant().value<QDBusArgument>();
        handleRet = qdbus_cast<bool>(argument);
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

MessageManagerAdaptor::MessageManagerAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.contact", "/msgmgr", "com.sinux.DBus.msgmgr", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<Message>();
    qDBusRegisterMetaType<MessageList>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.contact", "/msgmgr", "com.sinux.DBus.msgmgr", "signalMessageChanged",
                       this, SLOT(onDbusSignalMessageChanged(int, QDBusVariant)));
    sessionBus.connect("com.sinux.contact", "/msgmgr", "com.sinux.DBus.msgmgr", "signalMessageDownload",
                       this, SLOT(onDbusSignalMessageDownload(QDBusVariant)));

}
