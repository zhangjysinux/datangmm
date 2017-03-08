#ifndef CONTACTERDATA
#define CONTACTERDATA

#include <QString>
#include <QPair>
#include <QDBusArgument>

typedef QPair<int, QString> PhoneNumber;
typedef QList<PhoneNumber> PhoneNumbers;

enum ContacterType
{
    CONTACTERTYPE_ORGANIZATION = 1,
    CONTACTERTYPE_LOCAL = 2,
    CONTACTERTYPE_UNDEFINE = 3
};

enum TitleType
{
    BATTLION = 5,
    COMPANY = 4,
    PLATOON = 3,
    SQUAD = 2,
    SOLIDER = 1,
    UNDEFINE_LEADER = 0
};

enum ContacterHandle
{
    CONTACTER_ADD_HANDLE = 1,
    CONTACTER_DELETE_HANDLE = 2,
    CONTACTER_UPDATE_HANDLE = 3,
    CONTACTER_INIT_HANDLE = 4,
    CONTACTER_UNDEFINE_HANDLE = 5,
};

struct Contacter
{
    QString id;
    QString surname;
    QString name;
    int title;
    PhoneNumbers numbers;
    QString photoPath;
    QStringList leaders;
    QString snNumber;
    QString bluetooth;
    QString namePinyin;
    int type;//ContacterType
};

typedef QList<Contacter> ContacterList;
Q_DECLARE_METATYPE(Contacter)
Q_DECLARE_METATYPE(ContacterList)

// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Contacter &contacter)
{
    argument.beginStructure();
    QString id = contacter.id;
    QString surname = contacter.surname;
    QString name = contacter.name;
    int title = contacter.title;
    PhoneNumbers numbers = contacter.numbers;
    QString photoPath = contacter.photoPath;
    QStringList leaders = contacter.leaders;
    QString snNumber = contacter.snNumber;
    QString bluetooth = contacter.bluetooth;
    QString namePinyin = contacter.namePinyin;
    int type = contacter.type;
    argument << id;
    argument << surname;
    argument << name;
    argument << title;
    argument << numbers;
    argument << photoPath;
    argument << leaders;
    argument << snNumber;
    argument << bluetooth;
    argument << namePinyin;
    argument << type;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Contacter &contacter)
{
    QString id;
    QString surname;
    QString name;
    int title;
    PhoneNumbers numbers;
    QString photoPath;
    QStringList leaders;
    QString snNumber;
    QString bluetooth;
    QString namePinyin;
    int type;
    argument.beginStructure();
    argument >> id;
    argument >> surname;
    argument >> name;
    argument >> title;
    argument >> numbers;
    argument >> photoPath;
    argument >> leaders;
    argument >> snNumber;
    argument >> bluetooth;
    argument >> namePinyin;
    argument >> type;
    argument.endStructure();

    contacter.id = id;
    contacter.surname = surname;
    contacter.name = name;
    contacter.title = title;
    contacter.numbers = numbers;
    contacter.photoPath = photoPath;
    contacter.leaders = leaders;
    contacter.snNumber = snNumber;
    contacter.bluetooth = bluetooth;
    contacter.namePinyin = namePinyin;
    contacter.type = type;
    return argument;
}

static QString getTitleStringFromEnum(int titleType)
{
    QString str;
    switch (titleType) {
    case BATTLION:
        str = "营长";
        break;
    case COMPANY:
        str = "连长";
        break;
    case PLATOON:
        str = "排长";
        break;
    case SQUAD:
        str = "班长";
        break;
    case SOLIDER:
        str = "战士";
        break;
    default:
        str = "";
        break;
    }
    return str;
}

inline QDataStream &operator<<(QDataStream &out,const Contacter &contacter)
{
    out << contacter.id
        << contacter.surname
        << contacter.name
        << contacter.title
        << contacter.numbers
        << contacter.photoPath
        << contacter.leaders
        << contacter.snNumber
        << contacter.bluetooth
        << contacter.namePinyin
        << contacter.type;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Contacter &contacter)
{
    in  >> contacter.id
        >> contacter.surname
        >> contacter.name
        >> contacter.title
        >> contacter.numbers
        >> contacter.photoPath
        >> contacter.leaders
        >> contacter.snNumber
        >> contacter.bluetooth
        >> contacter.namePinyin
        >> contacter.type;
    return in;
}

#endif // CONTACTERDATA

