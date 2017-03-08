#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "UdpClient.h"
#include <iostream>
#define ACK_SEND_NUM 1
UdpClient::UdpClient(int udpPort)
{
    m_iUdpPort = udpPort;
}
UdpClient::~UdpClient()
{

}

void UdpClient::Init(int socketfd)
{
    sockfd = socketfd;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_iUdpPort);

}

void UdpClient::InitRemoteIp(std::string  remoteip)
{

}

bool UdpClient::SendAck(AckData &ackData)
{
    unsigned char ackBuffer[ACK_SIZE];
    ackData.Serialization(ackBuffer);
    for(int i = 0 ; i < ACK_SEND_NUM ; ++i)
    {
        sendto(sockfd, ackBuffer, ACK_SIZE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
     usleep(1000);
    }
}

void UdpClient::Close()
{
    close(sockfd);
}
void UdpClient::InitRemoteIp(std::vector<int> vRemoteIp)
{
    char initIp[IP_LEN];
    if(vRemoteIp.size() == 4)
    {
        sprintf(initIp,"%d.%d.%d.%d",vRemoteIp[0],vRemoteIp[1],vRemoteIp[2],vRemoteIp[3]);
    }
    printf ("init remote ip %s to  inet_addr!\n",initIp);

    servaddr.sin_addr.s_addr = inet_addr(initIp);
}
