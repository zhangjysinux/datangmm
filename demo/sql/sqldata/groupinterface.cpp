#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QDBusMetaType>
#include "groupinterface.h"

GroupInterface::GroupInterface(QObject *parent)
    : QObject(parent)
{
    qDBusRegisterMetaType<Group>();
    qDBusRegisterMetaType<GroupList>();

    //init contacter data
    m_groupModel.setTable("groups");
    m_groupModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_groupModel.select();
}

bool GroupInterface::onAddGroup(const Group &group)
{
    if(group.id == 0)
        return false;
    if(group.name.isEmpty())
        return false;
    if(checkGroupExist(group.id))
        return false;

    int rowCount = m_groupModel.rowCount();
    bool insertFlag = m_groupModel.insertRow(rowCount);
    if(!insertFlag)
        return false;
    bool setFlag = setGroup(rowCount, group);
    if(!setFlag)
        return false;

    emitChangeSignal(GROUP_ADD_HANDLE, group);
    m_groupModel.submitAll();
    return true;
}

bool GroupInterface::onRemoveGroup(const quint32 &id)
{
    if(id == 0)
        return false;

    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_groupModel.index(i, 0);
        if(m_groupModel.data(index).toUInt() == id)
        {
            Group changeGroup = getGroup(i);
            if(changeGroup.type != CONVERSION_GROUP)
                return false;

            m_groupModel.removeRow(i);
            emitChangeSignal(GROUP_DELETE_HANDLE, changeGroup);
            m_groupModel.submitAll();

            return true;
        }
    }
    return false;
}

bool GroupInterface::onUpdateGroup(const Group &group)
{
    if(group.id == 0)
        return false;
    if(group.name.isEmpty())
        return false;
    if(!checkGroupExist(group.id))
        return false;

    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_groupModel.index(i, 0);
        if(m_groupModel.data(index).toUInt() == group.id)
        {
            setGroup(i, group);
            Group group = getGroup(i);
            emitChangeSignal(GROUP_UPDATE_HANDLE, group);
            m_groupModel.submitAll();
            return true;
        }
    }
    return false;
}

QDBusVariant GroupInterface::onGetGroup(const quint32 &id)
{
    if(id == 0)
        return QDBusVariant();

    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_groupModel.index(i, 0);
        if(m_groupModel.data(index).toUInt() == id)
        {
            Group group = getGroup(i);
            QDBusVariant dbusValue;
            QVariant groupVar = QVariant::fromValue(group);
            dbusValue.setVariant(groupVar);
            return dbusValue;
        }
    }
    return QDBusVariant();
}

QDBusVariant GroupInterface::onGetGroups(const int &type)
{
    QList<quint32> ids;
    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex idIndex = m_groupModel.index(i, 0);
        QModelIndex typeIndex = m_groupModel.index(i, 7);
        quint32 id = m_groupModel.data(idIndex).toUInt();
        QString typeString = m_groupModel.data(typeIndex).toString();
        GroupType groupType = convertStringToType(typeString);
        if(groupType == type)
            ids.append(id);
    }

    QDBusVariant dbusValue;
    QVariant idsVar = QVariant::fromValue(ids);
    dbusValue.setVariant(idsVar);
    return dbusValue;
}

bool GroupInterface::checkGroupExist(const quint32 &id)
{
    for(int i = 0, iend = m_groupModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_groupModel.index(i, 0);
        if(m_groupModel.data(index).toUInt() == id)
            return true;
    }
    return false;
}

