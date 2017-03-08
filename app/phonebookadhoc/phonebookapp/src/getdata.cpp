#include <QModelIndexList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <qdbusconnection.h>
#include <QQuickItem>
#include <QThread>
#include <QtQml>
#include "../../../commondata/datatype/commondata.h"
#include "getdata.h"
#include "downloadcontactadaptor.h"
#include "fileshareadaptor.h"
#include <qmath.h>
#include <QtConcurrent/QtConcurrent>
#include <QTimer>
GetData::GetData(QObject *parent) :
    QObject(parent), m_sessionBus("")
{

    //    m_dataInterface = new QDBusInterface("com.sinux.Service", "/AddressBookMgr",
    //                                         "com.sinux.Service", QDBusConnection::sessionBus());
    //    if(!m_dataInterface->isValid())
    //    {
    //        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    //    }
    //    m_voipInterface = new QDBusInterface("com.sinux.voip","/",
    //                                         "com.sinux.DBus.voip",QDBusConnection::sessionBus());
    //    if(!m_voipInterface->isValid())
    //    {
    //        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    //    }

    //    m_dialManager = QT_DTT::DialManager::instance();
    //    m_digitalDialManager = QT_DTT::DigitalDialManager::instance();
    //    connect(m_digitalDialManager,SIGNAL(statusChanged(QString,int)),this,SLOT(onDigitalStatusChanged(QString,int)));
    //    connect(m_digitalDialManager,SIGNAL(pttGettedChanged(bool)),this,SLOT(onPttGettedChanged(bool)));


    m_sessionBus = QDBusConnection::sessionBus();

    m_sosInterface = new QDBusInterface("org.ofono", "/ril_0",
                                        "org.ofono.RadioSettings", QDBusConnection::systemBus());
    if (m_sosInterface->isValid())
        qDebug() << "succcccccccccccccccc";
    else
        qDebug() << "faillllllllllllllllll";

    m_sessionBus.connect("", "", "com.sinux.voipService", "signalDialState", this, SLOT(onNumberTest(int,int)));
    m_adaptor = ContacterInterfaceAdaptor::getInstance();
    connect(m_adaptor,SIGNAL(signalContacterChanged(int,Contacter)),this,SLOT(onContacterChanged(int,Contacter)));

    m_groupAdaptor = GroupInterfaceAdaptor::getInstance();
    connect(m_groupAdaptor,SIGNAL(signalGroupChanged(int,Group)),this,SLOT(onGroupChanged(int,Group)));
    connect(m_groupAdaptor,SIGNAL(signalGroupExchange()), this, SLOT(onSignalGroupExchange()));

    m_messageAdaptor = MessageManagerAdaptor::getInstance();
    connect(m_messageAdaptor,SIGNAL(signalMessageChanged(int,Message)),this,SLOT(onMessageChanged(int,Message)));
    connect(m_messageAdaptor,SIGNAL(signalMessageDownload(Message)),this,SLOT(onMessageDownload(Message)));
    connect(m_messageAdaptor, SIGNAL(signalPttStateChanged(QString,int)), this, SIGNAL(signalPttStateChanged(QString,int)));
    connect(m_messageAdaptor, SIGNAL(signalSendPercent(QString,QString,int,int)),this, SLOT(onSignalSendPercent(QString,QString,int,int)));
    connect(m_messageAdaptor, SIGNAL(signalReceivePercent(QString,QString,int,int)),this, SLOT(onSignalReceivePercent(QString,QString,int,int)));

    connect(m_messageAdaptor, SIGNAL(signalUpdateMessageCount(QString,int)), this, SIGNAL(signalUpdateMessageCount(QString,int)));

    m_historyInterface = CallHistoryInterfaceAdaptor::getInstance();
    connect(m_historyInterface,SIGNAL(signalCallHistoryChanged(QString,int)),this,SLOT(onCallHistoryChanged(QString,int)));

    m_bluetoothAdaptor = BluetoothInterfaceAdaptor::getInstance();
    connect(m_bluetoothAdaptor,SIGNAL(signalDeviceDiscovered(BluetoothAddressData)),this,SLOT(onSignalDeviceDiscovered(BluetoothAddressData)));
    QObject::connect(m_bluetoothAdaptor, SIGNAL(signalContacterReceived(int,Contacter)),
                     m_adaptor, SLOT(onContacterReceived(int,Contacter)));
    connect(m_bluetoothAdaptor, SIGNAL(signalPairConfirmation(BluetoothAddressData,QString)),
            this, SLOT(onSignalPairConfirmation(BluetoothAddressData,QString)));
    connect(m_bluetoothAdaptor, SIGNAL(signalConnectServerFinished(BluetoothAddressData)),
            this, SLOT(onSignalConnectFinished(const BluetoothAddressData&)));
    connect(m_bluetoothAdaptor, SIGNAL(signalPairFinished(BluetoothAddressData,int)),
            this, SLOT(onSignalPairFinished(BluetoothAddressData,int)));
    connect(m_bluetoothAdaptor, SIGNAL(signalMessageSended(int)),
            this, SIGNAL(signalContacterSended(int)));
    connect(m_bluetoothAdaptor, SIGNAL(signalConnectServerFailed(int)),
            this, SIGNAL(signalConnectServerFailed(int)));
    connect(m_bluetoothAdaptor, SIGNAL(signalFileSendPercent(QString,int,int,int)),
            this ,SLOT(onSignalBlueToothFileSendPercent(QString,int,int,int)));

    DownloadContactAdaptor::getInstance();

    m_time = QDateTime(QDate(1970,1,1));
    m_cachePages.clear();

    m_fileShareAdaptor = FileShareAdaptor::getInstance();
    connect(m_fileShareAdaptor, SIGNAL(signalConnectFinished(int)), this, SIGNAL(signalFileConnectFinished(int)));
    connect(m_fileShareAdaptor, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SIGNAL(signalFileSendPercent(QString,qint64,qint64)));
    connect(m_fileShareAdaptor, SIGNAL(signalReceivePercent(QString,qint64,qint64)), this, SIGNAL(signalFileReceivePercent(QString,qint64,qint64)));

    m_contacterShareAdaptor = ContacterShareAdaptor::getInstance();
    connect(m_contacterShareAdaptor, SIGNAL(signalConnectFinished(int)), this, SIGNAL(signalContacterShareConnectNetworkFinished(int)));
    connect(m_contacterShareAdaptor, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SIGNAL(signalContacterShareSendPercent(QString,qint64,qint64)));
//    connect(m_contacterShareAdaptor, SIGNAL(signalContacterReceived(Contacter)), this, SLOT(onSignalContacterShareContacterReceived(Contacter)));
    QObject::connect(m_contacterShareAdaptor, SIGNAL(signalContacterReceived(int,Contacter)),
                     m_adaptor, SLOT(onContacterReceived(int,Contacter)));
    m_dialManager = QT_DTT::DialManager::instance();
    connect(m_dialManager,SIGNAL(PttGettedChanged(bool)),this,SIGNAL(pttGettedChanged(bool)));
}

void GetData::testHistory()
{
    QStringList list;

    //    CallHistory history;
    //    history.dialTarget.append("101");
    //    history.dialNetworkType = PS_NETWORK_VOICE_DIAL; //PS_NETWORK_VOICE_DIAL;
    //    history.dialType = IN_DIAL; //IN_DIAL;
    //    history.dialTime = 89;
    //    history.dialDateStart = QDateTime::currentDateTime();
    //    history.dialDateEnd = QDateTime::currentDateTime();
    //    bool ret = m_historyInterface->addCallHistory(history);
    //    qDebug()<<"add ret = "<<ret;

    list = m_historyInterface->getCallHistoryList();
    qDebug()<<list;
    CallHistory historyy;
    for (int i = 0; i< list.count();i++){
        qDebug()<<"receive CallHistory  start~~~~~~~~~~~~~~~~~~~~";
        historyy = m_historyInterface->getCallHistory(list.at(i));
        qDebug()<<historyy.dialDateStart;
        qDebug()<<historyy.dialDateEnd;
        qDebug()<<historyy.dialHistoryId;
        qDebug()<<historyy.dialNetworkType;
        qDebug()<<historyy.dialTarget;
        qDebug()<<historyy.dialTime;
        qDebug()<<historyy.dialType;
        qDebug()<<"receive CallHistory  end   ~~~~~~~~~~~~~~~~~~~~";
    }
    qDebug()<<" CallHistory CONUT = "<<m_historyInterface->getCallHistoryCount();
    qDebug() << m_historyInterface->getCallHistoryBytarget(tr("101"));
    //    qDebug()<<" CallHistory remove "<< m_historyInterface->removeCallHistory("13");
    //    list = m_historyInterface->getCallHistoryList();
    //    for (int i = 0; i< list.count();i++){
    //        qDebug()<<" list value "<< i << list.at(i);
    //        qDebug()<<" value "<< m_historyInterface->getCallHistory(list.at(i)).dialTarget;
    //    }
    //    QStringList targetList;
    //    QString target = "13333333333";
    //    targetList = m_historyInterface->getCallHistoryBytarget(target);
    //    for (int j = 0;j < targetList.count();j++)
    //        qDebug()<<targetList.at(j);


}

