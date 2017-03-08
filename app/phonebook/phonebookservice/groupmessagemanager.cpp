#include <QNetworkInterface>
#include "groupmessagemanager.h"
#include "contacterinterface.h"
#include "groupinterface.h"
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include "connectiondatabase.h"
#include "adaptor/groupmessagesenderadaptorlte.h"
#include "adaptor/groupmessagesenderadaptorbt.h"
#include "adaptor/pointtopointadaptor.h"
#include "adaptor/p2ptransferadaptor.h"
#include "adaptor/adhocadaptor.h"
#include "adaptor/p2pmessagesendertcpadaptor.h"
#include "downloader.h"
#include "cnotification.h"
#include "cnotificationmanager.h"
#include "audiorecorder.h"


inline QString transMessageDateToString(const QDateTime &date)
{
    QString value = date.toString("yyyyMMddhhmmss");
    return value;
}

inline QDateTime transStringToMessageDate(const QString &date)
{
    QDateTime value = QDateTime::fromString(date, "yyyyMMddhhmmss");
    return value;
}

void GroupMessageManager::insertData(const QString& tableName)
{
    for(int i=0; i<3; i++)
    {
        Message msg;
        msg.common.id = QUuid::createUuid().toString();
        msg.common.userId = "45";
        msg.common.type = TEXT_MESSAGE;
        msg.common.state = 1;
        msg.common.groupId = "1001";
        msg.content = "我是字符数据" + QString::number(i+1).toLocal8Bit();
        recordMessage(msg);
    }
    m_sqlModel.submitAll();

    for(int i=0; i<3; i++)
    {
        Message msg;
        msg.common.id = QUuid::createUuid().toString();
        msg.common.userId = "45";
        msg.common.type = IMAGE_MESSAGE;
        msg.common.state = 1;
        msg.common.groupId = "1001";
        msg.content = "我是图片baby";
        msg.contentName = "messagefiles/image.jpg";
        msg.contentType = "jpg";
        recordMessage(msg);
    }

    for(int i=0; i<3; i++)
    {
        Message msg;
        msg.common.id = QUuid::createUuid().toString();
        msg.common.userId = "45";
        msg.common.type = AUDIO_MESSAGE;
        msg.common.state = 1;
        msg.common.groupId = "1001";
        msg.content = "我是 audio ";
        msg.contentName = "messagefiles/audio.mp3";
        msg.contentType = "mp3";
        recordMessage(msg);
    }

    for(int i=0; i<3; i++)
    {
        Message msg;
        msg.common.id = QUuid::createUuid().toString();
        msg.common.userId = "45";
        msg.common.type = VIDEO_MESSAGE;
        msg.common.state = 1;
        msg.common.groupId = "1001";
        msg.content = "我是 vedio ";
        msg.contentName = "messagefiles/video.mp4";
        msg.contentType = "mp4";
        recordMessage(msg);
    }
}

int GroupMessageManager::UpdPort = 10000;

