#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QDBusMetaType>
#include <QThread>
#include "groupinterface.h"
#include "connectiondatabase.h"
#include "adaptor/groupmessagesenderadaptorbt.h"
#include "adaptor/adhocadaptor.h"
#include "adaptor/p2pmessagesendertcpadaptor.h"

QString account;

GroupInterface *GroupInterface::m_instance = NULL;
GroupInterface::GroupInterface(QObject *parent)
    : QObject(parent),
      m_groupModel(0, ConnectionDatabase::getGblDb())
{
    //init contacter data
    m_groupModel.setTable("groups");
    m_groupModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_groupModel.select();

    //
    m_contacterInterface = ContacterInterface::getInstance();
    m_groupShareAdaptor = GroupShareAdaptor::getInstance();
    connect(m_groupShareAdaptor, SIGNAL(signalGroupReceived(Group)), this, SLOT(onGroupChangeToChangeGroupPeople(Group)));
    connect(m_groupShareAdaptor, SIGNAL(signalConnectFinished(int)), this, SLOT(onSignalGroupShareConnected(int)));
    connect(m_groupShareAdaptor, SIGNAL(signalSendPercent(QString,qint64,qint64)), this, SLOT(onSignalSendPercent(QString,qint64,qint64)));

    //timer
    m_timer.setSingleShot(true);
    m_timer.setInterval(500);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(groupChangeToChangeGroupPeople()));

    m_downloadAdaptor = DownloadContactAdaptor::getInstance();

//    QString ipEnd = ConnectionDatabase::getIp().section(".", -1);
//    if(ipEnd == "236")
//        account = "100110002";
//    else if(ipEnd == "237")
//        account = "100110003";
//    else if(ipEnd == "238")
//        account = "100110004";

    GroupMessageSenderAdaptorLte *lteInstance = GroupMessageSenderAdaptorLte::getInstance();
    Contacter localContacter = m_contacterInterface->getLocalInfo();
    QString lte_server_ip = ConnectionDatabase::getSetting("SETTING/LTE_SERVER_IP");
    lteInstance->login(localContacter.pocId, "123456", LTE_NETWORK, lte_server_ip, 4001);
    connect(GroupMessageSenderAdaptorLte::getInstance(), SIGNAL(recvGroupFromRemote(Group,int)),
            this, SLOT(onLteGroupReceived(Group,int)));
    connect(GroupMessageSenderAdaptorLte::getInstance(), SIGNAL(signalGroupChangedFinished(Group)),
            this, SLOT(onSignalGroupChangedFinished(Group)));

    connect(AdHocAdaptor::getInstance(), SIGNAL(signalRecvGroupFromRemote(Group,int)),
            this, SLOT(onLteGroupReceived(Group,int)));
}

GroupInterface *GroupInterface::getInstance()
{
    if(m_instance == NULL)
        m_instance = new GroupInterface();

    return m_instance;
}

Group GroupInterface::getGroup(const QString &id)
{
    return getGroupFromRowData(m_groupModel.findDataInColumn("id", id));
}

GroupList GroupInterface::getGroupList()
{
    GroupList groupList;
    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex idIndex = m_groupModel.index(i, 0);
        QString id = m_groupModel.data(idIndex).toString();

        groupList.push_back(getGroup(id));
    }

    return groupList;
}

Group GroupInterface::findGroupByName(const QString &groupName)
{
    Group group;

    int row = m_groupModel.findDataInColumn(1, groupName);
    if(row != -1)
    {
        group = getGroupFromRowData(m_groupModel.rowData(row));
    }

    return group;
}

bool GroupInterface::removeGroupById(const QString &id)
{
    return removeGroup(id);
}

QStringList GroupInterface::getGroupAddrsByType(const QString &groupId,
                                                int networkType,
                                                bool includeCreator)
{
    Group groupData = GroupInterface::getInstance()->getGroup(groupId);

    QStringList ipList;
    QStringList cIdList;
    if(includeCreator)
        cIdList << groupData.creator;
    cIdList  << groupData.leaders << groupData.members; //<< groupData.creator
    foreach(const QString &id, cIdList)
    {
        Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

        foreach(const PhoneNumber &number, contacter.numbers)
        {
            if(number.netType == networkType)
            {
                qDebug() << "number networkType" << networkType << id << contacter.id  << number.number;
                ipList.push_back(number.number);
            }
        }
    }

    return ipList;
}

