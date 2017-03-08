#ifndef COMMON_DEF_JJF_H
#define COMMON_DEF_JJF_H

#include <QString>
#include <QThread>
#include <QMetaType>
#include <QStringList>
#include <QDBusMetaType>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define MAX_MEMBER_COUNT    64


namespace QT_DTT_JJF
{

#define DEFAULT_NEGOTIATION_PORT    7182
#define CTS_PRINT (qDebug() <<"CommTransService:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")
#define CTS_PRINT_CLIENT (qDebug() << "Client:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")
#define CTS_PRINT_SERVER (qDebug() << "Server:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")


#define GROUP_PRINT  (qDebug() << "Group:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")
#define GROUP_TRACE_BEGIN  (qDebug() << "Group" << __FILE__ << __FUNCTION__ << "()" << __LINE__ << "begin" << endl)
#define GROUP_TRACE_END (qDebug() << "Group" << __FILE__ << __FUNCTION__ << "()" << __LINE__ << "end " << endl)

// app ids
#define COMM_TRANS_APP_ID_PHONEBOOK         "com.dtt.phonebook"
#define COMM_TRANS_APP_ID_2D_GIS            ""
#define COMM_TRANS_APP_ID_3D_GIS            ""
/** jjf sign 20160310 begin */
#define COMM_TRANS_APP_ID_MESSAGE           "com.syberos.message"
/** jjf sign 20160310 end */
#define COMM_TRANS_APP_ID_CONTACTSERVICE    ""
#define COMM_TRANS_SERVICE_ID_GROUPINTERCOM "com.dtt.groupintercom"

#define GROUP_INTERCOM_HETEROGENEOUS_COUNT  5

#define TEST_IP      "192.168.36.235"
#define TEST_NUMBER  "18709230264"
#define TEST_NUMBER1 "13619258821"

#define VOIP_SIP_PORT 7010
#define VOIP_RTP_PORT 6010
#define VOIP_RTP_PORT_RANGE 14

#define LTE_VOIP_SIP_PORT 7012
#define LTE_VOIP_RTP_PORT 6024
#define LTE_VOIP_RTP_PORT_RANGE 14

#define DEFAULT_UDP_PORT   23232
#define DEFAULT_TCP_PORT   8010

#define FILE_COMMUNICATE_PROPRIETARY_DIRECTORY "/data/home/user/JJF"
enum MsgType{Msg, FileName, CancelSend, Refuse, Accept, Complete};

/**
 * @brief 网络类型
 *
 */
enum NetworkType
{
    /*
     * 自组网
     */
    AD_HOC_NETWORK = 0x00000001,
    /*
     * LTE
     */
    LTE_NETWORK = 0x00000010,
    /*
     * 天通
     */
    SATELLITE_NETWORK = 0x00000100,
    /*
     * 数字对讲
     */
    DIGITAL_WALKIE_TALKIE_NETWORK = 0x00001000,
    /*
     * 战互网
     */
    BATTLE_NETWORK = 0x00010000,
    /*
     * test for sinux
     */
    WIFI_NETWORK = 0x00100000,
    /*
     * 未知网络
     */
    UNKNOW_NETWORK = 0x00000000,
};

/**
 * @brief 通道类型
 *
 */
enum AisleType
{
    /*
     * cs
     */
    CS_AISLE = 1,
    /*
     * ps
     */
    PS_AISLE,
    /*
     * 数字对讲
     */
    PTT_AISLE,
    MANET_AISLE,
};

enum optType
{
    CREATE_GROUP,
    DELETE_GROUP,
    UPDATE_GROUP,
};


enum DirectState
{
    /*
     * Client端
     */
    COMM_TRANS_CLIENT = 1,
    /*
     * Server端
     */
    COMM_TRANS_SERVER,
};

/**
 * @brief 业务类型
 *
 */
enum TransactionType
{
    /*
     * 指控报文(单播可靠)
     */
    CTRL_MSG_RELIABLE_TRANS = 1,
    /*
     * 情报信息(单播可靠)
     */
    INTELLIGENCE_MSG_RELIABLE_TRANS,
    /*
     * 指控报文(单播不可靠)
     */
    CTRL_MSG_UNRELIABLE_TRANS,
    /*
     * 情报信息(单播不可靠)
     */
    INTELLIGENCE_MSG_UNRELIABLE_TRANS,
    /*
     * 即时消息
     */
    IM_TRANS,
    /*
     * 文件
     */
    FILE_TRANS,
    /*
     * 图像
     */
    IMAGE_TRANS,
    /*
     * PTT语音
     */
    PTT_VOICE_TRANS,
    /*
     * 应用信令
     */
    APP_CTRL_CMD_TRANS,
    /*
     * 音频呼叫
     */
    VOICE_CALL_TRANS,
    /*
     * 视频呼叫
     */
    VIDEO_CALL_TRANS,
    /*
     * 多方语音
     */
    MUTIL_VOICE_CALL_TRANS,
    /*
     * 多方视频
     */
    MUTIL_VIDEO_CALL_TRANS,
    /*
     * 位置共享
     */
    POSITION_SHARED,
    /*
     * 切网操作
     */
    SWITCH_NETWORK,
    /*
     * 创建群组-创建人发起创建请求.1
     */
    GROUP_CREATE,
    /*
     * 创建群组-成员接收消息后向创建人反馈正在准备中.2
     */
    GROUP_CREATE_ACK_PREPARED,
    /*
     * 创建群组-创建人确认完各个成员的准备反馈消息.3
     */
    GROUP_CREATE_CONFIRMED,
    /*
     * 创建群组-成员向创建人反馈就绪.4
     */
    GROUP_CREATE_ACK_READY,
    /*
     * 创建群组-创建人确认创建群组完成.5
     */
    GROUP_CREATE_DONE,
    /*
     * 更新群组管理员
     */
    GROUP_UPDATE_ADMINS,
    /*
     * 更新群组名称
     */
    GROUP_UPDATE_NAME,
    /*
     * 增加群组成员
     */
    GROUP_ADD_MEMBERS,
    /*
     * 增加群组成员结果反馈
     */
    GROUP_ADD_MEMBER_ACK_RSP,
    /*
     * 添加成员最终结果反馈
     */
    GROUP_ADD_MEMBERS_COMPLETED,
    /*
     * 移除群组成员
     */
    GROUP_REMOVE_MEMBERS,
    /*
     * 移除群组成员结果反馈
     */
    GROUP_REMOVE_MEMBER_ACK_RSP,
    /*
     * 移除成员最终结果反馈
     */
    GROUP_REMOVE_MEMBERS_COMPLETED,
    /*
     * 删除群组
     */
    GROUP_REMOVE,
    /*
      * 群组编辑 jjf sign 20160709
      */
    GROUP_UPDATE,
    /*
     * 删除群组结果反馈
     */
    GROUP_REMOVE_ACK_RSP,
    /*
     * 删除群组最终结果反馈
     */
    GROUP_REMOVE_COMPLETED,
    /*
     * 指控报文(组播)
     */
    GROUP_CTRL_MSG_UNRELIABLE_TRANS,
    /*
     * 情报信息(组播)
     */
    GROUP_INTELLIGENCE_MSG_UNRELIABLE_TRANS,
    /*
     * PTT语音(组播)
     */
    GROUP_PTT_VOICE_TRANS,
    /*
     * 其他业务
     */
    OTHER_TRANS,
    /*
     * 未知业务
     */
    UNKNOW_TRANS = 0,
};

enum MessageType
{
    MSG_TYPE_TEXT,
    MSG_TYPE_FILE,
    MSG_TYPE_PICTURE,
    MSG_TYPE_RECORD,
    MSG_TYPE_VIDEO,
    MSG_TYPE_CMD,
    MSG_TYPE_POC_PTT_RECORD,
    MSG_TYPE_POC_PTT_PLAYER
};

enum OperateType
{
    OPERATE_CREATE,
    OPERATE_DELETE,
    OPERATE_UPDATE,
    OPERATE_SENT,
    OPERATE_RECV,
    OPERATE_GET_MEMBERS,
    OPERATE_UPLOAD,
    OPERATE_DOWNLOAD
};

enum MessageStatus
{
    MSG_STATUS_SENDING,
    MSG_STATUS_SENDED,
    MSG_STATUS_SEND_ERR,
    MSG_STATUS_RECEIVERING,
    MSG_STATUS_RECEIVERD,
    MSG_STATUS_RECEIVERED
};

/*
 *  线程状态
 */
enum ThreadState
{
  IDLE = 1,
  BUSY,
  N_STATES
};

enum ThreadType
{
    DEFAULT_THREAD = 0,
    TRANS_THREAD_MESSAGE,
    TRANS_THREAD_GROUP_MESSAGE,
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

enum GroupStatus
{
    GSTATUS_UNKNOWN = -1,
    GSTATUS_CREATE_POC_SUCCESS,
    GSTATUS_CREATE_POC_FAIL,
    GSTATUS_UPDATE_POC_SUCCESS,
    GSTATUS_UPDATE_POC_FAIL,
    GSTATUS_CREATE_FAIL,
    GSTATUS_CREATE_PREREADY,
    GSTATUS_CREATE_READY,
    GSTATUS_CREATE_SUCCESS,
    GSTATUS_DELETE_SUCCESS,
    GSTATUS_DELETE_FAIL,
    GSTATUS_POC_NO_LOGIN,
    GSTATUS_POC_LOGIN_NO_ID_OR_PWD,
    GSTATUS_POC_LOGIN_SUCCESS,
    GSTATUS_POC_LOGIN_FAILURE,
    GSTATUS_POC_LOGOUT_SUCCESS,
    GSTATUS_POC_LOGOUT_FAILURE,
    GSTATUS_SENT_MSG_SUCCESS,
    GSTATUS_SENT_MSG_FAIL,
    GSTATUS_FILE_UPLOAD_SUCCESS,
    GSTATUS_FILE_UPLOAD_FAIL,
    GSTATUS_FILE_DOWNLOAD_SUCCESS,
    GSTATUS_FILE_DOWNLOAD_FAIL,
};

enum RefuseCause
{
    REMOTE_REFUSE = 1,
    LOCAL_REFUSE = 2,
    LOCAL_ERROR,
    NETWORK_ERROR_REFUSE,
    RECEIVE_FILE_NAME_REPEAT,
};

/**
 * @brief user信息
 *
 */
typedef struct _userInfo
{
    /*
     * user ID
     */
    int userId;
    /*
     * 网络类型
     */
    int network;
    /*
     * 通讯地址(IP地址或者电话号码)
     */
    QString addr;
    /*
     * 应用ID
     */
    QString appId;
    /*
     * 业务ID
     */
    int optId;
    /*
     * 用户名字
     */
    QString userName;
    /*
     * LTE网络的IP地址
     */
    QString lte_ip;
    /*
     * LTE网络POC服务器的用户登录账户
     */
    QString lte_pocId;
    /*
     * 自组网的IP地址
     */
    QString ad_hoc_ip;
    /*
     * 天通对应的IP地址
     */
    QString satellite_ip;
    /*
     * 天通对应的电话号码
     */
    QString satellite_phone_number;
    /*
     * 数字对讲的电话号码
     */
    QString digital_number;
    /*
     * 战互网的IP地址
     */
    QString battle_ip;
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
        lte_pocId = "";
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
        lte_pocId =info.lte_pocId;
        ad_hoc_ip = info.ad_hoc_ip;
        satellite_ip = info.satellite_ip;
        satellite_phone_number = info.satellite_phone_number;
        digital_number = info.digital_number;
        battle_ip = info.battle_ip;
        wifi_ip = info.wifi_ip;
        return *this;
    }
} UserInfo;

/**
 * @brief  文件信息
 *
 */
struct FileInfo
{
    /*
     * 业务ID
     */
    int optId;
    /*
     * 网络类型
     */
    int network;
    /*
     * 是否接收
     */
    bool isAccept;
    /*
     * 应用名称
     */
    QString appName;
    /*
     * 应用ID
     */
    QString appId;
    /*
     * 发送端用户名称
     */
    QString sendUserName;
    /*
     * 接收端用户名称
     */
    QString receiveUserName;
    /*
     * 发送端文件名称
     */
    QString sendFilePath;
    /*
     * 接收端文件名称
     */
    QString receiveFilePath;
    /*
     * 发送端地址
     */
    QString sendAddress;
    /*
     * 接收端地址
     */
    QString receiveAddress;
    /*
     * 备注
     */
    QString remark;

public:
    /**
     * @brief 构造函数，初始化FileInfo
     *
     */
    FileInfo()
    {
        int optId = -1;
        int network = -1;
        bool isAccept = false;
        QString appName = "";
        QString appId = "";
        QString sendUserName = "";
        QString receiveUserName = "";
        QString sendFilePath = "";
        QString receiveFilePath = "";
        QString sendAddress = "";
        QString receiveAddress = "";
        QString remark = "";
    }