GroupMessageManager::GroupMessageManager(const QString &groupId, QObject *parent)
    : QObject(parent),
      m_groupId(groupId),
      m_groupIdP2P(groupId),
      m_currentSyncCount(0),
      m_sqlModel(0, ConnectionDatabase::getGblDb()),
      m_dialManager(QT_DTT::DialManager::instance()),
      m_pttGroupId(0),
      m_battleCallAdaptor(BattleCallAdator::getInstance()),
      m_voipServiceAdaptor(VoipServiceAdaptor::getInstance("com.sinux.voipPTT"))
{
    m_ttsEnable = true;
//    CTS_PRINT << "init LingYun SDK";
//    m_lingyun = LingYun::getInstance();
//    CTS_PRINT << "LingYun SDK init Succ";

    //p2pgroup_contacterid_networktype_1
    if(m_groupIdP2P.contains("p2pgroup_"))
    {
        m_p2pNetworkType = m_groupIdP2P.section("networktype_", 1, 1).toInt();
        m_groupIdP2P = m_groupIdP2P.section("_networktype_", 0, 0);
    }

    m_tableName = QString("group%1").arg(m_groupIdP2P);
    QSqlQuery query(ConnectionDatabase::getGblDb());
    query.exec(QString("create table %1 (id varchar(50), "
               "type int, state int, userid varchar(20), groupid varchar(20), date int8, showmessage varchar(200),isread int, "
               "accessorypath varchar(50), content varchar(500), contentname varchar(50), contenttype varchar(20), "
               "networktype int, longitude double, latitude double, height double)").arg(m_tableName));
    qDebug() << "create table: " << query.lastError();
    m_sqlModel.setTable(m_tableName);
    m_sqlModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_sqlModel.select();

    //ptt signal slots
//    connect(m_dialManager, SIGNAL(signalPttStateChanged(uint,int)),
//            this, SLOT(onSignalPttStateChanged(uint,int)));
    connect(m_voipServiceAdaptor, SIGNAL(signalPttRecordFileFinished(uint,QString)),
            this, SLOT(onSignalPttRecordFileFinished(uint,QString)));

    m_groupDataPath = QString("/home/user/sinux/groupmessages_%1/").arg(m_tableName);
    createPath(m_groupDataPath);

    //to do data from contact
    Group groupData = GroupInterface::getInstance()->getGroup(m_groupId);
    m_addressAdhoc = groupData.address;
//    QStringList ipList;
    QStringList cIdList;
    cIdList  << groupData.leaders << groupData.members; //<< groupData.creator
    qDebug() << cIdList;
//    foreach(const QString &id, cIdList)
//    {
//        Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);


//        foreach(const PhoneNumber &number, contacter.numbers)
//        {
//            if(number.netType == AD_HOC_NETWORK)
//            {
//                qDebug() << "number WIFI_NETWORK" << id << contacter.id  << number.number;
//                ipList.push_back(number.number);
//            }
//        }
//    }

    if(m_groupIdP2P.contains("p2pgroup_"))
    {
        if(m_p2pNetworkType != BATTLE_NETWORK)
        {
//            PointToPointAdaptor *p2pAdaptor = PointToPointAdaptor::getInstance();

//            QString contacterId = m_groupIdP2P.section("_", -1);
//            if(!contacterId.isEmpty())
//            {
//                Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(contacterId);
//                PhoneNumber phoneNumber = getPhoneNumberType(contacter.numbers, m_p2pNetworkType);
//    //            p2pAdaptor->setNetworkType(m_p2pNetworkType);
//                p2pAdaptor->onSetAddresses(QStringList(phoneNumber.number));
//                p2pAdaptor->onConnect();
//                m_groupMessageSender = p2pAdaptor;
//            }
            P2PMessageSenderTcpAdaptor *p2pAdaptor = P2PMessageSenderTcpAdaptor::getInstance();

            connect(p2pAdaptor, SIGNAL(signalNewMessage(Message)),
                    this, SLOT(onNewMessage(Message)));
            connect(p2pAdaptor, SIGNAL(signalConnectFinished(QString,int)),
                    this, SLOT(onSignalP2PConnected(QString,int)));
//            connect(p2pAdaptor, SIGNAL(signalSendPercent(QString,QString,int,int)),
//                    this, SLOT(onSignalSendPercent(QString,QString,int,int)));
//            connect(p2pAdaptor, SIGNAL(signalReceivePercent(QString,QString,int,int)),
//                    this, SLOT(onSignalReceivePercent(QString,QString,int,int)));

            QString contacterId = m_groupIdP2P.section("_", -1);
            if(!contacterId.isEmpty())
            {
                Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(contacterId);
                PhoneNumber phoneNumber = getPhoneNumberType(contacter.numbers, m_p2pNetworkType);
                p2pAdaptor->onConnect(m_groupId, phoneNumber.number);
                m_groupMessageSender = p2pAdaptor;
            }
        }
        else
        {
            //battle network
            P2pTransferAdaptor *p2pTransferAdaptor = P2pTransferAdaptor::getInstance();

            QString contacterId = m_groupIdP2P.section("_", -1);
            if(!contacterId.isEmpty())
            {
                Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(contacterId);
                Contacter localContacter = ContacterInterface::getInstance()->getLocalInfo();
                PhoneNumber phoneNumber = getPhoneNumberType(contacter.numbers, m_p2pNetworkType);

                p2pTransferAdaptor->initSocket(getPhoneNumberType(localContacter.numbers, m_p2pNetworkType).number, 0);
                p2pTransferAdaptor->connectToServer(phoneNumber.number);
                connect(p2pTransferAdaptor, SIGNAL(signalReceivePercent(QString,qint64,qint64)),
                        this, SLOT(onBattleSignalReceivePercent(QString,qint64,qint64)));
                connect(p2pTransferAdaptor, SIGNAL(signalSendPercent(QString,qint64,qint64)),
                        this, SLOT(onBattleSignalSendPercent(QString,qint64,qint64)));

                m_groupMessageSender = p2pTransferAdaptor;
            }
        }
    }
    else if(groupData.networkType == AD_HOC_NETWORK)
    {
        QStringList adhocIpList = GroupInterface::getInstance()->getGroupAddrsByType(m_groupId, groupData.networkType);

        AdHocAdaptor *adhocAdaptor = AdHocAdaptor::getInstance();

        connect(adhocAdaptor, SIGNAL(signalNewMessage(Message)),
                this, SLOT(onNewMessage(Message)));
        connect(adhocAdaptor, SIGNAL(signalSendPercent(QString,QString,int,int)),
                this, SLOT(onSignalSendPercent(QString,QString,int,int)));
        connect(adhocAdaptor, SIGNAL(signalReceivePercent(QString,QString,int,int)),
                this, SLOT(onSignalReceivePercent(QString,QString,int,int)));
        connect(adhocAdaptor, SIGNAL(signalPttStateChanged(int,QString)),
                this, SLOT(onAdhocPttStateChanged(int,QString)));

        QString changedAddr = groupData.address;
//        QString addr = changedAddr.section(".", -1);
//        changedAddr.remove("."+addr);
//        changedAddr.append("."+QString::number(addr.toInt()+30));
        int changedPort = groupData.port + (12000-10101);
        qDebug() << "adhoc addr: " << changedAddr << changedPort;

        adhocAdaptor->setGroupIpsAndPath(m_groupId,
                                         changedAddr,
                                         changedPort,
                                         adhocIpList,
                                         m_groupDataPath);
        //for addgroup after reboot
        adhocAdaptor->adHocCreateMsg(groupData.address);

        m_groupMessageSender = adhocAdaptor;
    }
    else if(groupData.networkType == LTE_NETWORK)
    {
        GroupMessageSenderAdaptorLte *lteSender = GroupMessageSenderAdaptorLte::getInstance();
        connect(lteSender, SIGNAL(signalNewMessage(Message)),
                this, SLOT(onNewMessage(Message)));
        connect(lteSender, SIGNAL(signalSendLoginStatus(int)),
                this, SLOT(onSignalSendLoginStatus(int)));
        connect(lteSender, SIGNAL(signalSendLoginStatusToApp(int)),
                this, SLOT(onSignalSendLoginStatusToApp(int)));
        connect(lteSender, SIGNAL(signalNotifyChannelConnect(QString,int)),
                this, SLOT(onSignalNotifyChannelConnect(QString,int)));
        connect(lteSender, SIGNAL(signalNotifyChannelDisconnnect(QString)),
                this, SLOT(onSignalNotifyChannelDisconnnect(QString)));
        connect(lteSender, SIGNAL(signalNotifyTalkBegin(QString)),
                this, SLOT(onSignalNotifyTalkBegin(QString)));
        connect(lteSender, SIGNAL(signalNotifyTalkEnd(QString,int)),
                this, SLOT(onSignalNotifyTalkEnd(QString,int)));
        m_groupMessageSender = lteSender;
    }
    else if(groupData.networkType == DIGITAL_WALKIE_TALKIE_NETWORK)
    {
        qDebug() << "init baotong sender";
        GroupMessageSenderAdaptorBt *btSender = GroupMessageSenderAdaptorBt::getInstance();

        connect(btSender, SIGNAL(notifyStatusSignal(int)),
                this, SLOT(onNotifyStatusSignal(int)));
        connect(btSender, SIGNAL(notifyPTTGroupSignal(QStringList)),
                this, SLOT(onNotifyPTTGroupSignal(QStringList)));
        connect(btSender, SIGNAL(incommMessageSignal(QT_DTT::GroupTransMsg)),
                this, SLOT(onIncommMessageSignal(QT_DTT::GroupTransMsg)));
        connect(btSender, SIGNAL(PttGroupMSGSignal(QString)),
                this, SLOT(onPttGroupMSGSignal(QString)));

        connect(btSender, SIGNAL(signalAllowSetPttState(int)),
                this, SLOT(onSignalAllowSetPttState(int)));
        connect(btSender, SIGNAL(signalCreateDeleteGroup(int)),
                this, SLOT(onSignalCreateDeleteGroup(int)));
        connect(btSender, SIGNAL(signalJoinBtGroup(int)),
                this, SLOT(onSignalJoinBtGroup(int)));
        connect(btSender, SIGNAL(signalNewMessage(Message)),
                this, SLOT(onNewMessage(Message)));
        connect(btSender, SIGNAL(signalSetBtPttState(int)),
                this, SLOT(onSignalSetBtPttState(int)));
        m_groupMessageSender = btSender;

        btSender->getPTTGroup(0);
    }
    else if(groupData.networkType == WIFI_NETWORK)
    {
        QStringList wifiIpList = GroupInterface::getInstance()->getGroupAddrsByType(m_groupId, groupData.networkType);

        QString changedAddr = groupData.address;
        QString addr = changedAddr.section(".", -1);
        changedAddr.remove("."+addr);
        changedAddr.append("."+QString::number(addr.toInt()+60));
        int changedPort = groupData.port + 60;
        qDebug() << "WIFI_NETWORK" << m_groupId << changedAddr << changedPort;

        GroupMessageController2 *adhocMessageSender = new GroupMessageController2(changedAddr, changedPort, groupData.port+400, m_groupId);
        connect(adhocMessageSender, SIGNAL(signalNewMessage(Message)), this, SLOT(onNewMessage(Message)));
        connect(adhocMessageSender, SIGNAL(signalReceivePercent(QString,QString,int,int)),
                this, SLOT(onSignalReceivePercent(QString,QString,int,int)));
        connect(adhocMessageSender, SIGNAL(signalSendPercent(QString,QString,int,int)),
                this, SLOT(onSignalSendPercent(QString,QString,int,int)));
        adhocMessageSender->setGroupIps(wifiIpList);
        m_groupDataPath = QString("/home/user/sinux/groupmessages_%1/").arg(m_tableName);
        createPath(m_groupDataPath);
        adhocMessageSender->setReceivePath(m_groupDataPath);
        adhocMessageSender->start();

        m_groupMessageSender = adhocMessageSender;
    }
    else
    {
        m_groupMessageSender = NULL;
    }

    //https://ss0.bdstatic.com/5aV1bjqh_Q23odCf/static/superman/img/spis7_d7c9959e.png

//    Downloader *d = new Downloader();
//    d->setId("111");
//    d->downloadFile("orange.png", "https://ss0.bdstatic.com/5aV1bjqh_Q23odCf/static/superman/img/spis7_d7c9959e.png");

    setCurrentSyncId();

    connect(&m_p2pVoiceAdaptor, SIGNAL(signalRecordFinished(int,QString,QString)),
            this, SLOT(onSignalRecordFinished(int,QString,QString)));
    connect(&m_p2pVoiceAdaptor, SIGNAL(signalCallP2PFinished(int,QString,int)),
            this, SLOT(onSignalCallP2PStateChanged(int,QString,int)));
    connect(m_battleCallAdaptor, SIGNAL(signalBattleCallStateChanged(int,QString,int)),
            this, SLOT(onSignalCallP2PStateChanged(int,QString,int)));
}