bool GroupInterface::addGroup(const Group &group)
{
    if(group.id.isEmpty())
        return false;
    if(group.name.isEmpty())
        return false;
    if(checkGroupExist(group.id))
        return false;

    Group aGroup = group;

    if(aGroup.networkType == DIGITAL_WALKIE_TALKIE_NETWORK)
    {
        QStringList addrs = getBtGroupAddrs();
        for(int i=0; i<10; i++)
        {
            int address = 26901 + i;
            QString addr = QString::number(address);
            if(!addrs.contains(addr))
            {
                aGroup.address = addr;
                qDebug() << "add baotong group address: " << addr;
                break;
            }
        }
    }

    int rowCount = m_groupModel.rowCount();
    bool insertFlag = m_groupModel.insertRow(rowCount);
    if(!insertFlag)
        return false;
    bool setFlag = setGroup(rowCount, aGroup);
    if(!setFlag)
        return false;

    m_groupModel.submitAll();
    emitChangeSignal(GROUP_ADD_HANDLE, aGroup);
    return true;
}

bool GroupInterface::addGroup2(const Group &group)
{
    int rowCount = m_groupModel.rowCount();
    bool insertFlag = m_groupModel.insertRow(rowCount);
    if(!insertFlag)
        return false;
    bool setFlag = setGroup(rowCount, group);
    if(!setFlag)
        return false;

    return true;
}

bool GroupInterface::removeGroup(const QString &id)
{
    if(id.isEmpty())
        return false;

    int row = m_groupModel.findDataInColumn(0, id);
    if(row != -1){
        Group changeGroup = getGroup(row);
        if(changeGroup.type != CONVERSION_GROUP)
            return false;

        m_groupModel.removeRow(row);
        m_groupModel.submitAll();
        emitChangeSignal(GROUP_DELETE_HANDLE, changeGroup);

        return true;
    }
    return false;
}

bool GroupInterface::removeGroups()
{
    SqlFunction::doSql("DELETE FROM groups");
    m_groupModel.select();
}

bool GroupInterface::updateGroup(const Group &group)
{
    if(group.id == 0)
        return false;
    if(group.name.isEmpty())
        return false;
    if(!checkGroupExist(group.id))
        return false;

    int row = m_groupModel.findDataInColumn(0, group.id);
    if(row != -1){
        setGroup(row, group);
        Group group = getGroup(row);
        m_groupModel.submitAll();
        emitChangeSignal(GROUP_UPDATE_HANDLE, group);
        return true;
    }
    return false;
}

void GroupInterface::addGroupNetwork(const Group &group)
{
    if(group.networkType == LTE_NETWORK)
    {
        QT_DTT_JJF::GroupInfo info;
        info.id = group.id;
        info.addr = group.address;
        info.network = group.networkType;
        info.creator = group.creator;
        info.name = group.name;
        info.priority = 20;


        QStringList cIdList;
        cIdList << group.creator  << group.members; //QStringList("236")//<< groupData.creator
        qDebug() << "addGroupNetwork: " << cIdList;
        QList<QT_DTT_JJF::UserInfo> userInfoList;
        for(int i=0; i<cIdList.count(); i++)
        {
            const QString &id = cIdList.value(i);
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

            QT_DTT_JJF::UserInfo userInfo;
            userInfo.userId = contacter.id.toInt();
            userInfo.network = group.networkType;
            userInfo.addr = getPhoneNumberType(contacter.numbers, group.networkType).number;
            userInfo.userName = contacter.surname+contacter.name;
            userInfo.lte_ip = getPhoneNumberType(contacter.numbers, group.networkType).number;

            userInfo.lte_pocId = contacter.pocId;
            userInfoList.push_back(userInfo);
        }

        info.baseMembers = userInfoList;
        info.memberSize = userInfoList.count();

        GroupMessageSenderAdaptorLte::getInstance()->createGroup(info);
    }
    else if(group.networkType == AD_HOC_NETWORK)
    {
        AdHocAdaptor *adhocAdaptor = AdHocAdaptor::getInstance();

        QT_DTT_ADHOC::GroupInfo info;
        info.id = group.id;

        info.addr = group.address;//43-96
        info.network = group.networkType;
        info.creator = group.creator;
        info.name = group.name;
        info.priority = 20;
        info.reserve = QString::number(group.port);


        QStringList cIdList;
        cIdList << group.creator  << group.members; //QStringList("236")//<< groupData.creator
        qDebug() << "addGroupNetwork: " << cIdList;
        QList<QT_DTT_ADHOC::UserInfo> userInfoList;
        for(int i=0; i<cIdList.count(); i++)
        {
            const QString &id = cIdList.value(i);
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

            QT_DTT_ADHOC::UserInfo userInfo;
            userInfo.userId = contacter.id.toInt();
            userInfo.network = group.networkType;
            userInfo.addr = getPhoneNumberType(contacter.numbers, group.networkType).number;
            userInfo.userName = contacter.surname+contacter.name;
            userInfo.ad_hoc_ip = getPhoneNumberType(contacter.numbers, group.networkType).number;

            userInfoList.push_back(userInfo);
        }

        info.baseMembers = userInfoList;
        info.memberSize = userInfoList.count();

        adhocAdaptor->createGroupInfo(info);
    }
}

