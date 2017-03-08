#include "MutiCastServer.h"

#include <map>
#include "UnCompleTab.h"
#include<sys/wait.h>
extern pthread_mutex_t m_lock;
extern MutiCastServer* mutiCastServerInstance;

extern pthread_mutex_t mclock;
extern pthread_cond_t cond;

void *MutiCastRun(void* ptr)
{
    mutiCastServerInstance->Run();
}

MutiCastServer::MutiCastServer(std::string strMcAddr, int  iMutiCastPort)
{
    m_strMcAddr =    strMcAddr;
    m_iMutiCastPort = iMutiCastPort;
}

MutiCastServer::~MutiCastServer()
{

}

void MutiCastServer::SetUnCompleTab(UnCompleTab* pUnCompleTab)
{
    m_pUnCompleTab = pUnCompleTab;
}

void MutiCastServer::Init(int  sockfd)
{
    printf ("muticast init !\n");
    //    struct ip_mreq mreq;
    /* create what looks like an ordinary UDP socket */
    fd = sockfd;
    //    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
    //    {
    //        perror("socket");
    //        exit(1);
    //    }
    /* set up destination address */
    if(m_strMcAddr.empty())
        return;
    if(m_iMutiCastPort <= 0)
        return;

    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(m_strMcAddr.c_str());
    addr.sin_port=htons(m_iMutiCastPort);
}

void MutiCastServer::Start()
{
    pthread_t id;
    int i,ret;
    printf ("muticast Create pthread !\n");
    ret=pthread_create(&id,NULL,MutiCastRun,NULL);
    if(ret!=0)
    {
        printf ("Create pthread error!\n");
        exit (1);
    }



    //pthread_join(id,NULL);
}

void MutiCastServer::SetmSFileData(SFileData* pSFileData)
{
    m_pSFileData = pSFileData;
}

void MutiCastServer::Run()
{
    int sendNum = 0;
    printf ("Muticastserver enter run ...............!\n");
    while (1)
    {
        //pthread_mutex_lock(&mclock);
        pthread_cond_wait(&cond, &mclock);
        printf ("muticast start send data................!\n");
        if(m_pUnCompleTab->m_mapUnCompleTab.empty())
        {
            //pthread_mutex_unlock(&mclock);
            continue;
        }
        sendNum = 0;
        while(1)
        {

            pthread_mutex_lock(&m_lock);
            std::vector<int> vUnSeq;
            m_pUnCompleTab->GetUnCompleteSegSeq(vUnSeq);
            if(vUnSeq.empty())
            {
                m_pUnCompleTab->ClearUnMap();
                pthread_mutex_unlock(&m_lock);
                break;
            }

            // check other ip disconnect
//            int disNum;
//            if(sendNum >20 && m_pUnCompleTab->IsIpDisCOnnect(disNum))
//            {
//                m_pUnCompleTab->ClearUnMap();
//                pthread_mutex_unlock(&m_lock);
//                break;
//            }



            int allsize = 0;
            std::map<int,AddressList>::iterator itmap = m_pUnCompleTab->m_mapUnCompleTab.begin();
            for(;itmap != m_pUnCompleTab->m_mapUnCompleTab.end();++itmap)
            {

                if(itmap->second.vAddressInfo.empty())
                {
                    continue;
                }

                unsigned char segdata[SEG_SIZE];
                int len = m_pSFileData->m_vSegDatas.at(itmap->first).length;

                m_pSFileData->m_vSegDatas.at(itmap->first).Serialization(segdata);

                if (sendto(fd,segdata, SEG_SIZE ,0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
                {
                    perror("sendto");
                    //exit(1);
                }
                allsize+=len;
                printf ("muticast start send data seq= %d len=%d sucessful! allsize=%d\n",itmap->first,len,allsize);
            }

            pthread_mutex_unlock(&m_lock);

            usleep(3115000);
            sendNum++;
        }
    }
}
