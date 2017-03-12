#include "adhocconnectadaptor.h"
#include <QDBusMetaType>
#include <QDBusReply>
#include <string>
#include <QDebug>
#include <QDBusError>

using std::string;


//AdhocConnectAdaptor *AdhocConnectAdaptor::instance = NULL;

//AdhocConnectAdaptor *AdhocConnectAdaptor::getInstance()
//{
//    if(instance == NULL)
//    {
//        instance = new AdhocConnectAdaptor();
//    }

//    return instance;
//}

AdhocConnectAdaptor::AdhocConnectAdaptor(QObject *parent) :
    QObject(parent),
    m_interface(serviceName,
                servicePath,
                serviceInterfaceName,
                QDBusConnection::systemBus())
{
#ifdef voipAdHocService

//    QDBusConnection bus = QDBusConnection::sessionBus();
//    qDebug() << "bus create-----";
//    if(!bus.registerService(VoipAdhocServiceName))
//        qDebug() << "bus error:" << bus.lastError().message();

//    bool regflag = bus.registerObject(VoipAdhocServicePath, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
//    if(!regflag)
//        qDebug() << "bus error:" << "register adhoc false" <<bus.lastError().message();

#endif

    qDBusRegisterMetaType<Stru_Msg_Header>();
    qDBusRegisterMetaType<Stru_mcmwnu_link_setup_req>();
    qDBusRegisterMetaType<Stru_mcmwnu_link_setup_rsp>();

    qDBusRegisterMetaType<Ril_stru_mcmwnu_link_status_ind>();
    qDBusRegisterMetaType<stru_mcmwnu_link_status_cnf>();
    qDBusRegisterMetaType<stru_mcmwnu_link_delt_req>();
    qDBusRegisterMetaType<stru_mcmwnu_link_delt_rsp>();

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(m_interface.isValid())
    {
        qDebug() << "ofnod is valid";
        sessionBus.connect(serviceName,
                           servicePath,
                           serviceInterfaceName,
                           "LinkSetupRsp",
                           this,
                           SLOT(onLinkSetupRsp(Stru_mcmwnu_link_setup_rsp)));

        sessionBus.connect(serviceName,
                           servicePath,
                           serviceInterfaceName,
                           "LinkStatusRsp",
                           this,
                           SLOT(onLinkStatusRsp(Ril_stru_mcmwnu_link_status_ind)));

        sessionBus.connect(serviceName,
                           servicePath,
                           serviceInterfaceName,
                           "LinkDelRsp",
                           this,
                           SLOT(onLinkDelRsp(stru_mcmwnu_link_delt_rsp)));
    }
    else
    {
        qDebug() << "m_interface error" << m_interface.lastError().message();
    }
    onGetDevIp();
}

QString AdhocConnectAdaptor::onGetDevIp()
{
    qDebug() << "AdhocConnectAdaptor::onGetDevIp";

    QString value;

    QDBusReply<QString> reply = m_interface.call("GetDevIp");
    if(reply.isValid())
    {
        value = reply.value();
        qDebug() << "GetDevIp reply success:";
    }
    else
    {
        qDebug() << "GetDevIp reply error:" << reply.error();
    }

    return value;
}

void AdhocConnectAdaptor::LinkSetup(const Stru_mcmwnu_link_setup_req &link_setup)
{
    qDebug() << "AdhocConnectAdaptor::onLinkSetup";

    char *link_setupP = (char*)&link_setup;

    QDBusReply<void> reply = m_interface.call("LinkSetup", QVariant::fromValue(link_setup));
    if(reply.isValid())
    {
        qDebug() << "LinkSetup reply success:";
    }
    else
    {
        qDebug() << "LinkSetup reply error:" << reply.error();
    }

    return;
}