void GetData::onContacterChanged(int type, Contacter index)
{

    qDebug()<<"onContacterChanged"<<index.id<<type;
    QString titleName;
    switch(index.title){
    case BATTLION:
        titleName = "营长";
        break;
    case COMPANY:
        titleName = "连长";
        break;
    case PLATOON:
        titleName = "排长";
        break;
    case SQUAD:
        titleName = "班长";
        break;
    case SOLIDER:
        titleName = "战士";
        break;
    default:
        titleName = "";
        break;
    }
    QString name = index.surname + " " + index.name;
    emit signalContacterChanged(type,index.id,name,titleName,index.namePinyin,QString("%1").arg(index.type));
}

void GetData::onNumberTest(int number,int index)
{
    Q_UNUSED(index);
    qDebug() << "signal test number:" << number;
    if (number == 0){
        emit signalComing();
    }
}

QString GetData::getData()
{
    qDebug()<<"getData!!!"<<"this is a test";
    QDBusReply<QString> reply2 = m_dataInterface->call("onGetAddressBook");
    if(reply2.isValid())
    {
        QString result = reply2.value();
        m_myData = result;
        qDebug() << "sinux test2 result:" << result;
        return m_myData;

    }
    else
    {
        qDebug() << "sinux test2 reply error:" << reply2.error();
    }
    return "test";

}

void GetData::onDigitalStatusChanged(QString handlerId, int status)
{
    qDebug()<<handlerId<<status;
}

void GetData::onPttGettedChanged(bool isGetted)
{
    qDebug()<<"signalPttGettedChanged";
    emit signalPttGettedChanged(isGetted);
}

void GetData::onSignalBlueToothFileSendPercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize)
{
    qDebug() << "onSignalBlueToothFileSendPercent" << size;
    if (dataType == -1)
        emit signalFileSendPercent(fileName, size, maxSize);
}

void GetData::pttSet(bool on)
{
    m_dialManager->baoTongSet(on);
    //m_digitalDialManager->pttSet(on);
}

unsigned GetData::joinGroup(int network, const QString &groupId)
{
    CTS_PRINT << "joinGroup " << network << groupId;
    m_messageAdaptor->joinGroup(network, groupId);
}

int GetData::leaveGroup(int network, const QString &groupId)
{
    qDebug() << "llllllllllllllllllllllllllvvvvvv" << network<<groupId;
    m_messageAdaptor->leaveGroup(network, groupId);
}

int GetData::getPttState(int network, const QString &groupId)
{
    m_messageAdaptor->getPttState(network, groupId);
}

int GetData::setPttState(int network, const QString &groupId, int state)
{
    m_messageAdaptor->setPttState(network, groupId, state);
}

bool GetData::setMessageIsread(const bool &isReadFlag, const QString &id, const QString &groupId)
{
    m_messageAdaptor->setMessageIsread(isReadFlag, id, groupId);
    Message m = m_messageAdaptor->getMessage(id, groupId);
    qDebug() << "setMessageIsread group:" << m.common.id << m.common.isRead;
}

void GetData::setSlect(QString id, int isSlect)
{
    Contacter contacter = m_adaptor->getContacter(id);
    contacter.selectedStatus = isSlect;
    m_adaptor->updateContacter(contacter);
    qDebug()<<"setSlect"<<isSlect;
}

QString GetData::getMessageDate(QDateTime lastTime ,QDateTime dateTime)
{
    int time;
    if (dateTime.toString("dd") != QDateTime::currentDateTime().toString("dd"))
    {
        //年月日是同一天切不是今天只显示一条时间
        if (m_time.toString("yy:MM:dd") == dateTime.toString("yy:MM:dd")){
            m_time = dateTime;
            return "";
        }
        else {
            m_time = dateTime;
            return dateTime.toString("MM月dd日 hh:mm");
        }
    }
    else if (dateTime.toString("yy:MM:dd") == QDateTime::currentDateTime().toString("yy:MM:dd")){
        //今天１０分钟显示一次
        time = dateTime.toTime_t() - lastTime.toTime_t();
        if (time > 600){
            m_time= dateTime;
            return dateTime.toString("hh:mm:ss");
        }
        else
            return "";
    }
}

QString GetData::getSmallUrl(QString url)
{
    return url.section(".", 0, -2) + "___" + "." + url.section(".", -1);
}

QString GetData::getFileNmae(QString url)
{
    return url.section("/", -1);
}

int GetData::fileConnect()
{
    qDebug()<<"fileConnect";

    return m_fileShareAdaptor->connect();
}

int GetData::fileDisConnect()
{
    qDebug()<<"fileDisConnect";

    return m_fileShareAdaptor->disconnect();
}

int GetData::sendFile(QString url)
{
    qDebug()<<"sendFile"<<url;

    return m_fileShareAdaptor->sendFile(url);
}

void GetData::setAddresses(const QStringList &addresses)
{
    qDebug()<<"setAddresses"<<addresses;
    m_fileShareAdaptor->setAddresses(addresses);
}

QString GetData::getAddress(QString userId, QString net)
{
    int netWork;
    QString ip;
    if (!net.isEmpty())
    {
        if (net == "WIFI")
            netWork = WIFI_NETWORK;
        else if (net == "自组网") {
            netWork = AD_HOC_NETWORK;
        } else if (net == "LTE") {
            netWork = LTE_NETWORK;
        } else if (net == "模块数字对讲") {
            netWork = DIGITAL_WALKIE_TALKIE_NETWORK;
        } else if (net == "战互网") {
            netWork = BATTLE_NETWORK;
        }
    }

    if (!userId.isEmpty())
    {
        Contacter myContacter =  m_adaptor->getContacter(userId);
        PhoneNumbers numbers = myContacter.numbers;
        for (int i = 0; i < numbers.count(); i++)
        {
            if (numbers.at(i).netType == netWork)
                ip = numbers.at(i).number;
        }
    }
    qDebug()<<"getAddress"<<userId<<net<<ip;
    return ip;
}

int GetData::getLocalNumCount()
{
    Contacter localContacter = m_adaptor->getLocalInfo();
    qDebug()<< "m_localId" << localContacter.name;
    return localContacter.numbers.count();
}

QString GetData::getLocalNum(int index)
{
    Contacter localContacter = m_adaptor->getLocalInfo();
    PhoneNumbers nums = localContacter.numbers;
    QString first;

    switch (nums.at(index).netType) {
    case AD_HOC_NETWORK:
        first = "自组网";
        break;
    case LTE_NETWORK:
        first = "LTE";
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        first = "模块数字对讲";
        break;
    case BATTLE_NETWORK:
        first = "战互网";
        break;
    case SATELLITE_NETWORK:
        first = "天通网";
        break;
    case WIFI_NETWORK:
        first = "WIFI";
        break;
    default:
        first = "";
        break;
    }
    return first;
}

QString GetData::transtoIp(QString text)
{
    QString ip;
    ip = text.remove("##");
    ip = text.replace(QString("*"), QString("."));
    return ip;
}

void GetData::clearGroupMessage(QString groupId)
{
    m_messageAdaptor->removeMessageGroupData(groupId);
}

void GetData::saveAsFile(const QString &sourceFileName)
{
    QString path = "/home/user/sinux/phonebook/";
    QFile file(path);
    if (!file.exists()){
        createPath(path);
    }
    cpFile(sourceFileName, path);
}

int GetData::getCachePageCount()
{
    return m_cachePages.count();
}

QString GetData::getCachePageValue(int index, QString key)
{
    if (index >= m_cachePages.count())
        return "";
    if (key == "groupname")
        return m_cachePages.at(index).groupName;
    else if (key == "groupId")
        return m_cachePages.at(index).groupId;
    else if (key == "groupnum")
        return m_cachePages.at(index).groupnum;
    else if (key == "netinfo")
        return m_cachePages.at(index).netinfo;
    return "";
}

