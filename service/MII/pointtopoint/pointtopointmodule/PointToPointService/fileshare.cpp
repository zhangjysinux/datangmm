#include "fileshare.h"
#include <QFile>
#include <QtConcurrent/QtConcurrent>

#define buffer_size 1024

FileShare * FileShare::newInstance()
{
    FileShare *fileShare = new FileShare();
    QThread *thread = new QThread();
    fileShare->moveToThread(thread);
    thread->start();
    return fileShare;
}

FileShare::FileShare(QObject *parent) :
    QObject(parent),
    m_receiveFilePath("/home/user/receivedfiles"),
    m_server(NULL)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    connect(this, SIGNAL(signalSetConnectType(int)), this, SLOT(onSetConnectType(int)));
    connect(this, SIGNAL(signalSetPort(int)), this, SLOT(onSetPort(int)));
    connect(this, SIGNAL(signalInitServer()), this, SLOT(onInitServer()));

    connect(this, SIGNAL(signalSetReceivePath(QString)), this, SLOT(onSetReceivePath(QString)));
    connect(this, SIGNAL(signalSetAddresses(IdNetworkMap)), this, SLOT(onSetAddresses(IdNetworkMap)));
    connect(this, SIGNAL(signalConnect()), this, SLOT(onConnect()));
    connect(this, SIGNAL(signalDisconnect()), this, SLOT(onDisconnect()));
    connect(this, SIGNAL(signalSendFile(QString)), this, SLOT(onSendFile(QString)));


//    m_defaultDelegate = new FileShareDelegate();
//    delegate = m_defaultDelegate;
}

FileShare::~FileShare()
{
//    delete m_defaultDelegate;
}

void FileShare::onSetConnectType(const int &netType)
{
    m_ifaceName = "wlan0";
}

void FileShare::onSetPort(const int &port)
{
    m_port = port;
}

void FileShare::onInitServer()
{
    if(m_server)
    {
        disconnect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        m_server->close();
        delete m_server;
    }

    m_server = new QTcpServer(this);
    m_server->listen(QHostAddress::AnyIPv4, m_port);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void FileShare::onSetReceivePath(const QString &path)
{
    m_receiveFilePath = path;
}

void FileShare::onSetAddresses(const IdNetworkMap &idNetworkMap)
{
//    QList<QStringList> values = idNetworkMap.values();
//    for(int i)
    m_idNetworkMap = idNetworkMap;
//    m_addresses = idNetworkMap;
//    m_addresses.removeDuplicates();
}


int FileShare::onConnect()
{
    QMutexLocker locker(&m_mutex);

    foreach(QTcpSocket *socket, m_connectedClientList)
    {
        disconnect(socket, SIGNAL(connected()), this, SLOT(onConnectServerFinished()));
        disconnect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnectServerFinished()));
        disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onConnectServerError(QAbstractSocket::SocketError)));

        socket->close();
        socket->deleteLater();
        qDebug() << "before connect, delete socket: " << socket;
    }

    m_connectedClientList.clear();
    m_wantConnectClientList.clear();
    m_connectedIds.clear();
    QStringList idList = m_idNetworkMap.keys();
    foreach(const QString &id, idList)
    {
        QStringList addresses = m_idNetworkMap.value(id);
        foreach(const QString &address, addresses)
        {
            QTcpSocket *socket = new QTcpSocket(this);
            socket->setProperty("socketid", id);
            m_wantConnectClientList.push_back(socket);

            QHostAddress addr(address);
            socket->connectToHost(addr, m_port, QIODevice::WriteOnly);
            connect(socket, SIGNAL(connected()), this, SLOT(onConnectServerFinished()));
            connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnectServerFinished()));
            connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onConnectServerError(QAbstractSocket::SocketError)));
        }
    }
}

bool FileShare::onIsConnected()
{
    QMutexLocker locker(&m_mutex);

    return m_wantConnectClientList.isEmpty();
}

int FileShare::onDisconnect()
{
    qDebug() << "onDisconnect start";
    for(QList<QTcpSocket*>::iterator it = m_connectedClientList.begin(); it != m_connectedClientList.end(); it++)
    {
        QTcpSocket *socket = *it;
        disconnect(socket, SIGNAL(connected()), this, SLOT(onConnectServerFinished()));
        disconnect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnectServerFinished()));
        disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onConnectServerError(QAbstractSocket::SocketError)));

        socket->disconnectFromHost();
        if (socket->state() == QAbstractSocket::UnconnectedState ||
               socket->waitForDisconnected(3000))
        {
            qDebug() << "onDisconnect: " << socket;
        }
        socket->close();
        socket->deleteLater();
    }
    m_connectedClientList.clear();
    qDebug() << "onDisconnect end";
}

int FileShare::onSendFile(const QString &fileName)
{
    QFile *file = new QFile(fileName);
    if(file->open(QIODevice::ReadOnly))
    {
        qDebug() << "file.open success:" << fileName;
        m_sendFiles2.push_back(file);
        onSendFiles();
//        if(m_future.isFinished())
//            m_future = QtConcurrent::run(this, &FileShareInterface::onSendFiles);
    }
    else
    {
        qDebug() << "file.open error:" << file->errorString() << fileName;
    }
}

