#include "pointtopointinterface.h"

PointToPointInterface *PointToPointInterface::instance = NULL;

PointToPointInterface *PointToPointInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new PointToPointInterface();
    }

    return instance;
}

PointToPointInterface::PointToPointInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));

//    m_share->delegate = this;
}

PointToPointInterface::~PointToPointInterface()
{
//    m_share->deleteLater();
}

void PointToPointInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void PointToPointInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void PointToPointInterface::onSetAddresses(const QStringList &addresses)
{
//    m_share->signalSetAddresses(addresses);
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

void PointToPointInterface::onSetAddresses(const IdNetworkMap &idNetworkMap)
{
    m_share->signalSetAddresses(idNetworkMap);
}

int PointToPointInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool PointToPointInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int PointToPointInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int PointToPointInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

void PointToPointInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void PointToPointInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void PointToPointInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
