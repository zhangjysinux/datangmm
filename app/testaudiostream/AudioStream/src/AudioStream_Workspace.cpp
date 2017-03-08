#include "AudioStream_Workspace.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QFile>
#include <QObject>
#include <QQmlContext>
#include <QQuickItem>

AudioStream_Workspace::AudioStream_Workspace()
    : CWorkspace(),
      m_isFirst(false),
      m_zhwIp(QString("168.32.100.61")),
      m_udpPort(8086),
      m_talkFlag(false),
      m_battleInterface("org.uhf",
                        "/uhf",
                        "org.uhf.cmd",
                        QDBusConnection::systemBus())
{

    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->rootContext()->setContextProperty("getData", this);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
    userInit();

    QObject *rootObject = qobject_cast<QObject *>(m_view->rootObject());
    connect(this, SIGNAL(signalText(QVariant)), rootObject, SLOT(onTextAppend(QVariant)));
}

void AudioStream_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)
    qDebug()<<Q_FUNC_INFO;
    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";

        foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
            qDebug() << "Device name: " << deviceInfo.deviceName();
        foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
            qDebug() << "Device name: " << deviceInfo.deviceName();

        initAudioOutput();
        initAudioInput();
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}

void AudioStream_Workspace::initAudioInput()
{
    qDebug()<<Q_FUNC_INFO;
    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(info.isNull()){
        qDebug() << "xxxxxxxxxxxxxxx";
    }
    if (!info.isFormatSupported(format)) {
       qWarning() << "Default format not supported, trying to use the nearest.";
       format = info.nearestFormat(format);
    }

    m_audioInput = new QAudioInput(format, this);
//    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    m_audioInputIODevice = m_audioInput->start();
    connect(m_audioInputIODevice,SIGNAL(readyRead()),this,SLOT(captureDataFromDevice()));
}

void AudioStream_Workspace::initAudioOutput()
{
    qDebug()<<Q_FUNC_INFO;
    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if(info.isNull()){
        qDebug() << "xxxxxxxxxxx";
    }
    if (!info.isFormatSupported(format)) {
       qWarning() << "Default format not supported, trying to use the nearest.";
       format = info.nearestFormat(format);
    }

    m_audioOutput = new QAudioOutput(format, this);
//    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    m_audioOutputIODevice = m_audioOutput->start();
}

void AudioStream_Workspace::userInit()
{
    m_serverUdp = new QUdpSocket();
    QHostAddress host("168.32.100.41");
    bool bind = m_serverUdp->bind(host, m_udpPort, QAbstractSocket::ShareAddress);
    qDebug() << "bind flag:" << bind;
    QObject::connect(m_serverUdp, SIGNAL(readyRead()),this,SLOT(onReadyRead()));

    m_sendUdp=new QUdpSocket();
    m_sendUdp->connectToHost(m_zhwIp, m_udpPort);

    m_header.append(0x55);
    m_header.append(0x45);
    m_header.append(0x03);
    m_header.append(0x24);
}


void AudioStream_Workspace::captureDataFromDevice()
{
    //qint64 maxSize= 4*1024;
    int maxSize=800;
//    while(m_talkFlag)
//    {
        QByteArray data = m_audioInputIODevice->read(maxSize);
        if(data.isEmpty() && data.length()<800){
           return;
        }

        if(!m_talkFlag)
            return;

        QByteArray sendData;
        sendData.append(m_header);
        QByteArray tempData=data.mid(0,800);
        QByteArray tempData2;
        for(int i=1;i<800;i+=2)
        {//交换高低位,除噪音
            tempData2[i-1]=tempData[i];
            tempData2[i]=tempData[i-1];
        }
        sendData.append(tempData2);
//        if(!m_isFirst)
//        {//第一次发送给电台,需要发送ptt指令
//            char byteArray[4];
//            byteArray[0]=0x03;
//            byteArray[1]=0x32;
//            byteArray[2]=0x00;
//            byteArray[3]=0x00;

//            m_sendUdp->write(byteArray,4);
//            m_isFirst=true;
//            QThread::msleep(50);
//        }
        m_sendUdp->write(sendData);
//        QThread::msleep(25);
//    }
}

void AudioStream_Workspace::onReadyRead()
{
   while(m_serverUdp->hasPendingDatagrams())
   {
       int pendingSize = m_serverUdp->pendingDatagramSize();
       QByteArray singleData;
       singleData.resize(pendingSize);
       m_serverUdp->readDatagram(singleData.data(), singleData.size());
       if(singleData[0]==0x03 && singleData[1]==0x82)
       {//上报电台当前工作状态
           QVariant log = "report server cur state...................";
              emit signalText(log);
              qDebug() << "report server cur state...................";
              continue;
       }
       else if(singleData[0]==0x03 && singleData[1]==0xf4)
       {//流控状态上报
           QVariant log = "report stream control state...................";
           emit signalText(log);
           qDebug() << "report stream control state...................";
           continue;
       }
       else if(singleData[0]==0x55 && singleData[1]==0x45)
       {
           QVariant log = "receive audio data...................";
           emit signalText(log);
           qDebug() << "receive audio data...................";
           //交换高低位,除噪音
           QByteArray tempArray1=singleData.mid(4,800);
           QByteArray writeArray;
           for(int i=1;i<800;i+=2)
           {
               writeArray[i-1]=tempArray1[i];
               writeArray[i]=tempArray1[i-1];
           }
           if(m_audioOutputIODevice)
           {//调用元心语音输出模块
               m_audioOutputIODevice->write(writeArray);
           }
       }
   }
}

void AudioStream_Workspace::onStartTalkClick()
{
    QVariant log = "start talk...................";
    emit signalText(log);
    qDebug() << "start talk...................";
    m_battleInterface.call(QDBus::NoBlock, "SetPPTStart");
//    char byteArray[4];
//    byteArray[0]=0x03;
//    byteArray[1]=0x32;
//    byteArray[2]=0x00;
//    byteArray[3]=0x00;

//    m_sendUdp->write(byteArray,4);
    m_talkFlag = true;
}

void AudioStream_Workspace::onEndTalkClick()
{
    QVariant log = "end talk...................";
    emit signalText(log);
    qDebug() << "end talk...................";
    m_battleInterface.call(QDBus::NoBlock, "SetPPTEnd");
//    char byteArray[4];
//    byteArray[0]=0x03;
//    byteArray[1]=0x33;
//    byteArray[2]=0x00;
//    byteArray[3]=0x00;

//    m_sendUdp->write(byteArray,4);
    m_talkFlag = false;
}
