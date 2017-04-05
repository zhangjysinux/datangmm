#include "voipdbusdata.h"
#include "voipinterface.h"
#include <SDL2/SDL.h>
#include <QCoreApplication>
#include <QDebug>
#include "voipaccount.h"
#include "voipvideoserver.h"
#include <pjsua2.hpp>
#include <pjsua.h>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QMutex>
#include <QNetworkInterface>
#include "adhocbus/adhocconnectadaptor.h"

#include <sys/shm.h>
#include<pjmedia-videodev/videodev.h>
#define BUFSIZE 2000000
using namespace pj;

Endpoint *ep;
//AudioMediaPlayer *player;
//AudioMediaPlayer *outgoingPlayer;
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

    shared = (struct SharedUseData*)shm;
    shared->width = width ;
    shared->height = height;

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

    if(shmdt(shm) == -1)
    {
        printf("shmdt failed\n");
    }

    pthread_mutex_lock(&m_lock);
    service->emitV4l2FrameData();
//    pthread_cond_signal(&cond);
//    frametype = local;
    pthread_mutex_unlock(&m_lock);
    //service->emitV4l2FrameData();

}

static void putFrame(unsigned char *buf,int *bSize)
{
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
    service->emitVoipFrameData();
//    pthread_cond_signal(&cond);
//    frametype = remote;
    pthread_mutex_unlock(&m_lock);
}

int ChangeVidInput()
{
    printf("$$$$$$$$$$$$$$$$$$$$$$$ service change vid input!!!!!!!!!!!!\n");
   return ChangeVideoInput();
}

void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    qDebug() << "initialize--------------------------------" << endl;

    ep = new Endpoint;
//    try{
        ep->libCreate();
//    }catch(Error&err){
//        qDebug()<<"---michaelzheng---Startup error:"<<err.info()<<endl;
//    }
    EpConfig ep_cfg;

    //config audio queality
//    ep_cfg.medConfig.clockRate = 8000;
//    ep_cfg.medConfig.sndClockRate = 4000;
//    ep_cfg.medConfig.audioFramePtime = 60;
#ifdef voipAdHocService
    // by michael zheng 2017.3.16 from 2 to 4
    ep_cfg.medConfig.quality = 4;
    // end by michael zheng
#else
    ep_cfg.medConfig.quality = 8;
#endif


    ep_cfg.logConfig.level = 5;
#ifdef voipAdHocService
    ep_cfg.logConfig.filename = "/home/user/log/adhocvoip.log";
#elif voipLteService
    ep_cfg.logConfig.filename = "/home/user/log/ltevoip.log";
#elif voipAdHocService
    ep_cfg.logConfig.filename = "/home/user/log/wifivoip.log";
#endif

    ep->libInit(ep_cfg);

#ifdef voipAdHocService
    ep->codecSetPriority("speex/8000/1", 200);
    qDebug() << "ep->codecSetPriority(\"speex/8000/1\", 200)";
#else
    ep->codecSetPriority("iLBC/8000/1", 200);
#endif



    CodecInfoVector codecVector = ep->codecEnum();
    qDebug() << "------codec" << endl;
    for(int i=0; i<codecVector.size(); i++)
    {
        CodecInfo *info = codecVector.at(i);
        qDebug() << "------codec: "
                 << info->codecId.data()
                 << info->priority
                 << endl;
    }

    TransportConfig tcfg;
    tcfg.port = sipPort;

#ifdef voipAdHocService
    QString adhocAddress;
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
        QString addr = address.toString();
        if(addr.contains("192.168.43.") || addr.contains("192.168.90."))
        {
            adhocAddress = addr;
            break;
        }
    }
    tcfg.boundAddress = adhocAddress.toStdString();
    qDebug() << "-----------------bind address: " << adhocAddress;

#endif

    ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    ep->libStart();

//    player = new AudioMediaPlayer;
//    player->createPlayer("/usr/share/voipService/清晨乐章2.wav");

//    outgoingPlayer = new AudioMediaPlayer;
//    outgoingPlayer->createPlayer("/usr/share/voipService/朝气蓬勃2.wav");

    AccountConfig acc_cfg;
