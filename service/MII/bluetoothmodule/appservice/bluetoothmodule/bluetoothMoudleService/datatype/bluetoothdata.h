#ifndef BLUETOOTHDATA_H
#define BLUETOOTHDATA_H

#include <QString>
#include <QDBusArgument>
#include <QDBusInterface>
//#include "../commondata/datatype/commondata.h"
#include "./datatype/commondata.h"

enum BluetoothServiceType
{
    BLUETOOTHSERVICETYPE_SERVER = 0,
    BLUETOOTHSERVICETYPE_CLIENT = 1,
    BLUETOOTHSERVICETYPE_NONE = 2
};

enum BluetoothTransDataType
{
    BLUETOOTHTRANSDATATYPE_CONTACTSHARE = 1000,
    BLUETOOTHTRANSDATATYPE_HISTORYSHARE,
    BLUETOOTHTRANSDATATYPE_GROUPSHARE,
    BLUETOOTHTRANSDATATYPE_GROUPMESSAGESHARE,
    BLUETOOTHTRANSDATATYPE_DBSHARE,
    BLUETOOTHTRANSDATATYPE_DIALHANDLERDATA,
    BLUETOOTHTRANSDATATYPE_DIALMANAGERDATA,
    BLUETOOTHTRANSDATATYPE_DIALHANDLERCMD,
    BLUETOOTHTRANSDATATYPE_FILESHARE
};

enum BluetoothTransDataHandleType
{
    BLUETOOTHTRANSDATAHANDLETYPE_ADD = 0,
    BLUETOOTHTRANSDATAHANDLETYPE_UPDATE = 1,
    BLUETOOTHTRANSDATAHANDLETYPE_DELETE = 2
};

struct BluetoothAddressData
{
    QString name;
    QString address;
    bool operator ==(const BluetoothAddressData &other)
    {
        if(name == other.name && address == other.address)
        {
            return true;
        }
        return false;
    }
};

struct BluetoothTransData
{
    int type;//BluetoothTransDataType
    int handleType;//BluetoothTransDataHandleType
    BluetoothAddressData addressData;
    int count;//count
    QByteArray transData;//structs --> number is count
    quint64 currentSize;
    quint64 totalSize;
    QString fileName;//if empty, not a file
    bool partOver;//if this is part of all, this indicate that part is over.
    bool partStart;
    BluetoothTransData()
    {
        partStart = true;
        partOver = true;
    }
};

struct BluetoothAppidReceiveDataCallback
{
    QString appid;
    QString dbusService;
    QString dbusPath;
    QString dbusInterface;
    QString function;
};

Q_DECLARE_METATYPE(BluetoothAddressData)
Q_DECLARE_METATYPE(BluetoothTransData)
Q_DECLARE_METATYPE(BluetoothAppidReceiveDataCallback)

// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const BluetoothAddressData &data)
{
    argument.beginStructure();
    QString name = data.name;
    QString address = data.address;

    argument << name;
    argument << address;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, BluetoothAddressData &data)
{
    QString name;
    QString address;

    argument.beginStructure();
    argument >> name;
    argument >> address;
    argument.endStructure();

    data.name = name;
    data.address = address;

    return argument;
}


// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const BluetoothTransData &data)
{
    argument.beginStructure();
    int type = data.type;
    int handleType = data.handleType;
    QString name = data.addressData.name;
    QString addr = data.addressData.address;
//    BluetoothAddressData addressData = data.addressData;
    int count = data.count;
    QByteArray transData = data.transData;
    quint64 currentSize = data.currentSize;
    quint64 totalSize = data.totalSize;
    QString fileName = data.fileName;
    bool partOver = data.partOver;
    bool partStart = data.partStart;

    argument << type;
    argument << handleType;
//    argument << addressData;
    argument << name;
    argument << addr;
    argument << count;
    argument << transData;
    argument << currentSize;
    argument << totalSize;
    argument << fileName;
    argument << partOver;
    argument << partStart;

    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, BluetoothTransData &data)
{
    int type;
    int handleType;
//    BluetoothAddressData addressData;
    QString name;
    QString addr;
    int count;
    QByteArray transData;
    quint64 currentSize;
    quint64 totalSize;
    QString fileName;
    bool partOver;
    bool partStart;

    argument.beginStructure();
    argument >> type;
    argument >> handleType;
//    argument >> addressData;
    argument >> name;
    argument >> addr;
    argument >> count;
    argument >> transData;
    argument >> currentSize;
    argument >> totalSize;
    argument >> fileName;
    argument >> partOver;
    argument >> partStart;
    argument.endStructure();

    data.type = type;
    data.handleType = handleType;
//    data.addressData = addressData;
    data.addressData.name = name;
    data.addressData.address = addr;
    data.count = count;
    data.transData = transData;
    data.currentSize = currentSize;
    data.totalSize = totalSize;
    data.fileName = fileName;
    data.partOver = partOver;
    data.partStart = partStart;

    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const BluetoothAppidReceiveDataCallback &data)
{
    argument.beginStructure();

    argument << data.appid;
    argument << data.dbusService;
    argument << data.dbusPath;
    argument << data.dbusInterface;
    argument << data.function;

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, BluetoothAppidReceiveDataCallback &data)
{
    argument.beginStructure();

    argument >> data.appid;
    argument >> data.dbusService;
    argument >> data.dbusPath;
    argument >> data.dbusInterface;
    argument >> data.function;

    argument.endStructure();
    return argument;
}


inline QDataStream &operator<<(QDataStream &out,const BluetoothTransData &data)
{
    out << data.type << data.handleType
        << data.addressData.name << data.addressData.address
        << data.count
        << data.transData
        << data.currentSize
        << data.totalSize
        << data.fileName
        << data.partOver
        << data.partStart;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, BluetoothTransData &data)
{
    in >> data.type >> data.handleType
       >> data.addressData.name >> data.addressData.address
       >> data.count
       >> data.transData
       >> data.currentSize
       >> data.totalSize
       >> data.fileName
       >> data.partOver
       >> data.partStart;
    return in;
}

#endif // BLUETOOTHDATA_H
