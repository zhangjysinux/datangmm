#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "MutiCastClient.h"
#define HELLO_PORT  10000
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256


MutiCastClient::MutiCastClient(std::string &mcIp,int mcPort)
{
    m_strMcIp = mcIp;
    m_iMcPort = mcPort;
}

MutiCastClient::~MutiCastClient()
{

}

void MutiCastClient::Init(int socketfd)
{
    struct ip_mreq mreq;
    u_int yes=1; /*** MODIFICATION TO ORIGINAL */
    fd = socketfd;
    /**** MODIFICATION TO ORIGINAL */
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
    {
        perror("Reusing ADDR failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        exit(1);
    }
    /*** END OF MODIFICATION TO ORIGINAL */

    /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
    addr.sin_port=htons(m_iMcPort);

    /* bind to receive address */
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0)
    {
        perror("bind ERROR#######################################");
        exit(1);
    }

    /* use setsockopt() to request that the kernel join a multicast group */
    printf("init muticast client mtuicast addrress%s!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",m_strMcIp.c_str());
    mreq.imr_multiaddr.s_addr=inet_addr(m_strMcIp.c_str());
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    printf("init sucessfull!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}


void MutiCastClient::Receive(SegData &segData)
{
    unsigned char segBuffer[SEG_SIZE];
    addrlen=sizeof(addr);
    if ((nbytes=recvfrom(fd,segBuffer,SEG_SIZE,0, (struct sockaddr *) &addr,(socklen_t*)&addrlen)) < 0)
    {
        perror("recvfrom");
        //exit(1);
    }
    segData.UnSerialization(segBuffer);
    //printf("receive segdata seq=%d len = %d\n",segData.segSeq,segData.length);
}