void GroupInterface::removeGroupNetwork(const Group &group)
{
    if(group.networkType == LTE_NETWORK)
    {
        QT_DTT_JJF::GroupInfo info;
        info.id = group.id;
        info.addr = group.address;
        info.network = group.networkType;
        info.creator = group.creator;
        info.name = group.name;
        info.priority = 20;


        QStringList cIdList;
        cIdList << group.creator  << group.members; //QStringList("236")//<< groupData.creator
        qDebug() << "addGroupNetwork: " << cIdList;
        QList<QT_DTT_JJF::UserInfo> userInfoList;
        for(int i=0; i<cIdList.count(); i++)
        {
            const QString &id = cIdList.value(i);
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

            QT_DTT_JJF::UserInfo userInfo;
            userInfo.userId = contacter.id.toInt();
            userInfo.network = group.networkType;
            userInfo.addr = getPhoneNumberType(contacter.numbers, group.networkType).number;
            userInfo.userName = contacter.surname+contacter.name;
            userInfo.lte_ip = getPhoneNumberType(contacter.numbers, group.networkType).number;

            userInfo.lte_pocId = contacter.pocId;
            userInfoList.push_back(userInfo);
        }

        info.baseMembers = userInfoList;
        info.memberSize = userInfoList.count();

        GroupMessageSenderAdaptorLte::getInstance()->removeGroup(info);
    }
    else if(group.networkType == AD_HOC_NETWORK)
    {
        AdHocAdaptor *adhocAdaptor = AdHocAdaptor::getInstance();

        QT_DTT_ADHOC::GroupInfo info;
        info.id = group.id;

        info.addr = group.address;//43-96
        info.network = group.networkType;
        info.creator = group.creator;
        info.name = group.name;
        info.priority = 20;
        info.reserve = QString::number(group.port);


        QStringList cIdList;
        cIdList << group.creator  << group.members; //QStringList("236")//<< groupData.creator
        qDebug() << "addGroupNetwork: " << cIdList;
        QList<QT_DTT_ADHOC::UserInfo> userInfoList;
        for(int i=0; i<cIdList.count(); i++)
        {
            const QString &id = cIdList.value(i);
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

            QT_DTT_ADHOC::UserInfo userInfo;
            userInfo.userId = contacter.id.toInt();
            userInfo.network = group.networkType;
            userInfo.addr = getPhoneNumberType(contacter.numbers, group.networkType).number;
            userInfo.userName = contacter.surname+contacter.name;
            userInfo.ad_hoc_ip = getPhoneNumberType(contacter.numbers, group.networkType).number;

            userInfoList.push_back(userInfo);
        }

        info.baseMembers = userInfoList;
        info.memberSize = userInfoList.count();

        adhocAdaptor->removeGroupInfo(info);
    }
    else if(group.networkType == DIGITAL_WALKIE_TALKIE_NETWORK)
    {
        GroupMessageSenderAdaptorBt *btSender = GroupMessageSenderAdaptorBt::getInstance();
        QStringList btAddrList = getBtGroupAddrs();
        if(btAddrList.contains(group.address))
            return ;

        int index = btSender->findPttGroupIndex(group.address);
        btSender->removeGroup(index);
    }
}

void GroupInterface::updateGroupNetwork(const Group &group)
{

}

