#ifndef VOIPDBUSDATA_H
#define VOIPDBUSDATA_H
#define SHARED_SIZE 1500000
#include <QDBusArgument>

enum VoipCallState
{
    CallStateUnknown,
    CallStateConnecting,
    CallStateConnected,
    CallStateDisconnected
};

struct VoipCallInfo
{
    QString remIpAddr;  // remote IP address
    bool isIncoming;
    int state;          // VoipCallState value
};

struct SharedUseData
{
    int width;//frame width
    int height;//frame height
    unsigned char buf[SHARED_SIZE]; //frame buf data
};

Q_DECLARE_METATYPE(VoipCallInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const VoipCallInfo &callInfo)
{
    argument.beginStructure();
    QString a1 = callInfo.remIpAddr;
    bool a2 = callInfo.isIncoming;
    int a3 = callInfo.state;
    argument << a1;
    argument << a2;
    argument << a3;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, VoipCallInfo &callInfo)
{
    QString a1;
    bool a2;
    int a3;
    argument.beginStructure();
    argument >> a1;
    argument >> a2;
    argument >> a3;
    argument.endStructure();
    callInfo.remIpAddr = a1;
    callInfo.isIncoming = a2;
    callInfo.state = a3;
    return argument;
}

#endif // VOIPDBUSDATA_H
