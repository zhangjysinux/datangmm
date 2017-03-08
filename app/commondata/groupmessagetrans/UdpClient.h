#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_
#include "SegData.h"
#include <string>
#include <vector>
class UdpClient
{
public:
    UdpClient(int udpPort);
    ~UdpClient();
    void Init(int socketfd);
    void InitRemoteIp(std::string remoteip);
    void InitRemoteIp(std::vector<int> vRemoteIp);
   bool SendAck(AckData &ackData);
   void Close();
private:
   int sockfd;
   struct sockaddr_in servaddr;
   int m_iUdpPort;
};
#endif
