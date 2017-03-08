#include <QAudioFormat>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QNetworkInterface>
#include <QThread>
#include "battlepttadaptor.h"

const QString UHFService = "org.uhf";
const QString UHFServicePath = "/uhf";
const QString UHFInterface = "org.uhf.cmd";
const QString UHFNetworkInterfaceName = "uhf0";
const QString BattleBroadcastIn = "168.32.100.41";
const QString BattleBroadcastOut = "168.32.100.61";
const quint32 BattleBroadcastPort = 8086;
const QString AudioCodec = "audio/pcm";
const quint32 AudioSampleRate = 16000;
const quint32 AudioChannelCount = 1;
const quint32 AudioSampleSize = 16;
const quint32 AudioInMaxSize = 800;

BattlePTTAdaptor *BattlePTTAdaptor::m_instance = NULL;
BattlePTTAdaptor *BattlePTTAdaptor::getInstane()
{
    if(m_instance == NULL)
        m_instance = new BattlePTTAdaptor();

    return m_instance;
}

BattlePTTAdaptor::BattlePTTAdaptor(QObject *parent) :
    QObject(parent), m_battleInterface(UHFService, UHFServicePath, UHFInterface, QDBusConnection::systemBus()),
    m_audioInput(NULL), m_audioInDevice(NULL), m_audioOutput(NULL), m_audioOutDevice(NULL),
    m_inSocket(NULL), m_pttState(false), m_testMode(false)
{
    initAudioInput();
    initAudioOutput();
    startTimer(1000);
}

BattlePTTAdaptor::~BattlePTTAdaptor()
{
    if(m_audioInput != NULL)
    {
        m_audioInput->stop();
        delete m_audioInput;
    }

    if(m_audioOutput != NULL)
    {
        m_audioOutput->stop();
        delete m_audioOutput;
    }
}

void BattlePTTAdaptor::pttStart()
{
    qDebug() << "...................control remote.....................";
    m_battleInterface.call(QDBus::NoBlock, "SetRemote", 0);
    m_pttState = true;
    qDebug() << "...................ptt start..................";
    m_battleInterface.call(QDBus::NoBlock, "SetPPTStart");
}

void BattlePTTAdaptor::pttEnd()
{
    qDebug() << "...................control remote.....................";
    m_battleInterface.call(QDBus::NoBlock, "SetRemote", 0);
    m_pttState = false;
    qDebug() << "...................ptt end..................";
    m_battleInterface.call(QDBus::NoBlock, "SetPPTEnd");
}

void BattlePTTAdaptor::setTestMode()
{
    m_testMode = !m_testMode;
    qDebug() << "...................cur test mode...............:" << m_testMode;
}

void BattlePTTAdaptor::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    if(m_inSocket == NULL)
        initBattleBoradcastIn();
}

void BattlePTTAdaptor::initAudioInput()
{
    qDebug() << "..................init AudioInput..........................";
    QAudioFormat format;
    format.setSampleRate(AudioSampleRate);
    format.setChannelCount(AudioChannelCount);
    format.setSampleSize(AudioSampleSize);
    format.setCodec(AudioCodec);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(info.isNull())
    {
        qDebug() << ".................default audio input device info is Null.............";
        return;
    }

    if(!info.isFormatSupported(format))
    {
        qDebug() << ".................default audio input device info not support pcm data.......";
        return;
    }

    m_audioInput = new QAudioInput(format, this);
    m_audioInDevice = m_audioInput->start();
    connect(m_audioInDevice, SIGNAL(readyRead()), this, SLOT(onAudioIn()));
}

void BattlePTTAdaptor::initAudioOutput()
{
    qDebug() << "............................init AudioOutput...................";
    QAudioFormat format;
    format.setSampleRate(AudioSampleRate);
    format.setChannelCount(AudioChannelCount);
    format.setSampleSize(AudioSampleSize);
    format.setCodec(AudioCodec);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if(info.isNull())
    {
        qDebug() << ".................default audio output device info is Null.............";
        return;
    }

    if(!info.isFormatSupported(format))
    {
        qDebug() << ".................default audio output device info not support pcm data.......";
        return;
    }

    m_audioOutput = new QAudioOutput(format, this);
    m_audioOutDevice = m_audioOutput->start();
}

