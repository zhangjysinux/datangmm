#include "UnCompleTab.h"
#include "SegData.h"
#include "connectcount.h"
#include <iostream>
UnCompleTab::UnCompleTab()
{

}

UnCompleTab::~UnCompleTab()
{

}

void UnCompleTab::AddTab(int segSeq,char*  ipBuffer,char* mcIpBuffer)
{
    printf ("add segSeq = %d ip=%s mc=%s\n",segSeq,ipBuffer,mcIpBuffer);
    AddressInfo addrinfo;
    addrinfo.ipV = AnalysIp(ipBuffer);

    if(addrinfo.ipV.size()!=4)
        printf ("add tab analyip error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    InsertSeqCount(addrinfo.ipV.at(3),segSeq);
    AddConnectCount(addrinfo.ipV.at(3));

    addrinfo.mcipV = AnalysIp(mcIpBuffer);
    printf ("add segSeq = %d ip=%s mc=%s empty insert!!!!!!\n",segSeq,ipBuffer,mcIpBuffer);
    if(m_mapUnCompleTab.empty())
    {
        AddressList addrlist;
        addrlist.vAddressInfo.push_back(addrinfo);
        m_mapUnCompleTab.insert(std::make_pair(segSeq,addrlist));
        printf ("add segSeq = %d ip=%s mc=%s  insert  return ojk!!!!!!\n",segSeq,ipBuffer,mcIpBuffer);
        return;
    }
    printf ("add segSeq = %d ip=%s mc=%s   insert!!!!!!\n",segSeq,ipBuffer,mcIpBuffer);
    std::map<int,AddressList>::iterator finder = m_mapUnCompleTab.find(segSeq);
    if(finder != m_mapUnCompleTab.end())
    {
        finder->second.vAddressInfo.push_back(addrinfo);
    }else
    {
        AddressList addrlist;
        addrlist.vAddressInfo.push_back(addrinfo);
        m_mapUnCompleTab.insert(std::make_pair(segSeq,addrlist));
    }
    printf ("add segSeq = %d ip=%s mc=%s ok!!!\n",segSeq,ipBuffer,mcIpBuffer);
    //std::cout<<m_mapUnCompleTab[5].vAddressInfo/*.at(0).ipV*/.size()<<std::endl;
    //std::cout<<m_mapUnCompleTab[5].vAddressInfo/*.at(0).ipV*/.size()<<std::endl;
}

void UnCompleTab::ProcessIpInfo(int segSeq, char*ipBuffer, char* mcIpBuffer)
{
    printf("remove ip from map seq =%d  ip=%s\n",segSeq,ipBuffer);
    AddressInfo addrinfo;
    addrinfo.ipV = AnalysIp(ipBuffer);
    if(addrinfo.ipV.size()!=4)
    {
        std::cout<<"RemoveIpInfo AnalysIp error!!!!!!!!!!!!!!!!!!!"<<std::endl;
        return;
    }

    RmSeqCount(addrinfo.ipV.at(3),segSeq);
    addCount(addrinfo.ipV.at(3));
    //    for(int i = 0 ; i < addrinfo.ipV.size();++i)
    //    {
    //        std::cout<<addrinfo.ipV.at(i)<<" ";
    //    }
    //                   std::cout<<std::endl;
    addrinfo.mcipV = AnalysIp(mcIpBuffer);
    //std::cout<<m_mapUnCompleTab[5].vAddressInfo/*.at(0).ipV*/.size()<<std::endl;
    std::map<int,AddressList>::iterator finder = m_mapUnCompleTab.find(segSeq);
    if(finder != m_mapUnCompleTab.end())
    {
        //printf("second.vAddressInfo seq list size:%d\n",finder->second.vAddressInfo.size());
        RemAddrFromList(finder->second.vAddressInfo,addrinfo);
    }else
    {
        return;
    }
    //    std::vector<int> vUnSeq;
    //    GetUnCompleteSegSeq(vUnSeq);
    //    printf("Un Complete Seq Num:%d\n",vUnSeq.size());
}

std::vector<int> UnCompleTab::AnalysIp(char* ipBuffer)
{
    int count = 0;
    for(int i = 0 ; i < IP_LEN;++i)
    {
        if(ipBuffer[i]=='.')
        {
            count++;
        }
        if(ipBuffer[i]=='\0' && count<3)
        {
            ipBuffer[i]= '.';
            count++;
        }
    }

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



void UnCompleTab::GetUnCompleteSegSeq(std::vector<int> &vUnCompelSeqs)
{
    std::map<int,AddressList>::iterator adriter = m_mapUnCompleTab.begin();
    for(;adriter != m_mapUnCompleTab.end();++adriter)
    {
        if(!adriter->second.vAddressInfo.empty())
        {
            std::vector<AddressInfo>::iterator iter = adriter->second.vAddressInfo.begin();
            for(;iter!=adriter->second.vAddressInfo.end();++iter)
            {
                if(isConnect(iter->ipV[3]))
                {
                    vUnCompelSeqs.push_back(adriter->first);
                }
                if(iter->ipV.size()!=4)
                {
                    vUnCompelSeqs.push_back(adriter->first);
                }
            }

        }
    }
}

bool UnCompleTab::IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp)
{
    if(vSourceIp.empty())
        return false;

    if(vDestiIp.empty())
        return false;

    if(vSourceIp.size() != vDestiIp.size())
    {
        //        std::cout<<"size not same "<<std::endl;
        //        std::cout<<"vSourceIpsize "<<vSourceIp.size()<<std::endl;
        //        std::cout<<vSourceIp.at(0)<<std::endl;
        //        std::cout<<"vDestiIpsize "<<vDestiIp.size()<<std::endl;
        return false;
    }else
    {
//        for(int i = 0 ; i < vSourceIp.size();++i)
//        {
            //std::cout<<vSourceIp.at(i)<<" "<<vDestiIp.at(i)<<std::endl;
            if(vSourceIp.at(3)!= vDestiIp.at(3))
            {
                return false;
            }
//        }
    }
    return true;
}

void UnCompleTab::RemAddrFromList(std::vector<AddressInfo> &vAddress,AddressInfo addrinfo)
{
    if(vAddress.empty())
        return;
    std::vector<AddressInfo> vNewAddress;
    std::vector<AddressInfo>::iterator itadr = vAddress.begin();
    for(;itadr != vAddress.end();++itadr)
    {
        if(!IsSameIp(itadr->ipV,addrinfo.ipV))
        {
            vNewAddress.push_back(*itadr);
        }else
        {
            printf("rem same address\n");
        }

    }
    vAddress = vNewAddress;
}

void UnCompleTab::ClearUnMap()
{
    m_mapUnCompleTab.clear();
    m_mapIpCount.clear();
    m_mapConnectCount.clear();
}

bool UnCompleTab::IsIpDisCOnnect(int &disConnectNum)
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

//bool UnCompleTab::IsSameGroupIp(std::vector<AddressInfo> &source,std::vector<AddressInfo> &dest)
//{
//    if(source.size()!= dest.size())
//        return false;

//    if(source.empty())
//        return false;

//   int srcId = GetGroupIpSum(source);
//   int destId = GetGroupIpSum(dest);

//   if(srcId == destId)
//       return true;
//   return false;
//}

int UnCompleTab::GetGroupIpSum(const std::vector<AddressInfo> &source)
{
    int groupSum = 0;
    for(int i = 0 ; i < source.size();++i)
    {
        groupSum+=GetSum(source[i].ipV);
    }
    return groupSum;
}

int UnCompleTab::GetSum(const std::vector<int> &vIp)
{
    int sum = 0;
    for(int i = 0 ; i < vIp.size();++i)
    {
        sum+=vIp[i];
    }
    return sum;
}
int UnCompleTab::getAllSize()
{
    return m_mapUnCompleTab.size();
}
void UnCompleTab::SetAllIpNum(int allIpNum)
{
    m_allIpNum = allIpNum;
}

bool UnCompleTab::IsSendSuccesed()
{
    if(m_mapIpCount.empty())
    {
        return true;
    }

    std::map<int,std::vector<int> >::iterator itmap =  m_mapIpCount.begin();
    for(;itmap != m_mapIpCount.end();++itmap)
    {
        if(itmap->second.empty())
            return true;
    }
    return false;
}

void UnCompleTab::InsertSeqCount(int ip,int seq)
{
    std::map<int,std::vector<int> >::iterator finder =  m_mapIpCount.find(ip);
    if(finder!= m_mapIpCount.end())
    {

        //InsertSeq(finder->secnod,seq);
    }else
    {
        std::vector<int> vSeqs;
        vSeqs.push_back(seq);
        m_mapIpCount.insert(std::make_pair(ip,vSeqs));
    }
}

void UnCompleTab::InsertSeq(std::vector<int> &vSeq,int seq)
{
    if(vSeq.empty())
    {
        vSeq.push_back(seq);
        return;
    }

    for(int i = 0; i < vSeq.size();++i)
    {
        if(vSeq.at(i)==seq)
            return;
    }
    vSeq.push_back(seq);
}

void UnCompleTab::RmSeq(std::vector<int> &vSeq,int seq)
{
    if(vSeq.empty())
    {
        vSeq.push_back(seq);
        return;
    }
    std::vector<int> newVSeq;
    for(int i = 0; i < vSeq.size();++i)
    {
        if(vSeq.at(i)!=seq)
            newVSeq.push_back(vSeq.at(i));
    }
    vSeq = newVSeq;
}

void UnCompleTab::RmSeqCount(int ip,int seq)
{
    std::map<int,std::vector<int> >::iterator finder =  m_mapIpCount.find(ip);
    if(finder!= m_mapIpCount.end())
    {
        //RmSeq(finder->secnod,seq);
    }
}

void UnCompleTab::AddConnectCount(int ip)
{
    if(m_mapConnectCount.empty())
    {
        ConnectCount* pConnectCount = new ConnectCount();
        m_mapConnectCount.insert(std::make_pair(ip,pConnectCount));
    }

    std::map<int ,ConnectCount*>::iterator findIter =  m_mapConnectCount.find(ip);
    if(findIter == m_mapConnectCount.end())
    {
        ConnectCount* pConnectCount = new ConnectCount();
        m_mapConnectCount.insert(std::make_pair(ip,pConnectCount));
    }
}

void UnCompleTab::addCount(int ip)
{
    std::map<int ,ConnectCount*>::iterator findIter =  m_mapConnectCount.find(ip);
    if(findIter != m_mapConnectCount.end())
    {
        ConnectCount* pConnectCount = findIter->second;
        pConnectCount->addCount();
    }
}
void UnCompleTab::updateCount()
{
    std::map<int,ConnectCount*>::iterator mapIter =  m_mapConnectCount.begin();
    for(;mapIter != m_mapConnectCount.end(); ++mapIter)
    {
        ConnectCount* pConnectCount = mapIter->second;
        pConnectCount->update();
    }
}
bool UnCompleTab::isConnect(int ip)
{
    std::map<int ,ConnectCount*>::iterator findIter =  m_mapConnectCount.find(ip);
    if(findIter != m_mapConnectCount.end())
    {
        ConnectCount* pConnectCount = findIter->second;
        return pConnectCount->getIsConnect();
    }
    return  true;
}
