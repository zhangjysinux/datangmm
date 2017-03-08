#ifndef COMMON_DEF_H
#define COMMON_DEF_H

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


namespace QT_DTT
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
 * @brief ÍøÂçÀàÐÍ
 *
 */
enum NetworkType
{
    /*
     * ×Ô×éÍø
     */
    AD_HOC_NETWORK = 0x00000001,
    /*
     * LTE
     */
    LTE_NETWORK = 0x00000010,
    /*
     * ÌìÍš
     */
    SATELLITE_NETWORK = 0x00000100,
    /*
     * Êý×Ö¶Ôœ²
     */
    DIGITAL_WALKIE_TALKIE_NETWORK = 0x00001000,
    /*
     * Õœ»¥Íø
     */
    BATTLE_NETWORK = 0x00010000,
    /*
     * test for sinux
     */
    WIFI_NETWORK = 0x00100000,
    /*
     * ÎŽÖªÍøÂç
     */
    UNKNOW_NETWORK = 0x00000000,
};

/**
 * @brief ÍšµÀÀàÐÍ
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
     * Êý×Ö¶Ôœ²
     */
    PTT_AISLE,
    MANET_AISLE,
};

enum DirectState
{
    /*
     * Client¶Ë
     */
    COMM_TRANS_CLIENT = 1,
    /*
     * Server¶Ë
     */
    COMM_TRANS_SERVER,
};

/**
 * @brief ÒµÎñÀàÐÍ
 *
 */
