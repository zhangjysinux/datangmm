#include "CfileData.h"
#include <stdio.h>
#include <stdlib.h>
CFileData::CFileData()
{
m_pFileBuffer = NULL;
}

CFileData::~CFileData()
{
    m_segNum = -1;
    m_fileLength = -1;
}

bool CFileData::AddSegData(SegData & segData)
{
    m_strFileName=segData.fileName;
    //printf ("add seg data to map!\n");
    std::map<int,SegData>::iterator findIter =   m_mapSegData.find(segData.segSeq);
    if(findIter== m_mapSegData.end())
    {
        m_mapSegData.insert(std::make_pair(segData.segSeq,segData));
        m_segNum = segData.segnum;
        m_fileLength = segData.filelength;
        m_strFileName=segData.fileName;
        //printf("file name %s\n",segData.fileName);
        //system("clear");
        printf(" file recive:%d-%d\n",m_mapSegData.size(),m_segNum);
        return true;
    }else
    {
        return false;
    }
}

bool CFileData::IsFileRecFinished()
{
    if(m_segNum != -1&& m_mapSegData.size() == m_segNum)
        return true;
    else
        return false;
}

void CFileData::CreateFile()
{
    printf("start creatre file...");
    printf("start fileLength size:%d\n",m_fileLength);
    if(m_fileLength!=-1)
    {
        m_pFileBuffer = new unsigned char[m_fileLength];
    }
    int maxseq = m_mapSegData.size();
    for(int i = 0 ; i < maxseq ; ++i)
    {
        int len = m_mapSegData[i].length;
        memcpy(m_pFileBuffer+i*MAX_SEG,m_mapSegData[i].buffer,len);
    }

    std::string str = m_strRecivePath;
    str = str+ m_strFileName;
    FILE *fp;
    fp = fopen( str.c_str(), "wb");
    if(fp == NULL)
    {
        printf("file fopen failure!\n");
        return ;
    }
    fwrite(m_pFileBuffer,m_fileLength,1,fp);
    fclose(fp);
    printf("creatre file ok");

}
void CFileData::ResetData()
{
    m_mapSegData.clear();
    m_segNum = -1;
    m_fileLength = -1;
    if(m_pFileBuffer)
    delete m_pFileBuffer;
    m_strFileName.clear();
}

void CFileData::SetRecivePath(std::string path)
{
    m_strRecivePath = path;
}

int CFileData::GetRecSize()
{
    return m_mapSegData.size();
}
std::string CFileData::GetRecName()
{
    return m_strFileName;

}
