#ifndef CONTACTERDATA
#define CONTACTERDATA

#include <QString>
#include <QPair>
#include <QDBusArgument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "commondata.h"
#include <algorithm>

class PhoneNumber
{
public:
    int netType;
    QString number;
    int online;
    int priority;
    PhoneNumber()
    {
        netType = 0;
        number = "";
        online = 1;
        priority = 0;
    }
};
typedef QList<PhoneNumber> PhoneNumbers;

typedef QPair<int, QString> OldPhoneNumber;
typedef QList<OldPhoneNumber> OldPhoneNumbers;

Q_DECLARE_METATYPE(PhoneNumber)
Q_DECLARE_METATYPE(PhoneNumbers)

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
    QByteArray photoData;//image data for transmit
    int selectedStatus;
    int handle; // for transmition when add, delete or update
    QString pocId;
    Contacter()
    {
        title = SOLIDER;
        photoPath = "null";
        leaders << "-1";
        snNumber = QUuid::createUuid().toString();
        bluetooth = "11:22:33:44:55:66";
    }
};

typedef QList<Contacter> ContacterList;
Q_DECLARE_METATYPE(Contacter)
Q_DECLARE_METATYPE(ContacterList)

// Marshall the data into a D-Bus argument

inline QDBusArgument &operator<<(QDBusArgument &argument, const PhoneNumber &phoneNumber)
{
    argument.beginStructure();

    argument << phoneNumber.netType;
    argument << phoneNumber.number;
    argument << phoneNumber.online;
    argument << phoneNumber.priority;

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, PhoneNumber &phoneNumber)
{
    argument.beginStructure();

    argument >> phoneNumber.netType;
    argument >> phoneNumber.number;
    argument >> phoneNumber.online;
    argument >> phoneNumber.priority;

    argument.endStructure();
    return argument;
}

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
    QByteArray photoData = contacter.photoData;
    int selectedStatus = contacter.selectedStatus;
    int handle = contacter.handle;
    QString pocId = contacter.pocId;
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
    argument << photoData;
    argument << selectedStatus;
    argument << handle;
    argument << pocId;
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
    QByteArray photoData;
    int selectedStatus;
    int handle;
    QString pocId;
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
    argument >> photoData;
    argument >> selectedStatus;
    argument >> handle;
    argument >> pocId;
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
    contacter.photoData = photoData;
    contacter.selectedStatus = selectedStatus;
    contacter.handle = handle;
    contacter.pocId = pocId;
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

inline QDataStream &operator<<(QDataStream &out,const PhoneNumber &phoneNumber)
{
    out << phoneNumber.netType
        << phoneNumber.number
        << phoneNumber.online
        << phoneNumber.priority;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, PhoneNumber &phoneNumber)
{
    in  >> phoneNumber.netType
        >> phoneNumber.number
        >> phoneNumber.online
        >> phoneNumber.priority;
    return in;
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
        << contacter.type
        << contacter.photoData
        << contacter.selectedStatus
        << contacter.handle
        << contacter.pocId;
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
        >> contacter.type
        >> contacter.photoData
        >> contacter.selectedStatus
        >> contacter.handle
        >> contacter.pocId;
    return in;
}

inline QString convertPhoneNumbersToString(const QList<PhoneNumber> &numbers)
{
    QJsonObject object;
    for(int i = 0, iend = numbers.count(); i < iend; i++)
    {
        QString type = convertNetworkTypeToString((NetworkType)numbers[i].netType);
        QString address = numbers[i].number;
        int online = numbers[i].online;
        int priority = numbers[i].priority;
        QJsonArray array;
        array.append(QJsonValue(address));
        array.append(QJsonValue(online));
        array.append(QJsonValue(priority));
        object.insert(type, array);
    }

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

inline QList<PhoneNumber> convertStringToPhoneNumbers(const QString &value)
{
    QList<PhoneNumber> numbers;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject object = document.object();
    QJsonObject::iterator start = object.begin();
    QJsonObject::iterator end = object.end();
    for(; start != end; start++)
    {
        PhoneNumber number;
        number.netType = convertStringToNetworkType(start.key());

        QJsonArray array = start.value().toArray();
        if(array.size() >= 3)
        {
            number.number = array.at(0).toString();
            number.online = array.at(1).toInt();
            number.priority = array.at(2).toInt();
        }

        numbers.append(number);
    }

    return numbers;
}

inline QString convertLeadersToString(const QStringList &leaders)
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

inline QStringList convertStringToLeaders(const QString &value)
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

inline QString converPhoneNumbersToString_old(const OldPhoneNumbers &numbers)
{
    QJsonObject object;
    for(int i = 0, iend = numbers.count(); i < iend; i++)
    {
        QString type = convertNetworkTypeToString((NetworkType)numbers[i].first);
        QString address = numbers[i].second;
        object.insert(type, address);
    }

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

inline OldPhoneNumbers converStringToPhoneNumbers_old(const QString &value)
{
    OldPhoneNumbers numbers;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject object = document.object();
    QJsonObject::iterator start = object.begin();
    QJsonObject::iterator end = object.end();
    for(; start != end; start++)
    {
        OldPhoneNumber number;
//        number.first = convertStringToNetworkType(start.key());
//        number.second = start.value().toString();

        numbers.append(number);
    }

    return numbers;
}

inline PhoneNumbers sortPhoneNumbersByPriority(const PhoneNumbers &numbers)
{
    PhoneNumbers nums = numbers;
    std::sort(nums.begin(), nums.end(), [](const PhoneNumber& x, const PhoneNumber& y){
        return x.priority > y.priority;
    });
    return nums;
}

inline PhoneNumber getPhoneNumberType(const PhoneNumbers &numbers, int networkType)
{
    PhoneNumber number;
    for(PhoneNumbers::const_iterator it=numbers.begin(); it!=numbers.end(); it++)
    {
        if(it->netType == networkType)
            number = *it;
    }
    return number;
}

#endif // CONTACTERDATA

