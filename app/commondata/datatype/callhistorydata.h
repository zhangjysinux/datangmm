#ifndef CALLHISTORYDATA_H
#define CALLHISTORYDATA_H
#include <QString>
#include <QDBusArgument>
#include <QDateTime>
#include "commondata.h"
enum HistoryType
{
    IN_DIAL = 1,
    OUT_DIAL = 2,
    MISS_DIAL = 3,
    MEETING_DIAL = 4,
    UNDEFINE_DIAL = 0
};

//enum HistoryNetwork
//{
//    PS_NETWORK_VOICE_DIAL = 1,
//    PS_NETWORK_VIDEO_DIAL = 2,
//    CS_NETWORK_VOICE_DIAL = 3,
//    CS_NETWORK_VOICE_MUL_DIAL = 4,
//    PS_NETWORK_VOICE_MUL_DIAL = 5,
//    UNDEFINE_NETWORK_DIAL = 0
//};
enum HistoryNetwork
{
    LTE_NETWORK_VOICE_DIAL = 1,
    LTE_NETWORK_VIDEO_DIAL = 2,
    CS_NETWORK_VOICE_DIAL = 3,
    CS_NETWORK_VOICE_MUL_DIAL = 4,
    PS_NETWORK_VOICE_MUL_DIAL = 5,
    ADC_NETWORK_VOICE_DIAL = 6,
    ADC_NETWORK_VIDEO_DIAL = 7,

    LTE_NETWORK_METTING_DIAL = 8,
    CS_NETWORK_VIDEO_DIAL = 9,
    ADC_NETWORK_METTING_DIAL = 10,
    WIFI_NETWORK_VOICE_DIAL = 11,
    WIFI_NETWORK_VIDEO_DIAL = 12,
    WIFI_NETWORK_METTING_DIAL = 13,
    BATTLE_NETWORK_VOICE_DIAL = 14,
    BATTLE_NETWORK_VIDEO_DIAL = 15,
    BATTLE_NETWORK_METTING_DIAL = 16,
    UNDEFINE_NETWORK_DIAL = 0
};

enum ChangeState
{
    HISTORY_ADD = 1,
    HISTORY_REMOVE = 2,
    HISTORY_UPDATE = 3,
    HISTORY_INIT = 4,
    UNDEFINE_HISTORY_STATE = 0
};

struct CallHistory
{
    QString dialHistoryId;
    QStringList dialTarget;//id
    QStringList dialTargetNumber;//number
    int dialType;
    QString dialTime;
    int dialNetworkType;
    QDateTime dialDateStart;
    QDateTime dialDateEnd;
};
typedef QList<CallHistory> CallHistoryList;
Q_DECLARE_METATYPE(CallHistory)
Q_DECLARE_METATYPE(CallHistoryList)

inline QDBusArgument &operator<<(QDBusArgument &argument, const CallHistory &callHistory)
{
    argument.beginStructure();
    QString dialHistoryId = callHistory.dialHistoryId;
    QStringList dialTarget = callHistory.dialTarget;
    QStringList dialTargetNumber = callHistory.dialTargetNumber;
    int dialType = callHistory.dialType;
    QString dialTime = callHistory.dialTime;
    int dialNetworkType = callHistory.dialNetworkType;
    QDateTime dialDateStart = callHistory.dialDateStart;
    QDateTime dialDateEnd = callHistory.dialDateEnd;

    argument << dialHistoryId;
    argument << dialTarget;
    argument << dialTargetNumber;
    argument << dialType;
    argument << dialTime;
    argument << dialNetworkType;
    argument << dialDateStart;
    argument << dialDateEnd;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, CallHistory &callHistory)
{
    QString dialHistoryId;
    QStringList dialTarget;
    QStringList dialTargetNumber;
    int dialType;
    QString dialTime;
    int dialNetworkType;
    QDateTime dialDateStart;
    QDateTime dialDateEnd;

    argument.beginStructure();
    argument >> dialHistoryId;
    argument >> dialTarget;
    argument >> dialTargetNumber;
    argument >> dialType;
    argument >> dialTime;
    argument >> dialNetworkType;
    argument >> dialDateStart;
    argument >> dialDateEnd;

    argument.endStructure();

    callHistory.dialHistoryId = dialHistoryId;
    callHistory.dialTarget = dialTarget;
    callHistory.dialTargetNumber = dialTargetNumber;
    callHistory.dialType = dialType;
    callHistory.dialTime = dialTime;
    callHistory.dialNetworkType = dialNetworkType;
    callHistory.dialDateStart = dialDateStart;
    callHistory.dialDateEnd = dialDateEnd;
    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const CallHistory &callHistory)
{
    out << callHistory.dialHistoryId;
    out << callHistory.dialTarget;
    out << callHistory.dialTargetNumber;
    out << callHistory.dialType;
    out << callHistory.dialTime;
    out << callHistory.dialNetworkType;
    out << callHistory.dialDateStart;
    out << callHistory.dialDateEnd;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, CallHistory &callHistory)
{
    in >> callHistory.dialHistoryId;
    in >> callHistory.dialTarget;
    in >> callHistory.dialTargetNumber;
    in >> callHistory.dialType;
    in >> callHistory.dialTime;
    in >> callHistory.dialNetworkType;
    in >> callHistory.dialDateStart;
    in >> callHistory.dialDateEnd;
    return in;
}