GroupMessageManager::~GroupMessageManager()
{
    onLeaveGroup(m_netType);
}

quint64 GroupMessageManager::sendMessage(const Message &msg)
{

//    if (msg.common.type == TEXT_MESSAGE && m_lingyun != NULL && m_ttsEnable)// && msg.common.groupId == m_groupId)
//        m_lingyun->transToTts(msg.content);
    if(m_groupId.contains("p2pgroup_"))
    {
        if(msg.common.type == AUDIO_MESSAGE)
        {
            QThread::msleep(2000);
        }
        else if(msg.common.type == VIDEO_MESSAGE)
        {
            QThread::msleep(3000);
        }
    }

    Message hostMsg = msg;
    if(hostMsg.common.groupId.isEmpty())
        hostMsg.common.groupId = QUuid::createUuid().toString();
    hostMsg.common.groupId = m_groupId;
    hostMsg.common.state = MESSAGE_SEND_SUCCESS;
    hostMsg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;
    hostMsg.senderRealName = hostMsg.contentName.section("/", -1);

//    Message hostMsg = sendMsg;
    Message netMsg = messageHostToNet(hostMsg);
    netMsg.addr = m_addressAdhoc;
    scaleMessageImage(msg);

    if(netMsg.common.type == LOCATION_MESSAGE_BACKGROUND)
    {
//        emit signalMessageChanged(MESSAGE_ADD_HANDLE, netMsg);
    }
    else
    {
        bool record = true;
        if(hostMsg.common.type == TEXT_MESSAGE && hostMsg.content.contains("GXData:"))
            record = false;

        if(record)
        {
            bool recordFlag = recordMessage(hostMsg);
            if(!recordFlag)
                return 0;
            m_sqlModel.submitAll();

            if(hostMsg.common.type == TEXT_MESSAGE)
            {
                MessageList msgs;
                msgs.append(hostMsg);
                emit signalMessageChangedToChangeRemoteMessage(MESSAGE_ADD_HANDLE, msgs);
            }
        }

    }

    qint64 sendCount = -1;
    if(m_groupMessageSender)
    {
        sendCount = m_groupMessageSender->sendMessage(netMsg);
        emit signalMessageChanged(MESSAGE_ADD_HANDLE, hostMsg);
        //-------------------------lte test-----------------------------------//
//        GroupMessageSenderAdaptorLte *lteSender = qobject_cast<GroupMessageSenderAdaptorLte*>(m_groupMessageSender);
//        if(lteSender != NULL)
//        {
//            GroupInterface *groupInterface = GroupInterface::getInstance();
//            Group group = groupInterface->getGroup(netMsg.common.groupId);
//            QString addr = group.address;
//            qDebug() << "group addr:" << addr;
//            lteSender->enterChart(addr);
//            lteSender->exitChart(addr);

////            lteSender->enterChart(groupId);
////            lteSender->talkRequest(groupId);
////            lteSender->talkRelease();
//        }
    }
    else
    {
        qDebug() << "can not send message in groupmessage manager, m_groupMessageSender==null";
    }

    return sendCount;
}

quint64 GroupMessageManager::resendMessage(const QString &msgId)
{
    Message message = getMessage(msgId);
    if(removeMessage(msgId))
    {
        qDebug() << "remove message success: " << msgId;
        sendMessage(message);
    }
}

QStringList GroupMessageManager::getMessageList()
{
//    QStringList msgList;
//    for(int i = m_sqlModel.rowCount() - 1, iend = 0; i >= iend; i--)
//    {
//        QModelIndex idIndex = m_sqlModel.index(i, 0);
//        QString msgId = m_sqlModel.data(idIndex).toString();
//        msgList.append(msgId);
//    }
//    return msgList;

    QString sql = QString("SELECT id FROM __tablename__");// ORDER BY date DESC
    QStringList idList = m_sqlModel.getColumnDataWithSql(sql);
    return idList;
}

QStringList GroupMessageManager::getMessageList(const QDateTime &startTime, const QDateTime &endTime)
{
    QString sql = QString("SELECT id FROM __tablename__ WHERE date>=%1 and date<=%2 ORDER BY date DESC")
            .arg(transMessageDateToString(startTime))
            .arg(transMessageDateToString(endTime));
    QStringList idList = m_sqlModel.getColumnDataWithSql(sql);
    return idList;
}

QStringList GroupMessageManager::getMessageList(const int &startIndex, const int &endIndex)
{
    QString sql = QString("SELECT id,date FROM __tablename__ ORDER BY date DESC LIMIT %1 OFFSET %2")
            .arg(QString::number(endIndex - startIndex + 1))
            .arg(QString::number(startIndex));
    QStringList idList = m_sqlModel.getColumnDataWithSql(sql);
    return idList;
}

Message GroupMessageManager::getMessage(const QString &id)
{
    Message msg = getMessage(m_sqlModel.findDataInColumn("id", id));
    return msg;
}

void GroupMessageManager::getMessageDownload(const QString &id)
{
//    Message msg = getMessage(id);
//    msg.common.state = MESSAGE_DOWNLOAD_SUCCESS;
//    emit signalMessageDownload(msg);
    Message msg = getMessage(id);
    QString url = msg.contentName;
    QString fileName = m_groupDataPath + url.section("/", -1);
    qDebug() << "getMessageDownload name: " << fileName;

    Downloader *d = new Downloader();
    connect(d, SIGNAL(downloadFileFinished(QString,QString)),
            this, SLOT(onGetMessageDownload(QString,QString)));
    connect(d, SIGNAL(downloadFileFinished(QString,QString)),
            d, SLOT(deleteLater()));
    d->setId(id);
    d->downloadFile(fileName, url);
}

void GroupMessageManager::onGetMessageDownload(const QString &id, const QString &fileName)
{
    if(!findMessage(id))
    {
        qDebug() << "did not find message: " << id;
        return;
    }

    Message msg = getMessage(id);
    msg.contentName = "file://"+fileName;
    msg.common.state = MESSAGE_DOWNLOAD_SUCCESS;
    if(updateMessage(msg))
    {
        scaleMessageImage(msg);
        qDebug() << "update message success: " << msg.common.id;
        emit signalMessageChanged(MESSAGE_UPDATE_HANDLE, msg);
    }
    else
    {
        qDebug() << "update message fail: " << msg.common.id;
    }
}

