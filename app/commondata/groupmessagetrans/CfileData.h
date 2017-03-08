#ifndef _CFILE_DATA_H_
#define _CFILE_DATA_H_
#include "SegData.h"
#include <vector>
#include <map>
#include <string>
class CFileData
{
public:
    CFileData();
    ~CFileData();
    bool AddSegData(SegData & segData);// true:  the segment data is new    false:the segmem has been received!
    bool IsFileRecFinished();
    void CreateFile();
    void ResetData();
    void SetRecivePath(std::string path);
    int GetRecSize();
    std::string GetRecName();
private:
    std::map<int,SegData> m_mapSegData;
    int m_segNum;
    int m_fileLength;
    unsigned char* m_pFileBuffer;
    std::string m_strFileName;
    std::string m_strRecivePath;
};
#endif
