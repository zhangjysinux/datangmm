#ifndef GROUPDATA
#define GROUPDATA

#include <QString>
#include <QDBusArgument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

enum GroupType
{
    CONVERSION_GROUP = 1,
    SUBORDINATE_GROUP = 2,
    BATTLE_GROUP = 3,
    Point_Point_Group = 4,
    UNDEFINE_GROUP = 5
};

enum GroupHandle
{
    GROUP_ADD_HANDLE = 1,
    GROUP_DELETE_HANDLE = 2,
    GROUP_UPDATE_HANDLE = 3,
    GROUP_INIT_HANDLE = 4,
    GROUP_UNDEFINE_HANDLE = 5,
};

struct Group
{
    QString id;
    QString name;
    QString iconPath;
    QString creator;
    QStringList members;
    QStringList leaders;
    QString address; //when lte: it is groupid in server
    int port;
    int networkType;
    int type;
    int handle; // for transmition when add, delete or update
    QString lteGroupId;
    QString parent;
    int orgType;//for organic group used
};

typedef QList<Group> GroupList;
Q_DECLARE_METATYPE(Group)
Q_DECLARE_METATYPE(GroupList)

// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Group &group)
{
    argument.beginStructure();
    QString id = group.id;
    QString name = group.name;
    QString iconPath = group.iconPath;
    QString creator = group.creator;
    QStringList members = group.members;
    QStringList leaders = group.leaders;
    QString address = group.address;
    int port = group.port;
    int networkType = group.networkType;
    int type = group.type;
    int handle = group.handle;
    QString lteGroupId = group.lteGroupId;
    QString parent = group.parent;
    int orgType = group.orgType;
    argument << id;
    argument << name;
    argument << iconPath;
    argument << creator;
    argument << members;
    argument << leaders;
    argument << address;
    argument << port;
    argument << networkType;
    argument << type;
    argument << handle;
    argument << lteGroupId;
    argument << parent;
    argument << orgType;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Group &group)
{
    QString id;
    QString name;
    QString iconPath;
    QString creator;
    QStringList members;
    QStringList leaders;
    QString address;
    int port;
    int networkType;
    int type;
    int handle;
    QString lteGroupId;
    QString parent;
    int orgType;
    argument.beginStructure();
    argument >> id;
    argument >> name;
    argument >> iconPath;
    argument >> creator;
    argument >> members;
    argument >> leaders;
    argument >> address;
    argument >> port;
    argument >> networkType;
    argument >> type;
    argument >> handle;
    argument >> lteGroupId;
    argument >> parent;
    argument >> orgType;
    argument.endStructure();

    group.id = id;
    group.name = name;
    group.iconPath = iconPath;
    group.creator = creator;
    group.members = members;
    group.leaders = leaders;
    group.address = address;
    group.port = port;
    group.networkType = networkType;
    group.type = type;
    group.handle = handle;
    group.lteGroupId = lteGroupId;
    group.parent = parent;
    group.orgType = orgType;
    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const Group &group)
{
    out << group.id;
    out << group.name;
    out << group.iconPath;
    out << group.creator;
    out << group.members;
    out << group.leaders;
    out << group.address;
    out << group.port;
    out << group.networkType;
    out << group.type;
    out << group.handle;
    out << group.lteGroupId;
    out << group.parent;
    out << group.orgType;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Group &group)
{
    in >> group.id;
    in >> group.name;
    in >> group.iconPath;
    in >> group.creator;
    in >> group.members;
    in >> group.leaders;
    in >> group.address;
    in >> group.port;
    in >> group.networkType;
    in >> group.type;
    in >> group.handle;
    in >> group.lteGroupId;
    in >> group.parent;
    in >> group.orgType;
    return in;
}


inline QString convertTypeToString(const GroupType &type)
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

inline GroupType convertStringToType(const QString value)
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

inline QString convertMembersToString(const QStringList &members)
{
    QJsonArray memberArray;
    for(int i = 0, iend = members.count(); i < iend; i++)
    {
        QJsonValue memberValue(members[i]);
        memberArray.append(memberValue);
    }

    QJsonDocument document(memberArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

inline QStringList convertStringToMembers(const QString &value)
{
    QStringList members;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        QString memberId = value.toString();
        members.append(memberId);
    }

    return members;
}

inline QString convertGroupLeadersToString(const QStringList &leaders)
{
    QJsonArray leaderArray;
    for(int i = 0, iend = leaders.count(); i < iend; i++)
    {
        QJsonValue leaderValue(leaders[i]);
        leaderArray.append(leaderValue);
    }

    QJsonDocument document(leaderArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

inline QStringList convertStringToGroupLeaders(const QString &value)
{
    QStringList leaders;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        QString leaderId = value.toString();
        leaders.append(leaderId);
    }

    return leaders;
}

#endif // GROUPDATA

