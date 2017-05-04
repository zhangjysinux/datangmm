#ifndef COMMONDATA
#define COMMONDATA

#include <QString>
#include <QMap>
#include <QDBusArgument>
#include <QDBusInterface>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QUuid>

#define CTS_PRINT (qDebug() <<"Sinux by Zhangjy adapter:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")
#ifdef ALL_NETWORK_TYPE
#define contacterServiceName "com.sinux.contact"
#endif
#ifdef LTE_NETWORK_TYPE
#define contacterServiceName "com.sinux.contactlte"
#endif
#ifdef ADHOC_TIANTONG_NETWORK_TYPE
#define contacterServiceName "com.sinux.contactadhoc"
#endif


enum NetworkType
{
    AD_HOC_NETWORK = 0x00000001,
    LTE_NETWORK = 0x00000010,
    SATELLITE_NETWORK = 0x00000100,
    DIGITAL_WALKIE_TALKIE_NETWORK = 0x00001000,
    BATTLE_NETWORK = 0x00010000,
    WIFI_NETWORK = 0x00100000,
    ADHOC_LTE_NETWORK = 0x01000000,
    UNKNOW_NETWORK = 0x00000000
};

//call state
enum VoipCallState
{
    CallStateUnknown,
    CallStateConnecting,
    CallStateConnected,
    CallStateDisconnected,
    CallStateTimeOut
};

struct PackageData
{
    unsigned int length;
    unsigned int id;
    unsigned int size;
    unsigned int maxSize;
    unsigned int index;
    int type;//content type
    bool isStart;
    bool isEnd;
    bool isCancel;
    QByteArray data;
    QString fileName;//if empty, not file
    QString uuid;
    QString sourceIp;
    int sourcePort;
    QString groupId;
    int fileType;
    QString gId;
    PackageData()
    {
        isCancel = false;
        uuid = QUuid::createUuid().toString();
    }
};

typedef QList<PackageData> PackageDataList;

typedef QMap<int,PackageData> IntPackageDataMap;

Q_DECLARE_METATYPE(PackageData)

typedef QMap<QString, QStringList> IdNetworkMap;

inline QDataStream &operator<<(QDataStream &out,const PackageData &data)
{
    out << data.length
        << data.id
        << data.size
        << data.maxSize
        << data.index
        << data.type
        << data.isStart
        << data.isEnd
        << data.isCancel
        << data.data
        << data.fileName
        << data.uuid
        << data.sourceIp
        << data.sourcePort
        << data.groupId
        << data.fileType
        << data.gId;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, PackageData &data)
{
    in >> data.length
       >> data.id
       >> data.size
       >> data.maxSize
       >> data.index
       >> data.type
       >> data.isStart
       >> data.isEnd
       >> data.isCancel
       >> data.data
       >> data.fileName
       >> data.uuid
       >> data.sourceIp
       >> data.sourcePort
       >> data.groupId
       >> data.fileType
       >> data.gId;
    return in;
}

inline QString convertNetworkTypeToString(const NetworkType &type)
{
    QString value;
    switch(type)
    {
    case AD_HOC_NETWORK:
        value = "ad_hoc_network";
        break;
    case LTE_NETWORK:
        value = "lte_network";
        break;
    case SATELLITE_NETWORK:
        value = "satellite_network";
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        value = "digital_walkie_talkie_network";
        break;
    case BATTLE_NETWORK:
        value = "battle_network";
        break;
    case ADHOC_LTE_NETWORK:
        value = "adhoc_lte_network";
        break;
    case WIFI_NETWORK:
        value = "wifi_network";
        break;
    case UNKNOW_NETWORK:
        value = "unknow_network";
        break;
    default:
        value = "unknow_network";
    }

    return value;
}

inline NetworkType convertStringToNetworkType(const QString &value)
{
    if(value == "ad_hoc_network")
        return AD_HOC_NETWORK;
    else if(value == "lte_network")
        return LTE_NETWORK;
    else if(value == "satellite_network")
        return SATELLITE_NETWORK;
    else if(value == "digital_walkie_talkie_network")
        return DIGITAL_WALKIE_TALKIE_NETWORK;
    else if(value == "battle_network")
        return BATTLE_NETWORK;
    else if(value == "adhoc_lte_network")
        return ADHOC_LTE_NETWORK;
    else if(value == "wifi_network")
        return WIFI_NETWORK;
    else if(value == "unknow_network")
        return UNKNOW_NETWORK;
    else
        return UNKNOW_NETWORK;

    return UNKNOW_NETWORK;
}