    /**
      * @brief 重载"=",可直接用一个FileInfo对象对另一个FileInfo对象进行赋值
      *
      */
    FileInfo& operator=(const FileInfo &info)
    {
        optId = info.optId;
        network = info.network;
        isAccept = info.isAccept;
        appName = info.appName;
        appId = info.appId;
        sendUserName = info.sendUserName;
        receiveUserName = info.receiveUserName;
        sendFilePath = info.sendFilePath;
        receiveFilePath = info.receiveFilePath;
        sendAddress = info.sendAddress;
        receiveAddress = info.receiveAddress;
        remark = info.remark;

        return *this;
    }
};

struct FileQueue
{
    FileInfo fileInfo;          //file info of queue
    int controlId;              // control id
    int dataId;                 // data id
    QString startTime;          //start time
};

struct KeyInfo
{
    int sendThreadMark;         //send thread mark
    int receiveThreadMark;      //receive thread mark
    int optId;                  //opt id
    bool isAccept;              //accept
    QString appId;              //app id
    QString sendFilePath;       //send file path
    QString receiveFilePath;    //receive file path
    QString sendAddress;        //send address
    QString receiveAddress;     //receive address
    QString sendUserName;       //send user name
    QString receiveUserName;    //receive user name
    QString remark;

public:
    /**
     * @brief 构造函数，初始化KeyInfo
     *
     */
    KeyInfo()
    {
        int sendThreadMark = -1;
        int receviceThreadMark = -1;
        int optId = -1;
        bool isAccept = false;
        QString appId = "";
        QString sendFilePath = "";
        QString receiveFilePath = "";
        QString sendAddress = "";
        QString receiveAddress = "";
        QString sendUserName = "";
        QString receiveUserName = "";
        QString remark = "";
    }

