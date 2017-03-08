#include "lingyun.h"
#include <QDebug>
#include <QFile>
#define DATA_PATH "/data/apps/com.sinux.phonebooklte
#define APP_PATH "/data/data/com.sinux.phonebooklte
#include <QAudioDeviceInfo>
#include <QtConcurrent/QtConcurrent>

LingYun *LingYun::instance = NULL;

LingYun *LingYun::getInstance()
{
    int ret = 0;
    if(instance == NULL){
        instance = new LingYun();
        ret = instance->init();
        if (ret == -1)
            return NULL;
    }
    return instance;
}

LingYun::LingYun(QObject *parent) :
    QObject(parent)
{
}

LingYun::~LingYun()
{
    unInit();
}

int LingYun::init()
{

    m_ttsSynth = new TtsSynth(this);
    HCI_ERR_CODE err = HCI_ERR_NONE;
    //char *config = "developerKey=712ddd892cf9163e6383aa169e0454e3,appKey=c85d54f1,cloudUrl=http://test.api.hcicloud.com:8888,authPath=/data/developer/,appChannel=1";
    QString init_config = "";
    init_config += "appKey=0d5d54e9";              //灵云应用序号
    init_config += ",developerKey=9ddabc2c7e26381298a8240ccb26656d"; //灵云开发者密钥
    init_config += ",cloudUrl=http://api.hcicloud.com:8888";         //灵云云服务的接口地址
    init_config += ",capKey=tts.local.synth";
    init_config += QString(",authpath=%1").arg("/data/data/com.sinux.phonebooklte");         //授权文件所在路径，保证可写
    init_config += QString(",logfilepath=%1").arg("/data/data/com.sinux.phonebooklte");   //日志的路径
    init_config += ",loglevel=5";								     //日志的等级
    init_config += ",logfilesize=512";

    char*  ch;
    QByteArray ba = init_config.toLatin1();
    ch=ba.data();
    qDebug() << "lingyun init = " << hci_get_sdk_version();
    err = hci_init(ch);

    if( err != HCI_ERR_NONE )
    {
        qDebug() << "hci_init return" <<  err << hci_get_error_info(err);
        return -1;
    }

    //检测授权，如果过期则更新授权
    if (!CheckAndUpdateAuth())
    {
        qDebug() << "CheckAndUpdateAuth failed";
        hci_release();
        return -1;
    }
    initAudioOutput();
    //transToTts("adfad");
//    m_ttsSynth->FunctionFunc("tts.local.synth","/data/apps/com.sinux.phonebooklte/libs","/home/user/tts");

//    unInit();
//    play();
}

int LingYun::unInit()
{
    qDebug() << "linyun release";
    hci_release();
}

void LingYun::play()
{
    m_sourceFile.setFileName("/home/user/synthResult.pcm");
    m_sourceFile.open(QIODevice::ReadOnly);
    m_localPlayFile.setDevice(&m_sourceFile);
    m_audioOutputIODevice = m_audioOutput->start();
    playDatatoDevice();
}

void LingYun::stopPlay()
{
    qDebug() << __FUNCTION__ << __LINE__;
    m_audioOutput->stop();
    m_sourceFile.close();
}

void LingYun::playDatatoDevice()
{
    int maxSize= 320;
    char data[320];
    int len = 0;
    while(!m_localPlayFile.atEnd()){
        usleep(5000);
        len = m_localPlayFile.readRawData(data, maxSize);
        if(len <= 0){
           break;
        }
        m_audioOutputIODevice->write(data, len);
        usleep(5000);
    }
    m_sourceFile.close();
}

void LingYun::initAudioOutput()
{
    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    qDebug() << __FUNCTION__ << __LINE__;
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if(info.isNull()){
        qDebug() << "xxxxxxxxxxx";
    }
    if (!info.isFormatSupported(format)) {
       qWarning() << "Default format not supported, trying to use the nearest.";
       format = info.nearestFormat(format);
    }

    m_audioOutput = new QAudioOutput(format, this);
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(onStateChanged(QAudio::State)));

}

void LingYun::writeToFile(QString msg)
{
    QFile file("/home/user/tts.txt");
    file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    file.write("      " + msg.toLocal8Bit());
    file.close();
    m_enable = false;
    m_ttsSynth->FunctionFunc("tts.local.synth","/data/apps/com.sinux.phonebooklte/libs","/home/user");
    play();
    if (m_msgList.length() == 0)
    {
        m_enable = true;
        return;
    }
    else if (m_msgList.length() > 0)
    {
        QString mymsg = m_msgList.at(0);
        m_msgList.removeAt(0);
        writeToFile(mymsg);
    }
}

void LingYun::transToTts(QString msg)
{
    if (m_enable == true)
        QtConcurrent::run(this,&LingYun::writeToFile,msg);
    else
        m_msgList.append(msg);
//    QtConcurrent::run(this,&LingYun::play);

//    writeToFile(msg);
//    m_ttsSynth->FunctionFunc("tts.local.synth","/data/apps/com.sinux.phonebooklte/libs","/home/user/tts");
//    play();
}

bool LingYun::CheckAndUpdateAuth()
{
    //获取过期时间
    int64 expire_time;
    int64 current_time = (int64)time( NULL );
    HCI_ERR_CODE err_code = hci_get_auth_expire_time( &expire_time );
    if( err_code == HCI_ERR_NONE )
    {
        //获取成功则判断是否过期
        if( expire_time > current_time )
        {
            //没有过期
            qDebug() << "auth can use continue";
            return true;
        }
    }

    //获取过期时间失败或已经过期
    //手动调用更新授权
    //HCI_ERR_CODE err_code;
    err_code = hci_check_auth();
    if( err_code == HCI_ERR_NONE )
    {
        //更新成功
        qDebug() << "check auth success";
        return true;
    }
    else
    {
        //更新失败
        qDebug() << "check auth return", err_code ,hci_get_error_info(err_code);
        return false;
    }
}

void LingYun::onStateChanged(QAudio::State state)
{
    qDebug() << "onStateChanged" << state;
}
