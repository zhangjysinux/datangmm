#include "battlecallinterface.h"

BattleCall * BattleCall::m_instance = NULL;
BattleCall *BattleCall::getInstance()
{
    if(m_instance == NULL)
        m_instance = new BattleCall();

    return m_instance;
}

BattleCall::BattleCall(QObject *parent)
    : QObject(parent),
      m_audioOutput(NULL),
      m_audioInput(NULL),
      m_audioOutputIODevice(NULL),
      m_audioInputIODevice(NULL),
      m_serverUdp(NULL),
      m_sendUdp(NULL),
      m_isFirst(true),
      m_battleInterface("org.uhf",
                        "/uhf",
                        "org.uhf.cmd",
                        QDBusConnection::systemBus()),
      m_bindFlag(false)
{
    transportInit();
    audioOutputInit();
    startTimer(1000);
}

int BattleCall::call(const QString &addr, int port)
{
    qDebug() << "battle call start...................";
    m_serverIp = addr;
    m_port = port;

    // udp transport init
    audioInputInit();
    QDBusReply<void> retValDbus = m_battleInterface.call("SetPPTStart");
    if(retValDbus.isValid())
        qDebug() << "call SetPPTStart";
    else
        qDebug() << "BattleCall SetPPTStart reply error:" << retValDbus.error();
    return 1;
}

int BattleCall::hangup()
{
    qDebug() << "battle call hangup...................";
    audioInputDestory();
    QDBusReply<void> retValDbus = m_battleInterface.call("SetPPTEnd");
    if(retValDbus.isValid())
        qDebug() << "call SetPPTEnd";
    else
        qDebug() << "BattleCall SetPPTEnd reply error:" << retValDbus.error();
    return 1;
}

int BattleCall::setPttState(int pttState)
{
//    if(pttState != 0 && pttState != 1 && pttState != 2)
//        return -1;

//    //listen
//    if(pttState == 0)
//    {
//        audioOutputDestroy();
//        audioInputDestory();
//    }
//    else if(pttState == 1)
//    {
//        audioOutputDestroy();
//    }
//    else if(pttState == 2)//speak
//    {
//        audioInputDestory();
//    }
    return 1;
}

int BattleCall::getPttState()
{
//    int pttState = 0;
//    if(m_audioInput == NULL && m_audioOutput == NULL)
//    {
//        pttState = 0;
//    }
//    else if(m_audioInput == NULL)
//    {
//        pttState = 2;
//    }
//    else if(m_audioOutput == NULL)
//    {
//        pttState = 1;
//    }
    return 1;
}

void BattleCall::audioInputInit()
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
    connect(m_audioInputIODevice,SIGNAL(readyRead()),this,SLOT(onCaptureDataFromDevice()));

}

void BattleCall::audioInputDestory()
{
    if(m_audioInput == NULL)
        return;

    m_audioInputIODevice->close();
    m_audioInputIODevice->deleteLater();
    m_audioInputIODevice = NULL;

    m_audioInput->stop();
    m_audioInput->deleteLater();
    m_audioInput = NULL;

    char byteArray[4];
    byteArray[0]=0x03;
    byteArray[1]=0x33;
    byteArray[2]=0x00;
    byteArray[3]=0x00;

    m_sendUdp->write(byteArray,4);
    m_isFirst = true;
}

void BattleCall::audioOutputInit()
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

void BattleCall::audioOutputDestroy()
{
    if(m_audioOutput == NULL)
        return;

    m_audioOutputIODevice->close();
    m_audioOutputIODevice->deleteLater();
    m_audioOutputIODevice = NULL;

    m_audioOutput->stop();
    m_audioOutput->deleteLater();
    m_audioOutput = NULL;
}

