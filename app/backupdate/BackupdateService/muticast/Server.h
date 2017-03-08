#ifndef _SERVER_H_
#define _SERVER_H_
#include <string>
#include <map>
#include <vector>
#include "SegData.h"

class UdpServer;
class MutiCastServer;
class SFileData;
class UnCompleTab;
class MutiCast;
class Server
{
public:
     Server(int netType,std::string mcAddr,int mcPort,int udpPort,MutiCast* pMutiCast,int groupId);
    ~Server();
    void Init();
    bool InitRemoteIps();
    void Start();
    void SetFileName(std::string fileName);
    void SetOtherMemIp(std::vector<std::string> vOtherMemIp);
    void SendData(int msgtype,int fileid,std::string path);//file ,image, video
    void DisplayProcess(double dComplete);//display sending process

private:
    void InitUnCompleTab(int maxSeq);
    bool GetLocalIp(std::string ifaname,std::string &localIp);
    std::vector<std::string> split(std::string str,std::string pattern);
    SFileData* m_pSFileData;
    unsigned char m_localAddrIp[IP_LEN];
    UnCompleTab* m_pUnCompleTab;
    std::vector<std::string> m_vRemoteInfos;
    int m_iGroupId;

};
#endif