bool GroupInterface::onAddGroup(const Group &group)
{
    CTS_PRINT << "test add";
    Group groupAdded = group;

    //find useable addr and port-----------------------------
    QStringList addrList;
    QList<int> portList;

    GroupList gList = getGroupList();
    for(int i=0; i<gList.size(); i++)
    {
        Group group = gList.value(i);
        addrList.push_back(group.address);
        portList.push_back(group.port);
    }

    int num = 0, num2 = 57, port = 10101;
    for(int i=0; i<500; i++)
    {
        QString addr = QString("239.0.%1.%2").arg(num).arg(num2);
        if(!addrList.contains(addr) && !portList.contains(port))
        {
            groupAdded.address = addr;
            groupAdded.port = port;
            break;
        }
        else
        {
            num2 ++;
            if(num2 > 255)
            {
                num2 = 0;
                num ++;
            }

            port ++;
        }
    }
    //find useable addr and port----------------------------
    qDebug() << "group address: " << groupAdded.address
             << "group port: " << groupAdded.port;
    groupAdded.creator = m_contacterInterface->getLocalInfo().id;
    bool flag = addGroup(groupAdded);
    if(groupAdded.type == BATTLE_GROUP)
    {
        emit signalGroupExchange();
    }
    if(flag)
    {
        addGroupNetwork(groupAdded);

        GroupList groups;
        groups.append(groupAdded);
        emit signalGroupChangedToChangeRemoteGroup(GROUP_ADD_HANDLE, groups);

        Group sendGroup = group;
        sendGroup.handle = GROUP_ADD_HANDLE;
        if(group.networkType == WIFI_NETWORK)
        {
            groupAdded.handle = GROUP_ADD_HANDLE;
            groupChangeToChangeGroupPeopleAddQueue(groupAdded);
        }
//        if(sendGroup.type == BATTLE_GROUP)
//            m_downloadAdaptor->onPostGroupBattle(sendGroup);
//        else
//            m_downloadAdaptor->onPostGroup(sendGroup);


    }
    return flag;
}

bool GroupInterface::onRemoveGroup(const QString &id)
{
    Group group = getGroup(id);
    bool flag = removeGroup(id);
    if(flag)
    {
        GroupList groups;
        groups.append(group);
        emit signalGroupChangedToChangeRemoteGroup(GROUP_DELETE_HANDLE, groups);

        group.handle = GROUP_DELETE_HANDLE;
        if(group.networkType == WIFI_NETWORK)
            groupChangeToChangeGroupPeopleAddQueue(group);
    }
    return flag;
}

bool GroupInterface::onUpdateGroup(const Group &group)
{
    bool flag = updateGroup(group);
    if(flag)
    {
        GroupList groups;
        groups.append(group);
        emit signalGroupChangedToChangeRemoteGroup(GROUP_UPDATE_HANDLE, groups);

        Group sendGroup = group;
        sendGroup.handle = GROUP_UPDATE_HANDLE;
        if(group.networkType == WIFI_NETWORK)
            groupChangeToChangeGroupPeopleAddQueue(sendGroup);
//        if(sendGroup.type == BATTLE_GROUP)
//            m_downloadAdaptor->onPostGroupBattle(sendGroup);
//        else
//            m_downloadAdaptor->onPostGroup(sendGroup);
    }
    return flag;
}

QDBusVariant GroupInterface::onGetGroup(const QString &id)
{
    if(id.isEmpty())
        return QDBusVariant();

    Group group = getGroup(id);
    QDBusVariant dbusValue;
    QVariant groupVar = QVariant::fromValue(group);
    dbusValue.setVariant(groupVar);
    return dbusValue;
}

QStringList GroupInterface::onGetGroups(const int &type)
{
    QStringList ids;
    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex idIndex = m_groupModel.index(i, 0);
        QModelIndex typeIndex = m_groupModel.index(i, 9);
        QString id = m_groupModel.data(idIndex).toString();
        QString typeString = m_groupModel.data(typeIndex).toString();
        GroupType groupType = convertStringToType(typeString);
        if(groupType == type)
            ids.append(id);
    }

    return ids;
}

QStringList GroupInterface::onGetSelfCreatedGroups(const int &type)
{
    QStringList ids;
    QString localId = m_contacterInterface->getLocalInfo().id;
    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex idIndex = m_groupModel.index(i, 0);
        QModelIndex typeIndex = m_groupModel.index(i, 9);
        QModelIndex creatorIndex = m_groupModel.index(i, 3);
        QString id = m_groupModel.data(idIndex).toString();
        QString typeString = m_groupModel.data(typeIndex).toString();
        QString creatorString = m_groupModel.data(creatorIndex).toString();
        GroupType groupType = convertStringToType(typeString);
        if(groupType == type/* && localId == creatorString*/)
            ids.append(id);
    }
    qDebug() << "onGetSelfCreatedGroups:" << ids;
    return ids;
}