Group GroupInterface::getGroup(const int &row)
{
    if(m_groupModel.rowCount() < row || row < 0)
        return Group();

    Group group;
    QModelIndex idIndex = m_groupModel.index(row, 0);
    group.id = m_groupModel.data(idIndex).toUInt();

    QModelIndex nameIndex = m_groupModel.index(row, 1);
    group.name = m_groupModel.data(nameIndex).toString();

    QModelIndex iconIndex = m_groupModel.index(row, 2);
    group.iconPath = m_groupModel.data(iconIndex).toString();

    QModelIndex membersIndex = m_groupModel.index(row, 3);
    QString membersValue = m_groupModel.data(membersIndex).toString();
    group.members = convertStringToMembers(membersValue);

    QModelIndex leadersIndex = m_groupModel.index(row, 4);
    QString leadersValue = m_groupModel.data(leadersIndex).toString();
    group.leaders = convertStringToLeaders(leadersValue);

    QModelIndex addressIndex = m_groupModel.index(row, 5);
    group.address = m_groupModel.data(addressIndex).toString();

    QModelIndex networkIndex = m_groupModel.index(row, 6);
    QString networkString = m_groupModel.data(networkIndex).toString();
    group.networkType = convertStringToNetworkType(networkString);

    QModelIndex typeIndex = m_groupModel.index(row, 7);
    QString typeString = m_groupModel.data(typeIndex).toString();
    group.type = convertStringToType(typeString);

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

    QModelIndex membersIndex = m_groupModel.index(row, 3);
    QString members = convertMembersToString(group.members);
    m_groupModel.setData(membersIndex, members);

    QModelIndex leadersIndex = m_groupModel.index(row, 4);
    QString leaders = convertLeadersToString(group.leaders);
    m_groupModel.setData(leadersIndex, leaders);

    QModelIndex addressIndex = m_groupModel.index(row, 5);
    m_groupModel.setData(addressIndex, group.address);

    QModelIndex networkIndex = m_groupModel.index(row, 6);
    QString networkString = convertNetworkTypeToString((NetworkType)group.networkType);
    m_groupModel.setData(networkIndex, networkString);

    QModelIndex typeIndex = m_groupModel.index(row, 7);
    QString typeString = convertTypeToString((GroupType)group.type);
    m_groupModel.setData(typeIndex, typeString);

    return true;
}

QString GroupInterface::convertMembersToString(const QList<quint32> &members)
{
    QJsonArray memberArray;
    for(int i = 0, iend = members.count(); i < iend; i++)
    {
        QJsonValue memberValue((qint64)members[i]);
        memberArray.append(memberValue);
    }

    QJsonDocument document(memberArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QList<quint32> GroupInterface::convertStringToMembers(const QString &value)
{
    QList<quint32> members;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        quint32 memberId = value.toInt();
        members.append(memberId);
    }

    return members;
}

QString GroupInterface::convertLeadersToString(const QList<quint32> &leaders)
{
    QJsonArray leaderArray;
    for(int i = 0, iend = leaders.count(); i < iend; i++)
    {
        QJsonValue leaderValue((qint64)leaders[i]);
        leaderArray.append(leaderValue);
    }

    QJsonDocument document(leaderArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QList<quint32> GroupInterface::convertStringToLeaders(const QString &value)
{
    QList<quint32> leaders;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        quint32 leaderId = value.toInt();
        leaders.append(leaderId);
    }

    return leaders;
}

QString GroupInterface::convertNetworkTypeToString(const NetworkType &type)
{
    QString value;
    switch(type)
    {
    case AD_HOC_NETWORK:
        value = "ad_hoc_network";
        break;
    case LTE_NETWORK:
        value = "lte_network";
        break;
    case SATELLITE_NETWORK:
        value = "satellite_network";
        break;
    case DIGITAL_WALKITE_TALKIE_NETWORK:
        value = "digital_walkite_talkie_network";
        break;
    case BATTLE_NETWORK:
        value = "battle_network";
        break;
    case UNKNOW_NETWORK:
        value = "unknow_network";
        break;
    default:
        value = "unknow_network";
    }

    return value;
}

NetworkType GroupInterface::convertStringToNetworkType(const QString &value)
{
    if(value == "ad_hoc_network")
        return AD_HOC_NETWORK;
    else if(value == "lte_network")
        return LTE_NETWORK;
    else if(value == "satellite_network")
        return SATELLITE_NETWORK;
    else if(value == "digital_walkite_talkie_network")
        return DIGITAL_WALKITE_TALKIE_NETWORK;
    else if(value == "battle_network")
        return BATTLE_NETWORK;
    else if(value == "unknow_network")
        return UNKNOW_NETWORK;
    else
        return UNKNOW_NETWORK;

    return UNKNOW_NETWORK;
}

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
