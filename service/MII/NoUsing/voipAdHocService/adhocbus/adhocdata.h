#ifndef ADHOCDATA_H
#define ADHOCDATA_H
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusError>
#include <QDebug>
#include <QDBusArgument>
#include <QMetaType>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct
{
    u16 u16Opcode;
    u16 u16FrameNum;
    u16 u16TimeSlotNum;
    u16 u16MsgLength;
}Stru_Msg_Header;

typedef struct
{
    Stru_Msg_Header struMsgHeader;		/* 系统消息头，u16Opcode固定取值为5 */
    uint8  u8ServType;		/* 业务类型，语音为8，视频为9  */
    uint8  u8PortNum;		/* 端口数量，不超过4*/
    uint8   u8Reserved; 	/*保留字段，默认为0*/
    uint8   u8ServiceNum;	/* 业务标识的有效位数*/
    uint8   au8ServiceId[64];		//业务标识，由发起端的主控模块分配唯一的标识，
    //在主控模块与自组网模块的通信中，用于识别业务。
    //同一个业务，建立正向链路和逆向链路时应采用与源节点一致的业务标识，
    //如果数组未填满，剩余空间固定填为0 */
    uint32 u32ServRate;	/* 业务速率，单位为bps，自组网模块根据该值确定资源的数量 */
    uint16 au16PortList[4];	/* 端口列表如果数组未填满，剩余空间固定填为0  */
    u32 u32DstId;		/*  目的节点标识，由点分格式的IP地址转换的无符号型数值 */
    u32 u32Src;			/*  源节点标识，由点分格式的IP地址转换的无符号型数值  */
}Stru_mcmwnu_link_setup_req;

typedef struct
{
    Stru_Msg_Header struMsgHeader;		/* 系统消息头，u16Opcode固定取值为6 */
    u8 u8ServType;		/* 业务类型，语音为8，视频为9  */
    u8 u8Result;			/* 建链结果，0为成功，1为失败 */
    u8 u8Reserved;  /*保留字段，默认为0*/
    u8 u8ServiceNum; /* 业务标识的有效位数*/
    u8 au8ServiceId[64];	/* 业务标识，与建链请求中的业务标识一致 ，如果数组未填满，剩余空间固定填为0  */
}Stru_mcmwnu_link_setup_rsp;

/*
自组网模块给主控模块发送链路状态指示消息

signal name="LinkStatusRsp"

<arg name="link_status_rsp" type="（（qqqq）yyyyay）" >

参数说明：
*/
typedef struct tag_STRU_WNUMCM_LINK_STATUS_IND
{
    Stru_Msg_Header struMsgHeader;             /* 系统消息头，u16Opcode固定取值为7 */
    u8 u8ServType;         /* 业务类型，语音为8，视频为9  */
    u8 u8Status;                     /* 链路状态，0为有效，1为无效 */
    u8 u8Reserved; /*保留字段，默认为0*/
    u8 u8ServiceNum; /* 业务标识的有效位数*/
    u8 au8ServiceId[64];       /* 业务标识，与建链请求中的业务标识一致，如果数组未填满，剩余空间固定填为0   */
}Ril_stru_mcmwnu_link_status_ind;

/*
主控模块给自组网模块回复链路状态确认消息

method name="LinkStatusConform"

in：<arg name="link_status_conform" type="（（qqqq）yyyyay）" >

out：none

参数说明：
*/

typedef struct
{
    Stru_Msg_Header RstruMsgHeader;          /* 系统消息头，u16Opcode固定取值为8 */
    u8 u8ServType;         /* 业务类型，语音为8，视频为9  */
    u8 u8Status;                     /* 业务状态，0为有效，1为无效 */
    u8 u8Reserved; /*保留字段，默认为0*/
    u8 u8ServiceNum; /* 业务标识的有效位数*/
    u8 au8ServiceId[64];       /* 业务标识，与建链请求中的业务标识一致，如果数组未填满，剩余空间固定填为0   */
}stru_mcmwnu_link_status_cnf;

/*
主控模块给自组网模块发送链路删除请求消息

method name="LinkDel"

in：<arg name="link_del_req" type="（（qqqq）yayyy）" >

out：none

参数说明：
*/

typedef struct
{
    Stru_Msg_Header RstruMsgHeader;          /* 系统消息头，u16Opcode固定取值为9 */
    u8 u8ServType;         /* 业务类型，语音为8，视频为9  */
    u8 au8Reserved[2];          /* 保留字段，默认为0 */
    u8 u8ServiceNum; /* 业务标识的有效位数*/
    u8 au8ServiceId[64];       /* 业务标识，与建链请求中的业务标识一致 */
}stru_mcmwnu_link_delt_req;


/*
自组网模块给主控模块发送链路删除响应消息

signal name="LinkDelRsp"

<arg name="link_del_rsp" type="（（qqqq）yayyay）" >

参数说明：
*/

typedef struct
{
    Stru_Msg_Header struMsgHeader;              /* 系统消息头，u16Opcode固定取值为10 */
    u8 u8ServType;          /* 业务类型，语音为8，视频为9  */
    u8 au8Reserved[2];           /* 保留字段，默认为0 */
    u8 u8ServiceNum; /* 业务标识的有效位数*/
    u8 au8ServiceId[64]; /* 业务标识，与建链请求中的业务标识一致 ，如果数组未填满，剩余空间固定填为0   */
}stru_mcmwnu_link_delt_rsp;


