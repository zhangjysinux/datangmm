#include "groupmessagecontroller2.h"
#include "sender.h"
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include "../../commondata/datatype/commondata.h"
#include "messagemanager.h"
#include <QNetworkInterface>

#define SEND_SIZE 1024

QString getLocalIp()
{
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
        qDebug() << "Address:" << address << address.protocol();
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                !address.toString().contains("100.100") &&
                !address.isLoopback())
        {
            return address.toString();
        }
    }
    return QString();
}



GroupMessageController2::GroupMessageController2(const QString &mcAddr,
                                                 int mcPort,
                                                 int udpPort,
                                                 const QString &groupId,
                                                 QObject *parent) :
    GroupMessageSenderAdaptor(parent)
{
    m_mutiCastAdaptor = MutiCastAdaptor::getInstance();

    connect(m_mutiCastAdaptor, SIGNAL(signalRecvComplete(int,int,int,int,int,QString,int)),
            this, SLOT(onSignalRecvComplete(int,int,int,int,int,QString,int)));
    connect(m_mutiCastAdaptor, SIGNAL(signalSendComplete(int,int,int,int,int,QString,int)),
            this, SLOT(onSignalSendComplete(int,int,int,int,int,QString,int)));

    m_groupIdStr = groupId;
    m_groupId = m_mutiCastAdaptor->onCreateMuticast(WIFI_NETWORK, mcAddr, mcPort, udpPort, groupId);
}

GroupMessageController2::~GroupMessageController2()
{
    m_mutiCastAdaptor->stop(m_groupId);
}

void GroupMessageController2::setReceivePath(const QString &path)
{
    m_mutiCastAdaptor->setRecivePath(m_groupId, path);
}

void GroupMessageController2::start()
{
    m_mutiCastAdaptor->init(m_groupId, m_ips);
    m_mutiCastAdaptor->start(m_groupId);

    qDebug() << "GroupMessageController2::start";
}

void GroupMessageController2::setGroupIps(const QStringList &ips)
{
    m_ips = ips;
}

qint64 GroupMessageController2::sendMessage(const Message& message)
{
    QString path = "/home/user/groupmessages/";
    createPath(path);

    m_sendFileName = "message_"+QUuid::createUuid().toString().remove("{").remove("}").remove("-")+".dat";
    m_sendFilePath = path+m_sendFileName;

    m_fileNameGroupId.insert(m_sendFileName, message.common.groupId);
    m_fileNameMessageId.insert(m_sendFileName, message.common.id);

    QFile file(m_sendFilePath);
    if(file.open(QIODevice::WriteOnly))
    {
        qDebug() << "message.dat open success..." << file.fileName();
        QDataStream out(&file);
        out << message;
        file.flush();
        file.close();
    }
    else
    {
        qDebug() << "message.dat open error: " << file.errorString();
    }

//    if(file.open(QIODevice::ReadOnly))
//    {
//        qDebug() << "message.dat open success...";
//        QDataStream in(&file);
//        Message message;
//        in >> message;
//    }

    //sending handle
    static int fileId = -1;
    if(fileId == -1)
    {
        QString ip = getLocalIp();
        unsigned int size = ip.section(".", -1).toInt();
        unsigned int maxCount = 4294836225 / 2;
        fileId = (maxCount / 255) * size;
    }

    qDebug() << "send muticast file id" << fileId;
    m_mutiCastAdaptor->sendData(message.common.type, fileId++, m_groupId, m_sendFileName, m_sendFilePath);
}

void GroupMessageController2::onSignalRecvComplete(int msgtype, int fileid, int groupId, int recvNum, int allNum, QString recFileName, int bIsSuccesd)
{
    if(groupId == m_groupId)
    {
        qDebug() << "onSignalReceiveComplete: " << recvNum << allNum << recFileName;

        if(recvNum == allNum)
        {
            qDebug() << "onSignalReceiveComplete finished:==============" << recFileName;

            QFile file(recFileName);
            if(file.open(QIODevice::ReadOnly))
            {
                qDebug() << "message.dat open success...";
                QDataStream in(&file);
                Message message;
                in >> message;
                if(!m_fileNameGroupId.contains(recFileName))
                {
                    m_fileNameGroupId.insert(recFileName, message.common.groupId);
                    m_fileNameMessageId.insert(recFileName, message.common.id);
                }
                QString groupid = m_fileNameGroupId.value(recFileName);
                QString id = m_fileNameMessageId.value(recFileName);
                message.common.groupId = groupid;
                message.common.id = id;
                message.common.state = MESSAGE_DOWNLOAD_SUCCESS;
                qDebug() << "onSignalReceiveComplete" << message.common.id << message.content;
                file.close();

                emit signalNewMessage(message);

//                m_fileNameGroupId.remove(recFileName);
//                m_fileNameMessageId.remove(recFileName);
            }
            else
            {
                qDebug() << "message.dat open error: " << file.errorString();
            }
        }
        if(!m_fileNameGroupId.contains(recFileName))
        {
            Message message;
            message.common.groupId = m_groupIdStr;
            message.common.state = MESSAGE_RECEIVING;
            message.common.type = msgtype;

            m_fileNameGroupId.insert(recFileName, message.common.groupId);
            m_fileNameMessageId.insert(recFileName, message.common.id);

            if(message.common.type != TEXT_MESSAGE)
                emit signalNewMessage(message);
        }

        QString groupid = m_fileNameGroupId.value(recFileName);
        QString id = m_fileNameMessageId.value(recFileName);

        emit signalReceivePercent(groupid, id, recvNum, allNum);
    }
}

void GroupMessageController2::onSignalSendComplete(int msgtype, int fileid, int groupId, int sendNum, int allNum, QString sendFileName, int bIsSuccesd)
{
    if(groupId == m_groupId)
    {
        qDebug() << "onSignalSendComplete: " << sendNum << allNum;
        QString groupid = m_fileNameGroupId.value(m_sendFileName);
        QString id = m_fileNameMessageId.value(m_sendFileName);
        if(bIsSuccesd == 0)
        {
            Message message = MessageManager::getInstance()->getMessage(id, groupid);
            message.common.state = MESSAGE_SEND_SUCCESS;
            signalNewMessage(message);
        }
        else if(sendNum == allNum)
        {
            Message message = MessageManager::getInstance()->getMessage(id, groupid);
            message.common.state = MESSAGE_SEND_SUCCESS;
            signalNewMessage(message);
        }

        emit signalSendPercent(groupid, id, sendNum, allNum);
    }
}


