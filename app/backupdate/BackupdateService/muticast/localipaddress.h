#ifndef _LOCAL_IP_ADDRESS_H_
#define _LOCAL_IP_ADDRESS_H_
#include <string>
#include <vector>
class LocalIaAddress
{
public:
    static LocalIaAddress *getInstance();
    ~LocalIaAddress();
    bool GetLocalIp(int netType,std::string ifaname,std::string &localIp);
    bool IsIpSame(std::string sourceIp,std::string &destIp);
private:
    explicit LocalIaAddress();
    bool IsSameIp(std::vector<int> vSourceIp,std::vector<int> vDestiIp);
    std::vector<int> AnalysIp(char* ipBuffer);
};
#endif
