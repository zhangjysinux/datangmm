#include "bluetoothclient.h"
#include <QBuffer>

#define SEND_SIZE 512

BluetoothClient::BluetoothClient(QObject *parent)
:   QObject(parent), m_socket(0)
{
    connect(&m_sendTimer, SIGNAL(timeout()), this, SLOT(onSendPacketData()));
}

BluetoothClient::~BluetoothClient()
{
    stopClient();
}

void BluetoothClient::startClient(const QBluetoothServiceInfo &remoteService)
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

int BluetoothClient::startClient(const QBluetoothAddress & address,
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

void BluetoothClient::stopClient()
{
    if(m_socket != NULL)
    {
        m_socket->disconnectFromService();
        m_socket->close();
        m_socket->deleteLater();
        m_socket = NULL;
    }
}

qint64 BluetoothClient::onSendMessage(const BluetoothTransData &message)
{
    if (!m_socket)
        return 0;

    m_cancel = false;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;

    unsigned int maxSize = data.size();
    unsigned int sumSize = 0;
    unsigned int size = SEND_SIZE;
    unsigned int currentSize = 0;
    unsigned int currentSize2 = 0;
    m_sendDataList.clear();
    for(int i=0; ;i++)
    {
        qDebug() << "send packet" << i << currentSize2 << maxSize;
        currentSize2 = currentSize + size;
        if(currentSize2 > maxSize)
        {
            currentSize2 = maxSize;
        }

        QByteArray singleData = data.mid(currentSize, currentSize2-currentSize);

        PackageData pData;
        pData.id = i;
        pData.size = currentSize2 - currentSize;
        pData.maxSize = maxSize;
        pData.index = i;
        pData.isStart = (i==0);
        pData.isEnd = (currentSize2 == maxSize);
        pData.data = singleData;

        m_sendMutex.lock();
        m_sendDataList.push_back(pData);
        m_sendMutex.unlock();

        currentSize = currentSize + size;

        if(pData.isEnd)
            break;
    }

    qDebug() << "m_sendDataList count: " << m_sendDataList.size() << m_socket;

    onSendPacketData();
}

qint64 BluetoothClient::onSendFile(const QString &fileName)
{
    if (!m_socket)
        return 0;

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
        getSendFileDataNext();
//        onSendPacketData();
    }
    else
    {
        qDebug() << "onSendFile: file has name and could not send again-->" << m_sendingFile.fileName();
    }
}

bool BluetoothClient::onCancelTransmit()
{
    m_cancel = true;
}

bool BluetoothClient::getSendFileDataNext()
{
    bool isStart = false;
    bool isEnd = false;
    QByteArray data;
    if(m_sendingFile.isOpen())
    {
        qDebug() << "m_sendingFile.pos()" << m_sendingFile.pos() << m_sendingFile.size();
        if(m_sendingFile.pos() == 0)
        {
            isStart = true;
            m_sendingStart = true;
        }
        else
        {
            m_sendingStart = false;
            isStart = false;
        }

        data = m_sendingFile.read(1024*10);


        if(m_sendingFile.atEnd())
        {
            isEnd = true;
            m_sendingOver = true;
        }
        else
        {
            m_sendingOver = false;
            isEnd = false;
        }

        if(data.isEmpty())
        {
            qDebug() << "getSendFileDataNext file read over, close sending file" << m_sendingFile.fileName();
            m_sendingFile.setFileName("");
            m_sendingFile.close();
            return false;
        }
    }
    else
    {
        qDebug() << "sending file not opened" << m_sendingFile.fileName() ;
        return false;
    }

    qDebug() << "start:" << m_sendingStart << "end: " << m_sendingOver << data.isEmpty();

    BluetoothTransData message;
    message.transData = data;
    message.partOver = m_sendingOver;
    message.partStart = m_sendingStart;
    message.fileName = m_sendingFile.fileName();

    onSendMessage(message);


//    unsigned int maxSize = data.size();
//    unsigned int sumSize = 0;
//    unsigned int size = SEND_SIZE-26;
//    unsigned int currentSize = 0;
//    unsigned int currentSize2 = 0;
//    m_sendDataList.clear();
//    for(int i=0; ;i++)
//    {
//        qDebug() << "send packet" << i << currentSize2 << maxSize;
//        currentSize2 = currentSize + size;
//        if(currentSize2 > maxSize)
//        {
//            currentSize2 = maxSize;
//        }

//        QByteArray singleData = data.mid(currentSize, currentSize2-currentSize);

//        PackageData pData;
//        pData.id = i;
//        pData.size = currentSize2 - currentSize;
//        pData.maxSize = maxSize;
//        pData.index = i;
//        pData.isStart = (isStart? (i==0) : false);
//        pData.isEnd = (isEnd? (currentSize2 == maxSize) : false);
//        pData.data = singleData;
//        pData.fileName = m_sendingFile.fileName();

//        m_sendMutex.lock();
//        m_sendDataList.push_back(pData);
//        m_sendMutex.unlock();

//        if(currentSize2 == maxSize)
//            break;

//        currentSize = currentSize + size;
//    }

//    qDebug() << "m_sendDataList count: " << m_sendDataList.size() << m_socket;

    return true;
}

