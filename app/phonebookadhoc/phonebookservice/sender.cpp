#include <QBuffer>
#include <QNetworkInterface>
#include <QDebug>
#include <QThread>
#include "sender.h"

Sender::Sender(QObject *parent)
    : QObject(parent), m_socket(NULL)
{
}

qint64 Sender::sendMessage(const Message &message)
{
    if(m_socket == NULL)
        return 0;

    QByteArray data;
    data.resize(sizeof(Message));

    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);

    QDataStream out(&buffer);
    out << message;
    buffer.close();
    qDebug() << "writeDatagram:" << buffer.data().size();

    unsigned int maxSize = data.size();
    unsigned int sumSize = 0;
    unsigned int size = maxSize;
    unsigned int currentSize = 0;
    unsigned int currentSize2 = 0;
    for(int i=0; ;i++)
    {
        //qDebug() << "send packet" << i;
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

        if(pData.isEnd && !pData.isStart)
            QThread::msleep(50);

        int sendSize = m_socket->writeDatagram(sendData, m_castHost, m_port);
        if(sendSize == -1)
            return sendSize;
        sumSize += sendSize;

        currentSize = currentSize + size;

        if(pData.isEnd)
            break;
    }

    return sumSize;//m_socket->writeDatagram(buffer.data(), m_castHost, m_port);
}

void Sender::setSocket(QUdpSocket *socket, const QString &address, const int &port)
{
    m_castHost = QHostAddress(address);
    m_port = port;
    m_socket = socket;
}
