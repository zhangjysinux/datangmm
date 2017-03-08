#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include<QString>
#include<QMetaType>
#include<QDBusMetaType>
#include<QDebug>


#define CTS_PRINT (qDebug() <<"CommTransService adapter:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")

/**
 * @brief 网络类型
 *
 */
enum NetworkType
{
    AD_HOC_NETWORK = 0x00000001,                ///< 自组网
    LTE_NETWORK = 0x00000010,                   ///< LTE
    SATELLITE_NETWORK = 0x00000100,             ///< 天通
    DIGITAL_WALKIE_TALKIE_NETWORK = 0x00001000, ///< 数字对讲
    BATTLE_NETWORK = 0x00010000,                ///< 战互网
    UNKNOW_NETWORK = 0x00000000,                ///< 未知网络
};

/**
 * @brief 业务类型
 *
 */
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

        return *this;
    }
} UserInfo;



Q_DECLARE_METATYPE(UserInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const UserInfo &user)
{
    argument.beginStructure();
    argument << user.userId << user.network << user.addr << user.appId << user.optId << user.userName 
        << user.lte_ip << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, UserInfo &user)
{
    argument.beginStructure();
    argument >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName 
        >> user.lte_ip >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip;

    argument.endStructure();

    return argument;
}


#endif

