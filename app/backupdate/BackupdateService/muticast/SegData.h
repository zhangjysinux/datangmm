#ifndef _SEG_DATA_H_
#define _SEG_DATA_H_

#define MAX_SEG_LEN 1600
#define IP_LEN 15
#define FILE_NAME_SIZE 50
#define ACK_SIZE (8+IP_LEN+IP_LEN)
#define SEG_SIZE (24+IP_LEN+MAX_SEG_LEN+FILE_NAME_SIZE)

#include <cstring>
struct SegData
{
    SegData()
    {
        fileID = -1;
        segSeq = -1;
        length = 0;
        msgtype = 0;
        filelength = 0;
        segnum = 0;
    }

    SegData& operator = (const SegData &t)
    {
        fileID=t.fileID;//sile
        segSeq=t.segSeq;
        length=t.length;// the segment data size
        filelength=t.filelength;// the file size
        segnum=t.segnum; // segment num
        msgtype=t.msgtype;
        //sourceIP=t.sourceIP;
        memcpy(sourceIP,t.sourceIP,IP_LEN);
        //buffer=t.buffer;
    }
    int fileID;//sile
    int segSeq;
    int length;// the segment data size
    int filelength;// the file size
    int segnum; // segment num
    int msgtype;
    char fileName[FILE_NAME_SIZE];
    char sourceIP[IP_LEN];
    unsigned char buffer[MAX_SEG_LEN];
    int Serialization(unsigned char* pBuffer )
    {
        memcpy(pBuffer,&fileID,4);
        memcpy(pBuffer+4,&segSeq,4);
        memcpy(pBuffer+8,&length,4);
        memcpy(pBuffer+12,&filelength,4);
        memcpy(pBuffer+16,&segnum,4);
        memcpy(pBuffer+20,&msgtype,4);
        memcpy(pBuffer+24,fileName,FILE_NAME_SIZE);
        memcpy(pBuffer+24+FILE_NAME_SIZE,sourceIP,IP_LEN);
        memcpy(pBuffer+24+FILE_NAME_SIZE+IP_LEN,buffer,MAX_SEG_LEN);
        return (24+FILE_NAME_SIZE+IP_LEN+MAX_SEG_LEN);
    }
    void UnSerialization(unsigned char* pBuffer)
    {
        memcpy(&fileID,pBuffer,4);
        memcpy(&segSeq,pBuffer+4,4);
        memcpy(&length,pBuffer+8,4);
        memcpy(&filelength,pBuffer+12,4);
        memcpy(&segnum,pBuffer+16,4);
        memcpy(&msgtype,pBuffer+20,4);
        memcpy(fileName,pBuffer+24,FILE_NAME_SIZE);
        memcpy(sourceIP,pBuffer+24+FILE_NAME_SIZE,IP_LEN);
        memcpy(buffer,pBuffer+24+FILE_NAME_SIZE+IP_LEN,MAX_SEG_LEN);
    }
};

struct AckData
{
    AckData()
    {
        fileID = -1;
        segSeq = -1;
    }
    AckData& operator = (AckData t)
    {
        fileID=t.fileID;//sile
        segSeq=t.segSeq;
        //destIP=t.destIP;// the segment data size
        //mcAdrr=t.mcAdrr;// the file size
    }
    int fileID;
    int segSeq;
    char destIP[IP_LEN];
    char mcAdrr[IP_LEN]; //muticast adress
    int Serialization(unsigned char* pBuffer )
    {
        memcpy(pBuffer,&fileID,4);
        memcpy(pBuffer+4,&segSeq,4);
        memcpy(pBuffer+8,destIP,IP_LEN);
        memcpy(pBuffer+8+IP_LEN,mcAdrr,IP_LEN);
    }
    void UnSerialization(unsigned char* pBuffer)
    {
        memcpy(&fileID,pBuffer,4);
        memcpy(&segSeq,pBuffer+4,4);
        memcpy(destIP,pBuffer+8,IP_LEN);
        memcpy(mcAdrr,pBuffer+8+IP_LEN,IP_LEN);
    }
};

#endif
