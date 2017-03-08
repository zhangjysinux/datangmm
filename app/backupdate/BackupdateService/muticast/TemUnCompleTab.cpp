#include "TemUnCompleTab.h"
#include "SegData.h"
#include <iostream>
TempUnCompleTab::TempUnCompleTab()
{

}

TempUnCompleTab::~TempUnCompleTab()
{

}





std::vector<int> TempUnCompleTab::AnalysIp(char* ipBuffer)
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
        return vecAddress;
}



void TempUnCompleTab::GetUnCompleteSegSeq(std::vector<int> &vUnCompelSeqs)
{
    std::map<int,AddressList>::iterator adriter = m_mapUnCompleTab.begin();
    for(;adriter != m_mapUnCompleTab.end();++adriter)
    {
        if(!adriter->second.vAddressInfo.empty())
        {
            vUnCompelSeqs.push_back(adriter->first);
        }
    }
}

bool TempUnCompleTab::IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp)
{
    if(vSourceIp.size() != vDestiIp.size())
    {
        //std::cout<<"size not same "<<std::endl;
        //std::cout<<"vSourceIpsize "<<vSourceIp.size()<<std::endl;
        //std::cout<<vSourceIp.at(0)<<std::endl;
        //std::cout<<"vDestiIpsize "<<vDestiIp.size()<<std::endl;
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



void TempUnCompleTab::ClearUnMap()
{
    m_mapUnCompleTab.clear();
}

bool TempUnCompleTab::IsIpDisCOnnect(int &disConnectNum)
{
    disConnectNum = -1;
    if(m_mapUnCompleTab.size() == 1)
    {
        disConnectNum = m_mapUnCompleTab.begin()->second.vAddressInfo.size();
        return false;
    }

    if(m_mapUnCompleTab.empty())
    {
        return false;
    }
    int beinNum = m_mapUnCompleTab.begin()->second.vAddressInfo.size();
    std::map<int,AddressList>::iterator itmap = m_mapUnCompleTab.begin();
    for(;itmap != m_mapUnCompleTab.end();++itmap)
    {
        if(itmap->second.vAddressInfo.size()!=beinNum)
            return false;
    }


    itmap = m_mapUnCompleTab.begin();
    //std::vector<AddressInfo> vv = itmap->second.vAddressInfo;
    int beginGroupSum =  GetGroupIpSum(itmap->second.vAddressInfo);
    int groupSum = 0;
    for(;itmap != m_mapUnCompleTab.end();++itmap)
    {
        //std::vector<AddressInfo> v = itmap->second.vAddressInfo;
        groupSum =  GetGroupIpSum(itmap->second.vAddressInfo);
        if(groupSum!=beginGroupSum)
            return false;
    }
    disConnectNum = m_mapUnCompleTab.begin()->second.vAddressInfo.size();

    return true;
}


int TempUnCompleTab::GetGroupIpSum(const std::vector<AddressInfo> &source)
{
    int groupSum = 0;
    for(int i = 0 ; i < source.size();++i)
    {
        groupSum+=GetSum(source[i].ipV);
    }
    return groupSum;
}

int TempUnCompleTab::GetSum(const std::vector<int> &vIp)
{
    int sum = 0;
    for(int i = 0 ; i < vIp.size();++i)
    {
        sum+=vIp[i];
    }
    return sum;
}
int TempUnCompleTab::getAllSize()
{
    return m_mapUnCompleTab.size();
}
bool TempUnCompleTab::IsSendSucessed()
{

}