typedef struct _handlerdata
{
    int network;
    int dialState;
    QString dialNum;
    QDateTime start;
    QDateTime end;
    bool isMetting;
    QStringList mettingTarget;
public:
    _handlerdata()
    {
        network = 0;
        dialState = 0;
        dialNum = "";
        isMetting = false;
    }
    _handlerdata& operator=(const _handlerdata info)
    {
        network = info.network;
        dialState = info.dialState;
        dialNum = info.dialNum;
        start = info.start;
        end = info.end;
        isMetting = info.isMetting;
        mettingTarget = info.mettingTarget;

        return *this;
    }
} HandlerData;

Q_DECLARE_METATYPE(HandlerData)

inline QDBusArgument &operator<<(QDBusArgument &argument, const HandlerData &data)
{
    argument.beginStructure();
    argument << data.network << data.dialState << data.dialNum << data.start << data.end << data.isMetting << data.mettingTarget;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, HandlerData &data)
{
    argument.beginStructure();
    argument >> data.network >> data.dialState >> data.dialNum >> data.start >> data.end >> data.isMetting >> data.mettingTarget;
    argument.endStructure();

    return argument;
}

enum TransactionType
{
   CTRL_MSG_RELIABLE_TRANS = 1,                 ///< 指控报文(单播可靠)
   INTELLIGENCE_MSG_RELIABLE_TRANS = 2,         ///< 情报信息(单播可靠)
   CTRL_MSG_UNRELIABLE_TRANS = 3,               ///< 指控报文(单播不可靠/组播)
   INTELLIGENCE_MSG_UNRELIABLE_TRANS = 4,       ///< 情报信息(单播不可靠/组播)
   IM_TRANS= 5,                                 ///< 即时消息
   FILE_TRANS = 6,                              ///< 文件
   IMAGE_TRANS = 7,                             ///< 图像
   PTT_VOICE_TRANS = 8,                         ///< PTT语音
   APP_CTRL_CMD_TRANS = 9,                      ///< 应用信令
   VOICE_CALL_TRANS = 10,                       ///< 音频呼叫
   VIDEO_CALL_TRANS = 11,                       ///< 视频呼叫
   MUTIL_VOICE_CALL_TRANS = 12,                 ///< 多方语音
   MUTIL_VIDEO_CALL_TRANS = 13,                 ///< 多方视频
   POSITION_SHARED = 14, 					    ///< 位置共享
   OTHER_TRANS,                                 ///< 其他业务
   UNKNOW_TRANS = 0,                            ///< 未知业务
};

enum ErrorCode
{
    SOCKET_CONNECT_SUCCESS = 1,
    SOCKET_CONNECT_FAIL,
    PORT_NEGO_SUCCESS,
    PORT_NEGO_FAIL,
    DIAL_SUCCESS,
    DIAL_FAIL,
};

enum DialTransStatus
{
    STATUS_NULL,
    STATUS_ACTIVE,
    STATUS_HELD,
    STATUS_DIALING,
    STATUS_ALERTING,
    STATUS_INCOMING,
    STATUS_WAITING,
    STATUS_DISCONNECTED,
};


/**
 * @brief user信息
 *
 */
typedef struct _userInfo
{
    int userId;               ///< user ID
    //NetworkType network;      ///< 网络类型
    int network;
    QString addr;             ///< 通讯地址(IP地址或者电话号码)
    QString appId;            ///< 应用ID
    int optId;                ///< 业务ID
    QString userName;         ///< 用户名字
    QString lte_ip;           ///< LTE网络的IP地址
    QString ad_hoc_ip;        ///< 自组网的IP地址
    QString satellite_ip;     ///< 天通对应的IP地址
    QString satellite_phone_number; ///< 天通对应的电话号码
    QString digital_number;   ///< 数字对讲的电话号码
    QString battle_ip;        ///< 战互网的IP地址
    QString wifi_ip;

public:
    /**
     * @brief 构造函数，初始化userinfo
     *
     */
    _userInfo()
    {
        userId = -1;
        network = 0;//UNKNOW_NETWORK;
        addr = "";
        appId = "";
        optId = -1;
        userName = "";
        lte_ip = "";
        ad_hoc_ip = "";
        satellite_ip = "";
        satellite_phone_number = "";
        digital_number = "";
        battle_ip = "";
        wifi_ip = "";
    }

    /**
     * @brief 重载"=",可直接用一个UserInfo对象对另一个UserInfo对象进行赋值
     *
     */
    _userInfo& operator=(const _userInfo info)
    {
        userId = info.userId;
        network = info.network;
        addr = info.addr;
        appId = info.appId;
        optId = info.optId;
        userName = info.userName;
        lte_ip = info.lte_ip;
        ad_hoc_ip = info.ad_hoc_ip;
        satellite_ip = info.satellite_ip;
        satellite_phone_number = info.satellite_phone_number;
        digital_number = info.digital_number;
        battle_ip = info.battle_ip;
        wifi_ip = info.wifi_ip;
        return *this;
    }
} UserInfo;


