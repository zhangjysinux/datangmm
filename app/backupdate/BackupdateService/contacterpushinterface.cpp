#include "contacterpushinterface.h"

ContacterPushInterface *ContacterPushInterface::instance = NULL;

ContacterPushInterface *ContacterPushInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new ContacterPushInterface();
    }

    return instance;
}

ContacterPushInterface::ContacterPushInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));

//    m_share->delegate = this;
}

ContacterPushInterface::~ContacterPushInterface()
{
//    m_share->deleteLater();
}

void ContacterPushInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void ContacterPushInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void ContacterPushInterface::onSetAddresses(const QStringList &addresses)
{
//    m_share->signalSetAddresses(addresses);
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

void ContacterPushInterface::onSetAddresses(const IdNetworkMap &idNetworkMap)
{
    m_share->signalSetAddresses(idNetworkMap);
}

int ContacterPushInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool ContacterPushInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int ContacterPushInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int ContacterPushInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

void ContacterPushInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void ContacterPushInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void ContacterPushInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
