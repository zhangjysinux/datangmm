#include "p2pmessagesendertcpadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QUuid>

P2PMessageSenderTcpAdaptor *P2PMessageSenderTcpAdaptor::instance = NULL;

P2PMessageSenderTcpAdaptor *P2PMessageSenderTcpAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new P2PMessageSenderTcpAdaptor();
    }

    return instance;
}

P2PMessageSenderTcpAdaptor::P2PMessageSenderTcpAdaptor(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_interface("com.sinux.backupdate", "/messagesenderinterface", "com.sinux.DBus.messagesenderinterface", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/messagesenderinterface",
                       "com.sinux.DBus.messagesenderinterface",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalConnectFinished(QString,int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/messagesenderinterface",
                       "com.sinux.DBus.messagesenderinterface",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalReceivePercent(QString,QString,int,QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/messagesenderinterface",
                       "com.sinux.DBus.messagesenderinterface",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalSendPercent(QString,QString,int,QString,qint64,qint64)));
}

P2PMessageSenderTcpAdaptor::~P2PMessageSenderTcpAdaptor()
{
}

qint64 P2PMessageSenderTcpAdaptor::sendMessage(const Message &message)
{
    onSendMessage(message);
}

void P2PMessageSenderTcpAdaptor::onSetPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "P2PMessageSenderTcpAdaptor::onSetPort" ;
    }
    else
        qDebug() << "P2PMessageSenderTcpAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

int P2PMessageSenderTcpAdaptor::onConnect(const QString &id, const QString &address)
{
    QDBusReply<int> retValDbus = m_interface.call("onConnect", id, address);
    if(retValDbus.isValid())
    {
        qDebug() << "P2PMessageSenderTcpAdaptor::onConnect" << id << address << retValDbus.value();
    }
    else
        qDebug() << "P2PMessageSenderTcpAdaptor onConnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

bool P2PMessageSenderTcpAdaptor::onIsConnected()
{
    QDBusReply<bool> retValDbus = m_interface.call("onIsConnected");
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onIsConnected" << retValDbus.value();
    }
    else
        qDebug() << "FileShareAdaptor onIsConnected reply error:" << retValDbus.error();

    return retValDbus.value();
}


int P2PMessageSenderTcpAdaptor::onDisconnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        qDebug() << "P2PMessageSenderTcpAdaptor::onDisconnect" << retValDbus.value();
    }
    else
        qDebug() << "P2PMessageSenderTcpAdaptor onDisconnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

int P2PMessageSenderTcpAdaptor::onSendFile(const QString &id, const QString &msgId, const QString &fileName, const int &msgType)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", id, msgId, fileName, msgType);
    if(retValDbus.isValid())
    {
        qDebug() << "P2PMessageSenderTcpAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "P2PMessageSenderTcpAdaptor onSendFile reply error:" << retValDbus.error();

    return retValDbus.value();
}

int P2PMessageSenderTcpAdaptor::onSendMessage(const Message &message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;

    if(data.isEmpty())
        qDebug() << "onSendMessage to datastream error---";

    QString fileName = "/home/user/grouptmp/groupdata"+QUuid::createUuid().toString().remove("{").remove("}").remove("-")+".dat";
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open success: " << file.fileName();
        file.write(data);
        file.flush();
        file.close();

        //file send
        onSendFile(message.common.groupId, message.common.id, fileName, message.common.type);
    }
    else
    {
        qDebug() << "file open error: " << file.errorString();
    }
}

void P2PMessageSenderTcpAdaptor::onSignalConnectFinished(const QString &id, const int &status)
{
    emit signalConnectFinished(id, status);
}

void P2PMessageSenderTcpAdaptor::onSignalReceivePercent(const QString &id,
                                                        const QString &msgId,
                                                        const int &msgType,
                                                        const QString &fileName,
                                                        const qint64 &size,
                                                        const qint64 &maxSize)
{
    if(size == maxSize)
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            qDebug() << "file open success: " << file.fileName();
            Message msg;
            QByteArray data = file.readAll();
            QDataStream in(&data, QIODevice::ReadOnly);
            in >> msg;
            qDebug() << "onSignalFileShareReceivePercent contacter: " << msg.common.id << msg.common.groupId << msg.content;

            file.close();
            QFile::remove(fileName);

            msg.common.state = MESSAGE_DOWNLOAD_SUCCESS;
            emit signalNewMessage(msg);
        }
        else
        {
            qDebug() << "file open error: " << file.errorString();
        }
    }

    if(!m_fileNameSet.contains(fileName))
    {
        Message message;
        message.common.groupId = id;
        message.common.state = MESSAGE_RECEIVING;
        message.common.type = msgType;

//        if(message.common.type != TEXT_MESSAGE)
//            emit signalNewMessage(message);

        m_fileNameSet.insert(fileName);
    }

    emit signalReceivePercent(id, msgId, size, maxSize);
}

void P2PMessageSenderTcpAdaptor::onSignalSendPercent(const QString &id,
                                                     const QString &msgId,
                                                     const int &msgType,
                                                     const QString &fileName,
                                                     const qint64 &size,
                                                     const qint64 &maxSize)
{
    if(size == maxSize)
        QFile::remove(fileName);
    emit signalSendPercent(id, msgId, size, maxSize);
}
