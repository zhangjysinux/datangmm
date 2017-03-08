#include "SfileData.h"
#include <stdio.h>
#include <iostream>
#include<stdlib.h>
#include <unistd.h>

SFileData::SFileData()
{
    fileLength = 0;
    fileBuffer = NULL;
    m_iMsgType = -1;
    m_iFileId = -1;
}

SFileData::~SFileData()
{
    delete fileBuffer;
}
void SFileData::SetFileName(std::string fileName)
{
    m_fileName = fileName;
}
bool SFileData::ReadFileData(std::string path)
{
    // do not forget  file length
    std::cout<<"ptah:"<<path<<std::endl;
    //printf("ptah:%s",path);
    unsigned long filesize = -1;
    FILE *fp;
    fp = fopen(path.c_str(), "rb");
    if(fp == NULL)
    {
        printf("file fopen failure!");
        return false;
    }

    printf("get lenth\n");
    fseek(fp, 0L, SEEK_END);
    printf("fseek to end\n");
    fileLength = ftell(fp);

    printf("read file size:%d\n",fileLength);


    fileBuffer =  new unsigned char[fileLength+1];
    printf("new buffer ok!\n");
    fseek(fp,0L,SEEK_SET);
    fread(fileBuffer,fileLength,1 ,fp);
    fileBuffer[fileLength]=0;
    fclose(fp);
    return true;
}

int SFileData::SplitFile(int msgtype,int fileid)
{
    m_iMsgType = msgtype;
    m_iFileId = fileid;
    m_bSendFinshedFirst = true;
    printf("start split file\n");
    int maxseg = MAX_SEG_LEN;
    int maxsegseq  =    fileLength/maxseg;
    int res = fileLength%maxseg;
     //std::cout<<"splitfile!!!!!!!!!!!!!!!!!!!!"<<fileBuffer<<std::endl
    int segNum=0;
    if(res==0)
    {
        segNum=maxsegseq;
    }else
    {
        segNum=maxsegseq+1;
    }
    for(int i = 0; i <  maxsegseq;++i)
    {
        SegData segData;
        segData.segSeq = i;
        segData.msgtype = msgtype;
        segData.fileID = fileid;
        segData.length = maxseg;
        segData.segnum = segNum;
        segData.filelength = fileLength;
        memcpy(segData.fileName,m_fileName.c_str(),m_fileName.size()+1);
        memcpy(segData.sourceIP,m_localAddrIp,IP_LEN);
        printf("source ip = :%s\n",segData.sourceIP);
        memcpy(segData.buffer,fileBuffer+i*maxseg,maxseg);
        //segData.checkSum = CheckSum(segData.buffer,MAX_SEG_LEN);
        m_vSegDatas.push_back(segData);
    }
    if(res!=0)
    {
        SegData segData;
        segData.segSeq = maxsegseq;
        segData.msgtype = msgtype;
        segData.fileID = fileid;
        segData.length = res;
        segData.segnum = segNum;
        segData.filelength = fileLength;
        memcpy(segData.fileName,m_fileName.c_str(),m_fileName.size()+1);
        memcpy(segData.sourceIP,m_localAddrIp,IP_LEN);
        memcpy(segData.buffer,fileBuffer+maxsegseq*maxseg,res);
        m_vSegDatas.push_back(segData);
    }

    printf("splited file num:%d\n",m_vSegDatas.size());
    return m_vSegDatas.size();
}

void SFileData::GetFileSeg(int segSeq,unsigned char* segBuffer)
{

}

void SFileData::SetLocalAddress( const char* localAddr)
{
    memcpy(m_localAddrIp,localAddr,IP_LEN);
}
void SFileData::SaveFile()
{
    FILE *fp;
    fp = fopen( "rec.txt", "wb");
    if(fp == NULL)
    {
        printf("file fopen failure!\n");
        return ;
    }
    fwrite(fileBuffer,fileLength,1,fp);
    fclose(fp);
    printf("creatre file ok");
}

int SFileData::CheckSum(unsigned char* segBuffer,int size)
{
    unsigned char value = *segBuffer;
    for(int i = 0 ; i < size ; ++i)
    {
        value+= (*segBuffer);
    }
    return (int)value;
}
void SFileData::ClearData()
{
    fileLength = 0;
    if(fileBuffer)
    {
      delete[] fileBuffer;
    }

    m_vSegDatas.clear();
    m_fileName.clear();
}
bool SFileData::isSendFinshedFirst()
{
    return m_bSendFinshedFirst;
}
void SFileData::setSendFinshedFirst(bool bFirst)
{
    m_bSendFinshedFirst = bFirst;
}
