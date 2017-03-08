#include "filerecvmanager.h"
#include "CfileData.h"
fileRecvManager::fileRecvManager()
{

}

fileRecvManager::~fileRecvManager()
{

}

std::string fileRecvManager::getFileName(int fileId)
{
    std::string recfileName;
    std::map<int,CFileData*>::iterator findIter =  m_mapFiles.find(fileId);
    if(findIter!=m_mapFiles.end())
    {
        CFileData* pFileData = findIter->second;
        return pFileData->GetRecName();
    }
    return recfileName;
}
bool fileRecvManager::addFileSeg(int fileId,SegData & segData)
{
    if(fileRecFinshed(fileId))
        return false;
    if(m_mapFiles.empty())
    {
        CFileData *pFileData = new CFileData();
        pFileData->SetRecivePath(m_strRecvPath);
        pFileData->AddSegData(segData);
        m_mapFiles.insert(std::make_pair(fileId,pFileData));
        return true;
    }
    std::map<int,CFileData*>::iterator findIter =  m_mapFiles.find(fileId);
    if(findIter!=m_mapFiles.end())
    {
        return findIter->second->AddSegData(segData);
    }else
    {
        CFileData *pFileData = new CFileData();
        pFileData->SetRecivePath(m_strRecvPath);
        pFileData->AddSegData(segData);
        m_mapFiles.insert(std::make_pair(fileId,pFileData));
        return true;
    }
}

void fileRecvManager::setRecvPath(std::string recvPath)
{
    m_strRecvPath = recvPath;
}


void fileRecvManager::InsertFinshedQune(int fileId)
{
    m_finishedFileIds.push_back(fileId);
    if(m_finishedFileIds.size()>FINSHED_NUM)
    {
        m_finishedFileIds.pop_front();
    }
}

bool fileRecvManager::createFileDel(int fileId)
{
    if(createFile(fileId))
    {
        deleteRecvFile(fileId);
        return true;
    }
    return false;
}

int fileRecvManager::getRecSize(int fileId)
{
    std::map<int,CFileData*>::iterator findIter =  m_mapFiles.find(fileId);
    if(findIter!=m_mapFiles.end())
    {
         return findIter->second->GetRecSize();
    }
    return -1;
}

int fileRecvManager::checkFinshed()
{
    if(m_mapFiles.empty())
        return -1;
    std::map<int,CFileData*>::iterator mapIter =  m_mapFiles.begin();
    for(;mapIter!=m_mapFiles.end();++mapIter)
    {

        if(mapIter->second->IsFileRecFinished())
        {
            return mapIter->first;
        }
    }
    return -1;
}

bool fileRecvManager::createFile(int fileId)
{
    if(m_mapFiles.empty())
        return false;
    std::map<int,CFileData*>::iterator findIter =  m_mapFiles.find(fileId);
    if(findIter!=m_mapFiles.end())
    {
        CFileData* pFileData = findIter->second;
        pFileData->CreateFile();
        pFileData->ResetData();
        InsertFinshedQune(fileId);
        delete pFileData;
        return true;
    }
    return false;
}

bool fileRecvManager::fileRecFinshed(int fileId)
{
    if(m_finishedFileIds.empty())
        return false;
    std::deque<int>::iterator itdeq =  m_finishedFileIds.begin();
    for(;itdeq != m_finishedFileIds.end();++itdeq)
    {
        if(*itdeq == fileId)
            return true;
    }
    return false;
}

void fileRecvManager::deleteRecvFile(int fileId)
{
    if(m_mapFiles.empty())
    {
        return ;
    }
    std::map<int,CFileData*> newMapFiles;
    std::map<int,CFileData*>::iterator itmap =  m_mapFiles.begin();
    for(;itmap != m_mapFiles.end();++itmap)
    {
        if(itmap->first != fileId)
        {
            newMapFiles.insert(std::make_pair(itmap->first,itmap->second));
        }
    }
    m_mapFiles = newMapFiles;
}
