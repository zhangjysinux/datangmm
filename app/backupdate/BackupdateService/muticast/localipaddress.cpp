#include "localipaddress.h"
#include <QHostInfo>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QNetworkInterface>
#include "SegData.h"
LocalIaAddress *LocalIaAddress::getInstance()
{

}
LocalIaAddress::~LocalIaAddress()
{

}
LocalIaAddress::LocalIaAddress()
{

}

bool LocalIaAddress::GetLocalIp(int netType,std::string ifaname,std::string &localIp)
{
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
//        qDebug() << "Address:" << address << address.protocol();
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                !address.toString().contains("100.100") &&
                !address.isLoopback())
        {
            localIp = address.toString().toStdString();
        }
    }
    return true;
}

bool LocalIaAddress::IsIpSame(std::string sourceIp,std::string &destIp)
{

    char  sourceaddr[IP_LEN];
    char  destaddr[IP_LEN];
    memcpy(sourceaddr,sourceIp.c_str(),IP_LEN);
    memcpy(destaddr,destIp.c_str(),IP_LEN);
    std::vector<int> vSourceIp;
    std::vector<int> vDestIp;
    vSourceIp = AnalysIp(sourceaddr);
    vDestIp = AnalysIp(destaddr);
    return IsSameIp(vSourceIp,vDestIp);
}
bool LocalIaAddress::IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp)
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
        //        std::cout<<"size not same "<<std::endl;
        //        std::cout<<"vSourceIpsize "<<vSourceIp.size()<<std::endl;
        //        //std::cout<<vSourceIp<<std::endl;
        //        std::cout<<"vDestiIpsize "<<vDestiIp.size()<<std::endl;
        return false;
    }else
    {
        for(int i = 0 ; i < vSourceIp.size();++i)
        {
            //std::cout<<vSourceIp.at(i)<<" "<<vDestiIp.at(i)<<std::endl;
            if(vSourceIp.at(i)!= vDestiIp.at(i))
            {
                return false;
            }
        }
    }
    return true;
}
std::vector<int> LocalIaAddress::AnalysIp( char* ipBuffer)
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
