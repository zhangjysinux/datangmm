#ifndef _MUTI_CAST_SERVER_H_
#define _MUTI_CAST_SERVER_H_
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <deque>
#include "SfileData.h"
#include "UnCompleTab.h"
#define HELLO_PORT 10000
#define HELLO_GROUP "225.0.0.37"
#define DISCONNECT_CHECK_NUM 10
class UnCompleTab;
class MutiCast;
class TempUnCompleTab;
class MutiCastServer
{
    friend class MutiCastServer;
public:
    MutiCastServer(std::string strMcAddr, int  iMutiCastPort,MutiCast* pMutiCast,int groupId);
    ~MutiCastServer();
    void Init(int  sockfd);
    void Init2();
    void Start();
    void SetmSFileData(SFileData* pSFileData);
    void Run();
    void SetUnCompleTab(UnCompleTab* pUnCompleTab);
    bool checkDisNum(int disNum);
private:
    int fd;
    int cnt;
    struct sockaddr_in addr;
    SFileData* m_pSFileData;
    UnCompleTab* m_pUnCompleTab;
    //TempUnCompleTab* m_pTempUnCompleTab;
    std::string m_strMcAddr;
    int m_iMutiCastPort;
    MutiCast* m_pMutiCast;
    std::deque<int> m_queDisNum;
    std::map<int,AddressList> m_mapTmpUnCompleTab;
    int m_iGroupId;

};
#endif
