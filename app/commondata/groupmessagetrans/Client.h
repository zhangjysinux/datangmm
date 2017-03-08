#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "SegData.h"
#include <vector>
#include <string>
class MutiCastClient;
class UdpClient;
class CFileData;
class MutiCast;
class Client
{
public:
    Client(std::string mcAdder,int mcPort,int udpPort,MutiCast* pMutiCast);
    ~Client();
    void Init();
    void Start();
    void Run();
    std::vector<int> AnalysIp( char* ipBuffer);
    unsigned int CreateIpValue(std::vector<int>  &vIp);
     void PrintLocalAllIp();
     bool GetLocalIp(std::string ifaname,std::string &localIp);
     void SetRecivePath(std::string recvPath);
private:
    bool IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp);

    int sockfd;
    MutiCastClient* m_pMutiCastClient;
    UdpClient* m_pUdpClient;
    CFileData* m_pCFileData;
    //char  m_localAddress[IP_LEN];
    std::string m_localAddress;
    int m_preFileID;
    std::vector<int> m_vPreSourceIp;
    MutiCast* m_pMutiCast;
    std::string preRecfileName;

};
#endif