Q_DECLARE_METATYPE(UserInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const UserInfo &user)
{
    argument.beginStructure();
    argument << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, UserInfo &user)
{
    argument.beginStructure();
    argument >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
        >> user.lte_ip >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip >> user.wifi_ip;

    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const UserInfo &user)
{
    out << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, UserInfo &user)
{
    in >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
       >> user.lte_ip >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
       >> user.digital_number >> user.battle_ip >> user.wifi_ip;
    return in;
}

inline void createPath(const QString &path)
{
    QStringList pathList = path.split("/", QString::SkipEmptyParts);
    QDir dir = QDir::rootPath();
    foreach(const QString &path, pathList)
    {
        if(dir.exists(path))
        {
            dir.cd(path);
        }
        else
        {
            if(dir.mkdir(path))
            {
                qDebug() << "create folder: " << dir.absolutePath() + "/" + path;
            }
        }
    }
}

inline QString getUseablePath(const QString &path)
{
    QString useablePath = path;
    for(int i=1; ;i++)
    {
        if(QFile::exists(useablePath))
        {
            QString name = path.section("/", -1);
            QString pa = path.section("/", 0, -2);

            QString nameLeft = name.section(".", 0, -2);
            QString nameRight = name.section(".", -1);
            if(name.contains("."))
            {
                useablePath = pa + "/" + nameLeft + QString("(%1).").arg(i) + nameRight;
            }
            else
            {
                useablePath = pa + "/" + nameRight + QString("(%1)").arg(i);
            }
        }
        else
        {
            break;
        }
    }
    return useablePath;
}

inline bool cpFile(const QString &sourceFileName, const QString &destPath)
{
    qDebug() << "copy lte voice file call.............................";
    QFile destFile(destPath + "/" + sourceFileName.section("/", -1));
    if(destFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "open file write:" << destFile.fileName();

        QFile sourceFile(sourceFileName);
        if(sourceFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "open file read: " << sourceFile.fileName();

            while(!sourceFile.atEnd())
            {
                destFile.write(sourceFile.read(1024));
            }

            sourceFile.close();
        }
        else
        {
            qDebug() << "open file fail: " << sourceFile.errorString();
            return false;
        }

        destFile.flush();
        destFile.close();
    }
    else
    {
        qDebug() << "open file fail: " << destFile.errorString();
        return false;
    }
}

inline bool cpFile2(const QString &sourceFileName, const QString &destPath)
{
    qDebug() << "copy lte voice file call.............................";
    QFile destFile(destPath);
    if(destFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "open file write:" << destFile.fileName();

        QFile sourceFile(sourceFileName);
        if(sourceFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "open file read: " << sourceFile.fileName();

            while(!sourceFile.atEnd())
            {
                destFile.write(sourceFile.read(1024));
            }

            sourceFile.close();
        }
        else
        {
            qDebug() << "open file fail: " << sourceFile.errorString();
            return false;
        }

        destFile.flush();
        destFile.close();
    }
    else
    {
        qDebug() << "open file fail: " << destFile.errorString();
        return false;
    }
}

inline bool saveFile(const QByteArray& content, const QString& fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(content);
        file.flush();
        file.close();
        return true;
    }
    qDebug() << "file.open: " << file.errorString();
    return false;
}

inline QByteArray loadFile(const QString& fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        return file.readAll();
    }
    qDebug() << "file.open: " << file.errorString();
    return "";
}

inline QString convertDateTimeToString(const QDateTime &date)
{
    QString value = date.toString("yyyyMMddhhmmsszzz");
    return value;
}

inline QDateTime convertStringToDateTime(const QString &date)
{
    QDateTime value = QDateTime::fromString(date, "yyyyMMddhhmmsszzz");
    return value;
}

#endif // COMMONDATA

