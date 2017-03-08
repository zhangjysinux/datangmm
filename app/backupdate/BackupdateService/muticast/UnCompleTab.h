#ifndef _UNCOMPLE_TAB_H_
#define _UNCOMPLE_TAB_H_
#include <map>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<list>
#include "SegData.h"
class ConnectCount;
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
    void ProcessIpInfo(int segSeq,char*ipBuffer,char* mcIpBuffer);
    void GetUnCompleteSegSeq(std::vector<int> &vUnCompelSeqs);
    void ClearUnMap();
    bool IsIpDisCOnnect(int &disConnectNum);
    int getAllSize();
    void SetAllIpNum(int allIpNum);
    bool IsSendSuccesed();
    void addCount(int ip);
    void updateCount();
    bool isConnect(int ip);
private:
    void AddConnectCount(int ip);
    std::vector<int> AnalysIp(char* ipBuffer);
    void InsertSeqCount(int ip,int seq);
    void InsertSeq(std::vector<int> &vSeq,int seq);
    void RmSeq(std::vector<int> &vSeq,int seq);
    void RmSeqCount(int ip,int seq);
    //bool IsSameGroupIp(std::vector<AddressInfo> &source,std::vector<AddressInfo> &dest);
    int GetGroupIpSum(const std::vector<AddressInfo> &source);
    int GetSum(const std::vector<int> &vIp);
    bool IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp);
    void RemAddrFromList(std::vector<AddressInfo> &vAddress,AddressInfo addrinfo);
    std::map<int,AddressList> m_mapUnCompleTab;//segSeq->addresinfo
    std::map<int,std::vector<int> > m_mapIpCount;
    std::map<int ,ConnectCount*> m_mapConnectCount;
    //std::vector<AddressInfo> vAddressInfo;
    int m_allIpNum;
};

#endif
