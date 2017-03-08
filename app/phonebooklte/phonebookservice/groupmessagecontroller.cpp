#include "groupmessagecontroller.h"
#include "sender.h"
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

#define SEND_SIZE 1024

GroupMessageController::GroupMessageController(QObject *parent) :
    QObject(parent),m_tcpSocket(NULL), m_isSending(false), m_isReceiving(false)
{
    QtConcurrent::run(this, &GroupMessageController::onSocketReceivedControll);
}

void GroupMessageController::setPort(const int &port)
{
    m_port = port;
}

void GroupMessageController::onConnectServer()
{
//    qDebug() << "onConnectServer--------------";
    onConnect();
//    QtConcurrent::run(this, &GroupMessageController::onConnect);
}

void GroupMessageController::onConnect()
{
    if(!m_tcpSocket)
    {
        m_tcpSocketList.clear();
        for(int i=0; i<20; i++)
        {
            QString ip = "192.168.1." + QString::number(i+2);
            connect(ip, m_port);
            qDebug() << "onConnect" << ip << m_port;
        }
    }
    else
    {
        //qDebug() << "check connect is exist";
    }
}

bool GroupMessageController::connect(const QString &ip, const int &port)
{
    QTcpSocket *tcpSocket = new QTcpSocket();

    QObject::connect(tcpSocket,SIGNAL(connected()),this,SLOT(onSocketConnected()));
    QObject::connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this, SLOT(onSocketReceived()));
    QObject::connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(QAbstractSocket::SocketError)));

    m_tcpSocketList.push_back(tcpSocket);

    QHostAddress serverAddr(ip);

    tcpSocket->connectToHost(serverAddr, port);
//    if (m_tcpSocket.waitForConnected(3000))
//        qDebug() << "connect success."+ip+":"+QString::number(port);
//    else
//        qDebug() << "connect fail."+ip+":"+QString::number(port);
}

qint64 GroupMessageController::sendMessage(const Message& message)
{
//    if(!m_isSending)
//    {
//        m_isSending = true;
//        QtConcurrent::run(this, &GroupMessageController::onSendMessage, message);
//    }
    onSendMessage(message);
}

qint64 GroupMessageController::onSendMessage(const Message& message)
{
    m_isSending = true;

    if(!m_tcpSocket)
        return -1;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;


    unsigned int maxSize = data.size();
    unsigned int sumSize = 0;
    unsigned int size = SEND_SIZE;
    unsigned int currentSize = 0;
    unsigned int currentSize2 = 0;
    for(int i=0; ;i++)
    {
        qDebug() << "send packet" << i;
        currentSize2 = currentSize + size;
        if(currentSize2 > maxSize)
        {
            currentSize2 = maxSize;
        }

        QByteArray singleData = data.mid(currentSize, currentSize2-currentSize);

        PacketData pData;
        pData.size = currentSize2 - currentSize;
        pData.maxSize = maxSize;
        pData.index = i;
        pData.isStart = (i==0);
        pData.isEnd = (currentSize2 == maxSize);
        pData.data = singleData;

        QByteArray sendData;
        QDataStream out(&sendData, QIODevice::WriteOnly);
        out << pData;

        int sendS = sendData.size();
        for(int k=0; k<size+22-sendS; k++)
        {
            sendData.append('A');
        }

//        if(pData.isEnd && !pData.isStart)
//            QThread::msleep(50);

//        uint count = sendData.size();
//        char countStr[4] = {0};
//        memcpy(countStr, (char*)(&count), sizeof(count));
//        QByteArray countString(countStr, 4);

        int sendSize = m_tcpSocket->write(sendData);
//        qDebug() << "flush: " << m_tcpSocket->flush();
//        qDebug() << "flush: " << m_tcpSocket->waitForBytesWritten();
//        qDebug() << "m_tcpSocket write: " << sendSize;
        if(sendSize == -1)
        {
            return sendSize;
        }
        sumSize += sendSize;

        currentSize = currentSize + size;

        if(pData.isEnd)
            break;
    }

    qDebug() << "m_tcpSocket write: " << sumSize;

    m_isSending = false;
    return sumSize;
}

void GroupMessageController::onSocketReceived()
{
    QByteArray data = m_tcpSocket->readAll();
    m_mutex.lock();
    m_queue.enqueue(data);
    m_mutex.unlock();
//    if(!m_isReceiving)
//    {
//        m_isReceiving = true;
//        QtConcurrent::run(this, &GroupMessageController::onSocketReceivedControll);
//    }

//    onSocketReceivedControll();
}