void GetData::addCachePage(QString groupName, QString groupId, bool bIsPtt, QString groupnum, QString netinfo)
{
    CachePage cachePage;
    cachePage.groupName = groupName;
    cachePage.groupId = groupId;
    cachePage.bIsPtt = bIsPtt;
    cachePage.groupnum = groupnum;
    cachePage.netinfo = netinfo;

    for (int i = 0; i < m_cachePages.count(); i++)
    {
        if (m_cachePages.at(i).groupId == groupId){
            m_cachePages.replace(i, cachePage);
            return;
        }
    }
    m_cachePages.append(cachePage);
    m_groupTabIndex = m_cachePages.count() - 1;
}

void GetData::removeCachePage(QString groupId)
{
    for (int i = 0; i < m_cachePages.count(); i++)
    {
        if (m_cachePages.at(i).groupId == groupId){
            m_cachePages.removeAt(i);
            m_groupTabIndex = m_cachePages.count() - 1;
            break;
        }
    }
}

void GetData::setTabIndex(QString groupId)
{
    for (int i = 0; i < m_cachePages.count(); i++)
    {
        if (m_cachePages.at(i).groupId == groupId){
            m_groupCurrentIndex = i;
            break;
        }
    }
}

int GetData::getTabIndex(QString groupId)
{
    for (int i = 0; i < m_cachePages.count(); i++)
    {
        if (m_cachePages.at(i).groupId == groupId){
            m_groupTabIndex = i;
            break;
        }
    }
    return m_groupTabIndex;
}

int GetData::getTabCurrentIndex()
{
    return m_groupCurrentIndex;
}

QString GetData::digitalDial(QString ip)
{
    //m_dialManager->dial();
    return "debugString";
}

void GetData::hangUp()
{
    int result;
    QDBusReply<int> reply = m_voipInterface->call("onStopDial");
    if (reply.isValid())
    {
        result = reply.value();
        qDebug() << "hangUp result:" << result;
    }
    else
    {
        qDebug() << "hangUp reply error:" << reply.error();
    }
    qDebug()<<"hang up";
}

void GetData::accept()
{
    int result;
    QDBusReply<int> reply = m_voipInterface->call("onAccept");
    if (reply.isValid())
    {
        result = reply.value();
        qDebug() << "accept result:" << result;
    }
    else
    {
        qDebug() << "accept reply error:" << reply.error();
    }
    qDebug()<<"accept";

}

void GetData::decline()
{
    int result;
    QDBusReply<int> reply = m_voipInterface->call("onAccept");
    if (reply.isValid())
    {
        result = reply.value();
        qDebug() << "decline result:" << result;
    }
    else
    {
        qDebug() << "decline reply error:" << reply.error();
    }
    qDebug()<<"decline";

}

QString GetData::getContacterNum()
{
    QStringList contacters = m_adaptor->getContacters(3);
    return QString("%1").arg(contacters.count());
}

void GetData::setView(QQuickView *pview)
{
    m_view = pview;
}

bool GetData::getGroups(bool bIsDelete)
{
    //    m_qmlRootItem = m_view->rootObject();
    //    m_qmlRootObject = qobject_cast<QObject*>(m_qmlRootItem);

    qDebug() << "------------------getGroups---------------------------";
    QStringList groupList;
    if (bIsDelete)
        groupList = m_groupAdaptor->getSelfCreatedGroups(CONVERSION_GROUP);
    else
        groupList = m_groupAdaptor->getGroups(CONVERSION_GROUP);
    qDebug() <<"bIsDelete:"<<bIsDelete<<groupList;
    for(int i = 0, iend = groupList.count(); i < iend; i++) {
        Group group = m_groupAdaptor->getGroup(groupList[i]);
        if (group.id == "") {
            continue;
        }
        qDebug() << "group id:" << group.id;
        qDebug() << "group name:" << group.name;
        qDebug() << "group members:" << group.members.count();
        qDebug() << "group leaders:" << group.leaders.count();
        qDebug() << "group network:" << group.networkType;
        QString num = QString("%1").arg(group.members.count() + group.leaders.count() + 1);
        QString networkType;
        switch (group.networkType) {
        case AD_HOC_NETWORK:
            networkType = "自组网";
            break;
        case LTE_NETWORK:
            networkType = "LTE";
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            networkType = "模块数字对讲";
            break;
        case BATTLE_NETWORK:
            networkType = "战互网";
            break;
        case SATELLITE_NETWORK:
            networkType = "天通网";
            break;
        case ADHOC_LTE_NETWORK:
            networkType = "默认";
            break;
        case WIFI_NETWORK:
            networkType = "WIFI";
            break;
        default:
            networkType = "";
            break;
        }
        emit signalSendGroupInfoToGroup(group.id, group.name, networkType, num , m_messageAdaptor->getSyncCount(group.id));
    }
    /*
        QStringList contacters = m_adaptor->getContacters(type);
        for(int i = 0, iend = contacters.count(); i < iend; i++)
        {
            Contacter contacter = m_adaptor->getContacter(contacters[i]);
            if(contacter.id == "")
            {
                continue;
            }
            QVariant name = contacter.surname + " " + contacter.name;
            QVariant title;
            switch (contacter.title) {
            case BATTLION:
                title = "营长";
                break;
            case COMPANY:
                title = "连长";
                break;
            case PLATOON:
                title = "排长";
                break;
            case SQUAD:
                title = "班长";
                break;
            case SOLIDER:
                title = "战士";
                break;
            default:
                title = "";
                break;
            }
            QVariant size = contacter.namePinyin;
            QVariant userid = contacter.id;
            QVariant contactertype = QString("%1").arg(contacter.type);
            QMetaObject::invokeMethod(m_qmlRootObject, "insertMainList",Q_ARG(QVariant,name),Q_ARG(QVariant,title),Q_ARG(QVariant,size)
                                      ,Q_ARG(QVariant,userid), Q_ARG(QVariant,contactertype));
        }
*/
}

void GetData::onGetGroups(int groupType)
{
    QStringList groupList;
    groupList = m_groupAdaptor->getGroups((GroupType)groupType);
    if (groupList.isEmpty())
    {
        emit signalGroupExchange(2);
        groupList = m_groupAdaptor->getGroups(SUBORDINATE_GROUP);
    }
    else
    {
        emit signalGroupExchange(3);
    }
    for(int i = 0, iend = groupList.count(); i < iend; i++) {
        Group group = m_groupAdaptor->getGroup(groupList[i]);
        if (group.id == "") {
            continue;
        }
        QString num = QString("%1").arg(group.members.count() + group.leaders.count() + 1);
        qDebug() << "onGetGroups" << groupType << group.members;
        QString networkType;
        switch (group.networkType) {
        case AD_HOC_NETWORK:
            networkType = "自组网";
            break;
        case LTE_NETWORK:
            networkType = "LTE";
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            networkType = "模块数字对讲";
            break;
        case BATTLE_NETWORK:
            networkType = "战互网";
            break;
        case SATELLITE_NETWORK:
            networkType = "天通网";
            break;
        case ADHOC_LTE_NETWORK:
            networkType = "默认";
            break;
        case WIFI_NETWORK:
            networkType = "WIFI";
            break;
        default:
            networkType = "";
            break;
        }
        Contacter contacter = m_adaptor->getContacter(group.creator);
        QString nameInfo = contacter.surname + contacter.name;
        emit signalSendOrganizationGroupInfoToGroup(group.id, group.parent,group.name, networkType, num , group.orgType,nameInfo, m_messageAdaptor->getSyncCount(group.id));

        qDebug() << "signalSendOrganizationGroupInfoToGroup" << group.id << group.parent << group.name <<networkType << num << group.orgType <<nameInfo;
        //emit signalSendGroupInfoToGroup(group.id, group.name, networkType, num , m_messageAdaptor->getSyncCount(group.id));
    }

}

void GetData::localPersonInfoInit()
{
    Contacter local;
    local = m_adaptor->getLocalInfo();
    m_localId = local.id;
    m_photoPath = local.photoPath;
    m_name = local.surname + " " + local.name;
    m_title = local.title;
}

quint64 GetData::sendMessage(const int type, const QString &msg , const QString &userId, const QString &groupID)
{
    qDebug()<<"------------------sendMessage start--------------------";
    Message sendMessage;
    sendMessage.contentName = msg;
    sendMessage.common.userId = userId;
    sendMessage.common.groupId = groupID;
    sendMessage.common.date = QDateTime::currentDateTime();
    sendMessage.common.type = type;

    int ret;
    ret = m_messageAdaptor->sendMessage(sendMessage);
    qDebug()<<msg << userId << groupID<<ret;
    qDebug()<<"------------------sendMessage end--------------------";
    return ret;
}