    /**
      * @brief 重载"=",可直接用一个KeyInfo对象对另一个KeyInfo对象进行赋值
      *
      */
    KeyInfo& operator=(const KeyInfo &info)
    {
        sendThreadMark = info.sendThreadMark;
        receiveThreadMark = info.receiveThreadMark;
        optId = info.optId;
        isAccept = info.isAccept;
        appId = info.appId;
        sendFilePath = info.sendFilePath;
        receiveFilePath = info.receiveFilePath;
        sendAddress = info.sendAddress;
        receiveAddress = info.receiveAddress;
        sendUserName = info.sendUserName;
        receiveUserName = info.receiveUserName;
        remark = info.remark;

        return *this;
    }
};

struct DatabaseInfo
{
    int threadMark;                 //thread mark
    int threadStatus;               //thread status
    int optId;                      //opt id
    int direction;                  //direction
    int status;                     //status
    int networkType;                //network type
    QString appId;                  //app id
    QString filePath;               //file path
    QString address;                //address
    QString userName;               //user name
    QString startTime;              //start time
    QString endTime;                //end time
    QString lastUpdateTime;         //last update time
    QString remark;

public:
    /**
     * @brief 构造函数，初始化DatabaseInfo
     *
     */
    DatabaseInfo()
    {
        threadMark = -1;
        threadStatus = -1;
        optId = -1;
        direction = -1;
        status = -1;
        networkType = -1;
        appId = "";
        filePath = "";
        address = "";
        userName = "";
        startTime = "";
        endTime = "";
        lastUpdateTime = "";
        remark = "";
    }

