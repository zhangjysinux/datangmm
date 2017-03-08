#include <QVariant>
#include "udpreceiver.h"

UdpReceiver *UdpReceiver::m_instance = NULL;
UdpReceiver::UdpReceiver(QObject *parent) :
    QObject(parent)
{
    m_socket = new QUdpSocket(this);
    m_socket->bind(QHostAddress::Any, 2002);

    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(onReadPendingDatagrams()));
}

UdpReceiver *UdpReceiver::getInstance()
{
    if(m_instance == NULL)
        m_instance = new UdpReceiver;

    return m_instance;
}

void UdpReceiver::onReadPendingDatagrams()
{
    while(m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString str = QString::fromLocal8Bit(datagram);
        qDebug() << "sender:" << sender.toString();
        qDebug() << "sender port:" << senderPort;
        qDebug() << "receive str:" << str;

        QVariant strVar = QVariant::fromValue(str);
        emit signalReceive(strVar);
    }
}
