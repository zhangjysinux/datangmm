#include "bluetoothserver.h"
#include <QFile>

static const QLatin1String serviceUuid("e8e10f96-1a70-4b27-9ccf-02010264e9c8");

#define SEND_SIZE 320

BluetoothServer::BluetoothServer(QObject *parent) :
    QObject(parent), m_rfcommServer(0)
{
    connect(&m_receivedTimer, SIGNAL(timeout()), this, SLOT(onReceivedDataRespond()));
}

BluetoothServer::~BluetoothServer()
{
}

void BluetoothServer::startServer(const QBluetoothAddress& localAdapter)
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

void BluetoothServer::stopServer()
{
    // Unregister service
    m_serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(m_clientSockets);

    // Close server
    delete m_rfcommServer;
    m_rfcommServer = 0;
}

qint64 BluetoothServer::onSendMessage(const BluetoothTransData &message)
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

void BluetoothServer::onClientConnected()
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

void BluetoothServer::onClientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());

    m_clientSockets.removeAll(socket);

    socket->deleteLater();
    qDebug() << "BluetoothServer::onClientDisconnected()";
}

void BluetoothServer::onReadSocket()
{
    qDebug() << "BluetoothServer::onReadSocket()";
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
        return;
    }

    if(!m_mapReceivedDataList.contains(socket))
        m_mapReceivedDataList.insert(socket, IntPackageDataMap());

    IntPackageDataMap &receivedDataList = m_mapReceivedDataList[socket];
    if(pData.isStart)
    {
        receivedDataList.clear();
    }

    if(!receivedDataList.contains(pData.index))
    {
        receivedDataList.insert(pData.index, pData);
        qDebug() << "receivedDataList data:" << pData.index << pData.isStart << pData.isEnd;

        onReceivedDataRespond();

        if(pData.isEnd)
        {
            BluetoothTransData message;
            QByteArray data;

            for(int i=0; i<pData.index+1; i++)
            {
                if(!receivedDataList.contains(i))
                    qDebug() << "receivedDataList --------------- last" << i;

                data.push_back(receivedDataList.value(i).data);
            }

            m_mapReceivedDataList.remove(socket);

            QDataStream in(&data, QIODevice::ReadOnly);
            in >> message;

            qDebug() << "partOver: " << message.partOver << message.fileName;

            if(message.fileName.isEmpty())
            {
                emit messageReceived(socket->peerName(),message);
            }
            else
            {
                if(message.partStart)
                {
                    QString fileName = message.fileName;
                    fileName = "/home/user/receivedfiles/" + fileName.section("/", -1);

                    QString tmpFileName = fileName;
                    for(int i=2; ;i++)
                    {
                        if(QFile::exists(tmpFileName))
                        {
                            tmpFileName = fileName;
                            if(tmpFileName.contains("."))
                            {
                                tmpFileName.replace(".", QString::number(i)+".");
                            }
                        }
                        else
                        {
                            fileName = tmpFileName;
                            break;
                        }
                    }

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
                    m_receivingfile.write(message.transData);
                    qDebug() << "m_receivingfile write data size: " << pData.data.size();
                }

                if(message.partOver)
                {
                    if(m_receivingfile.isOpen())
                    {
                        qDebug() << "m_receivingfile file close: " << m_receivingfile.fileName();
                        m_receivingfile.close();
                    }
                    BluetoothTransData message;
                    message.fileName = m_receivingfile.fileName();
                    emit messageReceived(socket->peerName(), message);
                }
            }
        }
    }
    else
    {
        qDebug() << "m_receivingfile contains current index " << pData.index;
    }
}

void BluetoothServer::onReceivedDataRespond()
{
    m_receivedTimer.stop();
    QList<QBluetoothSocket *> socketList = m_mapReceivedDataList.keys();
    foreach(QBluetoothSocket *socket, socketList)
    {
        IntPackageDataMap &receivedDataList = m_mapReceivedDataList[socket];
        PackageData pData = receivedDataList.value(receivedDataList.size()-1);
        pData.data.clear();

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