quint64 GetData::sendTextMessage(const QString &msg, const QString &userId, const QString &groupID)
{
    qDebug()<<"------------------sendTextMessage start--------------------";
    int ret;
    ret = m_messageAdaptor->sendTextMessage(groupID,msg);
    qDebug()<<msg << userId << groupID<<ret;
    qDebug()<<"------------------sendTextMessage end--------------------";
    return ret;
}

QString GetData::getLocalId()
{
    return m_localId;
}

int GetData::getLocalTitle()
{
    return m_title;
}

QString GetData::getContacterPhoto(QString userId)
{
    return m_adaptor->getContacter(userId).photoPath;
}

QString GetData::getLocalPhoto()
{
    return m_photoPath;
}

int GetData::getMessageList(const QString &groupId)
{
    QStringList  messageId;
    messageId = m_messageAdaptor->getMessageList(groupId);
    for (int i = messageId.count() - 1;i >= 0; i-- )
    {
        onMessageChanged(MESSAGE_ADD_HANDLE,m_messageAdaptor->getMessage(messageId.at(i),groupId));
    }

}

void GetData::onGetMessageList( const QString &groupId)
{
    m_groupId = groupId;
    QStringList role;
    role << "userId" << "flag" << "messageState" << "fileName" << "text" << "direction" << "messId" << "photoPath" << "isRead"<<"dateTime"<<"index";
    m_messageModel->setRoles(role);
    m_messageModel->setModelList(groupId);
    m_view->rootContext()->setContextProperty("m_messageModel", m_messageModel);

    //    QStringList  messageId;
    //    messageId = m_messageAdaptor->onGetMessageList(startIndex, endIndex, groupId);
    //    for (int i = 0; i < messageId.count(); i++ )
    //    {
    //        onMessageChanged(0,m_messageAdaptor->getMessage(messageId.at(i),groupId));
    //    }
}
QString GetData::getAdhocNetwork(QString id)
{
    Contacter contacter;
    QString first;
    contacter = m_adaptor->getContacter(id);
    for (int i = 0;i<contacter.numbers.count();i++){
        if (contacter.numbers[i].netType = AD_HOC_NETWORK){
            first = contacter.numbers[i].number;
            break;
        }
    }
    return first;
}

QString GetData::getFirstNetwork(QString id)
{
    Contacter contacter;
    QString first;
    contacter = m_adaptor->getContacter(id);
    switch (contacter.numbers[0].netType) {
    case AD_HOC_NETWORK:
        first = "自组网";
        break;
    case LTE_NETWORK:
        first = "LTE";
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        first = "模块数字对讲";
        break;
    case BATTLE_NETWORK:
        first = "战互网";
        break;
    case SATELLITE_NETWORK:
        first = "天通网";
        break;
    case WIFI_NETWORK:
        first = "WIFI";
        break;
    default:
        first = "";
        break;
    }
    qDebug() << first;
    return first;
}

QString GetData::getFirstNum(QString id)
{
    Contacter contacter;
    QString first;
    contacter = m_adaptor->getContacter(id);
    first = contacter.numbers[0].number;
    qDebug() << first;
    return first;
}

QString GetData::getLocalPersonName()
{
    return m_name;
}

void GetData::getContacter(QString id)
{

    bool ok;
    Contacter contacter;
    QStringList qLocalList;
    int dec = id.toInt(&ok,10);
    if (dec == -1) {
        contacter = m_adaptor->getLocalInfo();
    } else {
        contacter = m_adaptor->getContacter(id);
    }
    if(contacter.id.isEmpty())
    {
        qDebug() << "unvalid userid:" << id;
    }
    QString phoneName;
    QString titleName;
    QString name;
    switch (contacter.title) {
    case BATTLION:
        titleName = "营长";
        break;
    case COMPANY:
        titleName = "连长";
        break;
    case PLATOON:
        titleName = "排长";
        break;
    case SQUAD:
        titleName = "班长";
        break;
    case SOLIDER:
        titleName = "战士";
        break;
    default:
        titleName = "";
        break;
    }
    name = contacter.surname + " " + contacter.name;
    if(dec == -1)
    {
        qLocalList.append("职位");
        qLocalList.append(titleName);
        qLocalList.append("编号");
        qLocalList.append(QString("%1").arg(contacter.id));
        emit signalSendNameToPersonInfo(name,contacter.photoPath);
        for(int i = 0; i < qLocalList.count(); i++) {
            if(i%2 == 0)
                emit signalSendLocalInfoToPersonInfo(qLocalList.at(i),qLocalList.at(i+1));
        }
    } else {
        emit signalSendNameToPersonInfo(name,contacter.photoPath);
        emit signalContacterInfo("职位",titleName);
        emit signalContacterInfo("编号",QString("%1").arg(contacter.id));
    }
    for (int i = 0;i < contacter.numbers.count();i++){
        switch (contacter.numbers[i].netType) {
        case LTE_NETWORK:
            phoneName = "LTE";
            break;
        case AD_HOC_NETWORK:
            phoneName = "自组网";
            break;
        case SATELLITE_NETWORK:
            phoneName = "天通网";
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            phoneName = "模块数字对讲";
            break;
        case BATTLE_NETWORK:
            phoneName = "战互网";
            break;
        case WIFI_NETWORK:
            phoneName = "WIFI";
            break;
        case UNKNOW_NETWORK:
        default:
            phoneName = "";
            break;
        }
        emit signalContacterInfo(phoneName,contacter.numbers[i].number);
    }
    if (contacter.type == 1) {
        emit signalContacterInfo("设备序列号",contacter.snNumber);
    }
}

void GetData::updateLocalInfo(QString name, QString title, QString snNum, QString bluetooth)
{
    Contacter contacter = m_adaptor->getLocalInfo();
    contacter.surname = name.split(" ")[0];
    contacter.name = name.split(" ")[1];
    if  ("营长" == title)
        contacter.title = BATTLION;
    else if ("连长" == title)
        contacter.title = COMPANY;
    else if ("排长" == title)
        contacter.title = PLATOON;
    else if ("班长" == title)
        contacter.title = SQUAD;
    else if ("战士" == title)
        contacter.title = SOLIDER;
    else
        contacter.title = UNDEFINE_LEADER;
    contacter.numbers = m_phoneNumbers;
    contacter.snNumber = snNum;
    contacter.bluetooth = bluetooth;
    bool updateFlag = m_adaptor->updateLocalInfo(contacter);
    qDebug() << "updateContacter flag:" << updateFlag;
}

void GetData::updateContacter(QString name, QString title, QString num, QString photopath)
{
    qDebug() << "------------------updateContacter----------------------------";
    Contacter contacter = m_adaptor->getContacter(num);
    contacter.surname = name.split(" ")[0];
    contacter.name = name.split(" ")[1];
    qDebug()<<contacter.surname<<contacter.name<<title;
    if  ("营长" == title)
        contacter.title = BATTLION;
    else if ("连长" == title)
        contacter.title = COMPANY;
    else if ("排长" == title)
        contacter.title = PLATOON;
    else if ("班长" == title)
        contacter.title = SQUAD;
    else if ("战士" == title)
        contacter.title = SOLIDER;
    else
        contacter.title = UNDEFINE_LEADER;
    contacter.id = num;
    contacter.numbers = m_phoneNumbers;
    contacter.photoPath = photopath;
    bool updateFlag = m_adaptor->updateContacter(contacter);
    m_phoneNumbers.clear();
    qDebug() << "updateContacter flag:" << updateFlag;
    qDebug()<<name << title << num;
    qDebug() << "------------------end---------------------------";
}


void GetData::addContacter(QString name,QString title,QString photo)
{
    qDebug() << "------------------addContacter----------------------------";
    qDebug() << photo;
    Contacter addContacter;
    addContacter.surname = name.split(" ")[0];
    addContacter.name = name.split(" ")[1];
    if  ("营长" == title)
        addContacter.title = BATTLION;
    else if ("连长" == title)
        addContacter.title = COMPANY;
    else if ("排长" == title)
        addContacter.title = PLATOON;
    else if ("班长" == title)
        addContacter.title = SQUAD;
    else if ("战士" == title)
        addContacter.title = SOLIDER;
    else
        addContacter.title = UNDEFINE_LEADER;
    addContacter.numbers = m_phoneNumbers;
    addContacter.photoPath = photo;
    //    Leaders leaders;
    //    leaders.append(Leader((int)BATTLION_COMMANDER, 101));
    //    leaders.append(Leader((int)COMPANY_COMMANDER, 102));
    //    addContacter.leaders = leaders;
    bool addFlag = m_adaptor->addContacter(addContacter);
    m_phoneNumbers.clear();
    m_leaders.clear();
    qDebug() << "addContacter flag:" << addFlag;
    qDebug() << "------------------end---------------------------";

}