void GroupMessageManager::onSignalRecordFinished(int network, const QString &addr, const QString &fileName)
{
    qDebug() << "GroupMessageManager::onSignalRecordFinished";

    QString id = m_groupId.section("_", -1);
    Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);
    QString number = getPhoneNumberType(contacter.numbers, network).number;
    if(number == addr)
    {
        qDebug()<<"onSignalRecordFinished" << m_groupIdP2P;
        Message msg;
        msg.contentName = "file://"+fileName;
        msg.common.groupId = m_groupIdP2P;
        msg.common.type = AUDIO_MESSAGE;
        msg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;

        sendMessage(msg);
    }

    emit signalRecordFinished(network, addr, fileName);
}

void GroupMessageManager::onSignalCallP2PStateChanged(int network, const QString &addr, int state)
{
    qDebug() << "GroupMessageManager::onSignalCallP2PFinished";

    QString id = m_groupId.section("_", -1);
    Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);
    QString number = getPhoneNumberType(contacter.numbers, network).number;
    if(number == addr)
    {
        qDebug()<<"onSignalCallP2PFinished" << m_groupId;
        emit signalCallP2PStateChanged(network, m_groupId, state);
    }
}

void GroupMessageManager::onBattleSignalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    if(size == maxSize)
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            qDebug() << "message.dat open success...";
            QDataStream in(&file);
            Message message;
            in >> message;

            message.common.groupId = m_groupId;
//            message.common.id = id;
            message.common.state = MESSAGE_DOWNLOAD_SUCCESS;
            qDebug() << "onSignalReceiveComplete" << message.common.id << message.content;
            file.close();

            onNewMessage(message);

        }
        else
        {
            qDebug() << "message.dat open error: " << file.errorString();
        }
    }
}

void GroupMessageManager::onBattleSignalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    qDebug() << "GroupMessageManager::onBattleSignalSendPercent:"
             << fileName
             << size
             << maxSize;
}

void GroupMessageManager::onSignalP2PConnected(const QString &groupId, int state)
{
    qDebug() << "GroupMessageManager::onSignalP2PConnected: " << groupId << state;

    if(m_groupId == groupId)
    {
        emit signalP2PConnected(groupId, state);
    }
}

bool GroupMessageManager::setMessageIsread(const bool &isReadFlag, const QString &id)
{
    Message msg = getMessage(id);
    msg.common.isRead = (isReadFlag? 1 : 0);
    return updateMessage(msg);
//    return m_sqlModel.submitAll();
}