#ifdef voipAdHocService
    acc_cfg.idUri = "sip:192.168.90.90";
#else
    acc_cfg.idUri = "sip:192.168.1.90";
#endif
    acc_cfg.mediaConfig.transportConfig.port = rtpPort;
    acc_cfg.mediaConfig.transportConfig.portRange = rtpPortRange;
    acc_cfg.videoConfig.autoShowIncoming = true;
    acc_cfg.videoConfig.autoTransmitOutgoing = true;
    acc = new VoipAccount;
    acc->create(acc_cfg);

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
#ifdef voipAdHocService
    param.enc_fmt.det.vid.fps.num   = 10;
#else
    param.enc_fmt.det.vid.fps.num   = 8;
#endif

        param.enc_fmt.det.vid.fps.denum = 1;
        /* Bitrate range preferred: 512-1024kbps */
        //param.enc_fmt.det.vid.avg_bps = 2000;
        //param.enc_fmt.det.vid.max_bps = 4000;
        qDebug()<<"avg_bps" << param.enc_fmt.det.vid.avg_bps;
        qDebug()<<"max_bps" << param.enc_fmt.det.vid.max_bps;

        pjsua_vid_codec_set_param(&codec_id, &param);

}

void createPath(const QString &path)
{
    QStringList pathList = path.split("/", QString::SkipEmptyParts);
    QDir dir = QDir::rootPath();
    foreach(const QString &path, pathList)
    {
        if(dir.exists(path))
        {
            dir.cd(path);
        }
        else
        {
            if(dir.mkdir(path))
            {
                qDebug() << "create folder: " << dir.absolutePath() + "/" + path;
            }
        }
    }
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    //log dir
    QDir dir = QDir::rootPath();
    dir.cd("home");
    dir.cd("user");
    dir.mkdir("log");
    createPath("/home/user/sinux/voicerecords/");

#ifdef voipAdHocService
    qInstallMessageHandler(myMessageOutput);
#else
//    qInstallMessageHandler(myMessageOutput);
#endif

    system("cd /home/user/ && mkdir voip____");


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

//    VoipVideoServer *server = new VoipVideoServer();
//    server->start();

//    qDebug() << "pppppppppppppppppp" << sdlvideoacc.SDL_Callback<<endl;
    QCoreApplication a(argc, argv);
    //register type
    qRegisterMetaType<pj::OnIncomingCallParam>("OnIncomingCallParam");
    VoipInterface dBus;
    service = &dBus;

#ifdef voipAdHocService
    service->onInitialize(7010, 6010, 14);
    qDebug()<<"-----------------in voipAdHocService---6---" ;
#elif voipLteService
    qDebug()<<"-----------------in voipAdHocService---7---" ;
    service->onInitialize(5160, 5161, 20);
#elif voipWifiService
    qDebug()<<"-----------------in voipAdHocService---8---" ;
    service->onInitialize(5080, 33333, 10);
#endif


    qDebug() << "VOIP service start" << endl;
    int ret = a.exec();

    delete acc;
    if (recorder)
        delete recorder;
//    delete player;
    ep->libDestroy();
    qDebug()<<"-----------------in voipAdHocService---10---" ;
    delete ep;

    return ret;
}


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

//    QFile filee("/home/user/log/mylog.txt");
//    if(filee.open(QIODevice::WriteOnly|QIODevice::Text))
//    {
//        filee.write("ssssssssssss");
//        filee.flush();
////        file.close();
//    }

    static QMutex mutex;
//    QMutexLocker locker(&mutex);
    mutex.lock();
    QString logFileName;
#ifdef voipAdHocService
    logFileName = "adhoc";
#elif voipLteService
    logFileName = "lte";
#else
    logFileName = "wifi";
#endif

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
        break;
    default:
        break;
    }
    if(!file.isOpen()) {
        if(!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
        {
            return;
        }
    }
    localMsg = sprintStr.toLatin1();
    file.write(localMsg);
//    std::cout << msg.toLocal8Bit().data() << std::endl;
    file.flush();
    mutex.unlock();
}