void GetData::setPhoneList(QString key, QString value)
{
    qDebug()<< "setPhoneList" << key << value;
    PhoneNumber phoneNumber;
    phoneNumber.number = value;
    if (key == "自组网"){
        phoneNumber.netType = AD_HOC_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)AD_HOC_NETWORK, value));
    }else if (key == "LTE"){
        phoneNumber.netType = LTE_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)LTE_NETWORK, value));
    }else if (key == "天通网"){
        phoneNumber.netType = SATELLITE_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)SATELLITE_NETWORK, value));
    } else if (key == "战互网") {
        phoneNumber.netType = BATTLE_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)BATTLE_NETWORK, value));
    } else if (key == "模块数字对讲") {
        phoneNumber.netType = DIGITAL_WALKIE_TALKIE_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)DIGITAL_WALKIE_TALKIE_NETWORK, value));
    } else if (key == "WIFI") {
        phoneNumber.netType = WIFI_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)DIGITAL_WALKIE_TALKIE_NETWORK, value));
    }
    else{
        phoneNumber.netType = UNKNOW_NETWORK;
        //        m_phoneNumbers.append(PhoneNumber((int)UNKNOW_NETWORK, value));
    }
    m_phoneNumbers.append(phoneNumber);
}

bool GetData::getCurrentNetworkIsOnLine(const QString id,const QString net)
{
    NetworkType netType;
    if (net == "自组网"){
        netType = AD_HOC_NETWORK;
    }else if (net == "LTE"){
        netType = LTE_NETWORK;
    }else if (net == "天通网"){
        netType = SATELLITE_NETWORK;
    } else if (net == "战互网") {
        netType = BATTLE_NETWORK;
    } else if (net == "模块数字对讲") {
        netType = DIGITAL_WALKIE_TALKIE_NETWORK;
        return false;
    } else if (net == "WIFI") {
        netType = WIFI_NETWORK;
        return false;
    } else {
        netType = UNKNOW_NETWORK;
    }
    Contacter contacter = m_adaptor->getContacter(id);
    for (int i = 0; i < contacter.numbers.count(); i++) {
        if (contacter.numbers[i].netType == netType) {
            if (contacter.numbers[i].online == 1)
                return true;
            else
                return false;
        }
    }
    return false;
}

bool GetData::removeContacter(const QString id)
{
    qDebug()<<"removeContacter"<<id;
    bool ret = m_adaptor->removeContacter(id);
    return ret;
}

void GetData::onGroupChanged(int type, Group group)
{
    QString id = QString("%1").arg(group.id);
    QString num = QString("%1").arg(group.members.count() + group.leaders.count() + 1);
    QString networkType;
    switch (group.networkType) {
    case AD_HOC_NETWORK:
        networkType = "自组网";
        break;
    case LTE_NETWORK:
        networkType = "LTE";
        break;
    case SATELLITE_NETWORK:
        networkType = "天通网";
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        networkType = "模块数字对讲";
        break;
    case BATTLE_NETWORK:
        networkType = "战互网";
        break;
    case ADHOC_LTE_NETWORK:
        networkType = "默认";
        break;
    case WIFI_NETWORK:
        networkType = "WIFI";
        break;
    default:
        networkType = "";
        break;
    }
    qDebug() << "synccount === " << m_messageAdaptor->getSyncCount(group.id);
    emit signalGroupChanged(type,group.id,group.name,networkType,num, m_messageAdaptor->getSyncCount(group.id));
}

void GetData::onMessageChanged(int messageHandle, Message msg)
{

//    if (msg.common.type == 8)
//        return;

    if (messageHandle == 1 || messageHandle == 2 || messageHandle == 3)
        onGetMessageList(m_groupId);
    emit signalFreshMessage();
    return;
    /* MessageType,MessageState,MessageHandle,messageid
     * type =  MessageHandle
     * 数据变更类型，数据类型，数据状态，数据id
     */
    //flag 0:time  1:text 2:picture 3:files 4:AUDIO_MESSAGE 5:video
    QString value = "";
    int messageType = 0;
    if (msg.common.type == 1){
        value = msg.content.data();
        messageType = 1;
    }
    else if (msg.common.type == 2)
    {
        value = msg.contentName;
        messageType = 2;
    }
    else if (msg.common.type == 4)
    {
        value = msg.contentName;
        messageType = 4;
        qDebug()<<"onMessageChanged~~~~~~~~~~~~~~audio_message~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        return;
    }
    else if (msg.common.type == 5)
    {
        value = msg.contentName;
        messageType = 5;
        qDebug()<<"onMessageChanged~~~~~~~~~~~~~~~video ~~~~~~~~~~~~~~~~~~~~~~~"<<msg.contentName;
    }
    else if (msg.common.type == 3)
    {
        value = msg.contentName;
        messageType = 3;
        qDebug()<<"onMessageChanged~~~~~~~~~~~~~~~file ~~~~~~~~~~~~~~~~~~~~~~~"<<msg.contentName<<msg.senderRealName<<msg.common.state;
    }

    //    if (messageHandle == MESSAGE_ADD_HANDLE)
    //        emit signalMessageChanged(messageHandle, messageType, msg.common.state, value, msg.senderRealName, msg.common.userId, msg.common.id);
    //    else if (messageHandle == 0)
    emit signalMessageChanged(messageHandle, messageType, msg.common.state, value, msg.senderRealName, msg.common.userId, msg.common.id);
}

void GetData::onCallHistoryChanged(QString historyId,int type)
{
    qDebug()<<"onCallHistoryChanged:historyId"<<historyId<<type;
    if (type == 1 || type == 3) {
        emit signalCallHistoryChanged(type,historyId);
    } else {
        emit signalSendDeleteHistoryInfo(type,historyId);
    }

    /*
    QString titleName, time, networkInfo;
    Contacter contacter;
    bool bIsStranger = true;
    if (type == 1 || type == 3) {
        CallHistory callHistory = m_historyInterface->getCallHistory(historyId);
        if (callHistory.dialTarget.count() != 0) {
            bIsStranger = false;
            contacter = m_adaptor->getContacter(callHistory.dialTarget[0]);

            switch (contacter.title) {
            case BATTLION:
                titleName = "营长";
                break;
            case COMPANY:
                titleName = "连长";
                break;
            case PLATOON:
                titleName = "排长";
                break;
            case SQUAD:
                titleName = "班长";
                break;
            case SOLIDER:
                titleName = "战士";
                break;
            default:
                titleName = "";
                break;
            }
        }
        switch (callHistory.dialNetworkType) {
        case PS_NETWORK_VOICE_DIAL:
        case PS_NETWORK_VIDEO_DIAL:
        case PS_NETWORK_VOICE_MUL_DIAL:
            networkInfo = "LTE";
            break;
        case CS_NETWORK_VOICE_DIAL:
        case CS_NETWORK_VOICE_MUL_DIAL:
            networkInfo = "天通网";
            break;
        default:
            break;
        }
        QDateTime nowtime = QDateTime::currentDateTime();
        int timediff = callHistory.dialDateStart.daysTo(nowtime);
        if (timediff == 0) {
            time = callHistory.dialDateStart.toString("hh:mm:ss");
        } else if ((timediff <= 7) && (timediff >= 1)) {
            time = callHistory.dialDateStart.toString("dddd hh:mm");
        } else {
            time = callHistory.dialDateStart.toString("yyyy年M月d日 hh:mm");
        }
        if (bIsStranger) {
            emit signalCallHistoryChanged(type,historyId,"null",callHistory.dialType,
                                          contacter.surname + " " + contacter.name, titleName, callHistory.dialTargetNumber.at(0), time, networkInfo);
        } else {
            emit signalCallHistoryChanged(type,historyId,contacter.id,callHistory.dialType,
                                          contacter.surname + " " + contacter.name, titleName, callHistory.dialTargetNumber.at(0), time, networkInfo);
        }
    } else {
        emit signalSendDeleteHistoryInfo(type,historyId);
    }
 */
}