    /**
      * @brief 重载"=",可直接用一个DatabaseInfo对象对另一个DatabaseInfo对象进行赋值
      *
      */
    DatabaseInfo& operator=(const DatabaseInfo &info)
    {
        threadMark = info.threadMark;
        threadStatus = info.threadStatus;
        optId = info.optId;
        direction = info.direction;
        status = info.status;
        networkType = info.networkType;
        appId = info.appId;
        filePath = info.filePath;
        address = info.address;
        userName = info.userName;
        startTime = info.startTime;
        endTime = info.endTime;
        lastUpdateTime = info.lastUpdateTime;
        remark = info.remark;

        return *this;
    }
};

/**
 * @brief  PS 短信信息
 *
 */
struct MessageInfo
{
    int id;                 //id
    int type;               //type
    QString startTime;      //start time
    QString remoteUid;      //phone number or network ip
    QString freeText;       //content
    QString aisleType;      //aisle type
    QString appId;          //app id
    int optId;              //opt id
    QString appName;        //app name
    QString remark;         //remark

public:
    /**
     * @brief 构造函数，初始化messageinfo
     *
     */
    MessageInfo()
    {
        id = -1;
        type = -1;
        startTime = "";
        remoteUid = "";
        freeText = "";
        aisleType = "";
        appId = "";
        optId = -1;
        appName = "";
        remark = "";
    }

    /**
      * @brief 重载"=",可直接用一个MessageInfo对象对另一个MessageInfo对象进行赋值
      *
      */
    MessageInfo& operator=(const MessageInfo &info)
    {
        id = info.id;
        type = info.type;
        startTime = info.startTime;
        remoteUid = info.remoteUid;
        freeText = info.freeText;
        aisleType = info.aisleType;
        appId = info.appId;
        optId = info.optId;
        appName = info.appName;
        remark = info.remark;

        return *this;
    }
};

/**
 * @brief 网络信息
 *
 */
typedef struct _networkInfo
{
    /*
     * 网络类型
     */
    NetworkType type;
    /*
     * type对应的网络的IP地址
     */
    QString ipAddr;
    /*
     * type对应的网络的电话号码
     */
    QString phoneNumber;
    /*
     * type对应的网络是否可用
     */
    bool enable;

public:
    /**
     * @brief 构造函数，初始化NetworkInfo
     *
     */
    _networkInfo()
    {
        type = UNKNOW_NETWORK;
        ipAddr = "";
        phoneNumber = "";
        enable = false;
    }

    /**
     * @brief 构造函数，初始化NetworkInfo
     *
     */
    _networkInfo(NetworkType inType, QString inIpAddr, QString inPhoneNumber, bool inEnable)
    {
        type = inType;
        ipAddr = inIpAddr;
        phoneNumber = inPhoneNumber;
        enable = inEnable;
    }
} NetworkInfo;

/*
 * 业务对应的信息
 */
typedef struct _transmsg
{
    /*
     * 通讯对象的user信息
     */
    UserInfo user;
    /*
     * 通讯数据
     */
    QByteArray data;
    /*
     * 完成此业务对应的线程ID
     */
    int threadId;
} TransMsg;

/*
 * 业务线程信息
 */
typedef struct TransactionInfo
{
    /*
     * 线程ID
     */
    int     threadId;
    /*
     * 线程name
     */
    QString threadName;
    /*
     * 保留数据(系统使用)
     */
    quint32 reverse;
} TransactionInfo;

/*
 * 群组信息
 */
struct GroupInfo
{
    /*
     * 群组编号
     */
    QString         id;
    /*
     * 群组网络类型
     */
    int             network;
    /*
     * 群组地址, 根据network的值决定addr是那种网络的地址
     */
    QString         addr;
    /*
     * 群组名称
     */
    QString         name;
    /*
     * 群组创建者编号(adhoc/lte)
     */
    QString         creator;
    /*
     * 群组管理员编号(adhoc)
     */
    QString         admin;
    /*
     * 群组管理员列表(adhoc)
     */
    QStringList     admins;
    /*
     * 群组优先级(lte)
     */
    int             priority;
    /*
     * 群组频道号(lte)
     */
    QString         croomid;
    /*
     * 群组频道索引(lte)
     */
    int             sessionIndex;
    /*
     * 源群组成员个数(adhoc/lte)
     */
    int             memberSize;
    /*
     * 源群组成员(adhoc/lte)
     */
    QList<UserInfo> baseMembers;      //备注: 数组第一个成员为创建者
    /*
     * 成功加群的成员(adhoc/lte)
     */
    QStringList     successMembers;
    /*
     * 0: 380M-DMO  1:380M-TMO  2:400M-DMO 3:400M-TMO(baotong)
     */
    int             pttWorkMode;
    /*
     * 宝通群组号码(必须是10个号码，无需设置的则号码为0)
     */
    QStringList     pttGroups;
    /*
     * 频段(baotong)
     */
    QString         pttFreq;
    /*
     * 保留字段(服务填写)
     */
    QString         reserve;

public:
    /**
     * @brief 构造函数，初始化GroupInfo
     *
     */
    GroupInfo()
    {
        id = "";
        network = -1;
        addr = "";
        name = "";
        creator = "";
        admin = "";
        admins = QStringList();
        priority = -1;
        croomid = "";
        sessionIndex = -1;
        memberSize = -1;
        baseMembers = QList<UserInfo>();
        successMembers = QStringList();
        pttWorkMode = -1;
        pttGroups = QStringList();
        pttFreq = "";
        reserve = "";
    }

