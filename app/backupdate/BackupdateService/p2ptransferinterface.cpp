#include "p2ptransferinterface.h"
#include <QDBusConnection>
#include <QHostAddress>
#include <QNetworkInterface>
P2pTransferInterface *P2pTransferInterface::instance = NULL;
P2pTransferInterface *P2pTransferInterface::getInstance()
{
    if(instance == NULL)
    {
        instance = new P2pTransferInterface();
    }

    return instance;
}

P2pTransferInterface::P2pTransferInterface(QObject *parent) :
    QObject(parent)
{
    //initSocket("192.168.36.91",m_port);
    //connectToServer("192.168.36.248",m_port);

    connect(&m_sendTimer, SIGNAL(timeout()), this, SLOT(onSendPacketData()));
    //connect(&m_receivedTimer, SIGNAL(timeout()), this, SLOT(onReceivedDataRespond(PackageData)));

    //onSendFile("/home/user/DCIM/Camera/IMG_20160820_133323.jpg");
}

void P2pTransferInterface::initSocket(QString localIp, int port)
{
    qDebug() << "initSocket";
//    m_udpSocketRead = new QUdpSocket(this);
//    m_udpSocketWrite = new QUdpSocket(this);
//    foreach (QHostAddress address, QNetworkInterface::allAddresses())
//    {
//        qDebug() << "Address:" << address << address.protocol();
//        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
//                !address.toString().contains("100.100") &&
//                !address.isLoopback())
//        {
//            if (address.toString().left(10) == "192.168.15"){
//                localAddress.setAddress(address.toString());
//                break;
//            }
//            //return address.toString();
//        }
//    }
    localAddress.setAddress(localIp);

//    qDebug() << "open" << m_udpSocketRead->open(QIODevice::ReadWrite);
//    qDebug() << "bind" << m_udpSocketRead->bind(QHostAddress::Any, m_port);
//    connect(m_udpSocketRead, SIGNAL(readyRead()),
//            this, SLOT(onReadSocket()));
}

void P2pTransferInterface::onSetPort(int port)
{
    m_port = port;

    m_udpSocketRead = new QUdpSocket(this);
    m_udpSocketWrite = new QUdpSocket(this);

    qDebug() << "open" << m_udpSocketRead->open(QIODevice::ReadWrite);
    qDebug() << "bind" << m_udpSocketRead->bind(QHostAddress::Any, m_port);
    connect(m_udpSocketRead, SIGNAL(readyRead()),
            this, SLOT(onReadSocket()));
    //initSocket(m_port);
}

void P2pTransferInterface::readPendingDatagrams()
{
    while (m_udpSocketRead->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocketRead->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        m_udpSocketRead->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
    }
}

int P2pTransferInterface::sendMessage(QString msg)
{
    return m_udpSocketWrite->writeDatagram(msg.toLatin1(),localAddress,m_port);
}

void P2pTransferInterface::onConnectToServer(QString ip, int port)
{
    remoteAddress.setAddress(ip);
    m_udpSocketWrite->disconnectFromHost();
    m_udpSocketWrite->connectToHost(remoteAddress,port,QIODevice::ReadWrite);
}

int P2pTransferInterface::onSendFile(const QString &fileName)
{
    if (!m_udpSocketWrite)
        return -1;

    if(m_sendingFile.fileName().isEmpty())
    {
        m_sendingFile.setFileName(fileName);
        if(m_sendingFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "onSendFile file opened";
        }
        else
        {
            qDebug() << "onSendFile file not opened";
            return -1;
        }
        onSendPacketData();
    }
    else
    {
        qDebug() << "onSendFile: file has name and could not send again-->" << m_sendingFile.fileName();
    }
    return 1;
}

