#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "UdpServer.h"
#include "SegData.h"
#include "UnCompleTab.h"
#include <iostream>
extern UdpServer* udpServerInstance;
pthread_mutex_t m_lock;

void *UdpRun(void* ptr)
{
    udpServerInstance->Run();
}

UdpServer::UdpServer(int udpPort)
{
    m_iUdpPort = udpPort;
    m_sockfd = -1;
}

UdpServer::~UdpServer()
{

}

void UdpServer::Init(int  sockfd)
{
    InitUdpSocket(sockfd);
}

void UdpServer::Start()
{
    pthread_t id;
    int i,ret;
    ret=pthread_create(&id,NULL,UdpRun,NULL);
    if(ret!=0)
    {
        printf ("Create pthread error!\n");
        exit (1);
    }
    //    for(i=0;i<3;i++)
    //    {
    //        printf("This is the UdpServer.\n");
    //        sleep(1);
    //    }
    //pthread_join(id,NULL);
}

void UdpServer::Run()
{
    printf("enter UdpServer thread!.\n");
    unsigned char recvline[ACK_SIZE];
    while(true)
    {
        recvfrom(m_sockfd, recvline, ACK_SIZE, 0, NULL, NULL);
        ProecssAck(recvline);
    }
    close(m_sockfd);
}

void UdpServer::InitUdpSocket(int  sockfd)
{
    if(m_iUdpPort <= 0)
        return;
    m_sockfd = sockfd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_iUdpPort);
    bind(m_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

void UdpServer::ProecssAck(unsigned char* ackBuffer)
{

    AckData ackData;
    ackData.UnSerialization(ackBuffer);
    printf("process ack seq = %d romote addr=%s\n",ackData.segSeq,ackData.destIP);
    pthread_mutex_lock(&m_lock);
    m_pUnCompleTab->RemoveIpInfo(ackData.segSeq,ackData.destIP,ackData.mcAdrr);
    pthread_mutex_unlock(&m_lock);
}

void UdpServer::SetUnCompleTab(UnCompleTab* pUnCompleTab)
{
    m_pUnCompleTab = pUnCompleTab;
}