void GroupMessageManager::onNewMessage(Message msg)
{

    if(msg.common.groupId != m_groupId)
    {
        qDebug() << "----------------------------------------------message groupId: "
                 << msg.common.groupId
                 << "groupId: "
                 << m_groupId;
        if(msg.common.groupId.contains("p2pgroup_"))
        {
            if(m_groupIdP2P.section("_", -1) != msg.common.userId)
            {
                qDebug() << "p2pgroup != "
                         << m_groupIdP2P.section("_", -1)
                         << msg.common.userId;
                return;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        qDebug() << "message groupId and my groupid is same " << m_groupId;
    }

    if(m_groupId.contains("p2pgroup_"))
    {
        QString contacterId = m_groupIdP2P.section("_", -1);
        Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(contacterId);
        QString name = contacter.surname + contacter.name;

        CNotificationManager notiMgr;
        CNotification genoti;
        genoti.setTitle("收到"+name+"的新消息");
        genoti.setSubtitle("收到"+name+"的新消息");
        genoti.setAttributes(CAbstractNotification::HideDisplayTime);
        genoti.setVibrationMode(CAbstractNotification::ForceVibrationMode);
        genoti.setMarqueeText("收到"+name+"的新消息");

        QString result;
        result = notiMgr.sendNotification(genoti);
        qDebug() <<"notiMgr.sendNotification -----------------" <<result;
    }
    else
    {
        Group groupData = GroupInterface::getInstance()->getGroup(m_groupId);

        CNotificationManager notiMgr;
        CNotification genoti;
        genoti.setTitle("收到"+groupData.name+"的新消息");
        genoti.setSubtitle("收到"+groupData.name+"的新消息");
        genoti.setAttributes(CAbstractNotification::HideDisplayTime);
        genoti.setVibrationMode(CAbstractNotification::ForceVibrationMode);
        genoti.setMarqueeText("收到"+groupData.name+"的新消息");

        QString result;
        result = notiMgr.sendNotification(genoti);
        qDebug() <<"notiMgr.sendNotification -----------------" <<result;
    }

//    if (msg.common.type == TEXT_MESSAGE && msg.common.groupId == m_groupId && m_lingyun != NULL && m_ttsEnable == true)
//    {
//        if (m_msgid != msg.common.id){
//            CTS_PRINT << "LingYun transToTts";
//            m_lingyun->transToTts(msg.content);
//            m_msgid = msg.common.id;
//        }
//    }
    if(msg.common.type == LOCATION_MESSAGE_BACKGROUND)
    {
        emit signalMessageChanged(MESSAGE_ADD_HANDLE, msg);
        return;
    }
    else if(msg.common.type == TEXT_MESSAGE)
    {
        QString str = "order_messsage=";
        QString content = QString::fromLocal8Bit(msg.content);
        if(content.contains(str))
        {
            QString order = msg.content;
            order.remove(str);
            GroupChange groupChange = convertStringToGroupChange(order.toLocal8Bit());
            Group group = GroupInterface::getInstance()->getGroup(groupChange.groupId);
            group.type = BATTLE_GROUP;
            GroupInterface::getInstance()->onUpdateGroup(group);
            emit GroupInterface::getInstance()->signalGroupExchange();

            msg.common.type = ORDER_MESSAGE;
            msg.common.state = MESSAGE_DOWNLOAD_SUCCESS;
        }

        QString gxStr = "GXData:";
        if(content.contains(gxStr))
        {
            emit signalMessageChanged(MESSAGE_ADD_HANDLE, msg);
            return;
        }
    }
//    msg.common.groupId =
    Message netMsg = msg;
//    netMsg.common.state = MESSAGE_RECEIVE_SUCCESS;
//    if(netMsg.common.type == TEXT_MESSAGE)
//        netMsg.common.state = MESSAGE_RECEIVE_SUCCESS;

    QString localId = ContacterInterface::getInstance()->getLocalInfo().id;
    if(netMsg.common.userId == localId && !localId.isEmpty() && !netMsg.common.userId.isEmpty())
    {
//        receiveMsg.common.state = MESSAGE_RECEIVE_SUCCESS;
//        bool updateFlag = updateMessage(receiveMsg);
//        if(!updateFlag)
//        {
//            qDebug() << "group " << m_groupId << " update self msg " << receiveMsg.common.id << " fail";
//            return;
//        }
//        m_sqlModel.submitAll();
//        emit signalMessageChanged(MESSAGE_UPDATE_HANDLE, receiveMsg);
//        if(receiveMsg.common.type == TEXT_MESSAGE)
//        {
//            MessageList msgs;
//            msgs.append(receiveMsg);
//            emit signalMessageChangedToChangeRemoteMessage(MESSAGE_UPDATE_HANDLE, msgs);
//        }
    }
    else
    {
        qDebug() << "receive other user message";

        Message hostMsg = messageNetToHost(netMsg);
        qDebug() << "hostMsg: " << hostMsg.contentName;

        if(!findMessage(hostMsg.common.id))
        {
            scaleMessageImage(hostMsg);
            bool recordFlag = recordMessage(hostMsg);
            if(!recordFlag)
            {
                qDebug() << "group " << m_groupId << " record receive msg " << hostMsg.common.id << " fail";
                return;
            }
            onAddSyncCount(1);
            signalUpdateMessageCount(m_groupId, onGetSyncCount());
            qDebug() << "onGetSyncCount()" << onGetSyncCount();

            m_sqlModel.submitAll();
            emit signalMessageChanged(MESSAGE_ADD_HANDLE, hostMsg);
            if(hostMsg.common.type == TEXT_MESSAGE)
            {
                MessageList msgs;
                msgs.append(hostMsg);
                emit signalMessageChangedToChangeRemoteMessage(MESSAGE_ADD_HANDLE, msgs);
            }
        }
        else
        {
            scaleMessageImage(hostMsg);
            updateMessage(hostMsg);
            m_sqlModel.select();
//            m_sqlModel.submitAll();
            emit signalMessageChanged(MESSAGE_ADD_HANDLE, hostMsg);
            if(hostMsg.common.type == TEXT_MESSAGE)
            {
                MessageList msgs;
                msgs.append(hostMsg);
                emit signalMessageChangedToChangeRemoteMessage(MESSAGE_ADD_HANDLE, msgs);
            }
        }
    }
}

bool GroupMessageManager::recordMessage(Message &message)
{
    int row = insertLastRow(message.common.date);
    if(row < 0)
        return false;

    bool setFlag = true;
    QModelIndex idIndex = m_sqlModel.index(row, 0);
    setFlag &= m_sqlModel.setData(idIndex, message.common.id);

    QModelIndex typeIndex = m_sqlModel.index(row, 1);
    setFlag &= m_sqlModel.setData(typeIndex, message.common.type);

    QModelIndex stateIndex = m_sqlModel.index(row, 2);
    setFlag &= m_sqlModel.setData(stateIndex, message.common.state);

    QModelIndex userIdIndex = m_sqlModel.index(row, 3);
    setFlag &= m_sqlModel.setData(userIdIndex, message.common.userId);

    QModelIndex groupIdIndex = m_sqlModel.index(row, 4);
    setFlag &= m_sqlModel.setData(groupIdIndex, message.common.groupId);

    QModelIndex dateIndex = m_sqlModel.index(row, 5);
    QString dateStr = transMessageDateToString(message.common.date);
    setFlag &= m_sqlModel.setData(dateIndex, dateStr.toLongLong());

    QModelIndex showMessageIndex = m_sqlModel.index(row, 6);
    setFlag &= m_sqlModel.setData(showMessageIndex, message.common.showMsg);

    QModelIndex isreadIndex = m_sqlModel.index(row, 7);
    setFlag &= m_sqlModel.setData(isreadIndex, message.common.isRead);

    QModelIndex accessoryIndex = m_sqlModel.index(row, 8);
    setFlag &= m_sqlModel.setData(accessoryIndex, message.common.accessoryPath);

    QModelIndex contentIndex = m_sqlModel.index(row, 9);
    setFlag &= m_sqlModel.setData(contentIndex, message.content);

    QModelIndex contentNameIndex = m_sqlModel.index(row, 10);
    setFlag &= m_sqlModel.setData(contentNameIndex, message.contentName);

    QModelIndex contentTypeIndex = m_sqlModel.index(row, 11);
    setFlag &= m_sqlModel.setData(contentTypeIndex, message.contentType);

    QModelIndex networktypeIndex = m_sqlModel.index(row, 12);
    setFlag &= m_sqlModel.setData(networktypeIndex, message.networkType);

    QModelIndex longitudeIndex = m_sqlModel.index(row, 13);
    setFlag &= m_sqlModel.setData(longitudeIndex, message.longitude);

    QModelIndex latitudeIndex = m_sqlModel.index(row, 14);
    setFlag &= m_sqlModel.setData(latitudeIndex, message.latitude);

    QModelIndex heightIndex = m_sqlModel.index(row, 15);
    setFlag &= m_sqlModel.setData(heightIndex, message.height);

//    QModelIndex locationArray = m_sqlModel.index(row, 16);
//    setFlag &= m_sqlModel.setData(locationArray, message.locationArray);
    return setFlag;
}

void GroupMessageManager::scaleMessageImage(const Message &message)
{
    if(message.common.type == IMAGE_MESSAGE)
    {
        QString fileName = message.contentName;
        fileName.remove("file://");
        QString fileNameScaled = fileName;
        QString backjpg = "."+fileNameScaled.section(".", -1);
        fileNameScaled.replace(backjpg, "___"+backjpg);

        if(!QFile::exists(fileNameScaled))
        {
            qDebug() << "scaleMessageImage image not exist, start scale...";

            QImage img(fileName);
            if(img.size().width() > 300 || img.size().height() > 300)
            {
                QImage scaledImg;
                if(img.size().width() > img.size().height())
                {
                    scaledImg = img.scaledToWidth(300);
                }
                else
                {
                    scaledImg = img.scaledToHeight(300);
                }


                qDebug() << "scaleMessageImage image scaled ----, save flag: " << scaledImg.save(fileNameScaled) << fileNameScaled;
            }
            else
            {
                qDebug() << "scaleMessageImage image size less than 300" << img.size();
            }
        }
        else
        {
            qDebug() << "scaleMessageImage image exist, do not need scale...";
        }
    }
    else
    {
        qDebug() << "scaleMessageImage message type is not image";
    }
}

Message GroupMessageManager::messageHostToNet(const Message &msg)
{
    Message message = msg;
    if(message.common.type != TEXT_MESSAGE)
    {
        message.senderRealName = message.contentName.section("/", -1);
        QString fileName = message.contentName;
        fileName.remove("file://");
        if(msg.networkType != LTE_NETWORK)
            message.content = loadFile(fileName);
        message.fileSize = QFileInfo(message.contentName).size();
        message.contentName = fileName;
    }
    return message;
}

Message GroupMessageManager::messageNetToHost(const Message &msg)
{
    Message message = msg;
    if(message.common.type != TEXT_MESSAGE)
    {
        QDir dir = QDir::rootPath();
        dir.cd("home");
        dir.cd("user");
        QString fileName;
        if(msg.senderRealName.isEmpty())
            fileName = dir.absolutePath() + "/messagefiles/" + QUuid::createUuid().toString().remove("{").remove("}").remove("-");
        else
            fileName = dir.absolutePath() + "/messagefiles/" + msg.senderRealName;
//        fileName = dir.absolutePath() + "/messagefiles/" + msg.senderRealName;
        fileName = getUseablePath(fileName);
        if(message.common.type == IMAGE_MESSAGE)
        {
//            fileName = dir.absolutePath() + "/messagefiles/" + msg.senderRealName;
        }
        else if(message.common.type == VIDEO_MESSAGE)
        {
            fileName = dir.absolutePath() + "/messagefiles/" + msg.senderRealName;
        }
        else if(message.common.type == AUDIO_MESSAGE)
        {
            fileName = dir.absolutePath() + "/messagefiles/" + msg.senderRealName;
        }
        else
        {
            fileName = dir.absolutePath() + "/messagefiles/" + msg.senderRealName;
        }
        if(!message.content.isEmpty())
        {
            saveFile(message.content, fileName);
            message.content = "";
            message.contentName = "file://"+fileName;
        }
    }
    return message;
}

bool GroupMessageManager::updateMessage(const Message &message)
{
    m_sqlModel.findDataInColumn(0, message.common.id);

    bool findFlag = findMessage(message.common.id);
    if(!findFlag)
        return findFlag;

    QString key = message.common.id;
    QStringList values;
    values << QString()
           << QString::number(message.common.type)
           << QString::number(message.common.state)
           << message.common.userId
           << message.common.groupId
           << transMessageDateToString(message.common.date)
           << message.common.showMsg
           << QString::number(message.common.isRead)
           << message.common.accessoryPath
           << QString::fromLocal8Bit(message.content)
           << message.contentName
           << message.contentType
           << QString::number(message.networkType)
           << message.longitude
           << message.latitude
           << message.height;

    bool updateFlag = m_sqlModel.updateData(key, values);
    qDebug() << "updateFlag: " << updateFlag << values.value(9);

    return updateFlag;
}

bool GroupMessageManager::removeMessage(const QString &msgId)
{
    bool flag = SqlFunction::doSql(QString("DELETE FROM %1 WHERE id=%2").arg(m_tableName).arg(msgId));
    return flag;
}

Message GroupMessageManager::getMessage(const int &row)
{
    return getMessage(m_sqlModel.rowData(row));
}

Message GroupMessageManager::getMessage(const QStringList &rowData)
{
    if(rowData.isEmpty())
        return Message();

    Message msg;

    msg.common.id = rowData.value(0);

    msg.common.type = rowData.value(1).toInt();

    msg.common.state = rowData.value(2).toInt();

    msg.common.userId = rowData.value(3);

    msg.common.groupId = rowData.value(4);

    msg.common.date = transStringToMessageDate(rowData.value(5));

    msg.common.showMsg = rowData.value(6);

    msg.common.isRead = rowData.value(7).toInt();

    msg.common.accessoryPath = rowData.value(8);;

    msg.content = rowData.value(9).toLocal8Bit();

    msg.contentName = rowData.value(10);

    msg.contentType = rowData.value(11);
    msg.senderRealName = msg.contentName.section("/", -1);

    msg.networkType = rowData.value(12).toInt();

    msg.longitude = rowData.value(13);

    msg.latitude = rowData.value(14);

    msg.height = rowData.value(15);
    //msg.locationArray = rowData.value(16).toLocal8Bit();

    QString contentName = msg.contentName;
    msg.fileSize = QFileInfo(contentName.remove("file://")).size();
//    msg.content = QJsonDocument::fromJson( m_sqlModel.data(contentIndex).toByteArray());

    return msg;
}

bool GroupMessageManager::findMessage(const QString &id)
{
//    int row = -1;
//    for(int i = m_sqlModel.rowCount() - 1, iend = 0; i >= iend; i--)
//    {
//        QModelIndex idIndex = m_sqlModel.index(i, 0);
//        QString idValue = m_sqlModel.data(idIndex).toString();
//        if(idValue == id)
//        {
//            row = i;
//            break;
//        }
//    }
    QStringList rowData = m_sqlModel.findDataInColumn("id", id);
    if(rowData.value(0).isEmpty())
        return false;
    else
        return true;
//    return row;
}

int GroupMessageManager::insertLastRow(const QDateTime &time)
{
    int rowCount = m_sqlModel.rowCount();
    int row = rowCount;
    //find last 10 message time
    if(rowCount > 10)
    {
        for(int i = rowCount - 10, iend = rowCount; i < iend; i++)
        {
            QModelIndex dateIndex = m_sqlModel.index(i, 4);
            QString msgDateStr = m_sqlModel.data(dateIndex).toString();
            QDateTime msgDate = transStringToMessageDate(msgDateStr);
            if(msgDate > time)
                row = i;
        }
    }

    bool insertFlag = m_sqlModel.insertRow(row);
    if(!insertFlag)
        return -1;
    else
        return row;
}

void GroupMessageManager::onMessageReceived(const int &handleType, const MessageList &msgs)
{
    Message changedMsg = msgs.value(0);
    switch (handleType) {
    case MESSAGE_ADD_HANDLE:
    {
        bool recordFlag = recordMessage(changedMsg);
        if(!recordFlag)
            return;
        m_sqlModel.submitAll();
        emit signalMessageChanged(MESSAGE_ADD_HANDLE, changedMsg);
        break;
    }
    case MESSAGE_DELETE_HANDLE:
    {

        break;
    }
    case MESSAGE_UPDATE_HANDLE:
    {
        bool update = updateMessage(changedMsg);
        if(update)
        {
            signalMessageChanged(MESSAGE_UPDATE_HANDLE, changedMsg);
        }
        break;
    }
    case MESSAGE_INIT_HANDLE:
    {
        break;
    }

    default:
        break;
    }
}

void GroupMessageManager::onDbChanged()
{
    emit signalMessageChanged(MESSAGE_INIT_HANDLE, Message());
}

void GroupMessageManager::onAddSyncCount(const int &count)
{
    addSyncCount(count);
}

void GroupMessageManager::onClearSyncCount()
{
    clearSyncCount();
    signalUpdateMessageCount(m_groupId, getSyncCount());
}

int GroupMessageManager::onGetSyncCount()
{
    return getSyncCount();
}


void GroupMessageManager::addSyncCount(const int &count)
{
    m_currentSyncCount += count;
    return;
}

void GroupMessageManager::clearSyncCount()
{
    m_currentSyncCount = 0 ;
    setCurrentSyncId();
    return;
}

int GroupMessageManager::getSyncCount()
{
    return m_currentSyncCount;
}

void GroupMessageManager::setCurrentSyncId()
{
    QStringList idList = getMessageList(0,0);
    if(!idList.isEmpty())
    {
        m_currentSyncId = idList.value(0);
    }
}

//ptt
unsigned GroupMessageManager::onJoinGroup(int network)
{
    if(network == LTE_NETWORK)
    {
        GroupMessageSenderAdaptorLte * lteAdaptor =
                dynamic_cast<GroupMessageSenderAdaptorLte*>(m_groupMessageSender);
        if(lteAdaptor != NULL)
        {
            lteAdaptor->enterChart(m_groupId);
            lteAdaptor->setGroupForeground(m_groupId, true);
        }

        return 0;
    }
    else if(network == AD_HOC_NETWORK)
    {
        if(!m_pttGroupId)
        {
            Group groupData = GroupInterface::getInstance()->getGroup(m_groupId);
            if(groupData.address.isEmpty())
            {
                return m_pttGroupId;
            }
//            QString addr = groupData.address.section(".", -1);
//            groupData.address.remove("."+addr);
//            groupData.address.append("."+QString::number(addr.toInt()));
            AudioManager audioManager;
            audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_SELFORGNET);

            m_pttGroupId = m_voipServiceAdaptor->joinGroup(groupData.address, groupData.port);
            if(m_pttGroupId)
                m_voipServiceAdaptor->setPttState(m_pttGroupId, 1);

            m_netType = network;
        }
        return m_pttGroupId;
    }
    else if(network == WIFI_NETWORK)
    {
        if(!m_pttGroupId)
        {
            Group groupData = GroupInterface::getInstance()->getGroup(m_groupId);
            if(groupData.address.isEmpty())
            {
                return m_pttGroupId;
            }
//            QString addr = groupData.address.section(".", -1);
//            groupData.address.remove("."+addr);
//            groupData.address.append("."+QString::number(addr.toInt()+100));

            m_pttGroupId = m_voipServiceAdaptor->joinGroup(groupData.address, groupData.port);
            m_netType = network;
            if(m_pttGroupId)
                m_voipServiceAdaptor->setPttState(m_pttGroupId, 1);
        }
        return m_pttGroupId;
    }

    return 0;
}

int GroupMessageManager::onLeaveGroup(int network)
{
    if(network == LTE_NETWORK)
    {
        GroupMessageSenderAdaptorLte * lteAdaptor =
                dynamic_cast<GroupMessageSenderAdaptorLte*>(m_groupMessageSender);
        if(lteAdaptor != NULL)
            lteAdaptor->exitChart(m_groupId);
    }
    else if(network == AD_HOC_NETWORK)
    {
        int error = -1;
        if(m_pttGroupId)
        {
            error = m_voipServiceAdaptor->leaveGroup(m_pttGroupId);
            m_pttGroupId = 0;
        }
        return error;
    }
    else if(network == WIFI_NETWORK)
    {
        int error = -1;
        if(m_pttGroupId)
        {
            error = m_voipServiceAdaptor->leaveGroup(m_pttGroupId);
            m_pttGroupId = 0;
        }
        return error;
    }

    return 0;
}

int GroupMessageManager::onGetPttState(int network)
{
    if(network == LTE_NETWORK)
    {
        return 0;
    }
    else if(network == AD_HOC_NETWORK)
    {
        int state = -1;
        if(m_pttGroupId)
            state = m_voipServiceAdaptor->getPttState(m_pttGroupId);
        return state;
    }
    else if(network == WIFI_NETWORK)
    {
        int state = -1;
        if(m_pttGroupId)
            state = m_voipServiceAdaptor->getPttState(m_pttGroupId);
        return state;
    }

    return 0;
}

int GroupMessageManager::onSetPttState(int network, int state)
{
    if(network == LTE_NETWORK)
    {
        if(state == 2)
        {
            GroupMessageSenderAdaptorLte * lteAdaptor =
                    dynamic_cast<GroupMessageSenderAdaptorLte*>(m_groupMessageSender);
            if(lteAdaptor != NULL)
                lteAdaptor->talkRequest(m_groupId);
        }
        else if(state == 1)
        {
            GroupMessageSenderAdaptorLte * lteAdaptor =
                    dynamic_cast<GroupMessageSenderAdaptorLte*>(m_groupMessageSender);
            if(lteAdaptor != NULL)
                lteAdaptor->talkRelease();
        }
        return 0;
    }
    else if(network == AD_HOC_NETWORK)
    {
        int error = -1;
        AdHocAdaptor * adhocAdaptor =
                dynamic_cast<AdHocAdaptor*>(m_groupMessageSender);
        bool pttState = ((state == 2) ? true : false);
        if(adhocAdaptor != NULL)
            adhocAdaptor->onSetPtt(m_addressAdhoc, pttState);

        if(state == 1)
            m_voipServiceAdaptor->setPttState(m_pttGroupId, state);

        return 0;
    }
    else if(network == WIFI_NETWORK)
    {
        if(m_pttGroupId)
            m_voipServiceAdaptor->setPttState(m_pttGroupId, state);

//        emit signalPttGroupStateChanged(AD_HOC_NETWORK, m_groupId, state);
    }
    return -1;
}

void GroupMessageManager::onInGroupUi()
{
    if(m_groupId.contains("p2pgroup_"))
    {
        P2PMessageSenderTcpAdaptor *p2pAdaptor = P2PMessageSenderTcpAdaptor::getInstance();

        QString contacterId = m_groupId.section("_", 1,1);
        if(!contacterId.isEmpty())
        {
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(contacterId);
            PhoneNumber phoneNumber = getPhoneNumberType(contacter.numbers, m_p2pNetworkType);
            p2pAdaptor->onConnect(m_groupId, phoneNumber.number);
        }
//        PointToPointAdaptor *p2pAdaptor = PointToPointAdaptor::getInstance();

//        QString contacterId = m_groupId.section("_", -1);
//        if(!contacterId.isEmpty())
//        {
//            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(contacterId);
//            PhoneNumber phoneNumber = getPhoneNumberType(contacter.numbers, m_p2pNetworkType);
////            p2pAdaptor->setNetworkType(m_p2pNetworkType);
//            p2pAdaptor->onSetAddresses(QStringList(phoneNumber.number));
//            p2pAdaptor->onConnect();
//        }
    }
}

void GroupMessageManager::onOutGroupUi()
{
//    PointToPointAdaptor *p2pAdaptor = PointToPointAdaptor::getInstance();
//    p2pAdaptor->onDisconnect();
}

int GroupMessageManager::onCallP2P(int network, QString addr)
{
    if(network == BATTLE_NETWORK)
        return 0;//m_battleCallAdaptor->onCallBattle(addr);
    else
        return 0;//m_p2pVoiceAdaptor.onCallP2P(network, addr);
}

int GroupMessageManager::onHangupP2P(int network, QString addr)
{
    if(network == BATTLE_NETWORK)
        return 0;//m_battleCallAdaptor->onHangupBattle(addr);
    else
        return 0;//m_p2pVoiceAdaptor.onHangupP2P(network, addr);
}

int GroupMessageManager::onGetP2PState(int network, QString addr)
{
    if(network == BATTLE_NETWORK)
        return 0;
//        return m_battleCallAdaptor->onGetBattlePttState(addr);
    else
        return 0;//m_p2pVoiceAdaptor.onGetP2PState(network, addr);
}

int GroupMessageManager::onSetP2PState(int network, QString addr, int state)
{
    if(network == BATTLE_NETWORK && state == 2)
    {
        return m_battleCallAdaptor->onCallBattle(addr);
        //return m_battleCallAdaptor->onSetBattlePttState(addr, state);
    }
    else if (network == BATTLE_NETWORK && state == 1)
    {
        return m_battleCallAdaptor->onHangupBattle(addr);
    }
    else
    {
        if(state == 2)
        {
            AudioRecorder::getInstance()->setName(m_groupDataPath+ QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz"));
            AudioRecorder::getInstance()->record();
        }
        else if(state == 1)
        {
            AudioRecorder::getInstance()->stop();
            QString fileName = AudioRecorder::getInstance()->name();

            Message msg;
            msg.contentName = "file://"+fileName;
            msg.common.groupId = m_groupId;
            msg.common.type = AUDIO_MESSAGE;
            msg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;

            sendMessage(msg);
        }

        return 1;
        //        return m_p2pVoiceAdaptor.onSetP2PState(network, addr, state);
    }
}

void GroupMessageManager::onSetTts(QString groupId, bool on)
{
    if (m_groupId == groupId)
        m_ttsEnable = on;
}

bool GroupMessageManager::onGetTts()
{
    return m_ttsEnable;
}

void GroupMessageManager::onAdhocPttStateChanged(int status, QString ip)
{
    qDebug() << "----------GroupMessageManager::onAdhocPttStateChanged" << status << ip;
    if(m_addressAdhoc == ip)
    {
        int pttState = (status == 0) ? 2 : 1;
        if(m_pttGroupId)
            m_voipServiceAdaptor->setPttState(m_pttGroupId, pttState);

        emit signalPttGroupStateChanged(AD_HOC_NETWORK, m_groupId, pttState);
    }
}

void GroupMessageManager::onSignalPttStateChanged(unsigned groupId, int state)
{
    emit signalPttStateChanged(0, m_groupId, state);
}

void GroupMessageManager::onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    qDebug()<<"GroupMessageManager::onSignalPttRecordFileFinished";
//    Message msg;
//    msg.contentName = "file://"+fileName;
//    msg.common.groupId = m_groupId;
//    msg.common.type = AUDIO_MESSAGE;
//    msg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;

//    sendMessage(msg);
}

void GroupMessageManager::onSignalNotifyChannelConnect(QString groupId, int connectStatus)
{
    qDebug() << "GroupMessageManager::onSignalNotifyChannelConnect" << groupId << connectStatus;
    if(groupId == m_groupId)
    {
        emit signalPttGroupStateChanged(LTE_NETWORK, m_groupId, connectStatus);
    }
}

void GroupMessageManager::onSignalNotifyChannelDisconnnect(QString groupId)
{
    qDebug() << "GroupMessageManager::onSignalNotifyChannelDisconnnect" << groupId;
    if(groupId == m_groupId)
    {
        emit signalPttGroupStateChanged(LTE_NETWORK, m_groupId, 0);
    }
}

void GroupMessageManager::onSignalNotifyTalkBegin(QString groupId)
{
    qDebug() << "GroupMessageManager::onSignalNotifyTalkBegin" << groupId;
    if(groupId == m_groupId)
    {
        emit signalPttStateChanged(LTE_NETWORK, m_groupId, 2);
    }
}

void GroupMessageManager::onSignalNotifyTalkEnd(QString groupId, int errCode)
{
    return;
//    qDebug() << "GroupMessageManager::onSignalNotifyTalkEnd" << groupId << errCode;
//    if(groupId == m_groupId)
//    {
//        if(errCode == 0 ||
//                errCode == 1 ||
//                errCode == 3 ||
//                errCode == 4)
//        {
//            QString fileName = m_groupDataPath + "ltevoice_"+QUuid::createUuid().toString().remove("-").remove("{").remove("}") + ".pcm";
//            if(cpFile2("/home/user/JJF/stream_in.pcm", fileName))
//            {
//                Message msg;
//                msg.contentName = "file://"+fileName;
//                msg.common.groupId = m_groupIdP2P;
//                msg.common.type = AUDIO_MESSAGE;
//                msg.common.userId = ContacterInterface::getInstance()->getLocalInfo().id;

//                sendMessage(msg);
//                GroupMessageSenderAdaptorLte * lteAdaptor =
//                        dynamic_cast<GroupMessageSenderAdaptorLte*>(m_groupMessageSender);
//                lteAdaptor->groupLteResetTalkFile();
//                qDebug() << "send message voice lte: " << msg.contentName;
//            }
//        }

//        emit signalPttStateChanged(LTE_NETWORK, m_groupId, 1);
//    }
}

void GroupMessageManager::onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "GroupMessageManager::onSignalSendPercent";
    emit signalSendPercent(groupId, id, recvNum, allNum);
}

