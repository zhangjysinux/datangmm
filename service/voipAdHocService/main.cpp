#include "voipdbusdata.h"
#include "voipinterface.h"
#include <SDL2/SDL.h>
#include <QCoreApplication>
#include <QDebug>
#include "voipaccount.h"
#include "voipvideoserver.h"
#include <pjsua2.hpp>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QMutex>

#include <sys/shm.h>
#include<pjmedia-videodev/videodev.h>
#define BUFSIZE 2000000
using namespace pj;

Endpoint *ep;
AudioMediaPlayer *player;
AudioMediaPlayer *outgoingPlayer;
AudioMediaRecorder *recorder;
VoipAccount *acc;
VoipInterface *service;
unsigned char tmpbuf[BUFSIZE];

extern queuedata queue_data;
extern pthread_mutex_t m_lock;
extern pthread_cond_t cond;
extern FrameType frametype;
static void putV4l2Frame(unsigned char *buf,int width,int height,int pixfmt)
{
    printf("#########################put v4l2 frame\n");
    //printf("#######v4l2width:%d\n",width);
    //printf("#######v4l2height:%d\n",height);
//    printf("#######v4l2pixfmt:%d\n",pixfmt);
    void *shm = NULL;
    struct SharedUseData *shared = NULL;
    int shmid;
    //create shared memory
    shmid = shmget((key_t)8000, sizeof(struct SharedUseData), 0666|IPC_CREAT);
    if(shmid == -1)
    {
        printf("shmget failed\n");
    }
    shm = shmat(shmid, (void*)0,0);
    if(shm == (void*)-1)
    {
        printf("shmat failed\n");
    }
//    printf("Memory attached at %X\n", (int)shm);
    //shared memory seting
    shared = (struct SharedUseData*)shm;
    shared->width = width ;
    shared->height = height;

    //unsigned char tmpbuf[width*height*3];
    //unsigned char tmpmidbuf[width*height*3];
    //ycbycr->rgb24

//    for(int i = 0 ; i < width/2;++i)
//    {
//        for(int j = 0 ; j < height;++j)
//        {
//            int x = width/2-1-i;
//            int y = height-1-j;
//            tmpbuf[(y*width/2+x)*4]=buf[(j*width/2+i)*4+2];
//            tmpbuf[(y*width/2+x)*4+1]=buf[(j*width/2+i)*4+1];
//            tmpbuf[(y*width/2+x)*4+2]=buf[(j*width/2+i)*4];
//            tmpbuf[(y*width/2+x)*4+3]=buf[(j*width/2+i)*4+3];
//        }
//    }

    for(int i = 0 ; i < width*height/2;++i)
    {
        int cb = buf[i*4+1];
        int cr = buf[i*4+3];
        int y1= buf[i*4];
        int y2 = buf[i*4+2];
        int r =0;
        int g = 0;
        int b = 0;

        r = y1 + 1.370705*(cr-128);
        g = y1 - 0.698001*(cr-128)-0.337633*(cb-128);
        b = y1 + 1.732446*(cb-128);
        if(r>255) r = 255;
        if(g >255) g = 255;
        if(b >255)b = 255;
        if(r<0)r=0;
        if(g<0)g=0;
        if(b<0) b=0;
        shared->buf[i*6]=  r*220/256;
        shared->buf[i*6+1]= g*220/256;
        shared->buf[i*6+2]= b*220/256;



        r = y2 + 1.370705*(cr-128);
        g = y2 - 0.698001*(cr-128)-0.337633*(cb-128);
        b = y2 + 1.732446*(cb-128);
        if(r>255) r = 255;
        if(g >255) g = 255;
        if(b >255)b = 255;
        if(r<0)r=0;
        if(g<0)g=0;
        if(b<0) b=0;
        shared->buf[i*6+3]= r*220/256;
        shared->buf[i*6+4]= g*220/256;
        shared->buf[i*6+5]= b*220/256;
    }


//    for(int i = 0 ; i < width ; ++i)
//    {
//        for(int j = 0 ; j < height ; ++j)
//        {
//            int x = width-1-i;
//            int y = height-1-j;
//            memcpy(shared->buf+(x+y*width)*3,tmpbuf+(j*width+i)*3,3);
//        }
//    }

//    for(int i = 0; i < height ; ++i)
//    {
//        memcpy(tmpmidbuf+i*width*3, tmpbuf+(height-1-i)*width*3, width*3);
//    }

//    for(int i = 0; i < width ; ++i)
//    {
//        memcpy(shared->buf+i*height*3, tmpmidbuf+(width-1-i)*height*3, height*3);
//    }

    //memcpy(shared->buf, tmpbuf, width*height*3);

    if(shmdt(shm) == -1)
    {
        printf("shmdt failed\n");
    }
    pthread_mutex_lock(&m_lock);
    pthread_cond_signal(&cond);
    frametype = local;
    pthread_mutex_unlock(&m_lock);

}

