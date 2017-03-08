#include "bluetoothclient2.h"
#include <QBuffer>

#define SEND_SIZE 512

BluetoothClient2::BluetoothClient2(QObject *parent)
:   QObject(parent), m_socket(0)
{
    connect(&m_sendTimer, SIGNAL(timeout()), this, SLOT(onSendPacketData()));
}

BluetoothClient2::~BluetoothClient2()
{
    stopClient();
}

void BluetoothClient2::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (m_socket)
        return;

    // Connect to service
    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    m_socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadSocket()));
    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(m_socket, SIGNAL(error(QBluetoothSocket::SocketError)),
            this, SLOT(onError(QBluetoothSocket::SocketError)));
}

int BluetoothClient2::startClient(const QBluetoothAddress & address,
                                  const QBluetoothUuid & uuid)
{
    if (m_socket)
        return 0;

    // Connect to service
    qDebug() << "connect address" << address.toString();
    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadSocket()));
    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    m_socket->connectToService(address, uuid);
    qDebug() << "ConnectToService done";
    if(m_socket->error() == QBluetoothSocket::NoSocketError)
    {
        qDebug() << "Connect true";
        return 0;
    }
    else
    {
        int error = m_socket->error();
        delete m_socket;
        m_socket = NULL;
        qDebug() << "Connect false";
        return error;
    }
}

void BluetoothClient2::stopClient()
{
    if(m_socket != NULL)
    {
        m_socket->disconnectFromService();
        m_socket->close();
        m_socket->deleteLater();
        m_socket = NULL;
    }
}

qint64 BluetoothClient2::onSendMessage(const BluetoothTransData &message)
{
    if (!m_socket)
        return -1;

    if(m_sendingOver)
    {
        m_sendingOver = false;
        m_cancel = false;

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << message;

        QString fileName = "./bluetoothmessage_" + QUuid::createUuid().toString().remove("{").remove("}").remove("-") + ".dat";
        QFile file(fileName);\
        if(file.open(QIODevice::WriteOnly))
        {
            qDebug() << "BluetoothClient2::onSendMessage open file success... ";
            file.write(data);
            file.flush();
            file.close();
        }
        else
        {
            qDebug() << "BluetoothClient2::onSendMessage open fail..." << file.errorString();
        }

        m_sentDataType = 0;
        return sendFile(fileName);
    }

    return -1;
}

qint64 BluetoothClient2::onSendFile(const QString &fileName)
{
    if(m_sendingOver)
    {
        m_sendingOver = false;
        m_sentDataType = -1;
        return sendFile(fileName);
    }
    return -1;
}

qint64 BluetoothClient2::sendFile(const QString &fileName)
{
    if (!m_socket)
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

bool BluetoothClient2::onCancelTransmit()
{
    m_cancel = true;
}

void BluetoothClient2::onSendPacketData()
{
    if (!m_socket)
        return ;

    m_sendTimer.stop();

    m_sendMutex.lock();

    if(m_sendingFile.isOpen())
    {
        if(m_currentPackageData.fileName.isEmpty())
        {
            QByteArray buffer = m_sendingFile.read(SEND_SIZE);

            //send package data every times
            PackageData package;
            package.uuid = QUuid::createUuid().toString();
            package.data = buffer;
            package.fileName = m_sendingFile.fileName();
            package.size = m_sendingFile.pos();
            package.maxSize = m_sendingFile.size();
//            package.isStart = (m_sendingFile.pos() == (SEND_SIZE-1));
//            package.isEnd = (m_sendingFile.pos() == (m_sendingFile.size()-1));
            package.type = m_sentDataType;

            m_currentPackageData = package;
        }

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << m_currentPackageData;


        unsigned int length = data.size();
        char lengthStr[sizeof(int)] = {0};
        strncpy(lengthStr, (char *)(&length), sizeof(int));
        data.insert(0, lengthStr, sizeof(int));

        qint64 size = m_socket->write(data);
        if(size > 0)
        {
            qDebug() << "size: " << size << m_currentPackageData.size << m_currentPackageData.maxSize << m_currentPackageData.fileName;
        }

        //read over just remove it
        if(m_sendingFile.atEnd())
        {
            m_sendingFile.close();
            m_sendingFile.setFileName("");
        }
    }

    m_sendMutex.unlock();

    m_sendTimer.start(5000);
}

void BluetoothClient2::onReadSocket()
{
    if (!m_socket)
        return;

    unsigned int length = 0;
    PackageData pData;
    if(m_socket->bytesAvailable() < sizeof(int))
        return;
    else
    {
        QByteArray data = m_socket->peek(sizeof(int));
        length = *(int *)data.data();
        qDebug() << "length: " << length;
        if(m_socket->bytesAvailable() < length)
        {
            return;
        }
    }

    m_sendMutex.lock();

    m_socket->read(sizeof(int));
    QByteArray data = m_socket->read(length);
    QDataStream in(&data, QIODevice::ReadOnly);
    in >> pData;

    qDebug() << "receiveData: " << pData.id << pData.index;

    emit fileSendPercent(m_currentPackageData.fileName, m_currentPackageData.type, m_currentPackageData.size, m_currentPackageData.maxSize);

    if(!pData.isCancel)
    {
        if(m_currentPackageData.size == pData.size)
        {
            m_currentPackageData = PackageData();
        }
    }
    else
    {
        qDebug() << "cancel: ";
        m_sendingFile.close();
        m_sendingFile.setFileName("");
        emit cancelTransmit();
    }

    if(pData.size == pData.maxSize)
    {
        m_sendingOver = true;
    }

    m_sendMutex.unlock();

    onSendPacketData();
}

void BluetoothClient2::onConnected()
{
    BluetoothAddressData addr;
    addr.name = m_socket->peerName();
    addr.address = m_socket->peerAddress().toString();
    emit connectFinished(addr);
}

bool BluetoothClient2::isConnected()
{
    if(!m_socket)
        return false;
    qDebug() << "isConnected" << m_socket->state() << QBluetoothSocket::ConnectedState;
    return QBluetoothSocket::ConnectedState == m_socket->state();
}

bool BluetoothClient2::isFinished()
{
    if(!m_socket)
        return true;
    else
        return false;
}

void BluetoothClient2::onError(QBluetoothSocket::SocketError error)
{
    emit connectError(error);
}
