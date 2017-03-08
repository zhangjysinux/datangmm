#include "smssenderinterface.h"

SmsSenderInterface *SmsSenderInterface::instance = NULL;

SmsSenderInterface *SmsSenderInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new SmsSenderInterface();
    }

    return instance;
}

SmsSenderInterface::SmsSenderInterface(QObject *parent) :
    QObject(parent)
{
    m_share = FileShare::newInstance();
    connect(m_share, SIGNAL(signalConnectFinished(int)), this, SLOT(onFileShareConnectFinished(int)));
    connect(m_share, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SLOT(onFileShareReceivePercent(QString,qint64,qint64)));
    connect(m_share, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onFileShareSendPercent(QString,qint64,qint64)));
    onSetReceivePath(".");
//    m_share->delegate = this;
}

SmsSenderInterface::~SmsSenderInterface()
{
//    m_share->deleteLater();
}

void SmsSenderInterface::onSetPort(const int &port)
{
    m_share->signalSetPort(port);
    m_share->signalInitServer();
}

void SmsSenderInterface::onSetReceivePath(const QString &path)
{
    m_share->signalSetReceivePath(path);
}

void SmsSenderInterface::onSetAddresses(const QStringList &addresses)
{
//    m_share->signalSetAddresses(addresses);
    IdNetworkMap idNetworkMap;
    foreach(const QString &address, addresses)
    {
        idNetworkMap.insert(QUuid::createUuid().toString(), QStringList(address));
    }

    m_share->signalSetAddresses(idNetworkMap);
}

int SmsSenderInterface::onConnect()
{
    m_share->signalConnect();
    return 0;
}

bool SmsSenderInterface::onIsConnected()
{
    return m_share->onIsConnected();
}

int SmsSenderInterface::onDisconnect()
{
    m_share->signalDisconnect();
    return 0;
}

int SmsSenderInterface::onSendFile(const QString &fileName)
{
    m_share->signalSendFile(fileName);
    return 0;
}

int SmsSenderInterface::onSendData(const QByteArray &data)
{
    QString fileName = "tmp_message_"+QUuid::createUuid().toString().remove("{").remove("}").remove("-")+".dat";
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open success: " << file.fileName();
        file.write(data);
        file.flush();
        file.close();

        //file send
        onSendFile(fileName);
    }
    else
    {
        qDebug() << "file open error: " << file.errorString();
    }
}

void SmsSenderInterface::onFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void SmsSenderInterface::onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
    if(size == maxSize)
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            qDebug() << "file open success: " << file.fileName();
            QByteArray data = file.readAll();
            file.close();
            QFile::remove(fileName);
            emit signalDataReceived(data);
        }
        else
        {
            qDebug() << "file open error: " << file.errorString();
        }
    }
}

void SmsSenderInterface::onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
    if(size == maxSize)
    {
        QFile::remove(fileName);
    }
}