void FileShare::onSendFiles()
{
    while(1)
    {
        for(QList<QFile*>::iterator it = m_sendFiles.begin(); it != m_sendFiles.end(); it++)
        {
            QFile *file = *it;
            QByteArray buffer = file->read(buffer_size);

            //send package data every times
            for(QList<QTcpSocket*>::iterator itSocket = m_connectedClientList.begin(); itSocket != m_connectedClientList.end(); itSocket++)
            {
                QTcpSocket *socket = *itSocket;

                PackageData package;
                package.data = buffer;
                package.fileName = file->fileName();
                package.size = file->pos();
                package.maxSize = file->size();
                QByteArray data;
                QDataStream out(&data, QIODevice::WriteOnly);
                out << package;

                unsigned int length = data.size();
                char lengthStr[sizeof(int)] = {0};
                for(int i=0; i<sizeof(int); i++)
                    lengthStr[i] = 0;
                strncpy(lengthStr, (char *)(&length), sizeof(int));
                data.insert(0, lengthStr, sizeof(int));

                qint64 size = socket->write(data);

                bool flush = false;
                for(int i=0; socket->bytesToWrite() > 0; i++)
                {
                    flush = socket->flush();
                    if(socket->bytesToWrite() > 0)
                        QThread::msleep(1);
                    qDebug() << "flush time" << i;
                }

                //read over just remove it
                if(file->atEnd())
                {
                    m_sendFiles.erase(it);
                    file->close();
                    file->deleteLater();
                }

//                delegate->onFileShareSendPercent(package.fileName, package.size, package.maxSize);
                emit signalSendPercent(package.fileName, package.size, package.maxSize);
                qDebug() << "size: " << size << package.size << package.maxSize << "flush:" << flush;
            }
        }
        if(m_sendFiles.isEmpty())
        {
            if(m_sendFiles2.isEmpty())
            {
                break;
            }
            else
            {
                m_sendFiles.append(m_sendFiles2);
                m_sendFiles2.clear();
            }
        }
    }
}

void FileShare::onConnectServerFinished()
{
    QMutexLocker locker(&m_mutex);

    qDebug() << "onConnectServerFinished";
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        QString socketid = socket->property("socketid").toString();
        if(!m_connectedIds.contains(socketid))
        {
            m_connectedIds.insert(socketid);
            m_connectedClientList.push_back(socket);
        }
        else
        {
            socket->deleteLater();
        }

        m_wantConnectClientList.removeAll(socket);
    }

    if(m_wantConnectClientList.isEmpty())
    {
//        delegate->onFileShareConnectFinished(m_connectedClientList.size());
        emit signalConnectFinished(m_connectedClientList.size());
//        onSendFile("/home/user/myimage.jpg");
    }
}

void FileShare::onDisconnectServerFinished()
{
    qDebug() << "onDisconnectServerFinished";
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        m_connectedClientList.removeAll(socket);
        socket->deleteLater();
    }
}

void FileShare::onConnectServerError(QAbstractSocket::SocketError error)
{
    QMutexLocker locker(&m_mutex);

    qDebug() << "onConnectServerError" << error;
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        qDebug() << "onConnectServerError ip: "
                 << socket->peerAddress()
                 << socket->peerName()
                 << socket->peerPort();
        m_connectedClientList.removeAll(socket);
        socket->deleteLater();

        m_wantConnectClientList.removeAll(socket);
        if(m_wantConnectClientList.isEmpty())
        {
//            delegate->onFileShareConnectFinished(m_connectedClientList.size());
            emit signalConnectFinished(m_connectedClientList.size());
//            onSendFile("/home/user/myimage.jpg");
        }
    }
}

void FileShare::onNewConnection()
{
     qDebug() << "onNewConnection";
     if(m_server->hasPendingConnections())
     {
         QTcpSocket *newClient = m_server->nextPendingConnection();
         qDebug() << "onNewConnection" << newClient->peerName() << newClient->peerAddress().toString() << newClient->peerPort();
         connect(newClient, SIGNAL(readyRead()), this, SLOT(onNewMessage()));
     }
}

void FileShare::onNewMessage()
{
    qDebug() << "onNewMessage";
    QTcpSocket *newClient = static_cast<QTcpSocket *>(this->sender());
    if(newClient != NULL)
    {
        qDebug() << "onNewMessage: " << newClient;

        while(1)
        {
            PackageData pData;
            int length = 0;
            if(newClient->bytesAvailable() < sizeof(int))
                return;
            else
            {
                QByteArray data = newClient->peek(sizeof(int));
                length = *(int *)data.data();
                qDebug() << "length: " << length << "data.size" << data.size();
                if(newClient->bytesAvailable() < length+sizeof(int))
                {
                    return;
                }
            }

            newClient->read(sizeof(int));
            QByteArray data = newClient->read(length);
            QDataStream in(&data, QIODevice::ReadOnly);
            in >> pData;
            if(pData.fileName.isEmpty())
                qDebug() << "pData.fileName is empty";
            qDebug() << "receiveData: " << pData.fileName << pData.size << pData.maxSize;

            QString fileName = m_receiveFilePath + "/" + pData.fileName.section("/", -1);

            QFile *file = NULL;
            if(!m_receiveFilesMap.contains(fileName))
            {
                //open file in the start
                file = new QFile(fileName);
                if(file->open(QIODevice::WriteOnly))
                {
                    qDebug() << "file.open success: " << fileName;
                    m_receiveFilesMap.insert(fileName, file);
                }
                else
                {
                    qDebug() << "file.open error : " << file->errorString() << file->fileName();
                    continue;
                }
            }
            else
            {
                file = m_receiveFilesMap.value(fileName);
            }

            file->write(pData.data);

            //close file in the end
            if(pData.size == pData.maxSize)
            {
                qDebug() << "close file: " << file->fileName();
                file->flush();
                file->close();
                m_receiveFilesMap.remove(fileName);
            }

//            delegate->onFileShareReceivePercent(fileName, pData.size, pData.maxSize);
            emit signalReceivePercent(fileName, pData.size, pData.maxSize);
        }
    }
}
