#include "backgroundsyncinterface.h"
#include <QDBusReply>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QProcess>
#include <QTimer>
#include <QBuffer>
#include <QSqlQuery>
#include <QSqlError>
#include <QTemporaryFile>
#include "connectiondatabase.h"

BackgroundSyncInterface *BackgroundSyncInterface::instance = NULL;

BackgroundSyncInterface *BackgroundSyncInterface::getInstance()
{
    if(instance == NULL)
        instance = new BackgroundSyncInterface();

    return instance;
}

BackgroundSyncInterface::BackgroundSyncInterface(QObject *parent) :
    QObject(parent)
{
    m_transferInterface = BluetoothBackgroundTransferInterface::getInstance();
    connect(m_transferInterface, SIGNAL(signalDeviceDiscovered(BluetoothAddressData)),
            this, SLOT(onSignalDeviceDiscovered(BluetoothAddressData)));
    connect(m_transferInterface, SIGNAL(signalDeviceConnected(BluetoothAddressData)),
            this, SLOT(onSignalDeviceConnected(BluetoothAddressData)));
    connect(m_transferInterface, SIGNAL(signalMessageReceived(BluetoothTransData)),
            this, SLOT(onSignalMessageReceived(BluetoothTransData)));
//    initQueue();
//    onSendDb("contact.db");
//        Contacter contacter;
//        contacter.id = "110";
//        contacter.name = "orange";
//        contacter.surname = "li";
//        onSendContacter(-1, contacter);
}

bool BackgroundSyncInterface::onStart()
{
    int serviceType = m_transferInterface->onGetServiceType();
    if(serviceType == BLUETOOTHSERVICETYPE_SERVER)
    {

    }
    else if(serviceType == BLUETOOTHSERVICETYPE_CLIENT)
    {
        QTimer::singleShot(5000, m_transferInterface, SLOT(onStartFindRemoteBluetooth()));
//        m_transferInterface->onStartFindRemoteBluetooth();
    }
    else
    {
        qDebug() << "local did not install server of client";
    }
    return true;
}

qint64 BackgroundSyncInterface::onSendContacter(const int &handleType, const Contacter &contacter)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_CONTACTSHARE;
    data.handleType = handleType;

    QByteArray ba;
    ba.resize(sizeof(Contacter));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << contacter;

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendCallHistory(const int &handleType, const CallHistory &history)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_HISTORYSHARE;
    data.handleType = handleType;

    QByteArray ba;
    ba.resize(sizeof(CallHistory));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << history;

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendGroup(const int &handleType, const Group &group)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_GROUPSHARE;
    data.handleType = handleType;

    QByteArray ba;
    ba.resize(sizeof(Group));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << group;

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendGroupMessage(const int &handleType, const Message &groupMessage)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_GROUPMESSAGESHARE;
    data.handleType = handleType;

    QByteArray ba;
    ba.resize(sizeof(Message));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << groupMessage;

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendDb(const QString &fileName)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_DBSHARE;

    QByteArray ba;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        ba = file.readAll();
        file.close();
    }


    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendContacters(const int &handleType, const ContacterList &contacters)
{
    qDebug() << "onSendContacters count" << contacters.count();
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_CONTACTSHARE;
    data.handleType = handleType;
    data.count = contacters.count();

    QByteArray ba;
    ba.resize(sizeof(Contacter)*data.count);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    for(int i=0; i<data.count; i++)
        stream << contacters.at(i);

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendCallHistorys(const int &handleType, const CallHistoryList &historys)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_HISTORYSHARE;
    data.handleType = handleType;
    data.count = historys.count();

    QByteArray ba;
    ba.resize(sizeof(CallHistory)*data.count);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    for(int i=0; i<data.count; i++)
        stream << historys.at(i);

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendGroups(const int &handleType, const GroupList &groups)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_GROUPSHARE;
    data.handleType = handleType;
    data.count = groups.count();

    QByteArray ba;
    ba.resize(sizeof(Group)*data.count);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    for(int i=0; i<data.count; i++)
        stream << groups.at(i);

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

qint64 BackgroundSyncInterface::onSendGroupMessages(const int &handleType, const MessageList &groupMessages)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_GROUPMESSAGESHARE;
    data.handleType = handleType;
    data.count = groupMessages.count();

    QByteArray ba;
    ba.resize(sizeof(Message)*data.count);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    for(int i=0; i<data.count; i++)
        stream << groupMessages.at(i);

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "contacts");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);

    document.setObject(object);

    return m_transferInterface->onSendMessage(document.toBinaryData());
}

