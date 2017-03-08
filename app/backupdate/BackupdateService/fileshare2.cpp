#include "fileshare2.h"
#include <QFile>
#include <QtConcurrent/QtConcurrent>

#define buffer_size 1024

FileShare2 * FileShare2::newInstance()
{
    FileShare2 *fileShare = new FileShare2();
    QThread *thread = new QThread();
    fileShare->moveToThread(thread);
    thread->start();
    return fileShare;
}

FileShare2::FileShare2(QObject *parent) :
    QObject(parent),
    m_receiveFilePath("/home/user/receivedfiles"),
    m_server(NULL)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    connect(this, SIGNAL(signalSetConnectType(int)), this, SLOT(onSetConnectType(int)));
    connect(this, SIGNAL(signalSetPort(int)), this, SLOT(onSetPort(int)));
    connect(this, SIGNAL(signalInitServer()), this, SLOT(onInitServer()));

    connect(this, SIGNAL(signalSetReceivePath(QString)),
            this, SLOT(onSetReceivePath(QString)));
    connect(this, SIGNAL(signalConnect(QString,QString)),
            this, SLOT(onConnect(QString,QString)));
    connect(this, SIGNAL(signalDisconnect()), this, SLOT(onDisconnect()));
    connect(this, SIGNAL(signalSendFile(QString,QString,QString,int)),
            this, SLOT(onSendFile(QString,QString,QString,int)));
    connect(this, SIGNAL(signalSetLocalId(QString)),
            this, SLOT(onSetLocalId(QString)));
}

FileShare2::~FileShare2()
{
//    delete m_defaultDelegate;
}

void FileShare2::onSetConnectType(const int &netType)
{
    m_ifaceName = "wlan0";
}

void FileShare2::onSetPort(const int &port)
{
    m_port = port;
}

void FileShare2::onInitServer()
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

void FileShare2::onSetReceivePath(const QString &path)
{
    m_receiveFilePath = path;
}

int FileShare2::onConnect(const QString &id, const QString &address)
{
    QMutexLocker locker(&m_mutex);

    if(!m_idSocketMap.contains(id) && !id.isEmpty())
    {
        QTcpSocket *socket = new QTcpSocket(this);
        qDebug() << "fileshare2 onconnect: " << id << address;
//        socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        socket->setProperty("socketid", id);
        m_idSocketMap.insert(id, socket);


        QHostAddress addr(address);
        socket->connectToHost(addr, m_port, QIODevice::WriteOnly);
        connect(socket, SIGNAL(connected()), this, SLOT(onConnectServerFinished()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnectServerFinished()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onConnectServerError(QAbstractSocket::SocketError)));

    }
    else
    {
        QTcpSocket *socket = m_idSocketMap.value(id);
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            emit signalConnectFinished(id, 1);
        }
        else
        {
            emit signalConnectFinished(id, 0);
        }
    }
}

bool FileShare2::onIsConnected()
{
    QMutexLocker locker(&m_mutex);

    return !m_idSocketMap.isEmpty();
}

int FileShare2::onDisconnect()
{
    qDebug() << "onDisconnect start";
    for(QMap<QString,QTcpSocket*>::iterator it = m_idSocketMap.begin(); it != m_idSocketMap.end(); it++)
    {
        QTcpSocket *socket = it.value();
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
    m_idSocketMap.clear();
    qDebug() << "onDisconnect end";
}

int FileShare2::onSendFile(const QString &id,
                           const QString &msgId,
                           const QString &fileName,
                           const int &msgType)
{
    QFile *file = new QFile(fileName);
    if(file->open(QIODevice::ReadOnly))
    {
        qDebug() << "file.open success:" << fileName;
        file->setProperty("msgtype", msgType);
        file->setProperty("msgid", msgId);
        m_sendFiles2.push_back(qMakePair(id,file));
        onSendFiles();
    }
    else
    {
        qDebug() << "file.open error:" << file->errorString() << fileName;
    }
}

void FileShare2::onSetLocalId(const QString &id)
{
    m_id = id;
}

void FileShare2::onSendFiles()
{
    while(1)
    {
        if(m_sendFiles2.isEmpty())
            break;

        QPair<QString,QFile*> idFile = m_sendFiles2.first();
        QFile *file = idFile.second;

        qDebug() << "m_sendFiles2 size" << m_sendFiles2.size();

        while(!file->atEnd())
        {
            QByteArray buffer = file->read(buffer_size);
            qDebug() << "read buffer :" << buffer.size();
            //send package data every times
            QTcpSocket *socket = m_idSocketMap.value(idFile.first);
            if(socket == NULL)
            {
                qDebug() << "QTcpSocket not exists ";
                file->close();
                file->deleteLater();
                m_sendFiles2.clear();
                break;
            }

            QString socketId =socket->property("socketid").toString();
            int msgType = file->property("msgtype").toInt();
            QString msgId = file->property("msgid").toString();

            PackageData package;
            package.data = buffer;
            package.fileName = file->fileName();
            package.size = file->pos();
            package.maxSize = file->size();
            package.type = msgType;
            package.uuid = msgId;
            package.gId = m_id;
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
            if(size < 0)
            {
                m_sendFiles2.clear();
                file->close();
                file->deleteLater();
                return;
            }

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
                m_sendFiles2.clear();
                file->close();
                file->deleteLater();
            }

            emit signalSendPercent(socketId, msgId, msgType, package.fileName, package.size, package.maxSize);
            qDebug() << "size: " << size << package.size << package.maxSize << "flush:" << flush;
        }



    }
}

void FileShare2::onConnectServerFinished()
{
    QMutexLocker locker(&m_mutex);

    qDebug() << "onConnectServerFinished";
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        QString socketid = socket->property("socketid").toString();
        emit signalConnectFinished(socketid, 1);
    }
}

void FileShare2::onDisconnectServerFinished()
{
    qDebug() << "onDisconnectServerFinished";
    QTcpSocket *socket = static_cast<QTcpSocket *>(this->sender());
    if(socket != NULL)
    {
        QString socketid = socket->property("socketid").toString();
        m_idSocketMap.remove(socketid);
        socket->deleteLater();
        emit signalConnectFinished(socketid, 0);
    }
}

void FileShare2::onConnectServerError(QAbstractSocket::SocketError error)
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
        QString socketid = socket->property("socketid").toString();
        m_idSocketMap.remove(socketid);
        socket->deleteLater();
        emit signalConnectFinished(socketid, 0);
    }
}

void FileShare2::onConnectedClientError(QAbstractSocket::SocketError error)
{
    qDebug() << "onConnectedClientError" << error;
    QTcpSocket *newClient = static_cast<QTcpSocket *>(this->sender());
    if(newClient != NULL)
    {
//        qDebug() << "newClient->deleteLater();";
//        newClient->deleteLater();
    }
}

void FileShare2::onNewConnection()
{
     qDebug() << "onNewConnection";
     if(m_server->hasPendingConnections())
     {
         QTcpSocket *newClient = m_server->nextPendingConnection();
//         newClient->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
         qDebug() << "onNewConnection" << newClient->peerName() << newClient->peerAddress().toString() << newClient->peerPort();
         connect(newClient, SIGNAL(readyRead()), this, SLOT(onNewMessage()));
         connect(newClient, SIGNAL(error(QAbstractSocket::SocketError)),
                 this, SLOT(onConnectedClientError(QAbstractSocket::SocketError)));
     }
}

void FileShare2::onNewMessage()
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
            emit signalReceivePercent(pData.gId,
                                      pData.uuid,
                                      pData.type,
                                      fileName,
                                      pData.size,
                                      pData.maxSize);
        }
    }
}
