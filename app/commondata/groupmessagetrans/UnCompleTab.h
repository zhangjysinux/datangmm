#ifndef _UNCOMPLE_TAB_H_
#define _UNCOMPLE_TAB_H_
#include <map>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include "SegData.h"
struct AddressInfo
{
    std::vector<int> ipV;
    std::vector <int> mcipV;
    AddressInfo& operator = (const AddressInfo &t)
    {
        ipV=t.ipV;//sile
        mcipV=t.mcipV;
        return *this;
    }
};

struct AddressList
{
    std::vector<AddressInfo> vAddressInfo;
    AddressList& operator = (const AddressList &t)
    {
        vAddressInfo=t.vAddressInfo;
        return *this;
    }
};

class UnCompleTab
{
    friend class MutiCastServer;
public:
    UnCompleTab();
    ~UnCompleTab();
    void AddTab(int segSeq,char*ipBuffer,char* mcIpBuffer);
    void RemoveIpInfo(int segSeq,char*ipBuffer,char* mcIpBuffer);
     void GetUnCompleteSegSeq(std::vector<int> &vUnCompelSeqs);
     void ClearUnMap();
     bool IsIpDisCOnnect(int &disConnectNum);
private:
    std::vector<int> AnalysIp(char* ipBuffer);
    //bool IsSameGroupIp(std::vector<AddressInfo> &source,std::vector<AddressInfo> &dest);
    int GetGroupIpSum(const std::vector<AddressInfo> &source);
    int GetSum(const std::vector<int> &vIp);
    bool IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp);
    void RemAddrFromList(std::vector<AddressInfo> &vAddress,AddressInfo addrinfo);
    std::map<int,AddressList> m_mapUnCompleTab;//segSeq->addresinfo
    //std::vector<AddressInfo> vAddressInfo;
};

#endif
