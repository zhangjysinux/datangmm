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
    GROUP_UNDEFINE_HANDLE = 4,
};

struct Group
{
    quint32 id;
    QString name;
    QString iconPath;
    QList<quint32> members;
    QList<quint32> leaders;
    QString address;
    int networkType;
    int type;
    Group()
    {
        id = 0;
    }
};

typedef QList<Group> GroupList;
Q_DECLARE_METATYPE(Group)

// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Group &group)
{
    argument.beginStructure();
    quint32 id = group.id;
    QString name = group.name;
    QString iconPath = group.iconPath;
    QList<quint32> members = group.members;
    QList<quint32> leaders = group.leaders;
    QString address = group.address;
    int networkType = group.networkType;
    int type = group.type;
    argument << id;
    argument << name;
    argument << iconPath;
    argument << members;
    argument << leaders;
    argument << address;
    argument << networkType;
    argument << type;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Group &group)
{
    quint32 id;
    QString name;
    QString iconPath;
    QList<quint32> members;
    QList<quint32> leaders;
    QString address;
    int networkType;
    int type;
    argument.beginStructure();
    argument >> id;
    argument >> name;
    argument >> iconPath;
    argument >> members;
    argument >> leaders;
    argument >> address;
    argument >> networkType;
    argument >> type;
    argument.endStructure();

    group.id = id;
    group.name = name;
    group.iconPath = iconPath;
    group.members = members;
    group.leaders = leaders;
    group.address = address;
    group.networkType = networkType;
    group.type = type;
    return argument;
}

#endif // GROUPDATA