void AdhocConnectAdaptor::LinkSetup()
{
    Stru_mcmwnu_link_setup_req link_setup;

    memset((char*)&link_setup, sizeof(Stru_mcmwnu_link_setup_req), 0);
    link_setup.struMsgHeader.u16Opcode = 5;
    link_setup.struMsgHeader.u16FrameNum = 0;
    link_setup.struMsgHeader.u16TimeSlotNum = 0;
    link_setup.struMsgHeader.u16MsgLength = 0x0058;
    qDebug() << "u16Opcode"
             << link_setup.struMsgHeader.u16Opcode
             << link_setup.struMsgHeader.u16FrameNum
             << link_setup.struMsgHeader.u16TimeSlotNum
             << link_setup.struMsgHeader.u16MsgLength;
    link_setup.u8ServType = 8;
    link_setup.u8PortNum = 1;
    link_setup.u8Reserved = 0;
    link_setup.u8ServiceNum = 1;
    qDebug() << "u8ServType"
             << link_setup.u8ServType
             << link_setup.u8PortNum
             << link_setup.u8Reserved
             << link_setup.u8ServiceNum;
    for(int i=0; i<64; i++)
        link_setup.au8ServiceId[i] = 0;
    link_setup.au8ServiceId[0] = 1;

    for(int i=0; i<64; i++)
        qDebug() << "au8ServiceId" << i << link_setup.au8ServiceId[i];

    link_setup.u32ServRate = 8;
    link_setup.au16PortList[0] = 0x2711;
    link_setup.au16PortList[1] = 0;
    link_setup.au16PortList[2] = 0;
    link_setup.au16PortList[3] = 0;
    link_setup.u32DstId = iptouint("192.168.42.10");
    link_setup.u32Src = iptouint("192.168.42.9");

    qDebug() << "u32ServRate: "
             << link_setup.au16PortList[0]
             << link_setup.au16PortList[1]
             << link_setup.au16PortList[2]
             << link_setup.au16PortList[3]
             << link_setup.u32DstId
             << link_setup.u32Src;

    LinkSetup(link_setup);
}

void AdhocConnectAdaptor::LinkStatusConform(stru_mcmwnu_link_status_cnf link_status_cnf)
{
    qDebug() << "AdhocConnectAdaptor::LinkStatusConform";

    QDBusReply<void> reply = m_interface.call("LinkStatusConform", QVariant::fromValue(link_status_cnf));
    if(reply.isValid())
    {
        qDebug() << "LinkStatusConform reply success:";
    }
    else
    {
        qDebug() << "LinkStatusConform reply error:" << reply.error();
    }

    return;
}

void AdhocConnectAdaptor::LinkDel(stru_mcmwnu_link_delt_req link_delt_req)
{
    qDebug() << "AdhocConnectAdaptor::LinkDel";

    QDBusReply<void> reply = m_interface.call("LinkDel", QVariant::fromValue(link_delt_req));
    if(reply.isValid())
    {
        qDebug() << "LinkDel reply success:";
    }
    else
    {
        qDebug() << "LinkDel reply error:" << reply.error();
    }

    return;
}

void AdhocConnectAdaptor::buildConnect(const QString &srcAddr, const QString &destAddr, int port)
{
    Stru_mcmwnu_link_setup_req link_setup;

    memset((char*)&link_setup, sizeof(Stru_mcmwnu_link_setup_req), 0);
    link_setup.struMsgHeader.u16Opcode = 5;
    link_setup.struMsgHeader.u16FrameNum = 0;
    link_setup.struMsgHeader.u16TimeSlotNum = 0;
    link_setup.struMsgHeader.u16MsgLength = 0x0058;
    qDebug() << "u16Opcode"
             << link_setup.struMsgHeader.u16Opcode
             << link_setup.struMsgHeader.u16FrameNum
             << link_setup.struMsgHeader.u16TimeSlotNum
             << link_setup.struMsgHeader.u16MsgLength;
    link_setup.u8ServType = 8;
    link_setup.u8PortNum = 1;
    link_setup.u8Reserved = 0;
    link_setup.u8ServiceNum = 1;
    qDebug() << "u8ServType"
             << link_setup.u8ServType
             << link_setup.u8PortNum
             << link_setup.u8Reserved
             << link_setup.u8ServiceNum;
    for(int i=0; i<64; i++)
        link_setup.au8ServiceId[i] = 0;
    link_setup.au8ServiceId[0] = 1;

    for(int i=0; i<64; i++)
        qDebug() << "au8ServiceId" << i << link_setup.au8ServiceId[i];

    link_setup.u32ServRate = 16000;
    link_setup.au16PortList[0] = port;
    link_setup.au16PortList[1] = 0;
    link_setup.au16PortList[2] = 0;
    link_setup.au16PortList[3] = 0;
    link_setup.u32DstId = iptouint(destAddr.toLocal8Bit().data());
    link_setup.u32Src = iptouint(srcAddr.toLocal8Bit().data());

    qDebug() << "u32ServRate: "
             << link_setup.au16PortList[0]
             << link_setup.au16PortList[1]
             << link_setup.au16PortList[2]
             << link_setup.au16PortList[3]
             << link_setup.u32DstId
             << link_setup.u32Src;

    LinkSetup(link_setup);
}

