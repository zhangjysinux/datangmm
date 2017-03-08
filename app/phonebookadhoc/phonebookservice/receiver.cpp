#include <QBuffer>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include "receiver.h"
#include <QHostInfo>
#include "sender.h"

Receiver::Receiver(QObject *parent)
    : QObject(parent), m_socket(NULL)
{
}

void Receiver::setSocket(QUdpSocket *socket, const QString &address)
{
    QNetworkInterface wan0Interface = QNetworkInterface::interfaceFromName("wlan0");
    m_socket = socket;
    QHostAddress castHost(address);
    m_socket->leaveMulticastGroup(castHost, wan0Interface);
    bool joinFlag = m_socket->joinMulticastGroup(castHost, wan0Interface);
    if(!joinFlag)
        qDebug() << "join " << address << " fail.";
    else
        qDebug() << "join " << address << " success.";
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    qDebug() << "cast address:" << address;
    qDebug() << "local address:" << wan0Interface.name() << wan0Interface.index();
}

void Receiver::processPendingDatagrams()
{
    QByteArray datagram;
    PacketData tData;
    while (m_socket->hasPendingDatagrams())
    {
        int pendingSize = m_socket->pendingDatagramSize();

        QByteArray singleData;
        singleData.resize(pendingSize);
        m_socket->readDatagram(singleData.data(), singleData.size());

        QDataStream stream(&singleData, QIODevice::ReadOnly);
        stream >> tData;

        m_packetMap.insert(tData.index, tData.data);
        qDebug() << "m_socket->pendingDatagramSize()" << pendingSize << tData.index;

        if(tData.isEnd)
        {
            for(int i=0; i<=tData.index; i++)
            {
                QByteArray singleData = m_packetMap.value(i);
                if(singleData.isEmpty())
                {
                    qDebug() << "singleData index error " << i;
                    return;
                }
                datagram.append(singleData);
            }

            m_packetMap.clear();

            qDebug() << "datagram" << datagram.size();
            QBuffer buffer(&datagram);
            buffer.open(QIODevice::ReadOnly);

            QDataStream in(&buffer);
            Message message;
            in >> message;
            buffer.close();
            qDebug() << "message" << message.content << message.content.size();
            emit signalNewMessage(message);
        }
    }




//    QByteArray datagram;
//    while (m_socket->hasPendingDatagrams())
//    {
//        QByteArray singleData;
//        singleData.resize(m_socket->pendingDatagramSize());
//        m_socket->readDatagram(singleData.data(), singleData.size());
//        datagram.append(singleData);
//    }


//    qDebug() << "datagram" << datagram.size();
//    QBuffer buffer(&datagram);
//    buffer.open(QIODevice::ReadOnly);

//    QDataStream in(&buffer);
//    Message message;
//    in >> message;
//    buffer.close();
//    qDebug() << "message" << message.content << message.content.size();
//    emit signalNewMessage(message);
}