void BattlePTTAdaptor::initBattleBoradcastIn()
{
    qDebug() << "...........................init Battle in socket.....................";
    QNetworkInterface netInterface = QNetworkInterface::interfaceFromName(UHFNetworkInterfaceName);
    if(netInterface.isValid())
    {
        m_inSocket = new QUdpSocket();
        QHostAddress host(BattleBroadcastIn);
        bool inBind = m_inSocket->bind(host, BattleBroadcastPort, QAbstractSocket::ShareAddress);
        qDebug() << "...............in socket bind flag:" << inBind;
        if(!inBind)
        {
            delete m_inSocket;
            m_inSocket = NULL;
            return;
        }

        connect(m_inSocket, SIGNAL(readyRead()), this, SLOT(onBoradcastIn()));
        connect(m_inSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onBoradcastInError(QAbstractSocket::SocketError)));

        qDebug() << "...................control remote.....................";
        m_battleInterface.call(QDBus::NoBlock, "SetRemote", 0);
    }
}

void BattlePTTAdaptor::onBoradcastIn()
{
    if(m_inSocket == NULL)
        return;

    while(m_inSocket->hasPendingDatagrams())
    {
        int pendingSize = m_inSocket->pendingDatagramSize();
        QByteArray singleData;
        singleData.resize(pendingSize);
        m_inSocket->readDatagram(singleData.data(), singleData.size());
        if(singleData[0]==0x03 && singleData[1]==0x82)
        {
            //上报电台当前工作状态
            qDebug() << "..................radio work state............";
            continue;
        }
        else if(singleData[0]==0x03 && singleData[1]==0xf4)
        {
            //流控状态上报
            qDebug() << ".................stream control state..............";
            continue;
        }
        else if(singleData[0]==0x55 && singleData[1]==0x45)
        {
            qDebug() << ".................receive audio data...............";
            //交换高低位,除噪音
            QByteArray tempArray1=singleData.mid(4,800);
            QByteArray writeArray;
            for(int i=1;i<800;i+=2)
            {
                writeArray[i-1]=tempArray1[i];
                writeArray[i]=tempArray1[i-1];
            }

            if(m_testMode)
                continue;

            if(m_audioOutDevice != NULL)
                m_audioOutDevice->write(writeArray);
        }
    }
}

void BattlePTTAdaptor::onBoradcastInError(QAbstractSocket::SocketError error)
{
    qDebug() << "..................in socket error:" << error;
    delete m_inSocket;
    m_inSocket = NULL;
}

void BattlePTTAdaptor::onAudioIn()
{
    QByteArray data = m_audioInDevice->read(AudioInMaxSize);
    if(data.isEmpty() && (data.length() < AudioInMaxSize))
        return;

    if(!m_pttState)
        return;

    if(m_testMode)
        return;

    if(m_inSocket == NULL)
        return;

    QByteArray voiceData;
    voiceData.append(0x55);
    voiceData.append(0x45);
    voiceData.append(0x03);
    voiceData.append(0x24);

    QUdpSocket *outSocket = new QUdpSocket();
    outSocket->connectToHost(BattleBroadcastOut, BattleBroadcastPort);

    QByteArray voiceArray = data.mid(0,800);
    QByteArray voiceArrayTemp;
    for(int i = 1;i < AudioInMaxSize; i += 2)
    {
        //交换高低位,除噪音
        voiceArrayTemp[i-1]=voiceArray[i];
        voiceArrayTemp[i]=voiceArray[i-1];
    }

    voiceData.append(voiceArrayTemp);
    outSocket->write(voiceData);
    //QThread::msleep(25);

    delete outSocket;
}