void GroupMessageManager::onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "GroupMessageManager::onSignalReceivePercent";
    emit signalReceivePercent(groupId, id, recvNum, allNum);
}

void GroupMessageManager::onSignalSendLoginStatus(int status)
{
    ContacterInterface *contacter = ContacterInterface::getInstance();
    Contacter local = contacter->getLocalInfo();
    PhoneNumbers numbers = local.numbers;
    for(int i = 0, iend = numbers.count(); i < iend; i++)
    {
        if(numbers[i].netType == LTE_NETWORK)
        {
            if(status == 1)
                numbers[i].online = 1;
            else
                numbers[i].online = 0;
            break;
        }
    }

//    qDebug() << "GroupMessageManager::onSignalSendLoginStatus" << status;
}

void GroupMessageManager::onSignalSendLoginStatusToApp(int status)
{
    //    qDebug() << "GroupMessageManager::onSignalSendLoginStatusToApp" << status;
}

void GroupMessageManager::onNotifyStatusSignal(int status)
{
    qDebug() << "GroupMessageManager::onNotifyStatusSignal" << status;

}

void GroupMessageManager::onNotifyPTTGroupSignal(QStringList groupList)
{
    qDebug() << "GroupMessageManager::onNotifyPTTGroupSignal" << groupList;
    Group groupData = GroupInterface::getInstance()->getGroup(m_groupId);
    qDebug() << "groupData: " << groupData.address;
    GroupMessageSenderAdaptorBt *btSender = GroupMessageSenderAdaptorBt::getInstance();

    if(groupList.contains(groupData.address))
    {
        qDebug() << "groupData.address contains";
        //dial
    }
    else
    {
        qDebug() << "groupData.address not contains";
        int index = btSender->getUseableGroupIndex();
        btSender->addGroup(index, groupData.address);
    }
    btSender->addGroupMap(groupData.address, m_groupId);
}