inline QString callhistoryNetworkTypeConvertToString(HistoryNetwork network)
{
    QString value;
    switch(network)
    {
    case LTE_NETWORK_VOICE_DIAL:
        value = "LTE_NETWORK_VOICE_DIAL";
        break;
    case LTE_NETWORK_VIDEO_DIAL:
        value = "LTE_NETWORK_VIDEO_DIAL";
        break;
    case CS_NETWORK_VOICE_DIAL:
        value = "CS_NETWORK_VOICE_DIAL";
        break;
    case CS_NETWORK_VOICE_MUL_DIAL:
        value = "CS_NETWORK_VOICE_MUL_DIAL";
        break;
    case PS_NETWORK_VOICE_MUL_DIAL:
        value = "PS_NETWORK_VOICE_MUL_DIAL";
        break;
    case ADC_NETWORK_VOICE_DIAL:
        value = "ADC_NETWORK_VOICE_DIAL";
        break;
    case ADC_NETWORK_VIDEO_DIAL:
        value = "ADC_NETWORK_VIDEO_DIAL";
        break;
    case LTE_NETWORK_METTING_DIAL:
        value = "LTE_NETWORK_METTING_DIAL";
        break;
    case CS_NETWORK_VIDEO_DIAL:
        value = "CS_NETWORK_VIDEO_DIAL";
        break;
    case ADC_NETWORK_METTING_DIAL:
        value = "ADC_NETWORK_METTING_DIAL";
        break;
    case WIFI_NETWORK_VOICE_DIAL:
        value = "WIFI_NETWORK_VOICE_DIAL";
        break;
    case WIFI_NETWORK_VIDEO_DIAL:
        value = "WIFI_NETWORK_VIDEO_DIAL";
        break;
    case WIFI_NETWORK_METTING_DIAL:
        value = "WIFI_NETWORK_METTING_DIAL";
        break;
    case UNDEFINE_NETWORK_DIAL:
        value = "UNDEFINE_NETWORK_DIAL";
        break;
    default:
        value = "UNDEFINE_NETWORK_DIAL";
    }
    return value;
}

inline HistoryNetwork callhistoryStringConvertToNetworkType(const QString &str)
{
    HistoryNetwork value;

    if(str == "LTE_NETWORK_VOICE_DIAL")
        value = LTE_NETWORK_VOICE_DIAL;
    else if(str == "LTE_NETWORK_VIDEO_DIAL")
        value = LTE_NETWORK_VIDEO_DIAL;
    else if(str == "CS_NETWORK_VOICE_DIAL")
        value = CS_NETWORK_VOICE_DIAL;
    else if(str == "CS_NETWORK_VOICE_MUL_DIAL")
        value = CS_NETWORK_VOICE_MUL_DIAL;
    else if(str == "PS_NETWORK_VOICE_MUL_DIAL")
        value = PS_NETWORK_VOICE_MUL_DIAL;
    else if(str == "ADC_NETWORK_VOICE_DIAL")
        value = ADC_NETWORK_VOICE_DIAL;
    else if(str == "ADC_NETWORK_VIDEO_DIAL")
        value = ADC_NETWORK_VIDEO_DIAL;
    else if(str == "LTE_NETWORK_METTING_DIAL")
        value = LTE_NETWORK_METTING_DIAL;
    else if(str == "CS_NETWORK_VIDEO_DIAL")
        value = CS_NETWORK_VIDEO_DIAL;
    else if(str == "ADC_NETWORK_METTING_DIAL")
        value = ADC_NETWORK_METTING_DIAL;
    else if(str == "WIFI_NETWORK_VOICE_DIAL")
        value = WIFI_NETWORK_VOICE_DIAL;
    else if(str == "WIFI_NETWORK_VIDEO_DIAL")
        value = WIFI_NETWORK_VIDEO_DIAL;
    else if(str == "WIFI_NETWORK_METTING_DIAL")
        value = WIFI_NETWORK_METTING_DIAL;
    else if(str == "UNDEFINE_NETWORK_DIAL")
        value = UNDEFINE_NETWORK_DIAL;
    else
        value = UNDEFINE_NETWORK_DIAL;

    return value;
}

#endif // CALLHISTORYDATA_H
