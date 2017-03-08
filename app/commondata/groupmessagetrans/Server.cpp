#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include<iostream>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <QHostInfo>
#include <QNetworkInterface>
#include "Server.h"
#include "UdpServer.h"
#include "MutiCastServer.h"
#include "SfileData.h"
#include "UnCompleTab.h"
#include "MutiCast.h"
UdpServer* udpServerInstance;
MutiCastServer* mutiCastServerInstance;
pthread_mutex_t mclock;
pthread_cond_t cond;
Server::Server(std::string mcAddr,int mcPort,int udpPort,MutiCast* pMutiCast)
{
    m_pMutiCast = pMutiCast;
    udpServerInstance = new UdpServer(udpPort);
    mutiCastServerInstance = new MutiCastServer(mcAddr,mcPort);
    m_pSFileData = new SFileData();
    mutiCastServerInstance->SetmSFileData(m_pSFileData);
    m_pUnCompleTab = new UnCompleTab();
    udpServerInstance->SetUnCompleTab(m_pUnCompleTab);
    mutiCastServerInstance->SetUnCompleTab(m_pUnCompleTab);
}

Server::~Server()
{
    delete udpServerInstance;
    delete mutiCastServerInstance;
    delete m_pSFileData;
    delete m_pUnCompleTab;
}

void Server::Init()
{
    int  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    udpServerInstance->Init(sockfd);
    mutiCastServerInstance->Init(sockfd);

    std::string localIp;
    GetLocalIp(std::string("wlan0"),localIp);
    std::cout<<"local host ip:"<<localIp<<std::endl;
    m_pSFileData->SetLocalAddress(localIp.c_str());
}

bool Server::InitRemoteIps()
{
    m_vRemoteInfos.clear();
    std::ifstream infile("remoteip",std::ios::in);
    if(!infile)
        return false;
    std::cout<<"file open sucessful"<<std::endl;
    std::string   str;
    while(!infile.eof())
    {
        getline(infile,str);
        //infile.getline(str);

        if(!str.empty())
        {
                    m_vRemoteInfos.push_back(str);
                            std::cout<<"read ip:"<<str<<std::endl;
        }
    }
    infile.close();
    return true;
}

void Server::Start()
{
    udpServerInstance->Start();

    mutiCastServerInstance->Start();
}

 void Server::SetOtherMemIp(std::vector<std::string> vOtherMemIp)
 {
     m_vRemoteInfos = vOtherMemIp;
 }

 void Server::SetFileName(std::string fileName)
 {
     m_pSFileData->ClearData();
      m_pSFileData->SetFileName(fileName);
 }

void Server::SendData(std::string path)
{
    //InitRemoteIps();
    std::cout<<"send Data ptah:"<<path<<std::endl;
    printf (" enter send data!\n");
m_pUnCompleTab->ClearUnMap();


    if(!m_pSFileData->ReadFileData(path))
        return;
    m_pSFileData->SaveFile();
    int maxseq = m_pSFileData->SplitFile();
    printf (" InitUnCompleTab!\n");
    InitUnCompleTab(maxseq);

    pthread_mutex_lock(&mclock);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mclock);
}

void Server::DisplayProcess(double dComplete)
{

}

//void Server::GetLocalpAddress()
//{
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
//            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
//        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6)
//        { // check it is IP6
//            // is a valid IP6 Address
//            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//            char addressBuffer[INET6_ADDRSTRLEN];
//            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
//            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
//        }
//        ifAddrStruct=ifAddrStruct->ifa_next;
//    }
//}

void Server::InitUnCompleTab(int maxSeq)
{
    char  localaddr[IP_LEN];
    char  mucastaddr[IP_LEN];
    memcpy(mucastaddr,HELLO_GROUP,IP_LEN);
    for(int i = 0 ; i < maxSeq ; ++i)
    {
        std::cout<<"vRemoteInfos size:"<<m_vRemoteInfos.size()<<std::endl;
        for(int t = 0 ; t < m_vRemoteInfos.size() ; ++t)
        {
            memcpy(localaddr,m_vRemoteInfos.at(t).c_str(),IP_LEN);
            m_pUnCompleTab->AddTab(i,localaddr,mucastaddr);
        }
    }
}
std::vector<std::string> Server::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}
bool Server::GetLocalIp(std::string ifaname,std::string &localIp)
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

//    QString localHostName = QHostInfo::localHostName();
//    qDebug() << "LocalHostName:" << localHostName;

//    //获取IP地址
//    QHostInfo info = QHostInfo::fromName(localHostName);
//    qDebug() << "IP Address:" << info.addresses();
//    foreach(QHostAddress address, info.addresses())
//    {
//        if (address.protocol() == QAbstractSocket::IPv4Protocol)
//            qDebug() << "IPv4 Address:" << address.toString();
//    }

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
//                        break;
//            }
//            //printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
//        }
//        ifAddrStruct=ifAddrStruct->ifa_next;
//    }
//    printf("ifAddrStruct==NULL\n");
}
