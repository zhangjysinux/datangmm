#include <QDateTime>
#include <QHostAddress>
#include "udpsender.h"

UdpSender *UdpSender::m_instance = NULL;

UdpSender::UdpSender(QObject *parent) :
    QObject(parent)
{
    m_socket = new QUdpSocket(this);
}

UdpSender *UdpSender::getInstance()
{
    if(m_instance == NULL)
        m_instance = new UdpSender();

    return m_instance;
}

void UdpSender::sendText(QString text, QString address)
{
    QString sendText = QDateTime::currentDateTime().toString("HH:mm:ss:");
    sendText = sendText + text + "\n";

    qDebug() << "send address:" << address;
    qDebug() << "send text:" << sendText;
    QHostAddress host(address);
    QByteArray array = sendText.toLocal8Bit();
    m_socket->writeDatagram(array, host, 2002);
}