static void putFrame(unsigned char *buf,int *bSize)
{

    printf("#########################put frame\n");
    //check if BM
    unsigned char *dataBuffer = NULL;
    bool isBM = false;
    for (int i = 0; i < 200; i++)
    {
        if (buf[i] == 66 && buf[i + 1] == 77)
        {
            dataBuffer = &buf[i];
            isBM = true;
            //printf("is BM\n");
            break;
        }
    }

    if (!isBM)
        return ;
    // get width and height
    int width = 0;
    int height = 0;
    memcpy(&width, dataBuffer + 18, sizeof(width));
    memcpy(&height, dataBuffer + 22, sizeof(height));
    if (width == 0 || height == 0)
        return ;
   // printf("width:%d\n",width);


    void *shm = NULL;
    struct SharedUseData *shared = NULL;
    int shmid;
    //create shared memory
    shmid = shmget((key_t)1234, sizeof(struct SharedUseData), 0666|IPC_CREAT);
    if(shmid == -1)
    {
        printf("shmget failed\n");
    }
    shm = shmat(shmid, (void*)0,0);
    if(shm == (void*)-1)
    {
        printf("shmat failed\n");
    }
    //printf("Memory attached at %X\n", (int)shm);
    //shared memory seting
    shared = (struct SharedUseData*)shm;
    shared->width = width;
    shared->height = height;
    memcpy(shared->buf, dataBuffer, width*height*3);
    if(shmdt(shm) == -1)
    {
        printf("shmdt failed\n");
    }
    pthread_mutex_lock(&m_lock);
    pthread_cond_signal(&cond);
    frametype = remote;
    pthread_mutex_unlock(&m_lock);
}

int ChangeVidInput()
{
    printf("$$$$$$$$$$$$$$$$$$$$$$$ service change vid input!!!!!!!!!!!!\n");
   return ChangeVideoInput();
}

void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    ep = new Endpoint;
    ep->libCreate();
    EpConfig ep_cfg;
    ep_cfg.logConfig.level = 5;
    ep->libInit(ep_cfg);
    //    ep->codecSetPriority("speex/8000", 200);
    TransportConfig tcfg;
    tcfg.port = sipPort;
    ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    ep->libStart();

    player = new AudioMediaPlayer;
    player->createPlayer("/usr/share/voipService/清晨乐章2.wav");

    outgoingPlayer = new AudioMediaPlayer;
    outgoingPlayer->createPlayer("/usr/share/voipService/朝气蓬勃2.wav");

    AccountConfig acc_cfg;
    acc_cfg.idUri = "sip:192.168.199.155";
    acc_cfg.mediaConfig.transportConfig.port = rtpPort;
    acc_cfg.mediaConfig.transportConfig.portRange = rtpPortRange;
    acc_cfg.videoConfig.autoShowIncoming = true;
    acc_cfg.videoConfig.autoTransmitOutgoing = true;
    acc = new VoipAccount;
    acc->create(acc_cfg);

    // by michael zheng 2017.4.13
    pjsua_codec_info id[100];
    unsigned count=100;
    pjsua_vid_enum_codecs(id,&count);
    printf("count=%d\n",count);
    for(int i = 0; i < count;++i)
    {
        printf("codec_id:%s\n",id[i].codec_id.ptr);
    }

        const pj_str_t codec_id = {"h263p", 4};//H264
        pjmedia_vid_codec_param param;
        pjsua_vid_codec_get_param(&codec_id, &param);

        /* Modify param here */
        // from 10 ---20    设置帧率
        param.enc_fmt.det.vid.fps.num   = 20;
        param.enc_fmt.det.vid.fps.denum = 1;
        /* Bitrate range preferred: 512-1024kbps */
        //设置比特率
