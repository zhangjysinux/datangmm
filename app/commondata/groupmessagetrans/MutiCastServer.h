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
#include "SfileData.h"
#define HELLO_PORT 10000
#define HELLO_GROUP "225.0.0.37"
class UnCompleTab;
class MutiCastServer
{
    friend class MutiCastServer;
public:
    MutiCastServer(std::string strMcAddr, int  iMutiCastPort);
    ~MutiCastServer();
    void Init(int  sockfd);
    void Start();
    void SetmSFileData(SFileData* pSFileData);
    void Run();
    void SetUnCompleTab(UnCompleTab* pUnCompleTab);

private:
    int fd;
    int cnt;
    struct sockaddr_in addr;
    SFileData* m_pSFileData;
    UnCompleTab* m_pUnCompleTab;
    std::string m_strMcAddr;
    int m_iMutiCastPort;
};
#endif