void GetData::getAddGroupPersonInfo(const int &netType ,QString searchinfo, QString groupId, QStringList list)
{

    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<" << list;
    QStringList role;
    role << "personName" << "position" << "size" << "id"<<"type"<<"isSlect";;
    m_contacterModel->setRoles(role);
    QStringList members;
    if (groupId == "")
        members.clear();
    else {
        Group group = m_groupAdaptor->getGroup(groupId);
        members = group.members + group.leaders;
        members.append(group.creator);
        list.append(group.creator);
    }
    qDebug()<<netType<<searchinfo<<members<<groupId;
    m_contacterModel->setModelList(netType,1,searchinfo,list);
    m_view->rootContext()->setContextProperty("groupPersonModel", m_contacterModel);

    //    QStringList contacters;
    //    if (searchinfo == "") {
    //        contacters  = m_adaptor->getContactersExcept(1,m_members);
    //    } else {
    //        contacters  = m_adaptor->searchContactorsExcept(1, searchinfo, m_members);
    //    }
    //    for (int i = 0; i < contacters.count(); i++) {
    //        Contacter contacter = m_adaptor->getContacter(contacters[i]);
    //        if (contacter.id != 0) {
    //            QString titleName;
    //            switch (contacter.title) {
    //            case BATTLION:
    //                titleName = "营长";
    //                break;
    //            case COMPANY:
    //                titleName = "连长";
    //                break;
    //            case PLATOON:
    //                titleName = "排长";
    //                break;
    //            case SQUAD:
    //                titleName = "班长";
    //                break;
    //            case SOLIDER:
    //                titleName = "战士";
    //                break;
    //            default:
    //                titleName = "";
    //                break;
    //            }
    //            emit signalAddGroupPerson(contacter.id, contacter.surname + " " + contacter.name, titleName, contacter.namePinyin);
    //        }
    //    }
}

void GetData::getGroup(QString id)
{
    Group value;
    Contacter contacter;
    QString titleName;
    QString name;

    value = m_groupAdaptor->getGroup(id);
    /*bIsLeader: creater:1 leader:2 member:3*/
//    if (m_localId != value.creator) {
        contacter = m_adaptor->getContacter(value.creator);
        name = contacter.surname + " " + contacter.name;
        titleName = getTitleStringFromEnum(contacter.title);
        emit signalSendMemberInfoToGroup(contacter.id, "1",name, titleName, QString("%1").arg(contacter.type));
//    }
    qDebug()<<"getGroup leaders"<<value.leaders;
    for (int i = 0; i < value.leaders.count(); i++) {
//        if (m_localId == value.leaders[i]) {
//            continue;
//        } else {
            contacter = m_adaptor->getContacter(value.leaders[i]);
//        }
        name = contacter.surname + " " + contacter.name;
        titleName = getTitleStringFromEnum(contacter.title);
        emit signalSendMemberInfoToGroup(contacter.id, "2",name, titleName, QString("%1").arg(contacter.type));
    }
    qDebug()<<"getGroup members"<<value.members;
    for (int i = 0; i < value.members.count(); i++) {
//        if (m_localId == value.members[i]) {
//            continue;
//        } else {
            contacter = m_adaptor->getContacter(value.members[i]);
//        }
        name = contacter.surname + " " + contacter.name;
        titleName = getTitleStringFromEnum(contacter.title);
        emit signalSendMemberInfoToGroup(contacter.id, "3",name, titleName, QString("%1").arg(contacter.type));
    }
}

void GetData::updateGroup(QString id, QString name)
{
    Group group = m_groupAdaptor->getGroup(id);
    group.name = name;
    group.members = m_members;
    qDebug()<<"updateGroup"<<m_members;
    group.leaders = m_groupLeaders;
    qDebug()<<"updateGroup"<<m_groupLeaders;
    m_groupAdaptor->updateGroup(group);
    m_members.clear();
    m_groupLeaders.clear();
}

void GetData::setGroupMembers(QString id)
{
    m_members.append(id);
    qDebug()<<"setGroupMembers"<<m_members;
}

void GetData::clearGroupMembers(void)
{
    m_members.clear();
}

void GetData::getGroupMembers(QString groupId)
{
    Group group = m_groupAdaptor->getGroup(groupId);
    QStringList members = group.members;
    for (int i = 0; i < members.count(); i++) {
        Contacter contacter = m_adaptor->getContacter(members[i]);
        QString titleName;
        QString name;
        name = contacter.surname + " " + contacter.name;
        switch (contacter.title) {
        case BATTLION:
            titleName = "营长";
            break;
        case COMPANY:
            titleName = "连长";
            break;
        case PLATOON:
            titleName = "排长";
            break;
        case SQUAD:
            titleName = "班长";
            break;
        case SOLIDER:
            titleName = "战士";
            break;
        default:
            titleName = "";
            break;
        }
        emit signalSendGroupMembers(members[i],name,titleName);
    }
}

void GetData::getGroupLeaders(QString groupId)
{
    Group group = m_groupAdaptor->getGroup(groupId);
    QStringList leaders = group.leaders;

    for (int i = 0; i < leaders.count(); i++) {
        Contacter contacter = m_adaptor->getContacter(leaders[i]);
        QString titleName;
        QString name;
        name = contacter.surname + " " + contacter.name;
        switch (contacter.title) {
        case BATTLION:
            titleName = "营长";
            break;
        case COMPANY:
            titleName = "连长";
            break;
        case PLATOON:
            titleName = "排长";
            break;
        case SQUAD:
            titleName = "班长";
            break;
        case SOLIDER:
            titleName = "战士";
            break;
        default:
            titleName = "";
            break;
        }
        emit signalSendGroupLeaders(leaders[i],name,titleName);
    }
}

void GetData::setGroupLeaders(QString id)
{
    m_groupLeaders.append(id);
    qDebug()<<"setGroupLeaders"<<m_groupLeaders;
}

void GetData::clearGroupLeaders(void)
{
    m_groupLeaders.clear();
}

QString GetData::getLocalInGroupIdentify(QString id)
{

    Group value;
    value = m_groupAdaptor->getGroup(id);
    QStringList groupLeaders = value.leaders;
    QStringList members = value.members;
    if (m_localId == value.creator) {
        return "creater";
    }
    for (int i = 0; i < groupLeaders.count(); i ++) {
        if (m_localId == groupLeaders[i]) {
            return "leader";
        }
    }
    for (int i = 0; i < members.count(); i ++) {
        if (m_localId == members[i]) {
            return "member";
        }
    }
    return "noall";
}

void GetData::addGroup(QString name, QString networkType, QString iconPath, int groupType)
{
    Group addGroup;
    int num = rand()%10000;
    addGroup.id = QString("%1").arg(num);
    addGroup.name = name;
    if (networkType == "自组网") {
        addGroup.networkType = AD_HOC_NETWORK;
    } else if (networkType == "LTE") {
        addGroup.networkType = LTE_NETWORK;
    } else if (networkType == "模块数字对讲") {
        addGroup.networkType = DIGITAL_WALKIE_TALKIE_NETWORK;
    } else if (networkType == "默认") {
        addGroup.networkType = ADHOC_LTE_NETWORK;
    } else if (networkType == "WIFI") {
        addGroup.networkType = WIFI_NETWORK;
    } else if (networkType == "战互网") {
        addGroup.networkType = BATTLE_NETWORK;
    } else {
        addGroup.networkType = UNKNOW_NETWORK;
    }
    addGroup.iconPath = iconPath;
    addGroup.members = m_members;
    addGroup.creator = m_localId;
    addGroup.type = groupType;
    addGroup.address = QString("224.0.1.%1").arg(qrand()%255);
    addGroup.port = qrand()%1000 + 2000;
    m_groupAdaptor->addGroup(addGroup);
    m_members.clear();
}

bool GetData::removeGroup(const QString id)
{
    bool ret = m_groupAdaptor->removeGroup(id);
    qDebug() << "removeGroup ret:" << ret;
    return ret;
}

int GetData::getGroupNetType(const QString netinfo)
{
    if (netinfo == "自组网") {
        return AD_HOC_NETWORK;
    } else if (netinfo == "LTE") {
        return LTE_NETWORK;
    } else if (netinfo == "模块数字对讲") {
        return DIGITAL_WALKIE_TALKIE_NETWORK;
    } else if (netinfo == "默认") {
        return ADHOC_LTE_NETWORK;
    } else if (netinfo == "WIFI"){
        return WIFI_NETWORK;
    } else if (netinfo == "战互网"){
        return BATTLE_NETWORK;
    } else {
        return UNKNOW_NETWORK;
    }
    return UNKNOW_NETWORK;
}

void GetData::getCallHistoryList()
{
    QStringList role;
    role <<"id" <<"serialnum" <<"iconValue" <<"theName" <<"info"<<"num"<<"timeinfo"<<"netinfo";
    m_callHistoryModel->setRoles(role);
    m_callHistoryModel->setModelList(0,"");
    m_view->rootContext()->setContextProperty("historyTable",m_callHistoryModel);
}

bool GetData::bIsOrganization(const QString id)
{
    Contacter contacter = m_adaptor->getContacter(id);
    if (contacter.type == 1)
        return true;
    else
        return false;
}