//        param.enc_fmt.det.vid.avg_bps = 2000;
//        param.enc_fmt.det.vid.max_bps = 4000;
        qDebug()<<"avg_bps" << param.enc_fmt.det.vid.avg_bps;
        qDebug()<<"max_bps" << param.enc_fmt.det.vid.max_bps;

        //------------------------------ 提高画质
        param.dec_fmt.det.vid.fps.num = 30; //设置帧率
        param.enc_fmt.det.vid.fps.num = 10;
        param.dec_fmt.det.vid.avg_bps = 51200*2;//设置比特率
        param.dec_fmt.det.vid.max_bps = 51200*2;
        param.enc_fmt.det.vid.avg_bps = 51200*2;
        param.enc_fmt.det.vid.max_bps = 51200*2;
        param.ignore_fmtp = PJ_TRUE;//指定分辨率必须设置此值，否则将由双方通信协商
        pjsua_codec_set_priority(&codec_id, (pj_uint8_t)PJMEDIA_CODEC_PRIO_HIGHEST);

//        //------------------------------提高画质

        pjsua_vid_codec_set_param(&codec_id, &param);
    // end by michael zheng
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    //log dir
    QDir dir = QDir::rootPath();
    dir.cd("home");
    dir.cd("user");
    dir.mkdir("log");
    qInstallMessageHandler(myMessageOutput);

    putenv("SDL_VIDEODRIVER=dummy");
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        char buf[100] = {0};
        sprintf(buf, "Unable to init SDL: %s\n", SDL_GetError());
        qDebug() << buf << endl;
    }

    SDLVideoAcc sdlvideoacc;
    sdlvideoacc.SDL_Callback = &::putFrame;
    sdlvideoacc.g = 5;
    SDL_RegisterCallback(&sdlvideoacc);
    queue_data.state = invaild;
    queue_data.bsize = 0;
    //queue_data.buf = (unsigned char*)malloc(BUFFSIZE);
    //memset(queue_data.buf,0,BUFFSIZE);

    pjmedia_v4l2_callback callback;
    callback.putframe = &::putV4l2Frame;
    registerV4l2CallBack(&callback);

    VoipVideoServer *server = new VoipVideoServer();
    server->start();

    qDebug() << "pppppppppppppppppp" << sdlvideoacc.SDL_Callback<<endl;
    QCoreApplication a(argc, argv);

    //register type
    qRegisterMetaType<pj::OnIncomingCallParam>("OnIncomingCallParam");

    VoipInterface dBus;
    service = &dBus;
    qDebug() << "VOIP service start" << endl;
    int ret = a.exec();

    delete acc;
    if (recorder)
        delete recorder;
    delete player;
    ep->libDestroy();
    delete ep;

    return ret;
}


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString logFileName = "adhoc";
    logFileName .append( QDateTime::currentDateTime().toString("yyyy_MM") )
                .append( ".log" );
    static QFile file("/home/user/log/" + logFileName);
    QByteArray localMsg = msg.toLocal8Bit();
    QString sprintStr;
    switch (type) {
    case QtDebugMsg:
        sprintStr.sprintf("---Debug: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        sprintStr.sprintf("---Warning: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        sprintStr.sprintf("---Critical: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        sprintStr.sprintf("---Fatal: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
    if(!file.isOpen())
    {
        if(!file.open(QIODevice::WriteOnly))
        {
            return;
        }
    }
    localMsg = sprintStr.toLocal8Bit();
    file.write(localMsg);
    std::cout << msg.toLocal8Bit().data() << std::endl;
    file.flush();
}
