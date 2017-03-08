#include "contactershareinterface.h"

ContacterShareInterface *ContacterShareInterface::instance = NULL;

ContacterShareInterface *ContacterShareInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new ContacterShareInterface();
    }

    return instance;
}

ContacterShareInterface::ContacterShareInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));

//    m_share->delegate = this;
}

ContacterShareInterface::~ContacterShareInterface()
{
//    m_share->deleteLater();
}

void ContacterShareInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void ContacterShareInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void ContacterShareInterface::onSetAddresses(const QStringList &addresses)
{
//    m_share->signalSetAddresses(addresses);
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

void ContacterShareInterface::onSetAddresses(const IdNetworkMap &idNetworkMap)
{
    m_share->signalSetAddresses(idNetworkMap);
}

int ContacterShareInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool ContacterShareInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int ContacterShareInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int ContacterShareInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

void ContacterShareInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void ContacterShareInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void ContacterShareInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