bool GroupInterface::checkGroupExist(const QString &id)
{
    int row = m_groupModel.findDataInColumn(0, id);
    if(row != -1)
        return true;
    return false;
//    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
//    {
//        QModelIndex index = m_groupModel.index(i, 0);
//        if(m_groupModel.data(index).toString() == id)
//            return true;
//    }
//    return false;
}

Group GroupInterface::getGroup(const int &row)
{
    return getGroupFromRowData(m_groupModel.rowData(row));
}

Group GroupInterface::getGroupFromRowData(const QStringList &rowData)
{
    if(rowData.isEmpty())
        return Group();

    Group group;
    group.id = rowData.value(0);

    group.name = rowData.value(1);

    group.iconPath = rowData.value(2);

    group.creator = rowData.value(3);

    QString membersValue = rowData.value(4);
    group.members = convertStringToMembers(membersValue);

    QString leadersValue = rowData.value(5);
    group.leaders = convertStringToLeaders(leadersValue);

    group.address = rowData.value(6);

    group.port = rowData.value(7).toInt();

    QString networkString = rowData.value(8);
    group.networkType = convertStringToNetworkType(networkString);

    QString typeString = rowData.value(9);
    group.type = convertStringToType(typeString);

    group.parent = rowData.value(10);

    group.orgType = rowData.value(11).toInt();

    return group;
}

bool GroupInterface::setGroup(const int &row, const Group &group)
{
    QModelIndex idIndex = m_groupModel.index(row, 0);
    m_groupModel.setData(idIndex, group.id);

    QModelIndex nameIndex = m_groupModel.index(row, 1);
    m_groupModel.setData(nameIndex, group.name);

    QModelIndex iconIndex = m_groupModel.index(row, 2);
    m_groupModel.setData(iconIndex, group.iconPath);

    QModelIndex creatorIndex = m_groupModel.index(row, 3);
    m_groupModel.setData(creatorIndex, group.creator);

    QModelIndex membersIndex = m_groupModel.index(row, 4);
    QString members = convertMembersToString(group.members);
    m_groupModel.setData(membersIndex, members);

    QModelIndex leadersIndex = m_groupModel.index(row, 5);
    QString leaders = convertLeadersToString(group.leaders);
    m_groupModel.setData(leadersIndex, leaders);

    QModelIndex addressIndex = m_groupModel.index(row, 6);
    m_groupModel.setData(addressIndex, group.address);

    QModelIndex portIndex = m_groupModel.index(row, 7);
    m_groupModel.setData(portIndex, group.port);

    QModelIndex networkIndex = m_groupModel.index(row, 8);
    QString networkString = convertNetworkTypeToString((NetworkType)group.networkType);
    m_groupModel.setData(networkIndex, networkString);

    QModelIndex typeIndex = m_groupModel.index(row, 9);
    QString typeString = convertTypeToString((GroupType)group.type);
    m_groupModel.setData(typeIndex, typeString);

    QModelIndex parentIndex = m_groupModel.index(row, 10);
    m_groupModel.setData(parentIndex, group.parent);

    QModelIndex orgTypeIndex = m_groupModel.index(row, 11);
    m_groupModel.setData(orgTypeIndex, group.orgType);

    return true;
}