    /**
      * @brief 重载"=",可直接用一个GroupInfo对象对另一个GroupInfo对象进行赋值
      *
      */
    GroupInfo& operator=(const GroupInfo &info)
    {
        id = info.id;
        network = info.network;
        addr = info.addr;
        name = info.name;
        creator = info.creator;
        admin = info.admin;
        admins = info.admins;
        priority = info.priority;
        croomid = info.croomid;
        sessionIndex = info.sessionIndex;
        memberSize = info.memberSize;
        baseMembers = info.baseMembers;
        successMembers = info.successMembers;
        pttWorkMode = info.pttWorkMode;
        pttGroups = info.pttGroups;
        pttFreq = info.pttFreq;
        reserve = info.reserve;

        return *this;
    }
};

typedef struct GroupInfo GroupInfo;

/*
 * 群组消息
 */
typedef struct _GroupTransMsg
{
    /*
     * 群组消息编号
     */
    QString         id;
    /*
     * 群组POC编号
     */
    QString         ipocId;
    /*
     * 群组编号
     */
    QString         groupId;
    /*
     * 群组名称
     */
    QString         name;
    /*
     * 群组描述
     */
    QString         description;
    /*
     * 群组优先级(lte)
     */
    int             priority;
    /*
     * 群组网络类型
     */
    int             network;
    /*
     * 群组地址
     */
    QString addr;   //根据network的值决定addr是那种网络的地址
    /*
     * 群组消息类型
     */
    int             type;
    /*
     * 群组消息操作类型
     */
    int             optId;
    /*
     * 群组消息发送时间
     */
    QString         starttime;
    /*
     * 接收者/发送者
     */
    int             direction;
    /*
     * 群组消息状态
     */
    int             status;
    /*
     * 群组消息内容
     */
    QByteArray      text;
    /*
     * 群组消息附加内容
     */
    QString      extraMsg;
    /*
     * 发送消息的群组的用户数
     */
    int             userCount;
    /*
     * 源群组成员(adhoc/lte)
     */
    QList<UserInfo> baseMembers;      //备注: 数组第一个成员为创建者
    /*
     *备用
     */
    QString remark;
    /*
     * 完成此业务对应的线程ID
     */
    int             threadId;
} GroupTransMsg;

class CommTransServiceCallbacks
{
public:
    CommTransServiceCallbacks() {}
    ~CommTransServiceCallbacks() {}
    virtual void sendSignalToApp(QString data) {}
    virtual void sendSignalToApp(GroupTransMsg data) {}
};


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
#define OFONO_SERVICE                 "org.ofono"
#define OFONO_MANET_PATH              "/manet"
#define OFONO_MANET_INTERFACE         "org.ofono.Manet"
typedef struct
{
    /*
     * 根据不同的接口通信取不同的值，具体参考各个接口说明
     */
    uint16 opcode;    //quint16
    /*
     * 保留字段，默认为0
     */
    uint16 frameNum;
    /*
     * 保留字段，默认为0
     */
    uint16 timeSlotNum;
    /*
     * 消息长度，填写每个接口内容中不包括系统消息头的长度
     */
    uint16 msgLength;
} AdHocMsgHeader;

typedef struct
{
    /*
     * 系统消息头，u16Opcode固定取值为17
     */
    AdHocMsgHeader header;
    /*
     * 如果是通用群组，该字段表示该群组的成员数，成员数应计入本节点
     */
    uint16 memberNum;
    /*
     * 保留字段，默认为0
     */
    uint8 reserved[2];
    /*
     * 群组的组播地址，由点分格式的IP地址转换的无符号型数值
     */
    uint32 groupId;
    /*
     * 该群组的成员列表，每个成员由IP地址标识，其中应包括本节点的IP地址，如果数组未填满，剩余空间固定填为0
     */
    uint32 memberId[MAX_MEMBER_COUNT];
} AdHocGroupInfoCfgReq;

typedef struct
{
    /*
     * 系统消息头，u16Opcode固定取值为18
     */
    AdHocMsgHeader header;
    /*
     * 群组的组播地址，由点分格式的IP地址转换的无符号型数值
     */
    uint32 groupId;
    /*
     * 配置结果，0为成功，1为失败
     */
    uint8 result;
    /*
     * 保留字段，默认为0
     */
    uint8 reserved[3];
} AdHocGroupInfoCfgRsp;

typedef struct
{
    /*
     * 系统消息头，u16Opcode固定取值为17
     */
    AdHocMsgHeader header;
    /*
     * 节点优先级。0--3，0最高
     */
    quint8 pri;
    /*
     * 保留字段，默认为0
     */
    quint8 reserved[3];
    /*
     * 群组的组播地址，由点分格式的IP地址转换的无符号型数值
     */
    quint32 groupId;

} AdHocGroupPTTPushOnReq;

typedef struct
{
    /*
     * 系统消息头，u16Opcode固定取值为18
     */
    AdHocMsgHeader header;
    /* 配置结果，0为成功，1为失败
      */
    quint8 result;
    /*
       * 保留字段，默认为0
       */
    quint8 reserved[3];
    /*
     * 群组的组播地址，由点分格式的IP地址转换的无符号型数值
     */
    quint32 groupId;

} AdHocGroupPTTPushOnRsp;

typedef struct
{
    /*
     * 系统消息头，u16Opcode固定取值为18
     */
    AdHocMsgHeader header;
    /*PTT信道状态，0空闲，1占用*/
    quint8 status;

       /*保留字段，默认为0
       */
    quint8 reserved[3];
    /*
     * 群组的组播地址，由点分格式的IP地址转换的无符号型数值
     */
    quint32 groupId;

} AdHocGroupPTTStatusInd;


typedef struct
{
    /*
     * 系统消息头，u16Opcode固定取值为17
     */
    AdHocMsgHeader header;

    /*
     * 群组的组播地址，由点分格式的IP地址转换的无符号型数值
     */
    quint32 groupId;

} AdHocGroupPTTReleaseReq;



typedef struct
{
    quint8 result;
    quint32 groupId;
}GroupPttPushOnRspInfo;

typedef struct
{
    quint8 status;
    quint32 groupId;
}GroupPttStatusIndInfo;


}// namespace QT_DTT_JJF


