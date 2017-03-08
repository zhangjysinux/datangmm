#ifndef _FILE_RECV_MANAGER_H
#define _FILE_RECV_MANAGER_H
#include <map>
#include <deque>
#include "SegData.h"
#include <string>
#define FINSHED_NUM 5
class CFileData;
class fileRecvManager
{
public:
    fileRecvManager();
    ~fileRecvManager();
    bool addFileSeg(int fileId,SegData & segData);
    std::string getFileName(int fileId);
    void setRecvPath(std::string recvPath);
    int checkFinshed();
    bool createFileDel(int fileId);
    int getRecSize(int fileId);
private:
    bool createFile(int fileId);
    void InsertFinshedQune(int fileId);
    void deleteRecvFile(int fileId);
    bool fileRecFinshed(int fileId);
    std::map<int,CFileData*> m_mapFiles;
    std::deque<int> m_finishedFileIds;
    std::string m_strRecvPath;
};
#endif