Q_DECLARE_METATYPE(Stru_Msg_Header)
Q_DECLARE_METATYPE(Stru_mcmwnu_link_setup_req)
Q_DECLARE_METATYPE(Stru_mcmwnu_link_setup_rsp)
Q_DECLARE_METATYPE(Ril_stru_mcmwnu_link_status_ind)
Q_DECLARE_METATYPE(stru_mcmwnu_link_status_cnf)
Q_DECLARE_METATYPE(stru_mcmwnu_link_delt_req)
Q_DECLARE_METATYPE(stru_mcmwnu_link_delt_rsp)


inline QDBusArgument &operator<<(QDBusArgument &argument, const Stru_Msg_Header &info)
{
    argument.beginStructure();

    argument << info.u16Opcode;
    argument << info.u16FrameNum;
    argument << info.u16TimeSlotNum;
    argument << info.u16MsgLength;

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Stru_Msg_Header &info)
{
    argument.beginStructure();

    argument >> info.u16Opcode;
    argument >> info.u16FrameNum;
    argument >> info.u16TimeSlotNum;
    argument >> info.u16MsgLength;

    argument.endStructure();
    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const Stru_mcmwnu_link_setup_req &info)
{
    argument.beginStructure();

    argument << info.struMsgHeader;

    argument << info.u8ServType;
    argument << info.u8PortNum;
    argument << info.u8Reserved;
    argument << info.u8ServiceNum;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];
    argument.endArray();

    argument << info.u32ServRate;

    argument.beginArray(qMetaTypeId<uint16>());
    for(int i=0; i<4; i++)
        argument << info.au16PortList[i];
    argument.endArray();

    argument << info.u32DstId;
    argument << info.u32Src;

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Stru_mcmwnu_link_setup_req &info)
{
    argument.beginStructure();

    argument >> info.struMsgHeader;

    argument >> info.u8ServType;
    argument >> info.u8PortNum;
    argument >> info.u8Reserved;
    argument >> info.u8ServiceNum;

    argument.beginArray();
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];
    argument.endArray();

    argument >> info.u32ServRate;

    argument.beginArray();
    for(int i=0; i<4; i++)
        argument >> info.au16PortList[i];
    argument.endArray();

    argument >> info.u32DstId;
    argument >> info.u32Src;

    argument.endStructure();
    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const Stru_mcmwnu_link_setup_rsp &info)
{
    argument.beginStructure();

    argument << info.struMsgHeader;

    argument << info.u8ServType;
    argument << info.u8Result;
    argument << info.u8Reserved;
    argument << info.u8ServiceNum;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Stru_mcmwnu_link_setup_rsp &info)
{
    argument.beginStructure();

    argument >> info.struMsgHeader;

    argument >> info.u8ServType;
    argument >> info.u8Result;
    argument >> info.u8Reserved;
    argument >> info.u8ServiceNum;

    argument.beginArray();
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}


inline QDBusArgument &operator<<(QDBusArgument &argument, const Ril_stru_mcmwnu_link_status_ind &info)
{
    argument.beginStructure();

    argument << info.struMsgHeader;

    argument << info.u8ServType;
    argument << info.u8Status;
    argument << info.u8Reserved;
    argument << info.u8ServiceNum;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Ril_stru_mcmwnu_link_status_ind &info)
{
    argument.beginStructure();

    argument >> info.struMsgHeader;

    argument >> info.u8ServType;
    argument >> info.u8Status;
    argument >> info.u8Reserved;
    argument >> info.u8ServiceNum;

    argument.beginArray();
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const stru_mcmwnu_link_status_cnf &info)
{
    argument.beginStructure();

    argument << info.RstruMsgHeader;

    argument << info.u8ServType;
    argument << info.u8Status;
    argument << info.u8Reserved;
    argument << info.u8ServiceNum;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, stru_mcmwnu_link_status_cnf &info)
{
    argument.beginStructure();

    argument >> info.RstruMsgHeader;

    argument >> info.u8ServType;
    argument >> info.u8Status;
    argument >> info.u8Reserved;
    argument >> info.u8ServiceNum;

    argument.beginArray();
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}


inline QDBusArgument &operator<<(QDBusArgument &argument, const stru_mcmwnu_link_delt_req &info)
{
    argument.beginStructure();

    argument << info.RstruMsgHeader;
    argument << info.u8ServType;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<2; i++)
        argument << info.au8Reserved[i];
    argument.endArray();

    argument << info.u8ServiceNum;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];
    argument.endArray();
//    argument << info.au8ServiceId[0];

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, stru_mcmwnu_link_delt_req &info)
{
    argument.beginStructure();

    argument >> info.RstruMsgHeader;
    argument >> info.u8ServType;

    argument.beginArray();
    for(int i=0; i<2; i++)
        argument >> info.au8Reserved[i];
    argument.endArray();

    argument >> info.u8ServiceNum;

    argument.beginArray();
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];
    argument.endArray();
//    argument >> info.au8ServiceId[0];

    argument.endStructure();
    return argument;
}


inline QDBusArgument &operator<<(QDBusArgument &argument, const stru_mcmwnu_link_delt_rsp &info)
{
    argument.beginStructure();

    argument << info.struMsgHeader;
    argument << info.u8ServType;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<2; i++)
        argument << info.au8Reserved[i];
    argument.endArray();

    argument << info.u8ServiceNum;

    argument.beginArray(qMetaTypeId<uint8>());
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, stru_mcmwnu_link_delt_rsp &info)
{
    argument.beginStructure();

    argument >> info.struMsgHeader;
    argument >> info.u8ServType;

    argument.beginArray();
    for(int i=0; i<2; i++)
        argument >> info.au8Reserved[i];
    argument.endArray();

    argument >> info.u8ServiceNum;

    argument.beginArray();
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];
    argument.endArray();

    argument.endStructure();
    return argument;
}


#endif // ADHOCDATA_H