Q_DECLARE_METATYPE(QT_DTT_JJF::UserInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::UserInfo &user)
{
    argument.beginStructure();
    argument << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.lte_pocId << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::UserInfo &user)
{
    argument.beginStructure();
    argument >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
        >> user.lte_ip >> user.lte_pocId >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip >> user.wifi_ip;

    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out, const QT_DTT_JJF::UserInfo &user)
{
    out << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.lte_pocId << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;

    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT_JJF::UserInfo &user)
{
    in >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
        >> user.lte_ip >> user.lte_pocId >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip >> user.wifi_ip;

    return in;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::HandlerData)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::HandlerData &data)
{
    argument.beginStructure();
    argument << data.network << data.dialState << data.dialNum << data.start << data.end << data.isMetting << data.mettingTarget;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::HandlerData &data)
{
    argument.beginStructure();
    argument >> data.network >> data.dialState >> data.dialNum >> data.start >> data.end >> data.isMetting >> data.mettingTarget;
    argument.endStructure();

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::GroupInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::GroupInfo &group)
{
    argument.beginStructure();
    argument << group.id << group.network << group.addr;
    argument << group.name << group.creator << group.admin << group.admins << group.priority << group.memberSize;
    argument << group.croomid << group.sessionIndex << group.baseMembers;
    argument << group.successMembers << group.pttWorkMode << group.pttGroups << group.pttFreq << group.reserve;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::GroupInfo &group)
{
    QT_DTT_JJF::UserInfo user;
    argument.beginStructure();
    argument >> group.id >> group.network >> group.addr;
    argument >> group.name >> group.creator >> group.admin >> group.admins >> group.priority >> group.memberSize;
    argument >> group.croomid >> group.sessionIndex >> group.baseMembers;
    argument >> group.successMembers >> group.pttWorkMode >> group.pttGroups >> group.pttFreq >> group.reserve;
    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out, const QT_DTT_JJF::GroupInfo &group)
{
    out << group.id << group.network << group.addr;
    out << group.name << group.creator << group.admin << group.admins  << group.priority << group.memberSize;
    out << group.croomid << group.sessionIndex << group.baseMembers;
    out << group.successMembers << group.pttWorkMode << group.pttGroups << group.pttFreq << group.reserve;

    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT_JJF::GroupInfo &group)
{
    QT_DTT_JJF::UserInfo user;
    in >> group.id >> group.network >> group.addr >> group.name >> group.creator >> group.admin >> group.admins  >> group.priority
        >> group.memberSize;
    in >> group.croomid >> group.sessionIndex >> group.baseMembers;
    in >> group.successMembers >> group.pttWorkMode >> group.pttGroups >> group.pttFreq >> group.reserve;

    return in;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::GroupTransMsg)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::GroupTransMsg &groupmsg)
{
    argument.beginStructure();
    argument << groupmsg.id << groupmsg.ipocId << groupmsg.groupId  << groupmsg.name << groupmsg.description;
    argument << groupmsg.priority << groupmsg.network << groupmsg.addr;
    argument << groupmsg.type << groupmsg.optId << groupmsg.starttime << groupmsg.direction;
    argument << groupmsg.status << groupmsg.text << groupmsg.extraMsg << groupmsg.userCount << groupmsg.baseMembers << groupmsg.remark << groupmsg.threadId;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::GroupTransMsg &groupmsg)
{
    QT_DTT_JJF::UserInfo user;
    argument.beginStructure();
    argument >> groupmsg.id >> groupmsg.ipocId  >> groupmsg.groupId >> groupmsg.name >> groupmsg.description;
    argument >> groupmsg.priority >> groupmsg.network >> groupmsg.addr;
    argument >> groupmsg.type >> groupmsg.optId >> groupmsg.starttime >> groupmsg.direction;
    argument >> groupmsg.status >> groupmsg.text >> groupmsg.extraMsg >> groupmsg.userCount >> groupmsg.baseMembers  >> groupmsg.remark >> groupmsg.threadId;
    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out, QT_DTT_JJF::GroupTransMsg &groupmsg)
{
    out << groupmsg.id << groupmsg.ipocId << groupmsg.groupId << groupmsg.name << groupmsg.description;
    out << groupmsg.priority << groupmsg.network << groupmsg.addr;
    out << groupmsg.type << groupmsg.optId << groupmsg.starttime << groupmsg.direction;
    out << groupmsg.status << groupmsg.text << groupmsg.extraMsg << groupmsg.userCount << groupmsg.baseMembers  << groupmsg.remark << groupmsg.threadId;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT_JJF::GroupTransMsg &groupmsg)
{
    in >> groupmsg.id >> groupmsg.ipocId >> groupmsg.groupId >> groupmsg.name >> groupmsg.description;
    in >> groupmsg.priority >> groupmsg.network >> groupmsg.addr;
    in >> groupmsg.type >> groupmsg.optId >> groupmsg.starttime >> groupmsg.direction;
    in >> groupmsg.status >> groupmsg.text >> groupmsg.extraMsg >> groupmsg.userCount >> groupmsg.baseMembers >> groupmsg.remark >> groupmsg.threadId;
    return in;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::MessageInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::MessageInfo &info)
{
    argument.beginStructure();
    argument << info.id << info.type << info.startTime << info.remoteUid<< info.freeText<< info.aisleType
        << info.appId << info.optId << info.appName << info.remark;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::MessageInfo &info)
{
    argument.beginStructure();
    argument >> info.id >>  info.type >> info.startTime >> info.remoteUid >>  info.freeText >> info.aisleType
        >> info.appId >> info.optId >> info.appName >> info.remark;

    argument.endStructure();

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::FileInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::FileInfo &info)
{
    argument.beginStructure();

    argument << info.optId <<  info.network << info.isAccept << info.appName <<  info.appId << info.sendUserName
        << info.receiveUserName << info.sendFilePath << info.receiveFilePath << info.sendAddress << info.receiveAddress << info.remark;

    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::FileInfo &info)
{
    argument.beginStructure();

    argument >> info.optId >>  info.network >> info.isAccept >> info.appName >>  info.appId >> info.sendUserName
        >> info.receiveUserName >> info.sendFilePath >> info.receiveFilePath >> info.sendAddress >> info.receiveAddress >> info.remark;

    argument.endStructure();

    return argument;
}


Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocMsgHeader)
inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::AdHocMsgHeader &header)
{
    argument.beginStructure();
    argument << header.opcode << header.frameNum << header.timeSlotNum << header.msgLength;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocMsgHeader &header)
{
    argument.beginStructure();
    argument >> header.opcode >> header.frameNum >> header.timeSlotNum >> header.msgLength;
    argument.endStructure();

    return argument;
}

