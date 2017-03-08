#include "MutiCast.h"
#include "Server.h"
#include "Client.h"
Client* clientInstance;
MutiCast::MutiCast(int netType,std::string mcAddr,int mcPort,int udpPort,int groupId,QObject *parent):QObject(parent)
{
    m_iNetType = netType;
    m_pServer = new Server(netType,mcAddr,mcPort,udpPort,this,groupId);
    clientInstance = new Client(netType,mcAddr,mcPort,udpPort,this);
    m_iGroupId = groupId;
    qRegisterMetaType<std::string>("std::string");
}

MutiCast::~MutiCast()
{
    delete m_pServer;
    delete clientInstance;
}

bool MutiCast::init(std::vector<std::string > &vOtherMembers)
{
    setGroupIps(vOtherMembers);
    m_pServer->Init();
    clientInstance->Init();
    std::string localIp;
    clientInstance->GetLocalIp(std::string("eth0"),localIp);
    return true;
}

bool MutiCast::start()
{
    m_pServer->Start();
    clientInstance->Start();
    return true;
}
bool MutiCast::sendData(int msgtype,int fileid,std::string fileName,std::string path)
{
    m_pServer->SetFileName(fileName);
    m_pServer->SendData(msgtype,fileid,path);
    return true;
}

void MutiCast::setGroupIps(std::vector<std::string > &vOtherMembers)
{
    m_pServer->SetOtherMemIp(vOtherMembers);
}

void MutiCast::setRecivePath(std::string recPath)
{
    clientInstance->SetRecivePath(recPath);
}

void MutiCast::recvComplete(int msgtype,int fileid,int recvNum,int allNum,std::string recFileName ,int bIsSuccesd)
{
    emit signalRecvComplete(msgtype,fileid,m_iGroupId,recvNum,allNum,recFileName,bIsSuccesd);
}

void MutiCast::sendComplete(int msgtype,int fileid,int sendNum,int allNum,std::string sendFileName, int bIsSuccesd)
{
    emit signalSendComplete(msgtype,fileid,m_iGroupId,sendNum,allNum,sendFileName,bIsSuccesd);
}

bool MutiCast::stop()
{

}

void MutiCast::setGroupId(int guoupId)
{
    m_iGroupId = guoupId;
}
void MutiCast::sendFinished(bool bIsSuccesd)
{
    singalSendFinished(bIsSuccesd);
}
