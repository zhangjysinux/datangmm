#ifndef ADHOCDATA_H
#define ADHOCDATA_H
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusError>
#include <QDebug>
#include <QDBusArgument>

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
{     //(
    Stru_Msg_Header struMsgHeader;	//(qqqq)
    u16 u16MemberNum;	//q
    u8 au8Reserved[2];    //ay
    u32 u32GroupId;		//u
    u32 au32MemberId[64];	 //au
} stru_mcmwnu_group_info_cfg_req;  //) ((qqqq)qayuau)

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

Q_DECLARE_METATYPE(stru_mcmwnu_group_info_cfg_req)
Q_DECLARE_METATYPE(Stru_mcmwnu_link_setup_req)

inline QDBusArgument &operator<<(QDBusArgument &argument, const stru_mcmwnu_group_info_cfg_req &info)
{
    argument.beginStructure();

    argument << info.struMsgHeader.u16Opcode;
    argument << info.struMsgHeader.u16FrameNum;
    argument << info.struMsgHeader.u16TimeSlotNum;
    argument << info.struMsgHeader.u16MsgLength;

    argument << info.u16MemberNum;
    argument << info.au8Reserved[0];
    argument << info.au8Reserved[1];
    argument << info.u32GroupId;
    for(int i=0; i<64; i++)
        argument << info.au32MemberId[i];

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, stru_mcmwnu_group_info_cfg_req &info)
{
    argument.beginStructure();

    argument >> info.struMsgHeader.u16Opcode;
    argument >> info.struMsgHeader.u16FrameNum;
    argument >> info.struMsgHeader.u16TimeSlotNum;
    argument >> info.struMsgHeader.u16MsgLength;

    argument >> info.u16MemberNum;
    argument >> info.au8Reserved[0];
    argument >> info.au8Reserved[1];
    argument >> info.u32GroupId;
    for(int i=0; i<64; i++)
        argument >> info.au32MemberId[i];

    argument.endStructure();
    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const Stru_mcmwnu_link_setup_req &info)
{
    argument.beginStructure();

    argument << info.struMsgHeader.u16Opcode;
    argument << info.struMsgHeader.u16FrameNum;
    argument << info.struMsgHeader.u16TimeSlotNum;
    argument << info.struMsgHeader.u16MsgLength;

    argument << info.u8ServType;
    argument << info.u8PortNum;
    argument << info.u8Reserved;
    argument << info.u8ServiceNum;
    for(int i=0; i<64; i++)
        argument << info.au8ServiceId[i];

    argument << info.u32ServRate;
    for(int i=0; i<4; i++)
        argument << info.au16PortList[i];

    argument << info.u32DstId;
    argument << info.u32Src;

    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Stru_mcmwnu_link_setup_req &info)
{
    argument.beginStructure();

    argument >> info.struMsgHeader.u16Opcode;
    argument >> info.struMsgHeader.u16FrameNum;
    argument >> info.struMsgHeader.u16TimeSlotNum;
    argument >> info.struMsgHeader.u16MsgLength;

    argument >> info.u8ServType;
    argument >> info.u8PortNum;
    argument >> info.u8Reserved;
    argument >> info.u8ServiceNum;
    for(int i=0; i<64; i++)
        argument >> info.au8ServiceId[i];

    argument >> info.u32ServRate;
    for(int i=0; i<4; i++)
        argument >> info.au16PortList[i];

    argument >> info.u32DstId;
    argument >> info.u32Src;

    argument.endStructure();
    return argument;
}

#endif // ADHOCDATA_H
