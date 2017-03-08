#include "groupshareinterface.h"

GroupShareInterface *GroupShareInterface::instance = NULL;

GroupShareInterface *GroupShareInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new GroupShareInterface();
    }

    return instance;
}

GroupShareInterface::GroupShareInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));

//    m_share->delegate = this;
}

GroupShareInterface::~GroupShareInterface()
{
//    m_share->deleteLater();
}

void GroupShareInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void GroupShareInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void GroupShareInterface::onSetAddresses(const QStringList &addresses)
{
//    m_share->signalSetAddresses(addresses);
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

int GroupShareInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool GroupShareInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int GroupShareInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int GroupShareInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

void GroupShareInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void GroupShareInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void GroupShareInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
