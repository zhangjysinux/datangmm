#include "MutiCast.h"
#include "Server.h"
#include "Client.h"
Client* clientInstance;
MutiCast::MutiCast(std::string mcAddr,int mcPort,int udpPort)
{
    m_pServer = new Server(mcAddr,mcPort,udpPort,this);
    clientInstance = new Client(mcAddr,mcPort,udpPort,this);
}

MutiCast::~MutiCast()
{
    delete m_pServer;
    delete clientInstance;
}

bool MutiCast::Init(std::vector<std::string > &vOtherMembers)
{
    SetGroupIps(vOtherMembers);
   m_pServer->Init();
    clientInstance->Init();
    std::string localIp;
    clientInstance->GetLocalIp(std::string("wlan0"),localIp);
    return true;
}

bool MutiCast::Start()
{
   m_pServer->Start();
    clientInstance->Start();
    return true;
}
bool MutiCast::SendData(std::string fileName,std::string path)
{
    m_pServer->SetFileName(fileName);
    m_pServer->SendData(path);
    return true;
}

void MutiCast::SetGroupIps(std::vector<std::string > &vOtherMembers)
{
    m_pServer->SetOtherMemIp(vOtherMembers);
}

void MutiCast::SetRecivePath(std::string recPath)
{
    clientInstance->SetRecivePath(recPath);
}

void MutiCast::RecvComplete(int recvNum,int allNum,std::string recFileName)
{

}
void MutiCast::SendComplete(int sendNum,int allNum)
{

}

bool MutiCast::Stop()
{

}