void GroupMessageManager::onIncommMessageSignal(QT_DTT::GroupTransMsg msg)
{
//    qDebug() << "GroupMessageManager::onIncommMessageSignal" << msg.addr;
}

void GroupMessageManager::onPttGroupMSGSignal(QString msg)
{
    //    qDebug() << "GroupMessageManager::onPttGroupMSGSignal" << msg;
}

void GroupMessageManager::onSignalCreateDeleteGroup(int success)
{
    qDebug() << "GroupMessageManager::onSignalCreateDeleteGroup" << success;
    emit signalCreateDeleteGroup(m_groupId, success);
}

void GroupMessageManager::onSignalJoinBtGroup(int success)
{
    qDebug() << "GroupMessageManager::onSignalJoinBtGroup" << success;
    emit signalJoinBtGroup(m_groupId, success);
}

void GroupMessageManager::onSignalSetBtPttState(int success)
{
    qDebug() << "GroupMessageManager::onSignalSetBtPttState" << success;
    emit signalSetBtPttState(m_groupId, success);
}

void GroupMessageManager::onSignalAllowSetPttState(int allow)
{
    qDebug() << "GroupMessageManager::onSignalAllowSetPttState" << allow;
    emit signalAllowSetPttState(m_groupId, allow);
}

bool GroupMessageManager::clearData()
{
    //rm directory
    //m_groupDataPath
    QString order = QString("rm -rf %1").arg(m_groupDataPath);
    int sysRet = system(order.toLocal8Bit().data());
    qDebug() << "excute: " << order << "---result: " << sysRet;

    //rm table
    bool flag = SqlFunction::doSql(QString("DELETE FROM %1").arg(m_tableName));
    if(flag)
        emit signalMessageChanged(MESSAGE_DELETE_HANDLE, Message());

    return flag;
}

void GroupMessageManager::onSetBtPttState(int state)
{
    GroupMessageSenderAdaptorBt *btSender = GroupMessageSenderAdaptorBt::getInstance();
    btSender->GroupPTTSet(state == 2);
}

void GroupMessageManager::onJoinBtGroup()
{
    qDebug() << "GroupMessageManager::onJoinBtGroup";
    GroupMessageSenderAdaptorBt *btSender = GroupMessageSenderAdaptorBt::getInstance();
    Group groupData = GroupInterface::getInstance()->getGroup(m_groupId);
    btSender->GroupPTTDial(groupData.address, 0);
}

bool GroupMessageManager::onGetBtPttAllow()
{
    GroupMessageSenderAdaptorBt *btSender = GroupMessageSenderAdaptorBt::getInstance();
    return btSender->getPttSetAllow();
}