QString GroupInterface::convertMembersToString(const QStringList &members)
{
    QJsonArray memberArray;
    for(int i = 0, iend = members.count(); i < iend; i++)
    {
        QJsonValue memberValue(members[i].toInt());
        memberArray.append(memberValue);
    }

    QJsonDocument document(memberArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QStringList GroupInterface::convertStringToMembers(const QString &value)
{
    QStringList members;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        QString memberId = QString::number(value.toInt());
        members.append(memberId);
    }

    return members;
}

QString GroupInterface::convertLeadersToString(const QStringList &leaders)
{
    QJsonArray leaderArray;
    for(int i = 0, iend = leaders.count(); i < iend; i++)
    {
        QJsonValue leaderValue(leaders[i].toInt());
        leaderArray.append(leaderValue);
    }

    QJsonDocument document(leaderArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QStringList GroupInterface::convertStringToLeaders(const QString &value)
{
    QStringList leaders;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        QString leaderId = QString::number(value.toInt());
        leaders.append(leaderId);
    }

    return leaders;
}

//QString GroupInterface::convertNetworkTypeToString(const NetworkType &type)
//{
//    QString value;
//    switch(type)
//    {
//    case AD_HOC_NETWORK:
//        value = "ad_hoc_network";
//        break;
//    case LTE_NETWORK:
//        value = "lte_network";
//        break;
//    case SATELLITE_NETWORK:
//        value = "satellite_network";
//        break;
//    case DIGITAL_WALKIE_TALKIE_NETWORK:
//        value = "digital_walkie_talkie_network";
//        break;
//    case BATTLE_NETWORK:
//        value = "battle_network";
//        break;
//    case ADHOC_LTE_NETWORK:
//        value = "adhoc_lte_network";
//        break;
//    case UNKNOW_NETWORK:
//        value = "unknow_network";
//        break;
//    default:
//        value = "unknow_network";
//    }

//    return value;
//}

//NetworkType GroupInterface::convertStringToNetworkType(const QString &value)
//{
//    if(value == "ad_hoc_network")
//        return AD_HOC_NETWORK;
//    else if(value == "lte_network")
//        return LTE_NETWORK;
//    else if(value == "satellite_network")
//        return SATELLITE_NETWORK;
//    else if(value == "digital_walkie_talkie_network")
//        return DIGITAL_WALKIE_TALKIE_NETWORK;
//    else if(value == "battle_network")
//        return BATTLE_NETWORK;
//    else if(value == "adhoc_lte_network")
//        return ADHOC_LTE_NETWORK;
//    else if(value == "unknow_network")
//        return UNKNOW_NETWORK;
//    else
//        return UNKNOW_NETWORK;

//    return UNKNOW_NETWORK;
//}

QString GroupInterface::convertTypeToString(const GroupType &type)
{
    QString value;
    switch(type)
    {
    case CONVERSION_GROUP:
        value = "conversion";
        break;
    case SUBORDINATE_GROUP:
        value = "subordinate";
        break;
    case BATTLE_GROUP:
        value = "battle";
        break;
    case UNDEFINE_GROUP:
        value = "undefine";
        break;
    default:
        value = "undefine";
    }

    return value;
}

GroupType GroupInterface::convertStringToType(const QString value)
{
    if(value == "conversion")
        return CONVERSION_GROUP;
    else if(value == "subordinate")
        return SUBORDINATE_GROUP;
    else if(value == "battle")
        return BATTLE_GROUP;
    else if(value == "undefine")
        return UNDEFINE_GROUP;
    else
        return UNDEFINE_GROUP;

    return UNDEFINE_GROUP;
}

void GroupInterface::emitChangeSignal(int type, Group changeGroup)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(changeGroup);
    dbusValue.setVariant(dataVar);
    emit signalGroupChanged(type, dbusValue);
}

void GroupInterface::onGroupReceived(const int &handleType, const GroupList &groups)
{
    switch (handleType) {
    case GROUP_ADD_HANDLE:
        addGroup(groups.value(0));
        break;
    case GROUP_UPDATE_HANDLE:
        updateGroup(groups.value(0));
        break;
    case GROUP_DELETE_HANDLE:
    {
        foreach(const Group &group, groups)
            removeGroup(group.id);
    }
        break;
    case GROUP_INIT_HANDLE:
        break;
    default:
        break;
    }
}

void GroupInterface::onDbChanged()
{
    m_groupModel.select();
    emitChangeSignal(GROUP_INIT_HANDLE, Group());
}

void GroupInterface::onGroupDownload(const GroupList &groups)
{
    qDebug() << "onGroupDownload";
    if(!groups.isEmpty())
    {
        //remove all groups
        removeGroups();
    }

    int index = 1;
    foreach(Group group, groups)
    {
        if(group.id.isEmpty())
            group.id = QString::number(index++);
        qDebug() << "download group " << group.id
                 << "type: " << convertTypeToString(GroupType(group.type));

        addGroup2(group);
    }
    m_groupModel.submitAll();
    emitChangeSignal(GROUP_INIT_HANDLE, Group());
}

void GroupInterface::onLteGroupReceived(const Group &group, int operateType)
{
    switch (operateType) {
    case GROUP_ADD_HANDLE:
        addGroup(group);
        break;
    case GROUP_UPDATE_HANDLE:
    {
        if(checkGroupExist(group.id))
        {
            updateGroup(group);
        }
        else
        {
            addGroup(group);
        }
        break;
    }
    case GROUP_DELETE_HANDLE:
        removeGroup(group.id);
        break;
    case GROUP_INIT_HANDLE:
        break;
    default:
        break;
    }
    m_groupModel.submitAll();

    if(group.type == BATTLE_GROUP)
    {
        emit signalGroupExchange();
    }
}

void GroupInterface::groupChangeToChangeGroupPeopleAddQueue(const Group &group)
{
    m_sendGroupList.push_back(group);
    m_timer.stop();
    m_timer.start();
}

GroupList GroupInterface::getBtGroup()
{
    GroupList groupList;
    QStringList idList = onGetGroups(CONVERSION_GROUP);
    for(QStringList::iterator it=idList.begin(); it!=idList.end(); it++)
    {
        const Group &group = getGroup(*it);
        groupList.push_back(group);
    }

    return groupList;
}

QStringList GroupInterface::getBtGroupAddrs()
{
    QStringList groupAddrList;
    GroupList groupList = getBtGroup();
    foreach(const Group &group, groupList)
    {
        groupAddrList.push_back(group.address);
    }

    return groupAddrList;
}

void GroupInterface::groupChangeToChangeGroupPeople()
{
    if(!m_sendGroupList.isEmpty())
    {
        Group group = m_sendGroupList.first();

        //connect
        QStringList ipList;
        QStringList cIdList;
        cIdList  << group.leaders << group.members; //<< groupData.creator
        qDebug() << cIdList;
        foreach(const QString &id, cIdList)
        {
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

            foreach(const PhoneNumber &number, contacter.numbers)
            {
                if(number.netType == WIFI_NETWORK)
                {
                    qDebug() << "groupChangeToChangeRemoteGroup number WIFI_NETWORK" << id << contacter.id  << number.number;
                    ipList.push_back(number.number);
                }
            }
        }
    //    m_sendGroup = group;
        m_groupShareAdaptor->onSetAddresses(ipList);
        m_groupShareAdaptor->onConnect();
    }
}

//after lte create group return.
void GroupInterface::onSignalGroupChangedFinished(const Group &group)
{
    qDebug() << "group changed finished: " << group.id;
    if(group.type == BATTLE_GROUP)
    {
        emit signalGroupExchange();
    }
    else
    {
        switch (group.handle) {
        case GROUP_ADD_HANDLE:
            addGroup(group);
            break;
        case GROUP_UPDATE_HANDLE:
        {
            if(checkGroupExist(group.id))
                updateGroup(group);
            else
                addGroup(group);
            break;
        }
        case GROUP_DELETE_HANDLE:
            removeGroup(group.id);
            break;
        case GROUP_INIT_HANDLE:
            break;
        default:
            break;
        }
        m_groupModel.submitAll();
    }
}

void GroupInterface::onSignalGroupShareConnected(const int &status)
{
    qDebug() << "onSignalGroupShareConnected status" << status;
    if(!m_sendGroupList.isEmpty())
    {
        Group group = m_sendGroupList.first();
        m_sendGroupList.pop_front();

        if(status > 0)
        {
            m_groupShareAdaptor->onSendGroup(group);
        }
        else
        {
            m_timer.start();
        }
    }
    else
    {
        qDebug() << "group message is empty: not send group";
    }
}

void GroupInterface::onSignalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    qDebug() << "onSignalSendPercent file name" << fileName << size << maxSize;
    if(size == maxSize)
    {
//        m_groupShareAdaptor->onDisconnect();
        qDebug() << "m_groupShareAdaptor onDisconnect";
        m_timer.start();
    }
}

void GroupInterface::onGroupChangeToChangeGroupPeople(const Group &group)
{
    switch (group.handle) {
    case GROUP_ADD_HANDLE:
        addGroup(group);
        if(group.type == BATTLE_GROUP)
        {
            emit signalGroupExchange();
        }
        break;
    case GROUP_UPDATE_HANDLE:
        updateGroup(group);
        break;
    case GROUP_DELETE_HANDLE:
        removeGroup(group.id);
        break;
    case GROUP_INIT_HANDLE:
        break;
    default:
        break;
    }
}
