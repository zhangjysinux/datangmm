#ifndef _MUTI_CAST_CLIENT_H_
#define _MUTI_CAST_CLIENT_H_
#include "SegData.h"
#include <string>
class MutiCastClient
{
public:
    MutiCastClient(std::string &mcIp,int mcPort);
    ~MutiCastClient();
    void Init(int socketfd);
    void Receive(SegData &segData);
private:
    struct sockaddr_in addr;
    int fd, nbytes,addrlen;
    std::string m_strMcIp;
    int m_iMcPort;
};
#endif
