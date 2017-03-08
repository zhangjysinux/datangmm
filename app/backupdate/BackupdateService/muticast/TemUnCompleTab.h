#ifndef _TEMP_UNCOMPLE_TAB_H_
#define _TEMP_UNCOMPLE_TAB_H_
#include <map>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include "SegData.h"
#include "UnCompleTab.h"

class TempUnCompleTab
{
    friend class MutiCastServer;
public:
    TempUnCompleTab();
    ~TempUnCompleTab();
     void GetUnCompleteSegSeq(std::vector<int> &vUnCompelSeqs);
     void ClearUnMap();
     bool IsIpDisCOnnect(int &disConnectNum);
     int getAllSize();
     bool IsSendSucessed();
private:
    std::vector<int> AnalysIp(char* ipBuffer);
    //bool IsSameGroupIp(std::vector<AddressInfo> &source,std::vector<AddressInfo> &dest);
    int GetGroupIpSum(const std::vector<AddressInfo> &source);
    int GetSum(const std::vector<int> &vIp);
    bool IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp);
    std::map<int,AddressList> m_mapUnCompleTab;//segSeq->addresinfo
    int m_allIpNum;
};

#endif