void P2pTransferInterface::onSendPacketData()
{
    qDebug() << "onSendPacketData";
    if (!m_udpSocketWrite)
        return ;

    m_sendTimer.stop();
    m_sendMutex.lock();

    if(m_sendingFile.isOpen())
    {
        if(m_currentPackageData.fileName.isEmpty())// && m_sendingFile.isOpen())
        {
            QByteArray buffer = m_sendingFile.read(SEND_SIZE);

            //send package data every times
            PackageData package;
            package.uuid = QUuid::createUuid().toString();
            package.data = buffer;
            package.fileName = m_sendingFile.fileName();
            package.size = m_sendingFile.pos();
            package.maxSize = m_sendingFile.size();
            package.type = 0;//m_sentDataType;
            package.sourceIp = localAddress.toString();
            package.sourcePort = m_port;
            package.isEnd = m_sendingFile.atEnd();

            m_currentPackageData = package;
        }
    }

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << m_currentPackageData;

        unsigned int length = data.size();
        char lengthStr[sizeof(int)] = {0};
        strncpy(lengthStr, (char *)(&length), sizeof(int));
        data.insert(0, lengthStr, sizeof(int));

        qint64 size = m_udpSocketWrite->write(data);
        if(size > 0)
        {
            qDebug() << "size: " << size << m_currentPackageData.size << m_currentPackageData.maxSize << m_currentPackageData.fileName;
        }
        emit signalFileSendPercent(m_sendingFile.fileName(),m_currentPackageData.size,m_currentPackageData.maxSize);
        //read over just remove it
        if(m_sendingFile.atEnd())
        {
            m_sendingFile.close();
            m_sendingFile.setFileName("");
        }

    m_sendMutex.unlock();
    m_sendTimer.start(5000);
}


void P2pTransferInterface::onReadSocket()
{
    qDebug() << "onReadSocket";
    if (!m_udpSocketRead)
        return;

    unsigned int length = 0;
    PackageData pData;
    QHostAddress sender;
    quint16 senderPort;
    senderPort = m_port;

    if (m_udpSocketRead->pendingDatagramSize() < sizeof(int)) {
        return;
    }
    else
    {
        QByteArray datagram;
        datagram.resize(sizeof(int));
        datagram.resize(m_udpSocketRead->pendingDatagramSize());
        m_udpSocketRead->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
        length = *(int *)datagram.data();
        if (length != datagram.size()-4)
            return;

        QByteArray data;
        data = datagram.mid(4,-1);
        QDataStream in(&data, QIODevice::ReadOnly);
        in >> pData;
    }

    if (pData.type == 1 && pData.uuid == m_currentPackageData.uuid)
    {
        m_sendTimer.stop();
        m_sendMutex.unlock();
        m_currentPackageData = PackageData();
        if (!pData.isEnd)
            onSendPacketData();
        return;
    }

    if(pData.isCancel)
    {
        //emit cancelTransmit();
        if(m_receivingfile.isOpen())
        {
            qDebug() << "m_receivingfile file close: " << m_receivingfile.fileName();
            m_receivingfile.close();
        }
        return;
    }

    if(!m_receivingfile.isOpen())
    {
        QString fileName = "/home/user/receivedfiles/" + pData.fileName.section("/", -1);
        m_receivingfile.setFileName(fileName);
        if(m_receivingfile.open(QIODevice::WriteOnly))
        {
            qDebug() << "m_receivingfile file open success: " << m_receivingfile.fileName();
        }
        else
        {
            qDebug() << "m_receivingfile file open fail: " << m_receivingfile.fileName();
        }
    }

    if(m_receivingfile.isOpen())
    {
        if(m_receivingfile.pos() < pData.size)
        {
            m_receivingfile.write(pData.data);
            qDebug() << "m_receivingfile write data size: " << pData.data.size() <<pData.data;
        }
    }

    if(pData.size == pData.maxSize)
    {
        if(m_receivingfile.isOpen())
        {
            qDebug() << "m_receivingfile file close: " << m_receivingfile.fileName();
            m_receivingfile.close();
        }
    }

    emit signalFileReceivedPercent(m_receivingfile.fileName(),pData.size, pData.maxSize);

    m_mapReceivedDataList.insert(m_receivingfile.fileName(), pData);
    onReceivedDataRespond(pData);
}

void P2pTransferInterface::onReceivedDataRespond(PackageData pdata)
{
    //m_currentPackageData = PackageData();

    pdata.data.clear();
    pdata.type = 1;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << pdata;

    unsigned int length = data.size();
    char lengthStr[sizeof(int)] = {0};
    strncpy(lengthStr, (char *)(&length), sizeof(int));
    data.insert(0, lengthStr, sizeof(int));

    onConnectToServer(pdata.sourceIp,pdata.sourcePort);
    //connectToServer("192.168.36.91",77755);
    qint64 size = m_udpSocketWrite->write(data);
}

void P2pTransferInterface::onConnectToServer(QString ip)
{
    onConnectToServer(ip, m_port);
}