inline QDataStream& operator<<(QDataStream &argument, const QT_DTT_JJF::AdHocMsgHeader &header)
{
    argument << header.opcode << header.frameNum << header.timeSlotNum << header.msgLength;

    return argument;
}

inline QDataStream& operator>>(QDataStream &argument, QT_DTT_JJF::AdHocMsgHeader &header)
{
    argument >> header.opcode >> header.frameNum >> header.timeSlotNum >> header.msgLength;

    return argument;
}


Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupInfoCfgReq)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT_JJF::AdHocGroupInfoCfgReq &req)
{
    argument.beginStructure();
    argument << req.header;
    argument << req.memberNum;
    argument.beginArray(qMetaTypeId<uint8>());
    argument << req.reserved[0];
    argument << req.reserved[1];
    argument.endArray();
    argument << req.groupId;
    argument.beginArray(qMetaTypeId<uint32>());
    for (int i=0; i < MAX_MEMBER_COUNT; i++) {
        argument << req.memberId[i];
    }
    argument.endArray();
    argument.endStructure();

    return argument;
}

//Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupInfoCfgReq)

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocGroupInfoCfgReq &req)
{
    argument.beginStructure();
    argument >> req.header;
    argument >> req.memberNum;
    argument.beginArray();
    argument >> req.reserved[0];
    argument >> req.reserved[1];
    argument.endArray();
    argument >> req.groupId;
    argument.beginArray();
    for (int i=0; i < MAX_MEMBER_COUNT; i++) {
        argument >> req.memberId[i];
    }
    argument.endArray();
    argument.endStructure();

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupInfoCfgRsp)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::AdHocGroupInfoCfgRsp &rsp)
{
    argument.beginStructure();
    argument << rsp.header;
    argument << rsp.groupId;
    argument << rsp.result ;
    argument.beginArray(qMetaTypeId<uint8>());
    argument << rsp.reserved[0];
    argument << rsp.reserved[1];
    argument << rsp.reserved[2];
    argument.endArray();
    argument.endStructure();

    return argument;
}

//Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupInfoCfgRsp)

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocGroupInfoCfgRsp &rsp)
{
    argument.beginStructure();
    argument >> rsp.header;
    argument >> rsp.groupId;
    argument >> rsp.result;
    argument.beginArray();
    argument >> rsp.reserved[0];
    argument >> rsp.reserved[1];
    argument >> rsp.reserved[2];
    argument.endArray();
    argument.endStructure();

    return argument;
}

inline QDataStream& operator<<(QDataStream &argument, const QT_DTT_JJF::AdHocGroupInfoCfgRsp &rsp)
{
    argument << rsp.header;
    argument << rsp.groupId;
    argument << rsp.result ;
    argument << rsp.reserved[0];
    argument << rsp.reserved[1];
    argument << rsp.reserved[2];

    return argument;
}

