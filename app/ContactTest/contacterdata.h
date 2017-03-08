#ifndef CONTACTERDATA
#define CONTACTERDATA

#include <QString>
#include <QPair>
#include <QDBusArgument>

typedef QPair<int, QString> PhoneNumber;
typedef QList<PhoneNumber> PhoneNumbers;
typedef QPair<int, quint32> Leader;
typedef QList<Leader> Leaders;

enum PhoneNetwork
{
    AD_HOC_NETWORK = 0x00000001,
    LTE_NETWORK = 0x00000010,
    SATELLITE_NETWORK = 0x00000100,
    DIGITAL_WALKITE_TALKIE_NETWORK = 0x00001000,
    BATTLE_NETWORK = 0x00010000,
    UNKNOW_NETWORK = 0x00000000
};

enum LeaderType
{
    BATTLION_COMMANDER = 4,
    COMPANY_COMMANDER = 3,
    PLATOON_LEADER = 2,
    SQUAD_LEADER = 1,
    UNDEFINE_LEADER = 0
};

enum ContacterHandle
{
    CONTACTER_ADD_HANDLE = 1,
    CONTACTER_DELETE_HANDLE = 2,
    CONTACTER_UPDATE_HANDLE = 3,
    CONTACTER_UNDEFINE_HANDLE = 4,
};

struct Contacter
{
    quint32 userId;
    QString name;
    QString title;
    PhoneNumbers numbers;
    QString photoPath;
    Leaders leaders;
    Contacter()
    {
        userId = 0;
    }
};

typedef QList<Contacter> ContacterList;
Q_DECLARE_METATYPE(Contacter)

// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Contacter &contacter)
{
    argument.beginStructure();
    quint32 userId = contacter.userId;
    QString name = contacter.name;
    QString title = contacter.title;
    PhoneNumbers numbers = contacter.numbers;
    QString photoPath = contacter.photoPath;
    Leaders leaders = contacter.leaders;
    argument << userId;
    argument << name;
    argument << title;
    argument << numbers;
    argument << photoPath;
    argument << leaders;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Contacter &contacter)
{
    quint32 userId;
    QString name;
    QString title;
    PhoneNumbers numbers;
    QString photoPath;
    Leaders leaders;
    argument.beginStructure();
    argument >> userId;
    argument >> name;
    argument >> title;
    argument >> numbers;
    argument >> photoPath;
    argument >> leaders;
    argument.endStructure();

    contacter.userId = userId;
    contacter.name = name;
    contacter.title = title;
    contacter.numbers = numbers;
    contacter.photoPath = photoPath;
    contacter.leaders = leaders;
    return argument;
}

#endif // CONTACTERDATA