void AdhocConnectAdaptor::deleteConnect()
{
    qDebug() << "AdhocConnectAdaptor::deleteConnect()";
    stru_mcmwnu_link_delt_req link_delt_req;
    link_delt_req.RstruMsgHeader.u16Opcode = 9;
    link_delt_req.RstruMsgHeader.u16FrameNum = 0;
    link_delt_req.RstruMsgHeader.u16TimeSlotNum = 0;
    link_delt_req.RstruMsgHeader.u16MsgLength = 0x0044;

    link_delt_req.u8ServType = 8;
    link_delt_req.au8Reserved[0] = 0;
    link_delt_req.au8Reserved[1] = 0;

    link_delt_req.u8ServiceNum = 1;
    for(int i=0; i<64; i++)
    {
        link_delt_req.au8ServiceId[i] = 0;
    }
    link_delt_req.au8ServiceId[0] = 1;

    LinkDel(link_delt_req);
}

void AdhocConnectAdaptor::deleteConnect(stru_mcmwnu_link_status_cnf link_status_cnf)
{
    qDebug() << "AdhocConnectAdaptor::deleteConnect(stru_mcmwnu_link_status_cnf link_status_cnf)";

    stru_mcmwnu_link_delt_req link_delt_req;
    link_delt_req.RstruMsgHeader.u16Opcode = 9;
    link_delt_req.RstruMsgHeader.u16FrameNum = 0;
    link_delt_req.RstruMsgHeader.u16TimeSlotNum = 0;
    link_delt_req.RstruMsgHeader.u16MsgLength = 0x0044;

    link_delt_req.u8ServType = 8;
    link_delt_req.au8Reserved[0] = 0;
    link_delt_req.au8Reserved[1] = 0;

    link_delt_req.u8ServiceNum = link_status_cnf.u8ServiceNum;
    for(int i=0; i<64; i++)
    {
        link_delt_req.au8ServiceId[i] = link_status_cnf.au8ServiceId[i];
    }
    LinkDel(link_delt_req);
}

void AdhocConnectAdaptor::onLinkSetupRsp(Stru_mcmwnu_link_setup_rsp link_setup)
{
    qDebug() << "AdhocConnectAdaptor::onLinkSetupRsp ---------------------";
    qDebug() << "u16opcode: " << link_setup.struMsgHeader.u16Opcode
             << "u8ServType: " << link_setup.u8ServType
             << "u8Result: " << link_setup.u8Result
             << "u8ServiceNum: " << link_setup.u8ServiceNum
             << "au8ServiceId[0]: " << link_setup.au8ServiceId[0];

    return;
}

void AdhocConnectAdaptor::onLinkStatusRsp(Ril_stru_mcmwnu_link_status_ind link_status_rsp)
{
    qDebug() << "onLinkStatusRsp ---------------------";

    stru_mcmwnu_link_status_cnf config;
    config.RstruMsgHeader.u16Opcode = 8;
    config.RstruMsgHeader.u16FrameNum = link_status_rsp.struMsgHeader.u16FrameNum;
    config.RstruMsgHeader.u16TimeSlotNum = link_status_rsp.struMsgHeader.u16TimeSlotNum;
    config.RstruMsgHeader.u16MsgLength = link_status_rsp.struMsgHeader.u16MsgLength;

    config.u8ServType = link_status_rsp.u8ServType;
    config.u8Status = link_status_rsp.u8Status;
    config.u8Reserved = link_status_rsp.u8Reserved;
    config.u8ServiceNum = link_status_rsp.u8ServiceNum;
    for(int i=0; i<64; i++)
        config.au8ServiceId[i] = link_status_rsp.au8ServiceId[i];

    if (config.u8Status == 1)
        deleteConnect(config);

    LinkStatusConform(config);
}

void AdhocConnectAdaptor::onLinkDelRsp(stru_mcmwnu_link_delt_rsp link_delt_rsp)
{
    qDebug() << "AdhocConnectAdaptor::onLinkStatusRsp ---------------------";
}



