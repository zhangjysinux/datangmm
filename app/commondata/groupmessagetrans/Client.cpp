#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <QNetworkInterface>
#include <QHostInfo>
#include "Client.h"
#include "MutiCastClient.h"
#include "UdpClient.h"
#include "CfileData.h"
#include "SegData.h"
#include "MutiCast.h"
#include <iostream>
extern Client* clientInstance;
void *ClientRun(void* ptr)
{
    clientInstance->Run();

}

Client::Client(std::string mcAdder,int mcPort,int udpPort,MutiCast* pMutiCast)
{
    m_pMutiCastClient = new MutiCastClient(mcAdder,mcPort);
    m_pUdpClient = new UdpClient(udpPort);
    m_pCFileData = new CFileData();
    m_preFileID = -1;
    m_pMutiCast = pMutiCast;

}

Client::~Client()
{
    delete m_pMutiCastClient;
    delete m_pUdpClient;
    delete m_pCFileData;
}

void Client::Init()
{
    /* create what looks like an ordinary UDP socket */
    int fd = 0;
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    std::string localIp;
    clientInstance->GetLocalIp(std::string("eth0"),localIp);
   std::cout<<"local ip"<<localIp<<std::endl;
   m_localAddress = localIp;
    //memcpy(m_localAddress,localIp.c_str(),IP_LEN);
    m_pMutiCastClient->Init(fd);
    m_pUdpClient->Init(fd);
}

void Client::Start()
{
    pthread_t id;
    int i,ret;
    ret=pthread_create(&id,NULL,ClientRun,NULL);
    if(ret!=0)
    {
        printf ("Create pthread error!\n");
        exit (1);
    }
    //pthread_join(id,NULL);
}

void Client::Run()
{
    printf ("enter Client Run!!!!!!!!!!!!!!\n");
    while(1)
    {
        SegData segData;
        m_pMutiCastClient->Receive(segData);

        printf ("receive Data !!!!!!!!!!!!!!\n");
        std::vector<int> sourceIp = AnalysIp(segData.sourceIP);
        char localAddress[IP_LEN];
        memcpy(localAddress,m_localAddress.c_str(),IP_LEN);
        std::vector<int> localIp = AnalysIp(localAddress);
        if(IsSameIp(sourceIp,localIp))
            continue;

        if(segData.segSeq != -1 && segData.length!= 0)
        {
            //printf ("init remote ip %s to  inet_addr!\n",segData.sourceIP);
            char remoteipbuf[IP_LEN+1];
            memcpy(remoteipbuf,segData.sourceIP,IP_LEN);
            remoteipbuf[IP_LEN]=0;
           // remoteip = remoteipbuf；
           //std::string remoteip(remoteipbuf);

           //std::cout<<"remote ip!\n"<<remoteip<<std::endl;
            //std::vector<int> vSourceIp = AnalysIp(segData.sourceIP);

            if(m_vPreSourceIp.empty() || (!IsSameIp(sourceIp,m_vPreSourceIp)))
            {
                m_pUdpClient->InitRemoteIp(sourceIp);
            }

            m_preFileID = segData.fileID;
            // send ack to server
            AckData ackData;
            memcpy(ackData.destIP,m_localAddress.c_str(),IP_LEN);
            ackData.fileID= segData.fileID;
            ackData.segSeq = segData.segSeq;
            m_pUdpClient->SendAck(ackData);
            printf ("send ack to source ok !\n");
        }

        if(preRecfileName != segData.fileName)
        {
            std::string filename = m_pCFileData->GetRecName();
            m_pCFileData->AddSegData(segData);
            m_pMutiCast->RecvComplete(m_pCFileData->GetRecSize(),segData.segnum,filename);
        }


        //check if file translateing complete
        if(!m_pCFileData->IsFileRecFinished())
            continue;

        std::string recFileName = m_pCFileData->GetRecName();
        if(recFileName != preRecfileName.c_str())
        {

            m_pCFileData->CreateFile();
            std::string filename = m_pCFileData->GetRecName();
            m_pCFileData->ResetData();
            m_pMutiCast->RecvComplete(segData.segnum,segData.segnum,filename);

            preRecfileName = recFileName;
        }
    }
}

std::vector<int> Client::AnalysIp( char* ipBuffer)
{
    const  char *d = ".";
    int number;
    std::vector<int> vecAddress;
    char *p;
    p = strtok(ipBuffer,d);
    while(p)
    {
        number = atoi(p);
        vecAddress.push_back(number);
        p=strtok(NULL,d);
    }
    for(int i = 0 ; i < vecAddress.size() ; ++i)
        //std::cout<<vecAddress.at(i)<<" ";
        //std::cout<<std::endl;
        return vecAddress;
}
unsigned int Client::CreateIpValue(std::vector<int>  &vIp)
{
    unsigned int value = 0;
    if(vIp.size()!=4)
    {
        return -1;
    }else
    {
        value = vIp[0]*255*255*255+vIp[1]*255*255+vIp[2]*255+vIp[3];
    }
    return value;
}
bool Client::IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp)
{
    if(vSourceIp.empty())
    {
        return false;
    }

    if(vDestiIp.empty())
    {
        return false;
    }

    if(vSourceIp.size() != vDestiIp.size())
    {
        std::cout<<"size not same "<<std::endl;
        std::cout<<"vSourceIpsize "<<vSourceIp.size()<<std::endl;
        //std::cout<<vSourceIp<<std::endl;
        std::cout<<"vDestiIpsize "<<vDestiIp.size()<<std::endl;
        return false;
    }else
    {
        for(int i = 0 ; i < vSourceIp.size();++i)
        {
            std::cout<<vSourceIp.at(i)<<" "<<vDestiIp.at(i)<<std::endl;
            if(vSourceIp.at(i)!= vDestiIp.at(i))
            {
                return false;
            }
        }
    }
    return true;
}

void Client::PrintLocalAllIp()
{
    printf("start print local all ip\n");
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;
    getifaddrs(&ifAddrStruct);
    while (ifAddrStruct!=NULL)
    {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET)
        { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
        }
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    printf("ifAddrStruct==NULL\n");
}

void Client::SetRecivePath(std::string recvPath)
{
    m_pCFileData->SetRecivePath(recvPath);
}

bool Client::GetLocalIp(std::string ifaname,std::string &localIp)
{
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
        qDebug() << "Address:" << address;
        if(address.toString().contains("192.168.1."))
        {
            localIp = address.toString().toStdString();
        }
    }
    return true;
//    struct ifaddrs * ifAddrStruct=NULL;
//    void * tmpAddrPtr=NULL;
//    getifaddrs(&ifAddrStruct);
//    while (ifAddrStruct!=NULL)
//    {
//        if (ifAddrStruct->ifa_addr->sa_family==AF_INET)
//        { // check it is IP4
//            // is a valid IP4 Address
//            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//            char addressBuffer[INET_ADDRSTRLEN];
//            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
//            if(ifaname ==ifAddrStruct->ifa_name )
//            {
//                localIp = addressBuffer;
//                printf("%s IP Address %s\n", ifAddrStruct->ifa_name, localIp.c_str());
//                break;
//            }
//            //printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
//        }
//        ifAddrStruct=ifAddrStruct->ifa_next;
//    }
//    printf("ifAddrStruct==NULL\n");
}