enum TransactionType
{
    /*
     * Öž¿Ø±šÎÄ(µ¥²¥¿É¿¿)
     */
    CTRL_MSG_RELIABLE_TRANS = 1,
    /*
     * Çé±šÐÅÏ¢(µ¥²¥¿É¿¿)
     */
    INTELLIGENCE_MSG_RELIABLE_TRANS,
    /*
     * Öž¿Ø±šÎÄ(µ¥²¥²»¿É¿¿)
     */
    CTRL_MSG_UNRELIABLE_TRANS,
    /*
     * Çé±šÐÅÏ¢(µ¥²¥²»¿É¿¿)
     */
    INTELLIGENCE_MSG_UNRELIABLE_TRANS,
    /*
     * ŒŽÊ±ÏûÏ¢
     */
    IM_TRANS,
    /*
     * ÎÄŒþ
     */
    FILE_TRANS,
    /*
     * ÍŒÏñ
     */
    IMAGE_TRANS,
    /*
     * PTTÓïÒô
     */
    PTT_VOICE_TRANS,
    /*
     * ÓŠÓÃÐÅÁî
     */
    APP_CTRL_CMD_TRANS,
    /*
     * ÒôÆµºôœÐ
     */
    VOICE_CALL_TRANS,
    /*
     * ÊÓÆµºôœÐ
     */
    VIDEO_CALL_TRANS,
    /*
     * ¶à·œÓïÒô
     */
    MUTIL_VOICE_CALL_TRANS,
    /*
     * ¶à·œÊÓÆµ
     */
    MUTIL_VIDEO_CALL_TRANS,
    /*
     * Î»ÖÃ¹²Ïí
     */
    POSITION_SHARED,
    /*
     * ÇÐÍø²Ù×÷
     */
    SWITCH_NETWORK,
    /*
     * ŽŽœšÈº×é-ŽŽœšÈË·¢ÆðŽŽœšÇëÇó.1
     */
    GROUP_CREATE,
    /*
     * ŽŽœšÈº×é-³ÉÔ±œÓÊÕÏûÏ¢ºóÏòŽŽœšÈË·ŽÀ¡ÕýÔÚ×Œ±žÖÐ.2
     */
    GROUP_CREATE_ACK_PREPARED,
    /*
     * ŽŽœšÈº×é-ŽŽœšÈËÈ·ÈÏÍêž÷žö³ÉÔ±µÄ×Œ±ž·ŽÀ¡ÏûÏ¢.3
     */
    GROUP_CREATE_CONFIRMED,
    /*
     * ŽŽœšÈº×é-³ÉÔ±ÏòŽŽœšÈË·ŽÀ¡ŸÍÐ÷.4
     */
    GROUP_CREATE_ACK_READY,
    /*
     * ŽŽœšÈº×é-ŽŽœšÈËÈ·ÈÏŽŽœšÈº×éÍê³É.5
     */
    GROUP_CREATE_DONE,
    /*
     * žüÐÂÈº×é¹ÜÀíÔ±
     */
    GROUP_UPDATE_ADMINS,
    /*
     * žüÐÂÈº×éÃû³Æ
     */
    GROUP_UPDATE_NAME,
    /*
     * ÔöŒÓÈº×é³ÉÔ±
     */
    GROUP_ADD_MEMBERS,
    /*
     * ÔöŒÓÈº×é³ÉÔ±œá¹û·ŽÀ¡
     */
    GROUP_ADD_MEMBER_ACK_RSP,
    /*
     * ÌíŒÓ³ÉÔ±×îÖÕœá¹û·ŽÀ¡
     */
    GROUP_ADD_MEMBERS_COMPLETED,
    /*
     * ÒÆ³ýÈº×é³ÉÔ±
     */
    GROUP_REMOVE_MEMBERS,
    /*
     * ÒÆ³ýÈº×é³ÉÔ±œá¹û·ŽÀ¡
     */
    GROUP_REMOVE_MEMBER_ACK_RSP,
    /*
     * ÒÆ³ý³ÉÔ±×îÖÕœá¹û·ŽÀ¡
     */
    GROUP_REMOVE_MEMBERS_COMPLETED,
    /*
     * ÉŸ³ýÈº×é
     */
    GROUP_REMOVE,
    /*
      * Èº×é±àŒ­ jjf sign 20160709
      */
    GROUP_UPDATE,
    /*
     * ÉŸ³ýÈº×éœá¹û·ŽÀ¡
     */
    GROUP_REMOVE_ACK_RSP,
    /*
     * ÉŸ³ýÈº×é×îÖÕœá¹û·ŽÀ¡
     */
    GROUP_REMOVE_COMPLETED,
    /*
     * Öž¿Ø±šÎÄ(×é²¥)
     */
    GROUP_CTRL_MSG_UNRELIABLE_TRANS,
    /*
     * Çé±šÐÅÏ¢(×é²¥)
     */
    GROUP_INTELLIGENCE_MSG_UNRELIABLE_TRANS,
    /*
     * PTTÓïÒô(×é²¥)
     */
    GROUP_PTT_VOICE_TRANS,
    /*
     * ÆäËûÒµÎñ
     */
    OTHER_TRANS,
    /*
     * ÎŽÖªÒµÎñ
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
    OPERATE_GET_MEMBERS
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
 *  Ïß³Ì×ŽÌ¬
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
 * @brief userÐÅÏ¢
 *
 */
typedef struct _userInfo
{
    /*
     * user ID
     */
    int userId;
    /*
     * ÍøÂçÀàÐÍ
     */
    int network;
    /*
     * ÍšÑ¶µØÖ·(IPµØÖ·»òÕßµç»°ºÅÂë)
     */
    QString addr;
    /*
     * ÓŠÓÃID
     */
    QString appId;
    /*
     * ÒµÎñID
     */
    int optId;
    /*
     * ÓÃ»§Ãû×Ö
     */
    QString userName;
    /*
     * LTEÍøÂçµÄIPµØÖ·
     */
    QString lte_ip;
    /*
     * LTEÍøÂçPOC·þÎñÆ÷µÄÓÃ»§µÇÂŒÕË»§
     */
    QString lte_pocId;
    /*
     * ×Ô×éÍøµÄIPµØÖ·
     */
    QString ad_hoc_ip;
    /*
     * ÌìÍš¶ÔÓŠµÄIPµØÖ·
     */
    QString satellite_ip;
    /*
     * ÌìÍš¶ÔÓŠµÄµç»°ºÅÂë
     */
    QString satellite_phone_number;
    /*
     * Êý×Ö¶Ôœ²µÄµç»°ºÅÂë
     */
    QString digital_number;
    /*
     * Õœ»¥ÍøµÄIPµØÖ·
     */
    QString battle_ip;
    QString wifi_ip;

public:
    /**
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯userinfo
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
     * @brief ÖØÔØ"=",¿ÉÖ±œÓÓÃÒ»žöUserInfo¶ÔÏó¶ÔÁíÒ»žöUserInfo¶ÔÏóœøÐÐž³Öµ
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

/**
 * @brief  ÎÄŒþÐÅÏ¢
 *
 */
struct FileInfo
{
    /*
     * ÒµÎñID
     */
    int optId;
    /*
     * ÍøÂçÀàÐÍ
     */
    int network;
    /*
     * ÊÇ·ñœÓÊÕ
     */
    bool isAccept;
    /*
     * ÓŠÓÃÃû³Æ
     */
    QString appName;
    /*
     * ÓŠÓÃID
     */
    QString appId;
    /*
     * ·¢ËÍ¶ËÓÃ»§Ãû³Æ
     */
    QString sendUserName;
    /*
     * œÓÊÕ¶ËÓÃ»§Ãû³Æ
     */
    QString receiveUserName;
    /*
     * ·¢ËÍ¶ËÎÄŒþÃû³Æ
     */
    QString sendFilePath;
    /*
     * œÓÊÕ¶ËÎÄŒþÃû³Æ
     */
    QString receiveFilePath;
    /*
     * ·¢ËÍ¶ËµØÖ·
     */
    QString sendAddress;
    /*
     * œÓÊÕ¶ËµØÖ·
     */
    QString receiveAddress;
    /*
     * ±ž×¢
     */
    QString remark;

public:
    /**
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯FileInfo
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
      * @brief ÖØÔØ"=",¿ÉÖ±œÓÓÃÒ»žöFileInfo¶ÔÏó¶ÔÁíÒ»žöFileInfo¶ÔÏóœøÐÐž³Öµ
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
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯KeyInfo
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
      * @brief ÖØÔØ"=",¿ÉÖ±œÓÓÃÒ»žöKeyInfo¶ÔÏó¶ÔÁíÒ»žöKeyInfo¶ÔÏóœøÐÐž³Öµ
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
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯DatabaseInfo
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
      * @brief ÖØÔØ"=",¿ÉÖ±œÓÓÃÒ»žöDatabaseInfo¶ÔÏó¶ÔÁíÒ»žöDatabaseInfo¶ÔÏóœøÐÐž³Öµ
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
 * @brief  PS ¶ÌÐÅÐÅÏ¢
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
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯messageinfo
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
      * @brief ÖØÔØ"=",¿ÉÖ±œÓÓÃÒ»žöMessageInfo¶ÔÏó¶ÔÁíÒ»žöMessageInfo¶ÔÏóœøÐÐž³Öµ
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
 * @brief ÍøÂçÐÅÏ¢
 *
 */
typedef struct _networkInfo
{
    /*
     * ÍøÂçÀàÐÍ
     */
    NetworkType type;
    /*
     * type¶ÔÓŠµÄÍøÂçµÄIPµØÖ·
     */
    QString ipAddr;
    /*
     * type¶ÔÓŠµÄÍøÂçµÄµç»°ºÅÂë
     */
    QString phoneNumber;
    /*
     * type¶ÔÓŠµÄÍøÂçÊÇ·ñ¿ÉÓÃ
     */
    bool enable;

public:
    /**
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯NetworkInfo
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
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯NetworkInfo
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
 * ÒµÎñ¶ÔÓŠµÄÐÅÏ¢
 */
typedef struct _transmsg
{
    /*
     * ÍšÑ¶¶ÔÏóµÄuserÐÅÏ¢
     */
    UserInfo user;
    /*
     * ÍšÑ¶ÊýŸÝ
     */
    QByteArray data;
    /*
     * Íê³ÉŽËÒµÎñ¶ÔÓŠµÄÏß³ÌID
     */
    int threadId;
} TransMsg;

/*
 * ÒµÎñÏß³ÌÐÅÏ¢
 */
typedef struct TransactionInfo
{
    /*
     * Ïß³ÌID
     */
    int     threadId;
    /*
     * Ïß³Ìname
     */
    QString threadName;
    /*
     * ±£ÁôÊýŸÝ(ÏµÍ³Ê¹ÓÃ)
     */
    quint32 reverse;
} TransactionInfo;

/*
 * Èº×éÐÅÏ¢
 */
struct GroupInfo
{
    /*
     * Èº×é±àºÅ
     */
    QString         id;
    /*
     * Èº×éÍøÂçÀàÐÍ
     */
    int             network;
    /*
     * Èº×éµØÖ·, žùŸÝnetworkµÄÖµŸö¶šaddrÊÇÄÇÖÖÍøÂçµÄµØÖ·
     */
    QString         addr;
    /*
     * Èº×éÃû³Æ
     */
    QString         name;
    /*
     * Èº×éŽŽœšÕß±àºÅ(adhoc/lte)
     */
    QString         creator;
    /*
     * Èº×é¹ÜÀíÔ±±àºÅ(adhoc)
     */
    QString         admin;
    /*
     * Èº×é¹ÜÀíÔ±ÁÐ±í(adhoc)
     */
    QStringList     admins;
    /*
     * Èº×éÓÅÏÈŒ¶(lte)
     */
    int             priority;
    /*
     * Èº×éÆµµÀºÅ(lte)
     */
    QString         croomid;
    /*
     * Èº×éÆµµÀË÷Òý(lte)
     */
    int             sessionIndex;
    /*
     * ÔŽÈº×é³ÉÔ±žöÊý(adhoc/lte)
     */
    int             memberSize;
    /*
     * ÔŽÈº×é³ÉÔ±(adhoc/lte)
     */
    QList<UserInfo> baseMembers;      //±ž×¢: Êý×éµÚÒ»žö³ÉÔ±ÎªŽŽœšÕß
    /*
     * ³É¹ŠŒÓÈºµÄ³ÉÔ±(adhoc/lte)
     */
    QStringList     successMembers;
    /*
     * 0: 380M-DMO  1:380M-TMO  2:400M-DMO 3:400M-TMO(baotong)
     */
    int             pttWorkMode;
    /*
     * ±ŠÍšÈº×éºÅÂë(±ØÐëÊÇ10žöºÅÂë£¬ÎÞÐèÉèÖÃµÄÔòºÅÂëÎª0)
     */
    QStringList     pttGroups;
    /*
     * Æµ¶Î(baotong)
     */
    QString         pttFreq;
    /*
     * ±£Áô×Ö¶Î(·þÎñÌîÐŽ)
     */
    QString         reserve;

public:
    /**
     * @brief ¹¹Ôìº¯Êý£¬³õÊŒ»¯GroupInfo
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
      * @brief ÖØÔØ"=",¿ÉÖ±œÓÓÃÒ»žöGroupInfo¶ÔÏó¶ÔÁíÒ»žöGroupInfo¶ÔÏóœøÐÐž³Öµ
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
 * Èº×éÏûÏ¢
 */
typedef struct _GroupTransMsg
{
    /*
     * Èº×éÏûÏ¢±àºÅ
     */
    QString         id;
    /*
     * Èº×éPOC±àºÅ
     */
    QString         ipocId;
    /*
     * Èº×é±àºÅ
     */
    QString         groupId;
    /*
     * Èº×éÃû³Æ
     */
    QString         name;
    /*
     * Èº×éÃèÊö
     */
    QString         description;
    /*
     * Èº×éÓÅÏÈŒ¶(lte)
     */
    int             priority;
    /*
     * Èº×éÍøÂçÀàÐÍ
     */
    int             network;
    /*
     * Èº×éµØÖ·
     */
    QString addr;   //žùŸÝnetworkµÄÖµŸö¶šaddrÊÇÄÇÖÖÍøÂçµÄµØÖ·
    /*
     * Èº×éÏûÏ¢ÀàÐÍ
     */
    int             type;
    /*
     * Èº×éÏûÏ¢²Ù×÷ÀàÐÍ
     */
    int             optId;
    /*
     * Èº×éÏûÏ¢·¢ËÍÊ±Œä
     */
    QString         starttime;
    /*
     * œÓÊÕÕß/·¢ËÍÕß
     */
    int             direction;
    /*
     * Èº×éÏûÏ¢×ŽÌ¬
     */
    int             status;
    /*
     * Èº×éÏûÏ¢ÄÚÈÝ
     */
    QByteArray      text;
    /*
     * Èº×éÏûÏ¢žœŒÓÄÚÈÝ
     */
    QString      extraMsg;
    /*
     * ·¢ËÍÏûÏ¢µÄÈº×éµÄÓÃ»§Êý
     */
    int             userCount;
    /*
     * ÔŽÈº×é³ÉÔ±(adhoc/lte)
     */
    QList<UserInfo> baseMembers;      //±ž×¢: Êý×éµÚÒ»žö³ÉÔ±ÎªŽŽœšÕß

    /*
     * Íê³ÉŽËÒµÎñ¶ÔÓŠµÄÏß³ÌID
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
     * žùŸÝ²»Í¬µÄœÓ¿ÚÍšÐÅÈ¡²»Í¬µÄÖµ£¬ŸßÌå²Î¿Œž÷žöœÓ¿ÚËµÃ÷
     */
    uint16 opcode;    //quint16
    /*
     * ±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
     */
    uint16 frameNum;
    /*
     * ±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
     */
    uint16 timeSlotNum;
    /*
     * ÏûÏ¢³€¶È£¬ÌîÐŽÃ¿žöœÓ¿ÚÄÚÈÝÖÐ²»°üÀšÏµÍ³ÏûÏ¢Í·µÄ³€¶È
     */
    uint16 msgLength;
} AdHocMsgHeader;

typedef struct
{
    /*
     * ÏµÍ³ÏûÏ¢Í·£¬u16Opcode¹Ì¶šÈ¡ÖµÎª17
     */
    AdHocMsgHeader header;
    /*
     * Èç¹ûÊÇÍšÓÃÈº×é£¬žÃ×Ö¶Î±íÊŸžÃÈº×éµÄ³ÉÔ±Êý£¬³ÉÔ±ÊýÓŠŒÆÈë±ŸœÚµã
     */
    uint16 memberNum;
    /*
     * ±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
     */
    uint8 reserved[2];
    /*
     * Èº×éµÄ×é²¥µØÖ·£¬ÓÉµã·ÖžñÊœµÄIPµØÖ·×ª»»µÄÎÞ·ûºÅÐÍÊýÖµ
     */
    uint32 groupId;
    /*
     * žÃÈº×éµÄ³ÉÔ±ÁÐ±í£¬Ã¿žö³ÉÔ±ÓÉIPµØÖ·±êÊ¶£¬ÆäÖÐÓŠ°üÀš±ŸœÚµãµÄIPµØÖ·£¬Èç¹ûÊý×éÎŽÌîÂú£¬Ê£Óà¿ÕŒä¹Ì¶šÌîÎª0
     */
    uint32 memberId[MAX_MEMBER_COUNT];
} AdHocGroupInfoCfgReq;

typedef struct
{
    /*
     * ÏµÍ³ÏûÏ¢Í·£¬u16Opcode¹Ì¶šÈ¡ÖµÎª18
     */
    AdHocMsgHeader header;
    /*
     * Èº×éµÄ×é²¥µØÖ·£¬ÓÉµã·ÖžñÊœµÄIPµØÖ·×ª»»µÄÎÞ·ûºÅÐÍÊýÖµ
     */
    uint32 groupId;
    /*
     * ÅäÖÃœá¹û£¬0Îª³É¹Š£¬1ÎªÊ§°Ü
     */
    uint8 result;
    /*
     * ±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
     */
    uint8 reserved[3];
} AdHocGroupInfoCfgRsp;

typedef struct
{
    /*
     * ÏµÍ³ÏûÏ¢Í·£¬u16Opcode¹Ì¶šÈ¡ÖµÎª17
     */
    AdHocMsgHeader header;
    /*
     * œÚµãÓÅÏÈŒ¶¡£0--3£¬0×îžß
     */
    quint8 pri;
    /*
     * ±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
     */
    quint8 reserved[3];
    /*
     * Èº×éµÄ×é²¥µØÖ·£¬ÓÉµã·ÖžñÊœµÄIPµØÖ·×ª»»µÄÎÞ·ûºÅÐÍÊýÖµ
     */
    quint32 groupId;

} AdHocGroupPTTPushOnReq;

typedef struct
{
    /*
     * ÏµÍ³ÏûÏ¢Í·£¬u16Opcode¹Ì¶šÈ¡ÖµÎª18
     */
    AdHocMsgHeader header;
    /* ÅäÖÃœá¹û£¬0Îª³É¹Š£¬1ÎªÊ§°Ü
      */
     quint8 result;
    /*
       * ±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
       */
      quint8 reserved[3];
    /*
     * Èº×éµÄ×é²¥µØÖ·£¬ÓÉµã·ÖžñÊœµÄIPµØÖ·×ª»»µÄÎÞ·ûºÅÐÍÊýÖµ
     */
    quint32 groupId;

} AdHocGroupPTTPushOnRsp;

typedef struct
{
    /*
     * ÏµÍ³ÏûÏ¢Í·£¬u16Opcode¹Ì¶šÈ¡ÖµÎª18
     */
    AdHocMsgHeader header;
    /*PTTÐÅµÀ×ŽÌ¬£¬0¿ÕÏÐ£¬1ÕŒÓÃ*/
     quint8 status;

       /*±£Áô×Ö¶Î£¬Ä¬ÈÏÎª0
       */
      quint8 reserved[3];
    /*
     * Èº×éµÄ×é²¥µØÖ·£¬ÓÉµã·ÖžñÊœµÄIPµØÖ·×ª»»µÄÎÞ·ûºÅÐÍÊýÖµ
     */
    quint32 groupId;

} AdHocGroupPTTStatusInd;


typedef struct
{
    /*
     * ÏµÍ³ÏûÏ¢Í·£¬u16Opcode¹Ì¶šÈ¡ÖµÎª17
     */
    AdHocMsgHeader header;

    /*
     * Èº×éµÄ×é²¥µØÖ·£¬ÓÉµã·ÖžñÊœµÄIPµØÖ·×ª»»µÄÎÞ·ûºÅÐÍÊýÖµ
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


}// namespace QT_DTT


Q_DECLARE_METATYPE(QT_DTT::UserInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::UserInfo &user)
{
    argument.beginStructure();
    argument << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.lte_pocId << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::UserInfo &user)
{
    argument.beginStructure();
    argument >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
        >> user.lte_ip >> user.lte_pocId >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip >> user.wifi_ip;

    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out, const QT_DTT::UserInfo &user)
{
    out << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.lte_pocId << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;

    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT::UserInfo &user)
{
    in >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
        >> user.lte_ip >> user.lte_pocId >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip >> user.wifi_ip;

    return in;
}

Q_DECLARE_METATYPE(QT_DTT::HandlerData)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::HandlerData &data)
{
    argument.beginStructure();
    argument << data.network << data.dialState << data.dialNum << data.start << data.end << data.isMetting << data.mettingTarget;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::HandlerData &data)
{
    argument.beginStructure();
    argument >> data.network >> data.dialState >> data.dialNum >> data.start >> data.end >> data.isMetting >> data.mettingTarget;
    argument.endStructure();

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT::GroupInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::GroupInfo &group)
{
    argument.beginStructure();
    argument << group.id << group.network << group.addr;
    argument << group.name << group.creator << group.admin << group.admins << group.priority << group.memberSize;
    argument << group.croomid << group.sessionIndex << group.baseMembers;
    argument << group.successMembers << group.pttWorkMode << group.pttGroups << group.pttFreq << group.reserve;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::GroupInfo &group)
{
    QT_DTT::UserInfo user;
    argument.beginStructure();
    argument >> group.id >> group.network >> group.addr;
    argument >> group.name >> group.creator >> group.admin >> group.admins >> group.priority >> group.memberSize;
    argument >> group.croomid >> group.sessionIndex >> group.baseMembers;
    argument >> group.successMembers >> group.pttWorkMode >> group.pttGroups >> group.pttFreq >> group.reserve;
    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out, const QT_DTT::GroupInfo &group)
{
    out << group.id << group.network << group.addr;
    out << group.name << group.creator << group.admin << group.admins  << group.priority << group.memberSize;
    out << group.croomid << group.sessionIndex << group.baseMembers;
    out << group.successMembers << group.pttWorkMode << group.pttGroups << group.pttFreq << group.reserve;

    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT::GroupInfo &group)
{
    QT_DTT::UserInfo user;
    in >> group.id >> group.network >> group.addr >> group.name >> group.creator >> group.admin >> group.admins  >> group.priority
        >> group.memberSize;
    in >> group.croomid >> group.sessionIndex >> group.baseMembers;
    in >> group.successMembers >> group.pttWorkMode >> group.pttGroups >> group.pttFreq >> group.reserve;

    return in;
}

Q_DECLARE_METATYPE(QT_DTT::GroupTransMsg)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::GroupTransMsg &groupmsg)
{
    argument.beginStructure();
    argument << groupmsg.id << groupmsg.ipocId << groupmsg.groupId  << groupmsg.name << groupmsg.description;
    argument << groupmsg.priority << groupmsg.network << groupmsg.addr;
    argument << groupmsg.type << groupmsg.optId << groupmsg.starttime << groupmsg.direction;
    argument << groupmsg.status << groupmsg.text << groupmsg.extraMsg << groupmsg.userCount << groupmsg.baseMembers << groupmsg.threadId;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::GroupTransMsg &groupmsg)
{
    QT_DTT::UserInfo user;
    argument.beginStructure();
    argument >> groupmsg.id >> groupmsg.ipocId  >> groupmsg.groupId >> groupmsg.name >> groupmsg.description;
    argument >> groupmsg.priority >> groupmsg.network >> groupmsg.addr;
    argument >> groupmsg.type >> groupmsg.optId >> groupmsg.starttime >> groupmsg.direction;
    argument >> groupmsg.status >> groupmsg.text >> groupmsg.extraMsg >> groupmsg.userCount >> groupmsg.baseMembers  >> groupmsg.threadId;
    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out, QT_DTT::GroupTransMsg &groupmsg)
{
    out << groupmsg.id << groupmsg.ipocId << groupmsg.groupId << groupmsg.name << groupmsg.description;
    out << groupmsg.priority << groupmsg.network << groupmsg.addr;
    out << groupmsg.type << groupmsg.optId << groupmsg.starttime << groupmsg.direction;
    out << groupmsg.status << groupmsg.text << groupmsg.extraMsg << groupmsg.userCount << groupmsg.baseMembers  << groupmsg.threadId;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT::GroupTransMsg &groupmsg)
{
    in >> groupmsg.id >> groupmsg.ipocId >> groupmsg.groupId >> groupmsg.name >> groupmsg.description;
    in >> groupmsg.priority >> groupmsg.network >> groupmsg.addr;
    in >> groupmsg.type >> groupmsg.optId >> groupmsg.starttime >> groupmsg.direction;
    in >> groupmsg.status >> groupmsg.text >> groupmsg.extraMsg >> groupmsg.userCount >> groupmsg.baseMembers >> groupmsg.threadId;
    return in;
}

Q_DECLARE_METATYPE(QT_DTT::MessageInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::MessageInfo &info)
{
    argument.beginStructure();
    argument << info.id << info.type << info.startTime << info.remoteUid<< info.freeText<< info.aisleType
        << info.appId << info.optId << info.appName << info.remark;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::MessageInfo &info)
{
    argument.beginStructure();
    argument >> info.id >>  info.type >> info.startTime >> info.remoteUid >>  info.freeText >> info.aisleType
        >> info.appId >> info.optId >> info.appName >> info.remark;

    argument.endStructure();

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT::FileInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::FileInfo &info)
{
    argument.beginStructure();

    argument << info.optId <<  info.network << info.isAccept << info.appName <<  info.appId << info.sendUserName
        << info.receiveUserName << info.sendFilePath << info.receiveFilePath << info.sendAddress << info.receiveAddress << info.remark;

    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::FileInfo &info)
{
    argument.beginStructure();

    argument >> info.optId >>  info.network >> info.isAccept >> info.appName >>  info.appId >> info.sendUserName
        >> info.receiveUserName >> info.sendFilePath >> info.receiveFilePath >> info.sendAddress >> info.receiveAddress >> info.remark;

    argument.endStructure();

    return argument;
}


Q_DECLARE_METATYPE(QT_DTT::AdHocMsgHeader)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::AdHocMsgHeader &header)
{
    argument.beginStructure();
    argument << header.opcode << header.frameNum << header.timeSlotNum << header.msgLength;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocMsgHeader &header)
{
    argument.beginStructure();
    argument >> header.opcode >> header.frameNum >> header.timeSlotNum >> header.msgLength;
    argument.endStructure();

    return argument;
}

inline QDataStream& operator<<(QDataStream &argument, const QT_DTT::AdHocMsgHeader &header)
{
    argument << header.opcode << header.frameNum << header.timeSlotNum << header.msgLength;

    return argument;
}

inline QDataStream& operator>>(QDataStream &argument, QT_DTT::AdHocMsgHeader &header)
{
    argument >> header.opcode >> header.frameNum >> header.timeSlotNum >> header.msgLength;

    return argument;
}


Q_DECLARE_METATYPE(QT_DTT::AdHocGroupInfoCfgReq)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::AdHocGroupInfoCfgReq &req)
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

//Q_DECLARE_METATYPE(QT_DTT::AdHocGroupInfoCfgReq)

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocGroupInfoCfgReq &req)
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

Q_DECLARE_METATYPE(QT_DTT::AdHocGroupInfoCfgRsp)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::AdHocGroupInfoCfgRsp &rsp)
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

//Q_DECLARE_METATYPE(QT_DTT::AdHocGroupInfoCfgRsp)

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocGroupInfoCfgRsp &rsp)
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

inline QDataStream& operator<<(QDataStream &argument, const QT_DTT::AdHocGroupInfoCfgRsp &rsp)
{
    argument << rsp.header;
    argument << rsp.groupId;
    argument << rsp.result ;
    argument << rsp.reserved[0];
    argument << rsp.reserved[1];
    argument << rsp.reserved[2];

    return argument;
}

inline QDataStream& operator>>(QDataStream &argument, QT_DTT::AdHocGroupInfoCfgRsp &rsp)
{
    argument >> rsp.header;
    argument >> rsp.groupId;
    argument >> rsp.result;
    argument >> rsp.reserved[0];
    argument >> rsp.reserved[1];
    argument >> rsp.reserved[2];

    return argument;
}

Q_DECLARE_METATYPE(QT_DTT::AdHocGroupPTTPushOnReq)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::AdHocGroupPTTPushOnReq &req)
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



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocGroupPTTPushOnReq &req)
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

Q_DECLARE_METATYPE(QT_DTT::AdHocGroupPTTPushOnRsp)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::AdHocGroupPTTPushOnRsp &rsp)
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



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocGroupPTTPushOnRsp &rsp)
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


Q_DECLARE_METATYPE(QT_DTT::AdHocGroupPTTReleaseReq)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::AdHocGroupPTTReleaseReq &req)
{
    argument.beginStructure();
    argument << req.header;

    argument << req.groupId;

    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocGroupPTTReleaseReq &req)
{
    argument.beginStructure();
    argument >> req.header;

    argument >>  req.groupId;

    argument.endStructure();

    return argument;
}


Q_DECLARE_METATYPE(QT_DTT::AdHocGroupPTTStatusInd)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::AdHocGroupPTTStatusInd &rsp)
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



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::AdHocGroupPTTStatusInd &rsp)
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



Q_DECLARE_METATYPE(QT_DTT::GroupPttPushOnRspInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::GroupPttPushOnRspInfo &info)
{
    argument.beginStructure();

    argument << info.result ;

     argument << info.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::GroupPttPushOnRspInfo &info)
{
    argument.beginStructure();
    argument >> info.result ;
     argument >> info.groupId;
    argument.endStructure();
    return argument;
}

Q_DECLARE_METATYPE(QT_DTT::GroupPttStatusIndInfo)
inline QDBusArgument &operator<<(QDBusArgument &argument,const QT_DTT::GroupPttStatusIndInfo &info)
{
    argument.beginStructure();

    argument << info.status ;

     argument << info.groupId;
    argument.endStructure();

    return argument;
}



inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::GroupPttStatusIndInfo &info)
{
    argument.beginStructure();
    argument >> info.status ;
     argument >> info.groupId;
    argument.endStructure();
    return argument;
}



#endif