void BackgroundSyncInterface::onSignalDeviceDiscovered(const BluetoothAddressData &address)
{
    //9C:99:A0:1D:21:14 //daju 74:51 //00:02:2C
//    if(address.address.contains("00:02:2C") && m_remoteServerAddr.address.isEmpty())
//    {
//        m_remoteServerAddr = address;
//        qDebug() << "address.address" << address.address;
//        if(!m_remoteServerAddr.address.isEmpty())
//        {
//            m_transferInterface->onConnect(m_remoteServerAddr);
////            QThread::sleep(3);
////            Contacter contacter;
////            contacter.id = "110";
////            contacter.name = "orange";
////            contacter.surname = "li";
////            onSendContacter(contacter);
//        }
//    }
}

void BackgroundSyncInterface::onSignalDeviceConnected(const BluetoothAddressData &address)
{
    qDebug() << "onSignalDeviceConnected";
    onSendDb("contact.db");
//    Contacter contacter;
//    contacter.id = "110";
//    contacter.name = "orange";
//    contacter.surname = "li";
//    onSendContacter(-1, contacter);
//    m_transferInterface->onSendContacter(-1, contacter);
}

void BackgroundSyncInterface::onSignalMessageReceived(const BluetoothTransData &message)
{
    QByteArray transData = message.transData;
    QDataStream stream(&transData, QIODevice::ReadOnly);
    if(message.type == -1)
        qDebug() << "sync message received from server";
    switch (message.type) {
    case BLUETOOTHTRANSDATATYPE_CONTACTSHARE:
    {
        ContacterList contacters;
        for(int i=0; i<message.count; i++)
        {
            Contacter contacter;
            stream >> contacter;
            contacters.append(contacter);
            qDebug() << "receive contacter: " << contacter.id << contacter.surname << contacter.name;
        }

        emit signalContacterReceived(message.handleType, contacters);
        break;
    }
    case BLUETOOTHTRANSDATATYPE_HISTORYSHARE:
    {
        CallHistoryList callHistorys;
        for(int i=0; i<message.count; i++)
        {
            CallHistory callHistory;
            stream >> callHistory;
            callHistorys.append(callHistory);
        }

        emit signalCallHistoryReceived(message.handleType, callHistorys);
        break;
    }
    case BLUETOOTHTRANSDATATYPE_GROUPSHARE:
    {
        GroupList groups;
        for(int i=0; i<message.count; i++)
        {
            Group group;
            stream >> group;
            groups.append(group);
        }

        emit signalGroupReceived(message.handleType, groups);
        break;
    }
    case BLUETOOTHTRANSDATATYPE_GROUPMESSAGESHARE:
    {
        MessageList groupMsgs;
        for(int i=0; i<message.count; i++)
        {
            Message groupMsg;
            stream >> groupMsg;
            groupMsgs.append(groupMsg);
            qDebug() << "group message:" << groupMsg.common.groupId;
        }

        emit signalGroupMessageReceived(message.handleType, groupMsgs);
        break;
    }
    case BLUETOOTHTRANSDATATYPE_DBSHARE:
    {
        QByteArray ba = message.transData;

        ConnectionDatabase::closeDb();

        if(QFile::exists("contact.db"))
        {
            if(!QFile::remove("contact.db"))
            {
                qDebug() << "BLUETOOTHTRANSDATATYPE_DBSHARE" << "contact.db removed fail.................";
            }
        }

        QFile file("contact.db");
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ba);
            file.flush();
            file.close();
        }

        ConnectionDatabase::openDb();
        emit signalDbChanged();
    }
    default:
        break;
    }
}

