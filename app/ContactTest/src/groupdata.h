#ifndef GROUPDATA
#define GROUPDATA

#include <QString>
#include <QDBusArgument>

enum GroupType
{
    CONVERSION_GROUP = 1,
    SUBORDINATE_GROUP = 2,
    BATTLE_GROUP = 3,
    UNDEFINE_GROUP = 4
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
    QStringList members;
    QStringList leaders;
    QString address;
    int port;
    int networkType;
    int type;
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
    QStringList members = group.members;
    QStringList leaders = group.leaders;
    QString address = group.address;
    int port = group.port;
    int networkType = group.networkType;
    int type = group.type;
    argument << id;
    argument << name;
    argument << iconPath;
    argument << members;
    argument << leaders;
    argument << address;
    argument << port;
    argument << networkType;
    argument << type;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Group &group)
{
    QString id;
    QString name;
    QString iconPath;
    QStringList members;
    QStringList leaders;
    QString address;
    int port;
    int networkType;
    int type;
    argument.beginStructure();
    argument >> id;
    argument >> name;
    argument >> iconPath;
    argument >> members;
    argument >> leaders;
    argument >> address;
    argument >> port;
    argument >> networkType;
    argument >> type;
    argument.endStructure();

    group.id = id;
    group.name = name;
    group.iconPath = iconPath;
    group.members = members;
    group.leaders = leaders;
    group.address = address;
    group.port = port;
    group.networkType = networkType;
    group.type = type;
    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const Group &group)
{
    out << group.id;
    out << group.name;
    out << group.iconPath;
    out << group.members;
    out << group.leaders;
    out << group.address;
    out << group.port;
    out << group.networkType;
    out << group.type;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Group &group)
{
    in >> group.id;
    in >> group.name;
    in >> group.iconPath;
    in >> group.members;
    in >> group.leaders;
    in >> group.address;
    in >> group.port;
    in >> group.networkType;
    in >> group.type;
    return in;
}

#endif // GROUPDATA