void BluetoothClient::onSendPacketData()
{
    if (!m_socket)
        return ;

    m_sendTimer.stop();

    m_sendMutex.lock();

    if(!m_sendDataList.isEmpty())
    {
        PackageData pData = m_sendDataList.front();

        if(m_cancel)
        {
            pData.isCancel = true;

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out << pData;

            qint64 sendDataSize = m_socket->write(data);

            qDebug() << "send cancel";

            return;
        }

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << pData;

        unsigned int length = data.size();
        char lengthStr[sizeof(int)] = {0};
        strncpy(lengthStr, (char *)(&length), sizeof(int));
        data.insert(0, lengthStr, sizeof(int));

        qint64 sendDataSize = m_socket->write(data);
        if(sendDataSize > 0)
        {
            //sended
            qDebug() << "sendDataSize" << sendDataSize << pData.id << pData.index << pData.isStart << pData.isEnd;
        }
    }
//    else
//    {
//        getSendFileDataNext();
//        m_sendMutex.unlock();
//        m_sendTimer.start(200);
//        return;
//    }

    m_sendMutex.unlock();

    m_sendTimer.start(5000);
}

void BluetoothClient::onReadSocket()
{
    if (!m_socket)
        return;

//    BluetoothTransData message;
//    QByteArray data = m_socket->readAll();
//    QDataStream in(&data, QIODevice::ReadOnly);
//    in >> message;
    unsigned int length = 0;
    PackageData pData;
    if(m_socket->bytesAvailable() < sizeof(int))
        return;
    else
    {
        QByteArray data = m_socket->peek(sizeof(int));
//        QDataStream in(&data, QIODevice::ReadOnly);
//        in >> pData;
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

    if(!pData.isCancel)
    {
        if(!m_sendDataList.isEmpty())
        {
            PackageData &spData = m_sendDataList.front();
            if(spData.id == pData.id)
            {
                m_sendDataList.pop_front();
            }
            if(spData.isEnd)
            {
                if(m_sendingOver)
                {
                    emit messageSended(0);
                }
                else
                {
                    QTimer::singleShot(300, this, SLOT(getSendFileDataNext()));
                }
            }
        }
    }
    else
    {
        qDebug() << "cancel: ";
        m_sendDataList.clear();
        if(m_sendingFile.isOpen())
        {
            m_sendingFile.setFileName("");
            m_sendingFile.close();
        }
        emit cancelTransmit();
    }

    m_sendMutex.unlock();


    onSendPacketData();

//    emit messageReceived(m_socket->peerName(), message);
}

void BluetoothClient::onConnected()
{
    BluetoothAddressData addr;
    addr.name = m_socket->peerName();
    addr.address = m_socket->peerAddress().toString();
    emit connectFinished(addr);
}

bool BluetoothClient::isConnected()
{
    if(!m_socket)
        return false;
    qDebug() << "isConnected" << m_socket->state() << QBluetoothSocket::ConnectedState;
    return QBluetoothSocket::ConnectedState == m_socket->state();
}

bool BluetoothClient::isFinished()
{
    if(!m_socket)
        return true;
    else
        return false;
}

void BluetoothClient::onError(QBluetoothSocket::SocketError error)
{
    emit connectError(error);
}