qint64 BackgroundSyncInterface::sendMessage(const QByteArray &data)
{
    qint64 sendBytes = -1;
    if(!data.isEmpty())
    {
        bool pushFlag = pushSendData(data);
        qDebug() << "pushSendData" << pushFlag;
    }

    while( !isEmptySendData() )
    {
        QByteArray d = frontSendData();
        sendBytes = m_transferInterface->onSendMessage(d);
        if(sendBytes > 0)
        {
            popSendData();
        }
        else
        {
            break;
        }
    }

    return sendBytes;
}

bool BackgroundSyncInterface::initQueue()
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    bool flag = query.exec("CREATE TABLE IF NOT EXISTS waitingSendQueue (id int primary key, waitingSendData varchar(200))");
//    flag &= query.exec("CREATE TABLE IF NOT EXISTS waitingReceiveQueue (id int primary key, waitingReceiveData varchar(200))");
    qDebug() << query.lastError().text() << query.lastQuery();
    return flag;
}

bool BackgroundSyncInterface::pushSendData(const QByteArray &data)
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    QString key = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    QString val = QString::fromLatin1(data.data(), data.size());
    QString sql = QString("INSERT INTO waitingSendQueue VALUES('%1','%2')")
                        .arg(key).arg(val);
    bool flag = query.exec(sql);
    qDebug() << query.lastError().text() << query.lastQuery();
    return flag;
}

bool BackgroundSyncInterface::popSendData()
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    QString sql = QString("DELETE FROM waitingSendQueue WHERE id=min(id)");
    bool flag = query.exec(sql);
    qDebug() << query.lastError().text() << query.lastQuery();
    return flag;
}

QByteArray BackgroundSyncInterface::frontSendData()
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    QString sql = QString("SELECT waitingSendData FROM waitingSendQueue WHERE id=min(id)");
    bool flag = query.exec(sql);
    QByteArray bt;
    if(query.next())
    {
        QByteArray bt = query.value(0).toString().toLatin1();
    }
    qDebug() << query.lastError().text() << query.lastQuery();
    return bt;
}

bool BackgroundSyncInterface::isEmptySendData()
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    QString sql = QString("SELECT count(id) FROM waitingSendQueue");
    if(query.exec(sql))
    {
        if(query.next())
        {
            if(query.value(0).toInt() > 0)
                return true;
        }
    }
    return false;
}

//bool BackgroundSyncInterface::pushReceiveData(const QByteArray &data)
//{
//    QSqlQuery query(ConnectionDatabase::getGblDb());
//    QString key = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
//    QString val = QString::fromLatin1(data.data(), data.size());
//    QString sql = QString("INSERT INTO waitingReceiveQueue VALUES('%1','%2')")
//                        .arg(key).arg(val);
//    bool flag = query.exec(sql);
//    qDebug() << query.lastError().text() << query.lastQuery();
//    return flag;
//}

//bool BackgroundSyncInterface::popReceiveData()
//{
//    QSqlQuery query(ConnectionDatabase::getGblDb());
//    QString sql = QString("DELETE FROM waitingReceiveQueue WHERE id=min(id)");
//    bool flag = query.exec(sql);
//    qDebug() << query.lastError().text() << query.lastQuery();
//    return flag;
//}

//QByteArray BackgroundSyncInterface::frontReceiveData()
//{
//    QSqlQuery query(ConnectionDatabase::getGblDb());
//    QString sql = QString("SELECT waitingReceiveData FROM waitingReceiveQueue WHERE id=min(id)");
//    bool flag = query.exec(sql);
//    QByteArray bt;
//    if(query.next())
//    {
//        QByteArray bt = query.value(0).toString().toLatin1();
//    }
//    qDebug() << query.lastError().text() << query.lastQuery();
//    return bt;
//}

//bool BackgroundSyncInterface::isEmptyReceiveData()
//{
//    QSqlQuery query(ConnectionDatabase::getGblDb());
//    QString sql = QString("SELECT count(id) FROM waitingReceiveQueue");
//    if(query.exec(sql))
//    {
//        if(query.next())
//        {
//            if(query.value(0).toInt() > 0)
//                return true;
//        }
//    }
//    return false;
//}
