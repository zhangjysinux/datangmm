#include "MutiCastServer.h"
#include "MutiCast.h"
#include "TemUnCompleTab.h"
#include <map>
#include<sys/wait.h>
#include "externlockDefine.h"
#include "lockmarco.h"
extern MutiCastServer* mutiCastServerInstance;
//extern pthread_mutex_t m_lock;
//extern pthread_mutex_t mclock;
//extern pthread_cond_t cond;

void *MutiCastRun(void* ptr)
{
    mutiCastServerInstance->Run();
}

MutiCastServer::MutiCastServer(std::string strMcAddr, int  iMutiCastPort,MutiCast* pMutiCast,int groupId)
{
    m_strMcAddr =    strMcAddr;
    m_iMutiCastPort = iMutiCastPort;
    m_pMutiCast = pMutiCast;
    //m_pTempUnCompleTab = new TempUnCompleTab();
    m_iGroupId = groupId;
    //m_bSendFinshedFirst = true;
}

MutiCastServer::~MutiCastServer()
{

}

void MutiCastServer::SetUnCompleTab(UnCompleTab* pUnCompleTab)
{
    m_pUnCompleTab = pUnCompleTab;
}

void MutiCastServer::Init2()
{
    addr.sin_addr.s_addr=inet_addr(m_strMcAddr.c_str());
    addr.sin_port=htons(m_iMutiCastPort);
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
    int sendNum = 1;
    int onceNum =200;
    int count = 0;
    printf ("Muticastserver enter run ...............!\n");
    while (1)
    {
        //pthread_mutex_lock(&mclock);
        PTHREAD_MUTEX_MC_LOCK()
                //pthread_cond_wait(&cond1, &mclock1);
                PTHREAD_COND_WAIT()
                PTHREAD_MUTEX_MC_UNLOCK()
                //pthread_mutex_unlock(&mclock);
                //printf ("muticast start send data................!\n");

                //pthread_mutex_lock(&LOCK());
                PTHREAD_MUTEX_M_LOCK()
                if(m_pUnCompleTab->getAllSize()==0)
        {
            PTHREAD_MUTEX_M_UNLOCK()
                    continue;
        }
        PTHREAD_MUTEX_M_UNLOCK()

                count = 0;
        while(1)
        {
            bool bSendOk;
            int allNum = 0;
            //pthread_mutex_lock(&m_lock);
            PTHREAD_MUTEX_M_LOCK()
            bSendOk = m_pUnCompleTab->IsSendSuccesed();
            allNum = m_pUnCompleTab->m_mapUnCompleTab.size();
            onceNum = allNum/sendNum;
            PTHREAD_MUTEX_M_UNLOCK()




                    std::vector<int> vUnSeq;
            PTHREAD_MUTEX_M_LOCK()
                    m_pUnCompleTab->GetUnCompleteSegSeq(vUnSeq);
            PTHREAD_MUTEX_M_UNLOCK()

                    if(vUnSeq.empty())
            {
                //pthread_mutex_lock(&m_lock);
                PTHREAD_MUTEX_M_LOCK()
                        m_pUnCompleTab->ClearUnMap();
                //pthread_mutex_unlock(&m_lock);
                PTHREAD_MUTEX_M_UNLOCK()
                m_pMutiCast->sendComplete(m_pSFileData->m_iMsgType,m_pSFileData->m_iFileId,allNum,allNum,m_pSFileData->m_fileName,1);
                        break;
            }


            int finishNum = allNum-vUnSeq.size();
            m_pMutiCast->sendComplete(m_pSFileData->m_iMsgType,m_pSFileData->m_iFileId,finishNum,allNum,m_pSFileData->m_fileName,2);

            if(bSendOk && m_pSFileData->isSendFinshedFirst())
            {
                m_pMutiCast->sendFinished(true);
                m_pSFileData->setSendFinshedFirst(false);
                m_pMutiCast->sendComplete(m_pSFileData->m_iMsgType,m_pSFileData->m_iFileId,finishNum,allNum,m_pSFileData->m_fileName,1);
            }


            int allsize = 0;
            int sendsize = 0;
            if( vUnSeq.size() < onceNum)
            {
                sendsize = vUnSeq.size();
            }else
            {
                sendsize = onceNum;
            }


            for(int i = 0; i < sendsize;++i)
            {
                if(count > 100 && count%100 == 0)
                {
                    PTHREAD_MUTEX_M_LOCK()
                            m_pUnCompleTab->updateCount();
                    PTHREAD_MUTEX_M_UNLOCK()
                }
                unsigned char segdata[SEG_SIZE];
                int len = m_pSFileData->m_vSegDatas.at(vUnSeq.at(i)).length;
                m_pSFileData->m_vSegDatas.at(vUnSeq.at(i)).Serialization(segdata);
                if (sendto(fd,segdata, SEG_SIZE ,0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
                {
                    perror("sendto");
                }
                allsize+=len;
                //printf ("muticast start send data seq= %d len=%d sucessful! allsize=%d\n",itmap->first,len,allsize);
                usleep(30*1000);
                count++;
            }
            usleep(1000*1000);
        }
    }
}

bool MutiCastServer::checkDisNum(int disNum)
{
    m_queDisNum.push_back(disNum);
    if(m_queDisNum.size()>DISCONNECT_CHECK_NUM)
    {
        m_queDisNum.pop_front();
    }
    if(m_queDisNum.size()<DISCONNECT_CHECK_NUM)
    {
        return false;
    }else
    {
        std::deque<int>::iterator iter = m_queDisNum.begin();
        int beginnum = *iter;
        for(;iter != m_queDisNum.end();++iter)
        {
            if((*iter)!=beginnum)
                return false;
        }
        return true;
    }
}
