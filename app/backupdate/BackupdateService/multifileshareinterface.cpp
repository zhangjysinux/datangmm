#include "multifileshareinterface.h"
MultiFileShareInterface *MultiFileShareInterface::instance = NULL;
MultiFileShareInterface *MultiFileShareInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new MultiFileShareInterface();
    }

    return instance;
}


MultiFileShareInterface::MultiFileShareInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));
}

MultiFileShareInterface::~MultiFileShareInterface()
{
    m_share->deleteLater();
}

void MultiFileShareInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void MultiFileShareInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void MultiFileShareInterface::onSetAddresses(const QStringList &addresses)
{
//    m_share->signalSetAddresses(addresses);
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

int MultiFileShareInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool MultiFileShareInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int MultiFileShareInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int MultiFileShareInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

void MultiFileShareInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void MultiFileShareInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void MultiFileShareInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
