#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_
#include <pthread.h>
class UnCompleTab;
class UdpServer
{
public:
    UdpServer(int udpPort,int groupId);
    ~UdpServer();
    void Init(int  sockfd);
    void Start();
    void Run();
    void SetUnCompleTab(UnCompleTab* pUnCompleTab);
private:
    void InitUdpSocket(int  sockfd);
    void ProecssAck(unsigned char* ackBuffer);
    int m_sockfd;
    UnCompleTab* m_pUnCompleTab;
    int m_iUdpPort;
    int m_iGroupId;
};
#endif