void GetData::getCallHistoryBytarget(const QString userid,bool bIsStranger)
{
    CallHistory history;
    QStringList dialList;
    QString time,networkInfo;
    if (bIsStranger) {
        dialList = m_historyInterface->getCallHistoryByNumber(userid);
    } else {
        dialList = m_historyInterface->getCallHistoryBytarget(userid);
    }
    if (!bIsStranger) {
        Contacter contacter = m_adaptor->getContacter(userid);
        emit signalSendNameToPersonInfo(contacter.surname+ " " + contacter.name,contacter.photoPath);
    }
    for (int i = 0;i<dialList.count();i++)
    {
        history = m_historyInterface->getCallHistory(dialList.at(i));
        QDateTime nowtime = QDateTime::currentDateTime();
        int timediff = history.dialDateStart.daysTo(nowtime);
        if (timediff == 0) {
            time = history.dialDateStart.toString("hh:mm:ss");
        } else if ((timediff <= 7) && (timediff >= 1)) {
            time = history.dialDateStart.toString("dddd hh:mm");
        } else {
            time = history.dialDateStart.toString("yyyy年M月d日 hh:mm");
        }

        switch (history.dialNetworkType) {
        case LTE_NETWORK_VOICE_DIAL:
        case LTE_NETWORK_VIDEO_DIAL:
        case LTE_NETWORK_METTING_DIAL:
            networkInfo = "LTE";
            break;
        case CS_NETWORK_VOICE_DIAL:
        case CS_NETWORK_VOICE_MUL_DIAL:
        case CS_NETWORK_VIDEO_DIAL:
            networkInfo = "天通网";
            break;
        case ADC_NETWORK_VIDEO_DIAL:
        case ADC_NETWORK_VOICE_DIAL:
        case ADC_NETWORK_METTING_DIAL:
            networkInfo = "自组网";
            break;
        case WIFI_NETWORK_VOICE_DIAL:
        case WIFI_NETWORK_VIDEO_DIAL:
        case WIFI_NETWORK_METTING_DIAL:
            networkInfo = "WIFI";
            break;
        default:
            break;
        }
        emit signalSendHistoryInfo(history.dialHistoryId ,history.dialType, history.dialNetworkType,
                                   time,networkInfo,history.dialTargetNumber.at(0),history.dialTime);
    }
}

void GetData::removeCallHistory(const QString historyid, bool bIsStranger)
{
    QStringList dialList;
    if (bIsStranger) {
        dialList = m_historyInterface->getCallHistoryByNumber(historyid);
    } else {
        dialList = m_historyInterface->getCallHistoryBytarget(historyid);
    }
    for (int i = 0; i < dialList.count(); i++) {
        bool flag = m_historyInterface->removeCallHistory(dialList.at(i));
    }
}

void GetData::getSearchContactors(const int &nettype, const int &contacterType, const QString& searchText)
{
    qDebug()<<"getSearchContactors";
    QStringList role;
    //role <<"name" <<"position" <<"size" <<"id" <<"type"<<"personName"<<"isSlect";
    role <<"name" <<"position" <<"size" <<"id" <<"type"<<"isSlect";
    m_contacterModel->setRoles(role);
    m_contacterModel->setModelList(nettype,contacterType,searchText,m_members);
}

void GetData::getSearchContactors2(const int &nettype, const int &contacterType, const QString &searchText)
{
    qDebug()<<"getSearchContactors2";
    QStringList role;
    //role <<"name" <<"position" <<"size" <<"id" <<"type"<<"personName"<<"isSlect";
    role <<"name" <<"position" <<"size" <<"id" <<"type"<<"isSlect";
    m_contacterModel->setRoles(role);
    m_contacterModel->setModelList(nettype,contacterType,searchText);
}

void GetData::openblueToothdevice()
{
    bool flag = m_bluetoothAdaptor->openBluetooth();
    QThread::sleep(1);
    qDebug() << "openblueToothdevice" << flag;
}

void GetData::findblueToothDevice()
{
    bool flag = m_bluetoothAdaptor->startFindRemoteBluetooth();
    qDebug() << "findblueToothDevice" << flag;
}

void GetData::onSignalDeviceDiscovered(const BluetoothAddressData &data)
{
    qDebug() << "onSignalDeviceDiscovered" << data.address;
    QString contactId = m_adaptor->searchContacterBluetooth(data.address);
    Contacter ct;
    ct.surname = tr("未知");
    QString title = tr("");
    if(!contactId.isEmpty())
    {
        ct = m_adaptor->getContacter(contactId);
        title = getTitleStringFromEnum(ct.title);
    }

    emit signalSendBlueToothInfo(data.name, data.address, title, ct.surname + ct.name);
}

void GetData::onSignalPairConfirmation(const BluetoothAddressData &address, QString pin)
{
    qDebug() << "GetData::onSignalPairConfirmation";
    emit signalPairConfirmation(address.name, address.address, pin);
}

void GetData::onSignalPairFinished(const BluetoothAddressData &address, int pairing)
{
    qDebug() << "send signalPareFinished(address.name, address,address, pairing)";
    emit signalPairFinished(address.name, address.address, pairing);
}

void GetData::onSignalContacterReceived(const int &handleType, const Contacter &contacter)
{
    qDebug() << "GetData::onSignalContacterReceived";
    emit signalContacterReceived(handleType);
}

void GetData::onSignalConnectFinished(const BluetoothAddressData &address)
{
    qDebug() << "GetData::onSignalConnectFinished";
    emit signalConnectServerFinished(address.name, address.address);
}

void GetData::onMessageDownload(const Message &msg)
{
    onMessageChanged(MESSAGE_UPDATE_HANDLE,msg);
}

void GetData::getSearchCallHistory(int type, const QString &searchText)
{
    QStringList role;
    role <<"id" <<"serialnum" <<"iconValue" <<"theName" <<"info"<<"num"<<"timeinfo"<<"netinfo";
    m_callHistoryModel->setRoles(role);
    m_callHistoryModel->setModelList(type, searchText);
}


void GetData::getMessageDownload(const QString &id, const QString &groupId)
{
    m_messageAdaptor->getMessageDownload(id, groupId);
}

bool GetData::getContacterLeader(QString userId)
{
    return true;
}

void GetData::getLocalDeviceInfo()
{
    emit signalSendLocalDeviceInfo(
                m_bluetoothAdaptor->getLocalDeviceInfo().name,
                getTitleStringFromTitleType(m_adaptor->getLocalInfo().title),
                m_adaptor->getLocalInfo().surname + m_adaptor->getLocalInfo().name);
}

QString GetData::getTitleStringFromTitleType(int titleType)
{
    QString titleName;

    switch (titleType)
    {
    case BATTLION:
        titleName = "营长";
        break;
    case COMPANY:
        titleName = "连长";
        break;
    case PLATOON:
        titleName = "排长";
        break;
    case SQUAD:
        titleName = "班长";
        break;
    case SOLIDER:
        titleName = "战士";
        break;
    default:
        titleName = "";
        break;
    }

    return titleName;
}

void GetData::getCurrentNetworkInfo(const QString net)
{
    emit signalSendNetworkInfo(net);
}

void GetData::startPairing(const QString &name, const QString &address)
{
    BluetoothAddressData bluetoothData;
    bluetoothData.name = name;
    bluetoothData.address = address;

    m_bluetoothAdaptor->pairing(bluetoothData);
}

void GetData::pairingConfirmation(bool confirm)
{
    m_bluetoothAdaptor->pairingConfirmation(confirm);
}

//connect device
bool GetData::connectDevice(const QString &name, const QString &address)
{
    BluetoothAddressData bluetoothData;
    bluetoothData.name = name;
    bluetoothData.address = address;

    return m_bluetoothAdaptor->connect(bluetoothData);
}

//send contact
bool GetData::sendContact(QString id)
{
    Contacter con = m_adaptor->getContacter(id, true);
    qDebug() << con.id << con.name << con.photoData.isEmpty();
    if (m_bluetoothAdaptor->sendContacter(con) >= 0)
        return true;
    else
        return false;
    //    m_bluetoothAdaptor->sendFile("/home/user/myimage.jpg");
}

bool GetData::isBlutoothOpened()
{
    return m_bluetoothAdaptor->isBluetoothOpened();
}

void GetData::getImageMessageList()
{
    m_messageModel->getImageList();
}

void GetData::clearSyncCount(const QString &groupId)
{
    qDebug() << "clearSyncCount in app" << groupId;
    m_messageAdaptor->clearSyncCount(groupId);
}

int GetData::getSyncCount(const QString &groupId)
{
    return m_messageAdaptor->getSyncCount(groupId);
}

int GetData::getMessageCount()
{
    return m_messageModel->getCount();
}

