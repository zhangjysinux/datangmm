#ifndef CSDATA
#define CSDATA

#include <QObject>
#include <QDBusArgument>
#include "../../commondata/datatype/commondata.h"
#include <QDateTime>
///*
///
#include "../../commondata/datatype/contacterdata.h"
//typedef QPair<int, QString> PhoneNumber;
//typedef QList<PhoneNumber> PhoneNumbers;

enum ChangeStates{

    HANDLER_ADD = 1,
    HANDLER_REMOVE = 2,
    HANDLER_UPDATA = 3,
    HANDLER_UNDEFINE = 0

};

typedef struct _handlerManager
{
    QString handlerId;               ///< user ID
    QString dialNumber;              ///< 所拨打的电话号码
    int network;                     ///< 网络类型  取自 common.h::NetworkType
    int state;                       ///< 通话业务的状态  取自 common.h::DialTransStatus
    bool isIncoming;                 ///< ture : 当前通话是呼入电话; false : 当前是呼出电话
    bool isMultiparty;               ///< ture : 当前通话是电话会议; false : 当前通话不是电话会议
    bool isMuted;                    ///< true : 当前是静音 ;       false : 当前不是静音
    bool isHandsfree ;               ///< true : 当前是免提 ;       false : 当前不是免提
    bool isRecord ;                  ///< true : 当前是录音 ;       false : 当前不是录音
    QDateTime startTime;
    bool isAudio;
    bool isOnline;
public:
    _handlerManager()
    {
        handlerId = "";
        dialNumber = "";
        network = 0;
        state = 0;
        isIncoming = false;
        isMultiparty = false;
        isMuted = false;
        isHandsfree = false;
        isRecord = false;
        startTime = QDateTime::currentDateTime();
        isAudio = true;
        isOnline = true;
    }

    _handlerManager& operator=(const _handlerManager info)
    {
        handlerId = info.handlerId;
        dialNumber = info.dialNumber;
        network = info.network;
        state = info.state;
        isIncoming = info.isIncoming;
        isMultiparty = info.isMultiparty;
        isMuted = info.isMuted;
        isHandsfree = info.isHandsfree;
        isRecord = info.isRecord;
        startTime = info.startTime;
        isAudio = info.isAudio;
        isOnline = info.isOnline;
        return *this;
    }

}HandlerManager;
typedef QList<HandlerManager> HandlerManagerList;
Q_DECLARE_METATYPE(HandlerManager)
inline QDBusArgument &operator<<(QDBusArgument &argument, const HandlerManager &user)
{
    argument.beginStructure();
    argument << user.handlerId << user.dialNumber << user.network << user.state
             << user.isIncoming << user.isMultiparty << user.isMuted << user.isHandsfree
             << user.isRecord << user.startTime << user.isAudio << user.isOnline;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, HandlerManager &user)
{
    argument.beginStructure();
    argument >> user.handlerId >> user.dialNumber >> user.network >> user.state
             >> user.isIncoming >> user.isMultiparty >> user.isMuted >> user.isHandsfree
             >> user.isRecord >> user.startTime >> user.isAudio >> user.isOnline;
    argument.endStructure();
    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const HandlerManager &handler)
{
    out << handler.handlerId << handler.dialNumber << handler.network << handler.state
             << handler.isIncoming << handler.isMultiparty << handler.isMuted << handler.isHandsfree
             << handler.isRecord << handler.startTime << handler.isAudio << handler.isOnline;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, HandlerManager &handler)
{
    in >> handler.handlerId >> handler.dialNumber >> handler.network >> handler.state
       >> handler.isIncoming >> handler.isMultiparty >> handler.isMuted >> handler.isHandsfree
       >> handler.isRecord >> handler.startTime >> handler.isAudio >> handler.isOnline;
    return in;
}

typedef struct _dialManagerData
{
    QString audioMode;
    bool isMicrophoneMuted;
    bool record;

public:
    _dialManagerData()
    {
        audioMode = "earpiece";
        isMicrophoneMuted = false;
        record = false;
    }

    _dialManagerData& operator=(const _dialManagerData info)
    {
        audioMode = info.audioMode;
        isMicrophoneMuted = info.isMicrophoneMuted;
        record = info.record;
        return *this;
    }
}DialManagerData;

inline QDataStream &operator<<(QDataStream &out,const DialManagerData &dialManagerData)
{
    out << dialManagerData.audioMode << dialManagerData.isMicrophoneMuted << dialManagerData.record;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, DialManagerData &dialManagerData)
{
    in >> dialManagerData.audioMode >> dialManagerData.isMicrophoneMuted >> dialManagerData.record;
    return in;
}

typedef struct _cmdData
{
    QString cmd;
    QString handlerId;
    QString arg;
    int netWork;
    bool enable;
    UserInfo info;
public:
    _cmdData()
    {
        cmd = "";
        handlerId = "";
        netWork = -1;
        enable = false;
    }
}CmdData;

inline QDataStream &operator<<(QDataStream &out,const CmdData &cmdData)
{
    out << cmdData.cmd << cmdData.handlerId << cmdData.arg << cmdData.netWork << cmdData.enable << cmdData.info;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, CmdData &cmdData)
{
    in >> cmdData.cmd >> cmdData.handlerId >> cmdData.arg >> cmdData.netWork >> cmdData.enable >> cmdData.info;
    return in;
}

#endif // CSDATA
