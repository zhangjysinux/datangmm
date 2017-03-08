#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include<QString>
#include<QMetaType>
#include<QDBusMetaType>
#include<QDebug>
#include <QDateTime>
namespace QT_DTT
{
#define CTS_PRINT (qDebug() <<"CommTransService adapter:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")

/**
 * @brief ��������
 *
 */
enum NetworkType
{
    AD_HOC_NETWORK = 0x00000001,                ///< ������
    LTE_NETWORK = 0x00000010,                   ///< LTE
    SATELLITE_NETWORK = 0x00000100,             ///< ��ͨ
    DIGITAL_WALKIE_TALKIE_NETWORK = 0x00001000, ///< ���ֶԽ�
    BATTLE_NETWORK = 0x00010000,                ///< ս����
    WIFI_NETWORK = 0x00100000,
    ADHOC_LTE_NETWORK = 0x01000000,
    UNKNOW_NETWORK = 0x00000000,                ///< δ֪����
};

/**
 * @brief ҵ������
 *
 */
enum TransactionType
{
   CTRL_MSG_RELIABLE_TRANS = 1,                 ///< ָ�ر���(�����ɿ�)
   INTELLIGENCE_MSG_RELIABLE_TRANS = 2,         ///< �鱨��Ϣ(�����ɿ�) 
   CTRL_MSG_UNRELIABLE_TRANS = 3,               ///< ָ�ر���(�������ɿ�/�鲥)
   INTELLIGENCE_MSG_UNRELIABLE_TRANS = 4,       ///< �鱨��Ϣ(�������ɿ�/�鲥)
   IM_TRANS= 5,                                 ///< ��ʱ��Ϣ
   FILE_TRANS = 6,                              ///< �ļ�
   IMAGE_TRANS = 7,                             ///< ͼ��
   PTT_VOICE_TRANS = 8,                         ///< PTT����
   APP_CTRL_CMD_TRANS = 9,                      ///< Ӧ������
   VOICE_CALL_TRANS = 10,                       ///< ��Ƶ����
   VIDEO_CALL_TRANS = 11,                       ///< ��Ƶ����
   MUTIL_VOICE_CALL_TRANS = 12,                 ///< �෽����
   MUTIL_VIDEO_CALL_TRANS = 13,                 ///< �෽��Ƶ 
   POSITION_SHARED = 14, 					    ///< λ�ù���
   OTHER_TRANS,                                 ///< ����ҵ��
   UNKNOW_TRANS = 0,                            ///< δ֪ҵ��
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
 * @brief user��Ϣ
 *
 */
typedef struct _userInfo
{
    int userId;               ///< user ID
    //NetworkType network;      ///< ��������
    int network;
    QString addr;             ///< ͨѶ��ַ(IP��ַ���ߵ绰����)
    QString appId;            ///< Ӧ��ID
    int optId;                ///< ҵ��ID
    QString userName;         ///< �û�����
    QString lte_ip;           ///< LTE�����IP��ַ
    QString ad_hoc_ip;        ///< ��������IP��ַ
    QString satellite_ip;     ///< ��ͨ��Ӧ��IP��ַ
    QString satellite_phone_number; ///< ��ͨ��Ӧ�ĵ绰����
    QString digital_number;   ///< ���ֶԽ��ĵ绰����
    QString battle_ip;        ///< ս������IP��ַ
    QString wifi_ip;

public:
	/**
	 * @brief ���캯������ʼ��userinfo
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
     * @brief ����"=",��ֱ����һ��UserInfo�������һ��UserInfo������и�ֵ
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

}


Q_DECLARE_METATYPE(QT_DTT::UserInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QT_DTT::UserInfo &user)
{
    argument.beginStructure();
    argument << user.userId << user.network << user.addr << user.appId << user.optId << user.userName 
        << user.lte_ip << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QT_DTT::UserInfo &user)
{
    argument.beginStructure();
    argument >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName 
        >> user.lte_ip >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
        >> user.digital_number >> user.battle_ip >> user.wifi_ip;

    argument.endStructure();

    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const QT_DTT::UserInfo &user)
{
    out << user.userId << user.network << user.addr << user.appId << user.optId << user.userName
        << user.lte_ip << user.ad_hoc_ip << user.satellite_ip << user.satellite_phone_number
        << user.digital_number << user.battle_ip << user.wifi_ip;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, QT_DTT::UserInfo &user)
{
    in >> user.userId >> user.network >> user.addr >> user.appId >> user.optId >> user.userName
       >> user.lte_ip >> user.ad_hoc_ip >> user.satellite_ip >> user.satellite_phone_number
       >> user.digital_number >> user.battle_ip >> user.wifi_ip;
    return in;
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

#endif

