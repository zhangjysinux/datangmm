#ifndef CONTACTERDATA
#define CONTACTERDATA

#include <QString>
#include <QPair>
#include <QDBusArgument>

typedef QPair<int, QString> PhoneNumber;
typedef QList<PhoneNumber> PhoneNumbers;

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
    CONTACTER_UNDEFINE_HANDLE = 4,
};

struct Contacter
{
    quint32 id;
    QString name;
    int title;
    PhoneNumbers numbers;
    QString photoPath;
    QList<quint32> leaders;
    QString snNumber;
    QString bluetooth;
    Contacter()
    {
        id = 0;
    }
};

typedef QList<Contacter> ContacterList;
Q_DECLARE_METATYPE(Contacter)

// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Contacter &contacter)
{
    argument.beginStructure();
    quint32 id = contacter.id;
    QString name = contacter.name;
    int title = contacter.title;
    PhoneNumbers numbers = contacter.numbers;
    QString photoPath = contacter.photoPath;
    QList<quint32> leaders = contacter.leaders;
    QString snNumber = contacter.snNumber;
    QString bluetooth = contacter.bluetooth;
    argument << id;
    argument << name;
    argument << title;
    argument << numbers;
    argument << photoPath;
    argument << leaders;
    argument << snNumber;
    argument << bluetooth;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Contacter &contacter)
{
    quint32 id;
    QString name;
    int title;
    PhoneNumbers numbers;
    QString photoPath;
    QList<quint32> leaders;
    QString snNumber;
    QString bluetooth;
    argument.beginStructure();
    argument >> id;
    argument >> name;
    argument >> title;
    argument >> numbers;
    argument >> photoPath;
    argument >> leaders;
    argument >> snNumber;
    argument >> bluetooth;
    argument.endStructure();

    contacter.id = id;
    contacter.name = name;
    contacter.title = title;
    contacter.numbers = numbers;
    contacter.photoPath = photoPath;
    contacter.leaders = leaders;
    contacter.snNumber = snNumber;
    contacter.bluetooth = bluetooth;
    return argument;
}

#endif // CONTACTERDATA