void GroupMessageController::onSocketReceivedControll()
{
//    m_isReceiving = true;
//    if(!m_tcpSocket)
//        QThread::msleep(10);

    int size = SEND_SIZE;
    QByteArray datagram;
    PacketData tData;

    QByteArray singleData;

    while ( 1 )
    {
        m_mutex.lock();
        if(!m_queue.isEmpty())
        {
            m_data.append( m_queue.dequeue() );
            m_mutex.unlock();
        }
        else
        {
            if(m_data.isEmpty())
            {
                m_mutex.unlock();
                QThread::msleep(5);
                continue;
            }
        }
        m_mutex.unlock();

        if(m_data.size() < size + 22)
        {
            continue;
        }
        else
        {
            singleData = m_data.left(size + 22) ;
            m_data.remove(0, size + 22);
        }

        QDataStream stream(&singleData, QIODevice::ReadOnly);
        stream >> tData;

        m_packetMap.insert(tData.index, tData.data);
//        qDebug() << "m_packetMap.insert" << tData.index << tData.size;

        if(tData.isEnd)
        {
            for(int i=0; i<=tData.index; i++)
            {
                QByteArray singleData = m_packetMap.value(i);
                if(singleData.isEmpty())
                {
                    qDebug() << "singleData index error " << i;
                    m_packetMap.clear();
                    m_data.clear();
                    continue;
                }
                datagram.append(singleData);
            }

            m_packetMap.clear();

            qDebug() << "datagram" << datagram.size();

            QDataStream in(&datagram, QIODevice::ReadOnly);
            Message message;
            in >> message;
            datagram.clear();
            qDebug() << "message" << message.content << message.content.size();
            emit signalNewMessage(message);
            continue;
        }
    }
//    m_isReceiving = false;


//    while ( m_tcpSocket->bytesAvailable() > 0 )
//    {
//        QByteArray singleData;
//        if(m_tcpSocket->bytesAvailable() < size + 22)
//        {
//            m_tcpSocket->waitForReadyRead();
//            continue;
//        }
//        else
//        {
//            singleData = m_tcpSocket->read(size + 22);
//        }

//        QDataStream stream(&singleData, QIODevice::ReadOnly);
//        stream >> tData;

//        m_packetMap.insert(tData.index, tData.data);
//        qDebug() << "m_packetMap.insert" << tData.index << tData.size;

//        if(tData.isEnd)
//        {
//            for(int i=0; i<=tData.index; i++)
//            {
//                QByteArray singleData = m_packetMap.value(i);
//                if(singleData.isEmpty())
//                {
//                    qDebug() << "singleData index error " << i;
//                    m_packetMap.clear();
//                    return;
//                }
//                datagram.append(singleData);
//            }

//            m_packetMap.clear();

//            qDebug() << "datagram" << datagram.size();

//            QDataStream in(&datagram, QIODevice::ReadOnly);
//            Message message;
//            in >> message;
//            qDebug() << "message" << message.content << message.content.size();
//            emit signalNewMessage(message);
//            break;
//        }
//    }
//    m_isReceiving = false;
}

void GroupMessageController::onSocketConnected()
{
    QTcpSocket *tcpSocket = static_cast<QTcpSocket *>(this->sender());
    if(m_tcpSocket == NULL)
    {
        m_tcpSocket = tcpSocket;
        qDebug() << "onSocketConnected" << m_tcpSocket->peerAddress().toString();
    }
    else
    {
        QObject::disconnect(tcpSocket,SIGNAL(connected()),this,SLOT(onSocketConnected()));
        QObject::disconnect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
        QObject::disconnect(tcpSocket,SIGNAL(readyRead()),this, SLOT(onSocketReceived()));
        QObject::disconnect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onError(QAbstractSocket::SocketError)));
        tcpSocket->close();
        tcpSocket->deleteLater();
        qDebug() << "onSocketConnected delete";
    }
}

void GroupMessageController::onSocketDisconnected()
{
    qDebug() << "onSocketDisconnected";
    QTcpSocket *tcpSocket = static_cast<QTcpSocket *>(this->sender());
    if(tcpSocket == NULL)
        return;

    tcpSocket->close();
    delete tcpSocket;
    m_tcpSocketList.removeAll(tcpSocket);

    if(tcpSocket == m_tcpSocket)
        m_tcpSocket = NULL;
}

void GroupMessageController::onError(QAbstractSocket::SocketError error)
{
//    QTcpSocket *tcpSocket = static_cast<QTcpSocket *>(this->sender());
//    if(tcpSocket == NULL)
//        return;
//    qDebug() << "error" << error << tcpSocket->errorString();
//    tcpSocket->close();
//    delete tcpSocket;
//    m_tcpSocketList.removeAll(tcpSocket);
}

