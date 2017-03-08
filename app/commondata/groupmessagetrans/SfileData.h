#ifndef _SFILE_DATA_H_
#define _SFILE_DATA_H_
#include <string>
#include <vector>
#include "SegData.h"
class SFileData
{
    friend class MutiCastServer;
public:
    SFileData();
    ~SFileData();
    bool ReadFileData(std::string path);
    void SetFileName(std::string fileName);
    int SplitFile();
    void GetFileSeg(int segSeq,unsigned char* segBuffer);
    void SetLocalAddress(const char* localAddr);
    void SaveFile();
    int CheckSum(unsigned char* segBuffer,int size);
    void ClearData();
private:
    unsigned char* fileBuffer;
    unsigned long fileLength;
    std::vector<SegData> m_vSegDatas;
     char m_localAddrIp[IP_LEN];
    std::string m_fileName;
};
#endif