inline QDataStream& operator>>(QDataStream &argument, QT_DTT_JJF::AdHocGroupInfoCfgRsp &rsp)
{
    argument >> rsp.header;
    argument >> rsp.groupId;
    argument >> rsp.result;
    argument >> rsp.reserved[0];
    argument >> rsp.reserved[1];
    argument >> rsp.reserved[2];

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupPTTPushOnReq)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::AdHocGroupPTTPushOnReq &req)
{
    argument.beginStructure();
    argument << req.header;
    argument << req.pri ;
    argument.beginArray(qMetaTypeId<uint8>());
    argument << req.reserved[0];
    argument << req.reserved[1];
    argument << req.reserved[2];
    argument.endArray();
    argument << req.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocGroupPTTPushOnReq &req)
{
    argument.beginStructure();
    argument >> req.header;
     argument >> req.pri ;
    argument.beginArray();
    argument >> req.reserved[0];
    argument >> req.reserved[1];
    argument >> req.reserved[2];
    argument.endArray();
    argument >> req.groupId;
    argument.endStructure();

    return argument;
}
inline QDataStream &operator<<(QDataStream &argument,const QT_DTT_JJF::AdHocGroupPTTPushOnReq &req)
{

    argument << req.header;
    argument << req.pri ;

    argument << req.reserved[0];
    argument << req.reserved[1];
    argument << req.reserved[2];

    argument << req.groupId;


    return argument;
}

inline const QDataStream &operator>>(QDataStream &argument, QT_DTT_JJF::AdHocGroupPTTPushOnReq &req)
{

    argument >> req.header;
     argument >> req.pri ;

    argument >> req.reserved[0];
    argument >> req.reserved[1];
    argument >> req.reserved[2];

    argument >> req.groupId;


    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupPTTPushOnRsp)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::AdHocGroupPTTPushOnRsp &rsp)
{
    argument.beginStructure();
    argument << rsp.header;
    argument << rsp.result ;
    argument.beginArray(qMetaTypeId<uint8>());
    argument << rsp.reserved[0];
    argument << rsp.reserved[1];
    argument << rsp.reserved[2];
    argument.endArray();
    argument << rsp.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocGroupPTTPushOnRsp &rsp)
{
    argument.beginStructure();
    argument >> rsp.header;
    argument >> rsp.result;
    argument.beginArray();
    argument >> rsp.reserved[0];
    argument >> rsp.reserved[1];
    argument >> rsp.reserved[2];
    argument.endArray();
    argument >> rsp.groupId;
    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &argument,const QT_DTT_JJF::AdHocGroupPTTPushOnRsp &rsp)
{
    argument << rsp.header;
    argument << rsp.result;
    argument << rsp.reserved[0];
    argument << rsp.reserved[1];
    argument << rsp.reserved[2];
    argument << rsp.groupId;

    return argument;
}



inline QDataStream &operator>>(QDataStream &argument, QT_DTT_JJF::AdHocGroupPTTPushOnRsp &rsp)
{
    argument >> rsp.header;
    argument >> rsp.result;
    argument >> rsp.reserved[0];
    argument >> rsp.reserved[1];
    argument >> rsp.reserved[2];
    argument >> rsp.groupId;

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupPTTReleaseReq)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::AdHocGroupPTTReleaseReq &req)
{
    argument.beginStructure();
    argument << req.header;
    argument << req.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocGroupPTTReleaseReq &req)
{
    argument.beginStructure();
    argument >> req.header;
    argument >>  req.groupId;
    argument.endStructure();

    return argument;
}


Q_DECLARE_METATYPE(QT_DTT_JJF::AdHocGroupPTTStatusInd)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::AdHocGroupPTTStatusInd &rsp)
{
    argument.beginStructure();
    argument << rsp.header;
    argument << rsp.status ;
    argument.beginArray(qMetaTypeId<uint8>());
    argument << rsp.reserved[0];
    argument << rsp.reserved[1];
    argument << rsp.reserved[2];
    argument.endArray();
    argument << rsp.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::AdHocGroupPTTStatusInd &rsp)
{
    argument.beginStructure();
    argument >> rsp.header;
    argument >> rsp.status;
    argument.beginArray();
    argument >> rsp.reserved[0];
    argument >> rsp.reserved[1];
    argument >> rsp.reserved[2];
    argument.endArray();
    argument >> rsp.groupId;
    argument.endStructure();

    return argument;
}



Q_DECLARE_METATYPE(QT_DTT_JJF::GroupPttPushOnRspInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::GroupPttPushOnRspInfo &info)
{
    argument.beginStructure();
    argument << info.result ;
    argument << info.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::GroupPttPushOnRspInfo &info)
{
    argument.beginStructure();
    argument >> info.result ;
    argument >> info.groupId;
    argument.endStructure();
    return argument;
}

Q_DECLARE_METATYPE(QT_DTT_JJF::GroupPttStatusIndInfo)
inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT_JJF::GroupPttStatusIndInfo &info)
{
    argument.beginStructure();
    argument << info.status ;
    argument << info.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT_JJF::GroupPttStatusIndInfo &info)
{
    argument.beginStructure();
    argument >> info.status ;
    argument >> info.groupId;
    argument.endStructure();
    return argument;
}
#endif