void BattleCall::transportInit()
{
//    qDebug() << "transportInit start.................";
    QNetworkInterface interfaces = QNetworkInterface::interfaceFromName("uhf0");
    if(interfaces.isValid())
    {
        m_serverUdp = new QUdpSocket();
        QHostAddress host("168.32.100.41");
        m_bindFlag = m_serverUdp->bind(host, 8086, QAbstractSocket::ShareAddress);
        qDebug() << "server udp bind flag:" << m_bindFlag;
        QObject::connect(m_serverUdp,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        QObject::connect(m_serverUdp,SIGNAL(error(QAbstractSocket::SocketError)),
                         this, SLOT(onServerError(QAbstractSocket::SocketError)));

        m_sendUdp = new QUdpSocket();
        m_sendUdp->connectToHost("168.32.100.61", 8086);

        m_header.append(0x55);
        m_header.append(0x45);
        m_header.append(0x03);
        m_header.append(0x24);
    }
}

void BattleCall::transportDestroy()
{
    m_serverUdp->abort();
    m_serverUdp->close();
    m_serverUdp->deleteLater();
    m_serverUdp = NULL;

    m_sendUdp->abort();
    m_sendUdp->close();
    m_sendUdp->deleteLater();
    m_sendUdp = NULL;
}

void BattleCall::onCaptureDataFromDevice()
{
    //qint64 maxSize= 4*1024;
    int maxSize=800;
    while(true){
        QByteArray data = m_audioInputIODevice->read(maxSize);
        if(data.isEmpty() && data.length()<800){
           break;
        }
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

        if(m_isFirst)
        {//第一次发送给电台,需要发送ptt指令
            char byteArray[4];
            byteArray[0]=0x03;
            byteArray[1]=0x32;
            byteArray[2]=0x00;
            byteArray[3]=0x00;

            m_sendUdp->write(byteArray,4);
            m_isFirst = false;
            QThread::msleep(50);
        }
        m_sendUdp->write(sendData);
        QThread::msleep(25);
    }

}

void BattleCall::onReadyRead()
{

    while(m_serverUdp->hasPendingDatagrams())
    {
        int pendingSize = m_serverUdp->pendingDatagramSize();
        QByteArray singleData;
        singleData.resize(pendingSize);
        m_serverUdp->readDatagram(singleData.data(), singleData.size());
        if(singleData[0]==0x03 && singleData[1]==0x82)
        {
           qDebug() << "report current server state............";
           //上报电台当前工作状态
           continue;
        }
        else if(singleData[0]==0x03 && singleData[1]==0xf4)
        {
            qDebug() << "report current stream control state.............";
            //流控状态上报
            continue;
        }
        else if(singleData[0]==0x55 && singleData[1]==0x45)
        {
            qDebug() << "receive audio data...............................";
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

void BattleCall::onServerError(QAbstractSocket::SocketError error)
{
    qDebug() << "server udp server error:" << error;
    m_bindFlag = false;
}

void BattleCall::timerEvent(QTimerEvent *event)
{
//    if(!m_bindFlag)
//        transportInit();

//    if(m_bindFlag == false)
//    {
//        QNetworkInterface interfaces = QNetworkInterface::interfaceFromName("uhf0");
//        qDebug() << "network interface uhf0 valid:" << interfaces.isValid();
//        qDebug() << "bind flag false";
//    }
    QObject::timerEvent(event);
}

BattleCallInterface *BattleCallInterface::instance = NULL;

BattleCallInterface *BattleCallInterface::getInstance()
{
    if(instance == NULL)
        instance = new BattleCallInterface();

    return instance;
}

BattleCallInterface::BattleCallInterface(QObject *parent) :
    QObject(parent)
{
    BattleCall::getInstance();
}

int BattleCallInterface::onCallBattle(const QString &addr)
{
//    if(addr.isEmpty())
//        return -1;

//    qDebug() << "BattleCallInterface::onCallBattle" << addr;
//    int ret = 0;
//    if(!m_addrCallMap.contains(addr))
//    {
//        qDebug() << "BattleCallInterface::onCallBattle" << addr;
//        qDebug() << "BattleCallInterface::onCallBattle" << m_port;
//        BattleCall* call = new BattleCall();
//        ret = call->call(addr, m_port);

//        m_addrCallMap.insert(addr, call);
//    }
//    else
//    {
//        qDebug() << "BattleCallInterface::onCallBattle already exists: " << addr;
//    }
    BattleCall::getInstance()->call(addr, 8086);
    int ret = 1;
    return ret;
}

int BattleCallInterface::onHangupBattle(const QString &addr)
{
//    if(addr.isEmpty())
//        return -1;

//    int ret = 0;
//    if(m_addrCallMap.contains(addr))
//    {
//        BattleCall* call = m_addrCallMap.value(addr);
//        ret = call->hangup();
//    }
    BattleCall::getInstance()->hangup();
    int ret = 1;
    return ret;
}

int BattleCallInterface::onSetBattlePttState(const QString &addr, int pttState)
{
    if(addr.isEmpty())
        return -1;
    if(pttState == 0 | pttState == 1 || pttState == 2)
        return -1;

    int ret;
    if(m_addrCallMap.contains(addr))
    {
        BattleCall* call = m_addrCallMap.value(addr);
        ret = call->setPttState(pttState);
    }
    return ret;
}

int BattleCallInterface::onGetBattlePttState(const QString &addr)
{
    if(addr.isEmpty())
        return -1;

    int ret = 0;
    if(m_addrCallMap.contains(addr))
    {
        BattleCall* call = m_addrCallMap.value(addr);
        ret = call->getPttState();
    }
    return ret;
}

int BattleCallInterface::onSetPort(int port)
{
    m_port = port;
}



