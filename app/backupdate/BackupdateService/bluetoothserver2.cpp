#include "bluetoothserver2.h"
#include <QFile>

static const QLatin1String serviceUuid("e8e10f96-1a70-4b27-9ccf-02010264e9c8");

BluetoothServer2::BluetoothServer2(QObject *parent) :
    QObject(parent), m_rfcommServer(0)
{
    connect(&m_receivedTimer, SIGNAL(timeout()), this, SLOT(onReceivedDataRespond()));
//    connect(this, SIGNAL(fileReceivedPercent(QString,int,qint64,qint64)),
//            this, SLOT(onFileReceivedPercent(QString,int,qint64,qint64)));
}

BluetoothServer2::~BluetoothServer2()
{
}

void BluetoothServer2::startServer(const QBluetoothAddress& localAdapter)
{
    if (m_rfcommServer)
        return;

    m_rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_rfcommServer, SIGNAL(newConnection()), this, SLOT(onClientConnected()));
    bool result = m_rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "Cannot bind server to" << localAdapter.toString();
        return;
    }

    //serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    QBluetoothServiceInfo::Sequence classId;

    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             classId);

    classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("sinux_bluetooth_service"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("sinux bluetooth service"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("sinux.com.cn"));

    m_serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(m_rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    m_serviceInfo.registerService(localAdapter);
}

void BluetoothServer2::stopServer()
{
    // Unregister service
    m_serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(m_clientSockets);

    // Close server
    delete m_rfcommServer;
    m_rfcommServer = 0;
}

qint64 BluetoothServer2::onSendMessage(const BluetoothTransData &message)
{
    qint64 writed = 0;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;
    foreach (QBluetoothSocket *socket, m_clientSockets)
    {
        if(socket)
        {
            writed = socket->write(data);
        }
    }
    return writed;
}

void BluetoothServer2::onClientConnected()
{
    QBluetoothSocket *socket = m_rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    m_clientSockets.append(socket);

    BluetoothAddressData addr;
    addr.name = socket->peerName();
    addr.address = socket->peerAddress().toString();

    emit clientConnected(addr);
//    BluetoothTransData message;
//    message.type = 123;
//    message.transData = "I am bluetooth server";
//    onSendMessage(message);
    qDebug() << "onClientConnected-------------------------------";
}

void BluetoothServer2::onClientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());

    m_clientSockets.removeAll(socket);

    socket->deleteLater();
    qDebug() << "BluetoothServer2::onClientDisconnected()";
}

void BluetoothServer2::onReadSocket()
{
    qDebug() << "BluetoothServer2::onReadSocket()";
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    unsigned int length = 0;
    PackageData pData;
    if(socket->bytesAvailable() < sizeof(int))
    {
        return;
    }
    else
    {
        QByteArray data = socket->peek(sizeof(int));
        length = *(int *)data.data();
        qDebug() << "length: " << length;
        if(socket->bytesAvailable() < length)
        {
            return;
        }
    }

    socket->read(sizeof(int));
    QByteArray data = socket->read(length);
    QDataStream in(&data, QIODevice::ReadOnly);
    in >> pData;

    if(pData.isCancel)
    {
        emit cancelTransmit();
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
            qDebug() << "m_receivingfile write data size: " << pData.data.size();
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

    emit fileReceivedPercent(m_receivingfile.fileName(), pData.type, pData.size, pData.maxSize);

    m_mapReceivedDataList.insert(socket, pData);
    onReceivedDataRespond();
}

void BluetoothServer2::onReceivedDataRespond()
{
    m_receivedTimer.stop();
    foreach(QBluetoothSocket *socket, m_clientSockets)
    {
        PackageData pData = m_mapReceivedDataList.value(socket);
        pData.data.clear();

        if(m_lastUuid != pData.uuid)
        {
            m_lastUuid = pData.uuid;
            m_sendLastUuidCount = 0;
        }
        else
        {
            m_sendLastUuidCount ++;
        }

        if(m_sendLastUuidCount > 9)
        {
            m_receivedTimer.stop();
            return;
        }

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << pData;

        unsigned int length = data.size();
        char lengthStr[sizeof(int)] = {0};
        strncpy(lengthStr, (char *)(&length), sizeof(int));
        data.insert(0, lengthStr, sizeof(int));

        qint64 sendSize = socket->write(data);
        qDebug() << "onReceivedDataRespond: " << sendSize << data.size();
    }
    m_receivedTimer.start(1000);
}

//void BluetoothServer2::onFileReceivedPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize)
//{

//}