bool GetData::isFileExists(QString filePath)
{
    filePath = filePath.mid(7,filePath.count()-7);
    QFile file(filePath);
    return file.exists();
}

int GetData::getCallHistoryNet(const QString id)
{
    return m_historyInterface->getCallHistory(id).dialNetworkType;
}

void GetData::disconnectDevice()
{
    m_bluetoothAdaptor->disconnect();
}


void GetData::onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "GetData::onSignalSendPercent";
    emit signalSendPercent(groupId, id, recvNum, allNum);
}

void GetData::onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum)
{
    qDebug() << "GetData::onSignalReceivePercent" << recvNum << allNum;
    emit signalRecvPercent(groupId, id, recvNum, allNum);
}

int GetData::contacterShareStartConnectNetwork(const QString &contacterId)
{
    Contacter con = m_adaptor->getContacter(contacterId, true);
    QStringList ipList;

    foreach(const PhoneNumber &number, con.numbers)
    {
        qDebug() << "number " << contacterId << con.id  << number.number;
        ipList.push_back(number.number);
        break;
    }
    m_contacterShareAdaptor->setAddresses(ipList);
    qDebug() << "GetData::contacterShareStartConnectNetwork" << ipList;

    m_contacterShareAdaptor->connect();
}

int GetData::contacterShareDisconnectNetwork()
{
    m_contacterShareAdaptor->disconnect();
    qDebug() << "GetData::contacterShareDisconnectNetwork";
}

int GetData::contacterShareStartShare(const QString &contacterId)
{
    qDebug() << "GetData::contacterShareStartShare";
    Contacter con = m_adaptor->getContacter(contacterId, true);
    qDebug() << con.id << con.name << con.photoData.isEmpty();
    m_contacterShareAdaptor->sendContacter(con);
}


void GetData::onSignalContacterShareContacterReceived(const Contacter &contacter)
{
    qDebug() << "GetData::onSignalContacterShareContacterReceived" << contacter.surname << contacter.name;

}

int GetData::intToNetworkType(int val)
{
    int network = UNKNOW_NETWORK;
    switch (val) {
    case 0:
        network = AD_HOC_NETWORK;
        break;
    case 1:
        network = LTE_NETWORK;
        break;
    case 2:
        network = SATELLITE_NETWORK;
        break;
    case 3:
        network = DIGITAL_WALKIE_TALKIE_NETWORK;
        break;
    case 4:
        network = BATTLE_NETWORK;
        break;
    case 5:
        network = WIFI_NETWORK;
        break;
    case 6:
        network = ADHOC_LTE_NETWORK;
        break;
    case 7:
        network = UNKNOW_NETWORK;
        break;
    default:
        break;
    }
    return network;
}

quint64 GetData::resendMessage(const QString &msgId, QString groupId)
{
    m_messageAdaptor->resendMessage(m_messageAdaptor->getMessage(msgId, groupId));
}

void GetData::baoTongDial(QString phoneNum)
{
    m_dialManager->baoTongDial(phoneNum);
}

void GetData::baoTongHangUp()
{
    m_dialManager->baoTongHangUp();
}

void GetData::baoTongSet(bool on)
{
    m_dialManager->baoTongSet(on);
}

bool GetData::baoTongGet()
{
    return m_dialManager->baoTongGet();
}

bool GetData::baoTongPttAllow()
{
    return m_dialManager->baoTongPttAllow();
}

void GetData::joinBtGroup(const QString &groupId)
{
    m_messageAdaptor->joinBtGroup(groupId);
}

void GetData::setBtPttState(const QString &groupId, int state)
{
    m_messageAdaptor->setBtPttState(groupId,state);
}

bool GetData::getBtPttAllow(const QString &groupId)
{
    return m_messageAdaptor->getBtPttAllow(groupId);
}

void GetData::sendBlueToothFile(QString fileName)
{
    m_bluetoothAdaptor->sendFile(fileName);
}

void GetData::onInGroupUi(const QString &groupId)
{
    m_messageAdaptor->onInGroupUi(groupId);
}

void GetData::onOutGroupUi(const QString &groupId)
{
    m_messageAdaptor->onOutGroupUi(groupId);
}

QString GetData::sortPhoneNumbers(const QString &userId, QString key)
{
    QString networkType;
    if (key == "value")
        return sortPhoneNumbersByPriority(m_adaptor->getContacter(userId).numbers).at(0).number;
    else if (key == "net")
    {
        switch (sortPhoneNumbersByPriority(m_adaptor->getContacter(userId).numbers).at(0).netType) {
        case AD_HOC_NETWORK:
            networkType = "自组网";
            break;
        case LTE_NETWORK:
            networkType = "LTE";
            break;
        case SATELLITE_NETWORK:
            networkType = "天通网";
            break;
//        case DIGITAL_WALKIE_TALKIE_NETWORK:
//            networkType = "模块数字对讲";
//            break;
        case BATTLE_NETWORK:
            networkType = "战互网";
            break;
//        case ADHOC_LTE_NETWORK:
//            networkType = "默认";
//            break;
        case WIFI_NETWORK:
            networkType = "WIFI";
            break;
        default:
            networkType = "";
            break;
        }
        return networkType;
    }
}

QStringList GetData::getGroupValue(QString groupId)
{
    QStringList list;
    Group group = m_groupAdaptor->getGroup(groupId);
    if (group.id.isEmpty())
        return list;

    QString networkType;
    switch (group.networkType) {
    case AD_HOC_NETWORK:
        networkType = "自组网";
        break;
    case LTE_NETWORK:
        networkType = "LTE";
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        networkType = "模块数字对讲";
        break;
    case BATTLE_NETWORK:
        networkType = "战互网";
        break;
    case SATELLITE_NETWORK:
        networkType = "天通网";
        break;
    case ADHOC_LTE_NETWORK:
        networkType = "默认";
        break;
    case WIFI_NETWORK:
        networkType = "WIFI";
        break;
    default:
        networkType = "";
        break;
    }

    list.append(group.name);
    list.append(group.id);
    list.append("false");
    list.append(QString("%1").arg(group.members.count() + group.leaders.count() + 1));
    list.append(networkType);
    return list;
}

QStringList GetData::getLocation(QString msgId, QString groupId)
{
    QStringList list;
    Message msg = m_messageAdaptor->getMessage(msgId,groupId);
    if (!msg.common.id.isEmpty())
    {
        list.append(msg.longitude);
        list.append(msg.latitude);
        list.append(msg.height);
    }
    return list;
}

void GetData::emitGetPosition(int type, QString groupid)
{
    m_messageAdaptor->onGetPosition(type,groupid);
}

int GetData::onCallP2P(int network, const QString &groupId)
{
    return m_messageAdaptor->onCallP2P(network, groupId);
}

int GetData::onHangupP2P(int network, const QString &groupId)
{
    return m_messageAdaptor->onHangupP2P(network, groupId);
}

int GetData::onGetP2PState(int network, const QString &groupId)
{
    return m_messageAdaptor->onGetP2PState(network, groupId);
}

int GetData::onSetP2PState(int network, const QString &groupId, int state)
{
    return m_messageAdaptor->onSetP2PState(network, groupId,state);
}

void GetData::onSetPropertyType()
{
    onSetPropertyValue(1);
    QTimer::singleShot(20000, this, SLOT(onSetProperty()));
}

void GetData::onSetTts(QString groupId, bool on)
{
    m_messageAdaptor->onSetTts(groupId, on);
}

bool GetData::onGetTts(QString groupId)
{
    return m_messageAdaptor->onGetTts(groupId);
}

void GetData::onSetProperty()
{
    onSetPropertyValue(99);
}

void GetData::onSetPropertyValue(int type)
{


    if (type == 1)
    {
        QString value = "SetSosMode";
        u_int32_t ret = 1;
        QDBusReply<void> historyReply = m_sosInterface->call("SetProperty", value, QVariant::fromValue(QDBusVariant(ret)));
        if(historyReply.isValid())
            qDebug() << "onSetPropertyValue reply succ:";
        else
            qDebug() << "onSetPropertyValue reply error:" << historyReply.error();
    }
    else
    {
        QString value = "SendSosMsg";
        u_int32_t ret = 99;
        QDBusReply<void> historyReply = m_sosInterface->call("SetProperty", value, QVariant::fromValue(QDBusVariant(ret)));
        if(historyReply.isValid()){
            emit signalSosReturn(1);
            qDebug() << "SendSosMsg reply succ:";
        }
        else {
            emit signalSosReturn(0);
            qDebug() << "SendSosMsg reply error:" << historyReply.error();
        }
    }
}

void GetData::onSignalGroupExchange()
{
    onGetGroups(3);
}
