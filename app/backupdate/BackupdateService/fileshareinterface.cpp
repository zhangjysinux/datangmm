#include "fileshareinterface.h"
FileShareInterface *FileShareInterface::instance = NULL;
FileShareInterface *FileShareInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new FileShareInterface();
    }

    return instance;
}


FileShareInterface::FileShareInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));
}

FileShareInterface::~FileShareInterface()
{
    m_share->deleteLater();
}

void FileShareInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void FileShareInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void FileShareInterface::onSetAddresses(const QStringList &addresses)
{
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

int FileShareInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool FileShareInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int FileShareInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int FileShareInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

void FileShareInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void FileShareInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void FileShareInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}

