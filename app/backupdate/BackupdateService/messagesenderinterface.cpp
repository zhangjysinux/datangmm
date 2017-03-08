#include "messagesenderinterface.h"

MessageSenderInterface *MessageSenderInterface::instance = NULL;

MessageSenderInterface *MessageSenderInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new MessageSenderInterface();
    }

    return instance;
}

MessageSenderInterface::MessageSenderInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare2::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(QString,int)),
            this, SLOT(onSignalConnectFinished(QString,int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,QString,int,QString,qint64,qint64)),
            this, SLOT(onSignalReceivePercent(QString,QString,int,QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,QString,int,QString,qint64,qint64)),
            this, SLOT(onSignalSendPercent(QString,QString,int,QString,qint64,qint64)));

//    m_share->delegate = this;
}

MessageSenderInterface::~MessageSenderInterface()
{
//    m_share->deleteLater();
}

void MessageSenderInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void MessageSenderInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

int MessageSenderInterface::onConnect(const QString &id, const QString &address)
{
    m_share->signalConnect(id, address);
    return 0;
}

bool MessageSenderInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int MessageSenderInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int MessageSenderInterface::onSendFile(const QString &id,
                                       const QString &msgId,
                                       const QString &fileName,
                                       const int &msgType)
{
    m_share->signalSendFile(id, msgId, fileName, msgType);
    return 0;
}

void MessageSenderInterface::onSetLocalId(const QString &id)
{
    m_share->signalSetLocalId(id);
    return;
}

void MessageSenderInterface::onSignalConnectFinished(const QString &id, const int &status)
{
    emit signalConnectFinished(id, status);
}

void MessageSenderInterface::onSignalReceivePercent(const QString &id,
                                                    const QString &msgId,
                                                    const int &msgType,
                                                    const QString &fileName,
                                                    const qint64 &size,
                                                    const qint64 &maxSize)
{
    emit signalReceivePercent(id, msgId, msgType, fileName, size, maxSize);
}

void MessageSenderInterface::onSignalSendPercent(const QString &id,
                                                 const QString &msgId,
                                                 const int &msgType,
                                                 const QString &fileName,
                                                 const qint64 &size,
                                                 const qint64 &maxSize)
{
    emit signalSendPercent(id, msgId, msgType, fileName, size, maxSize);
}



