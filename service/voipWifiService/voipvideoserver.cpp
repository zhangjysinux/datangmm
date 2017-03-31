#include "voipvideoserver.h"
#include "voipinterface.h"
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <QDebug>
#define MYPORT  8887
#define QUEUE   20


struct queuedata;
queuedata queue_data;
FrameType frametype;
pthread_mutex_t m_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
extern VoipInterface *service;
VoipVideoServer::VoipVideoServer()
{

}
void VoipVideoServer::run()
{

    while (true)
    {
        pthread_mutex_lock(&m_lock);
        pthread_cond_wait(&cond, &m_lock);
        if(frametype == remote)
        {
           service->emitVoipFrameData();
        }else if(frametype == local)
        {
           service->emitV4l2FrameData();
        }
        pthread_mutex_unlock(&m_lock);
    }
}
