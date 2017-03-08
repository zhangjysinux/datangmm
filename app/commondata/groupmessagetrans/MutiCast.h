#ifndef _MUTI_CAST_H_
#define _MUTI_CAST_H_
#include<stdio.h>
#include <string>
#include <vector>
class Server;
class Client;
class MutiCast
{
public:
    MutiCast(std::string mcAddr,int mcPort,int udpPort);
    ~MutiCast();
    bool Init(std::vector<std::string > &vOtherMembers);
    bool Start();
    bool SendData(std::string fileName,std::string path);
    void SetRecivePath(std::string recPath);
    bool Stop();
    virtual void RecvComplete(int recvNum,int allNum,std::string recFileName);
    virtual void SendComplete(int sendNum,int allNum);
private:
    void SetGroupIps(std::vector<std::string > &vOtherMembers);
    Server* m_pServer;
    Client* m_pClient;
};
#endif
