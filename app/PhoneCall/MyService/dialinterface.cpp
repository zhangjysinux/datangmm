#include "dialinterface.h"
#include <QFile>
#include <QDBusMetaType>
#include <QDebug>

using namespace QT_DTT;
DialInterface *DialInterface::m_instance = NULL;
DialInterface::DialInterface(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType<MiniDialData>();
    qDBusRegisterMetaType<HandlerManager>();
    qDBusRegisterMetaType<HandlerManagerList>();
    qDBusRegisterMetaType<HandlerData>();
    qDBusRegisterMetaType<BluetoothTransData>();
    m_contacterAdaptor = ContacterInterfaceAdaptor::getInstance();
    m_buluTooth = BluetoothBackgroundTransferInterface::getInstance();
    m_videoHandler.handlerId = "";
    if (HOSTMACHINE){
        m_dialManager = QT_DTT::DialManager::instance();
        connect(m_dialManager,SIGNAL(callHandlerIdChanged(QString,QString)),this,SLOT(onCallHandlerIdChanged(QString,QString)));
        connect(m_dialManager,SIGNAL(signalVideoCallHandlerIdChanged(QString,QString)),this,SLOT(onVideoCallHandlerIdChanged(QString,QString)));
        connect(m_dialManager,SIGNAL(statusChanged(QString,int)),this,SLOT(onStatusChanged(QString,int)));
        connect(m_dialManager,SIGNAL(audioModeChanged(int)),this,SLOT(onAudioModeChanged(int)));
        connect(m_dialManager,SIGNAL(microphoneMuteChanged(int)),this,SLOT(onMicrophoneMuteChanged(int)));
        connect(m_dialManager,SIGNAL(dialError(int)),this,SIGNAL(dialError(int)));
        connect(m_dialManager,SIGNAL(confCallFinished(int,QString)),this,SLOT(onConfCallFinished(int,QString)));
        connect(m_dialManager,SIGNAL(multipartyChanged(QString,bool)),this,SLOT(onMultipartyChanged(QString,bool)));
        connect(m_dialManager,SIGNAL(signalHandlerEnd(HandlerData)),this,SIGNAL(signalHandlerEnd(HandlerData)));
        connect(m_dialManager,SIGNAL(signalMettingEnd(HandlerData)),this,SIGNAL(signalMettingEnd(HandlerData)));
        connect(m_dialManager,SIGNAL(signalMettingHandlerDown(QString,QString)),this,SIGNAL(signalMettingHandlerDown(QString,QString)));
        connect(m_dialManager,SIGNAL(signalVoipFrameData()),this,SIGNAL(signalVoipFrameData()));
        connect(m_dialManager,SIGNAL(signalV4l2FrameData()),this,SIGNAL(signalV4l2FrameData()));
        connect(m_dialManager,SIGNAL(signalVidChanged(bool)),this,SIGNAL(signalVidChanged(bool)));
    }
    m_start = false;
    m_isMini = false;
    connect(m_buluTooth,SIGNAL(signalMessageReceived(BluetoothTransData)),SLOT(onHandlerReceived(BluetoothTransData)));

//    m_voip = new VoipServiceAdaptor();
//    m_voip->initialize();
//    connect(m_voip, SIGNAL(signalIncomingCall(int,QString)), this, SLOT(onVideoCallInComming(int,QString)));
}

DialInterface *DialInterface::getInstance()
{
    if(m_instance == NULL)
        m_instance = new DialInterface();

    return m_instance;
}

void DialInterface::onMyDebug(QString mess)
{
    qDebug()<<"myDebug_________________"<<mess;
}

void DialInterface::onCallHandlerIdChanged(QString dialingNumber, QString handlerId)
{
    m_mutex.lock();
    CTS_PRINT << "onCallHandlerIdChanged in service  " << dialingNumber << handlerId;
    HandlerManager temp;
    QT_DTT::DialHandler *handler =  m_dialManager->getHandlerById(handlerId);
    if (handler == NULL){
        CTS_PRINT << "err getHandlerById return NULL";
        m_mutex.unlock();
        return;
    }
    temp.dialNumber = handler->dialNumber();
    temp.handlerId = handler->handlerId();
    temp.isIncoming = handler->isIncoming();
    temp.state = handler->status();
    temp.isMultiparty = handler->isMultiparty();
    if (temp.isMultiparty && temp.state == 5)
        temp.state = 1;
    temp.network = handler->network();
    temp.isMuted = m_dialManager->isMicrophoneMuted(temp.network);
    if (m_dialManager->audioMode(temp.network) == "ihf")
        temp.isHandsfree = true;    ///@return "ihf"表示免提， "earpiece"表示听筒
    else
        temp.isHandsfree = false;
    temp.isRecord = false;
    temp.isAudio = false;
    for (int i = 0, iend = m_handlerList.count(); i < iend; i++)
    {
        if (m_handlerList.at(i).handlerId == handlerId){
            m_handlerList.replace(i, temp);
            emit signalHandlerListChanged(HANDLER_UPDATA,m_handlerList.at(i));
            sendHandlerToBlueTooth(HANDLER_UPDATA , temp);
            sendhandlerToMiniPhoneCall(HANDLER_UPDATA , temp);
            m_mutex.unlock();
            return;
        }
    }

    m_handlerList.append(temp);
    //抽象结构体并发送到蓝牙和界面
    //新电话启动界面

    if (m_handlerList.count() == 1 && temp.isIncoming == true && temp.isAudio == false) {
        CTS_PRINT << "emit signalStartApp" << dialingNumber << handlerId;
        emit signalStartApp(dialingNumber, handlerId);
    }
    else if (m_handlerList.count() == 1 && temp.isIncoming == false && temp.isAudio == false)
    {
        CTS_PRINT << "emit signalStartMake" << temp.network << temp.dialNumber;
        emit signalStartMake(temp.network, temp.dialNumber);
    }
//    else if (m_handlerList.count() == 1 && temp.isIncoming == true && temp.isAudio == true)
//    {
//        emit signalVideoCallInComming(temp.handlerId, temp.dialNumber);
//    }
//    else if (m_handlerList.count() == 1 && temp.isIncoming == false && temp.isAudio == true)
//    {
//        //emit signalStartVideo(temp.network, temp.dialNumber);
//    }

    startManager = temp;
    CTS_PRINT << "emit signalHandlerListChanged" << temp.network << temp.dialNumber;
    emit signalHandlerListChanged(HANDLER_ADD,temp);
    sendHandlerToBlueTooth(HANDLER_ADD , temp);
    sendhandlerToMiniPhoneCall(HANDLER_ADD , temp);
    m_mutex.unlock();

}

void DialInterface::onVideoCallHandlerIdChanged(QString dialingNumber, QString handlerId)
{
    if (m_videoHandler.handlerId == "")
    {
        QT_DTT::DialHandler *handler =  m_dialManager->getHandlerById(handlerId);
        m_videoHandler.dialNumber = handler->dialNumber();
        m_videoHandler.handlerId = handler->handlerId();
        m_videoHandler.isIncoming = handler->isIncoming();
        m_videoHandler.state = handler->status();
        m_videoHandler.isMultiparty = handler->isMultiparty();
        m_videoHandler.network = handler->network();
        m_videoHandler.isMuted = m_dialManager->isMicrophoneMuted(m_videoHandler.network);
        if (m_dialManager->audioMode(m_videoHandler.network) == "ihf")
            m_videoHandler.isHandsfree = true;    ///@return "ihf"表示免提， "earpiece"表示听筒
        else
            m_videoHandler.isHandsfree = false;
        m_videoHandler.isRecord = false;
        m_videoHandler.isAudio = true;
        m_videoHandler.startTime = QDateTime::currentDateTime();
        if (m_videoHandler.isIncoming == true)
            emit signalVideoCallInComming(m_videoHandler.handlerId, m_videoHandler.dialNumber);
        emit signalVideoHandlerChanged(m_videoHandler);
    }
}

void DialInterface::onStatusChanged(QString handlerId, int status)
{
    //挂断信号，删除对象,
    //其它信号更新对象
    CTS_PRINT << handlerId << status;
    if (handlerId == m_videoHandler.handlerId)
    {
        CTS_PRINT << "onVideoHandlerChanged" << handlerId << status;

        m_videoHandler.state = status;
        emit signalVideoHandlerChanged(m_videoHandler);
        if (status == STATUS_DISCONNECTED || status == STATUS_NULL)
        {
            m_videoHandler.handlerId = "";
        }
        return;
    }

    if (status == STATUS_DISCONNECTED || status == STATUS_NULL)
    {
        for (int i = 0, iend = m_handlerList.count(); i < iend; i++)
        {
            if (m_handlerList.at(i).handlerId == handlerId){

                emit signalHandlerListChanged(HANDLER_REMOVE,m_handlerList.at(i));
                sendHandlerToBlueTooth(HANDLER_REMOVE , m_handlerList.at(i));
                sendhandlerToMiniPhoneCall(HANDLER_REMOVE , m_handlerList.at(i));
                m_handlerList.removeAt(i);
                break;
            }
        }
    }
    else
    {
        for (int i = 0, iend = m_handlerList.count(); i < iend; i++)
        {
            if (m_handlerList.at(i).handlerId == handlerId){
                CTS_PRINT << "onStatusChanged in service "<< handlerId << status << m_handlerList.count();
                HandlerManager temp = m_handlerList.at(i);
                temp.state = status;
                if (status == 1 && m_handlerList.at(i).state == 3)
                    temp.startTime = QDateTime::currentDateTime();
                m_handlerList.replace(i, temp);
                emit signalHandlerListChanged(HANDLER_UPDATA,m_handlerList.at(i));
                sendHandlerToBlueTooth(HANDLER_UPDATA,m_handlerList.at(i));
                sendhandlerToMiniPhoneCall(HANDLER_UPDATA , m_handlerList.at(i));
                break;
            }
        }
    }

    CTS_PRINT << "onStatusChanged end";
}

void DialInterface::onDial(const int &net, QString phoneNum)
{
    //这里拿到userid 需要自己去查数据
    CTS_PRINT << " onDial in phonecall service";
    QString userId;
    Contacter myContacter;
    UserInfo userInfo;

    if (net == 0 )
    {
        myContacter = m_contacterAdaptor->getContacter(phoneNum);
        for (int i = 0;i < myContacter.numbers.count();i++){
            userInfo.addr = myContacter.numbers[i].number;
            switch (myContacter.numbers[i].netType) {
            case LTE_NETWORK:
                userInfo.lte_ip = myContacter.numbers[i].number;
                break;
            case AD_HOC_NETWORK:
                userInfo.ad_hoc_ip = myContacter.numbers[i].number;
                break;
            case SATELLITE_NETWORK:
                userInfo.satellite_phone_number = myContacter.numbers[i].number;
                break;
            case DIGITAL_WALKIE_TALKIE_NETWORK:
                userInfo.digital_number = myContacter.numbers[i].number;
                break;
            case BATTLE_NETWORK:
                userInfo.battle_ip = myContacter.numbers[i].number;
                break;
            case WIFI_NETWORK:
                userInfo.wifi_ip = myContacter.numbers[i].number;
                break;
            default:
                break;
            }
        }
        userInfo.network = UNKNOW_NETWORK;
    }
    else {
        userInfo.addr = phoneNum;
        switch (net) {
        case AD_HOC_NETWORK:
            userInfo.ad_hoc_ip = phoneNum;
            userInfo.network = AD_HOC_NETWORK;
            break;
        case LTE_NETWORK:
            userInfo.lte_ip = phoneNum;
            userInfo.network = LTE_NETWORK;
            break;
        case SATELLITE_NETWORK :
            userInfo.satellite_phone_number = phoneNum;
            userInfo.network = SATELLITE_NETWORK;
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            userInfo.digital_number = phoneNum;
            userInfo.network = DIGITAL_WALKIE_TALKIE_NETWORK;
            break;
        case BATTLE_NETWORK:
            userInfo.battle_ip = phoneNum;
            userInfo.network = BATTLE_NETWORK;
            break;
        case WIFI_NETWORK:
            userInfo.wifi_ip = phoneNum;
            userInfo.network = WIFI_NETWORK;
        case UNKNOW_NETWORK:
            break;
        default:
            break;
        }
        userId = m_contacterAdaptor->searchContacterNumber(3,phoneNum);
        if (!userId.isEmpty()){
            myContacter = m_contacterAdaptor->getContacter(userId);
            for (int i = 0;i < myContacter.numbers.count();i++){
                switch (myContacter.numbers[i].netType) {
                case LTE_NETWORK:
                    userInfo.lte_ip = myContacter.numbers[i].number;
                    break;
                case AD_HOC_NETWORK:
                    userInfo.ad_hoc_ip = myContacter.numbers[i].number;
                    break;
                case SATELLITE_NETWORK:
                    userInfo.satellite_phone_number = myContacter.numbers[i].number;
                    break;
                case DIGITAL_WALKIE_TALKIE_NETWORK:
                    userInfo.digital_number = myContacter.numbers[i].number;
                    break;
                case BATTLE_NETWORK:
                    userInfo.battle_ip = myContacter.numbers[i].number;
                    break;
                case WIFI_NETWORK:
                    userInfo.wifi_ip = myContacter.numbers[i].number;
                    break;
                default:
                    break;
                }
            }
        }
    }

    userInfo.userId = 1;
    //userInfo.network = UNKNOW_NETWORK;
    //userInfo.addr = phoneNum;
    userInfo.appId = "appId";
    userInfo.userName = "test";
    if (HOSTMACHINE){
        m_dialManager->dial(userInfo);
    }
    else {
        CmdData cmdData;
        cmdData.cmd = "dial";
        cmdData.info = userInfo;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE,cmdData);
    }
    CTS_PRINT << "---onDial in service~~~~~~~"<<"network"<<userInfo.network<<"addr"<<userInfo.addr<<"ad_hoc"<<userInfo.ad_hoc_ip
           <<"lte_ip"<<userInfo.lte_ip<<"satellite_phone_number"<<userInfo.satellite_phone_number<<userInfo.wifi_ip;
}

int DialInterface::onMakeVideo(int net, QString phoneNum)
{
    CTS_PRINT << "onMakeVideo in service " << net;
    QString userId;
    Contacter myContacter;
    UserInfo userInfo;
    int ret = -1;

    userInfo.addr = phoneNum;
    switch (net) {
    case AD_HOC_NETWORK:
        userInfo.ad_hoc_ip = phoneNum;
        userInfo.network = AD_HOC_NETWORK;
        break;
    case LTE_NETWORK:
        userInfo.lte_ip = phoneNum;
        userInfo.network = LTE_NETWORK;
        break;
    case SATELLITE_NETWORK :
        userInfo.satellite_phone_number = phoneNum;
        userInfo.network = SATELLITE_NETWORK;
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        userInfo.digital_number = phoneNum;
        userInfo.network = DIGITAL_WALKIE_TALKIE_NETWORK;
        break;
    case BATTLE_NETWORK:
        userInfo.battle_ip = phoneNum;
        userInfo.network = BATTLE_NETWORK;
        break;
    case WIFI_NETWORK:
        userInfo.wifi_ip = phoneNum;
        userInfo.network = WIFI_NETWORK;
    case UNKNOW_NETWORK:
        break;
    default:
        break;
    }
    userId = m_contacterAdaptor->searchContacterNumber(3,phoneNum);
    if (!userId.isEmpty()){
        myContacter = m_contacterAdaptor->getContacter(userId);
        for (int i = 0;i < myContacter.numbers.count();i++){
            switch (myContacter.numbers[i].netType) {
            case LTE_NETWORK:
                userInfo.lte_ip = myContacter.numbers[i].number;
                break;
            case AD_HOC_NETWORK:
                userInfo.ad_hoc_ip = myContacter.numbers[i].number;
                break;
            case SATELLITE_NETWORK:
                userInfo.satellite_phone_number = myContacter.numbers[i].number;
                break;
            case DIGITAL_WALKIE_TALKIE_NETWORK:
                userInfo.digital_number = myContacter.numbers[i].number;
                break;
            case BATTLE_NETWORK:
                userInfo.battle_ip = myContacter.numbers[i].number;
                break;
            case WIFI_NETWORK:
                userInfo.wifi_ip = myContacter.numbers[i].number;
                break;
            default:
                break;
            }
        }
    }

    userInfo.userId = 1;
    userInfo.appId = "appId";
    userInfo.userName = "test";
    if (HOSTMACHINE){
        ret = m_dialManager->onMakeVideo(userInfo);
    }
    return ret;
}

void DialInterface::onHangup(const QString &handId)
{
    qDebug()<<"onHangup in service by"<<handId;
    if (handId.isEmpty() || handId.isNull() || handId == "")
        return;
    if (HOSTMACHINE)
    {
        DialHandler *temp = m_dialManager->getHandlerById(handId);
        if (temp == NULL)
            return;
        temp->hangup();
    }
    else
    {
        CmdData cmdData;
        cmdData.cmd = "hangup";
        cmdData.handlerId = handId;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }
    qDebug()<<"onHangup in service end";

}

void DialInterface::onAnswer(const QString &handId)
{
    qDebug()<<"onAnswer in service by"<<handId;
    if (handId.isEmpty() || handId.isNull() || handId == "")
        return;
    if (HOSTMACHINE)
    {
        DialHandler *temp = m_dialManager->getHandlerById(handId);
        if (temp == NULL)
            return;
        temp->answer();

        for (int i = 0;i < m_handlerList.count(); i++)
        {
            if (handId == m_handlerList.at(i).handlerId){
                HandlerManager manager = m_handlerList.at(i);
                manager.startTime = QDateTime::currentDateTime();
                m_handlerList.replace(i,manager);
            }

        }
    }
    else
    {
        CmdData cmdData;
        cmdData.cmd = "answer";
        cmdData.handlerId = handId;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }

}

void DialInterface::onHold(const QString &handId, const bool &on)
{
    qDebug()<<"onHold in service by"<<handId<<on;
    if (handId.isEmpty() || handId.isNull() || handId == "")
        return;
    if (HOSTMACHINE)
    {
        DialHandler *temp = m_dialManager->getHandlerById(handId);
        if (temp == NULL)
            return;
        temp->hold(on);
    }
    else
    {
        CmdData cmdData;
        cmdData.cmd = "hold";
        cmdData.handlerId = handId;
        cmdData.enable = on;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }
}

void DialInterface::onConfCallFinished(int network, QString message)
{
    //    for (int i = 0; i< m_handlerList.count(); i++)
    //    {
    //        if (m_handlerList.at(i).network == network)
    //        {
    //            HandlerManager temp;
    //            QT_DTT::DialHandler *handler =  m_dialManager->getHandlerById(m_handlerList.at(i).handlerId);
    //            if (handler == NULL){
    //                qDebug()<< "err getHandlerById return NULL";
    //                return;
    //            }
    //            temp.dialNumber = handler->dialNumber();
    //            temp.handlerId = handler->handlerId();
    //            temp.isIncoming = handler->isIncoming();
    //            temp.isMultiparty = true;//handler->isMultiparty();
    //            temp.network = handler->network();
    //            temp.state = handler->status();
    //            temp.isMuted = m_dialManager->isMicrophoneMuted(temp.network);
    //            if (m_dialManager->audioMode(temp.network) == "ihf")
    //                temp.isHandsfree = true;    ///@return "ihf"表示免提， "earpiece"表示听筒
    //            else
    //                temp.isHandsfree = false;
    //            temp.isRecord = false;
    //            emit signalHandlerListChanged(HANDLER_UPDATA, temp);
    //        }
    //    }
}

void DialInterface::onSetAudioMode(int network, const QString &mode)
{
    qDebug()<<"onSetAudioMode "<<network<<mode;
    if (HOSTMACHINE)
        m_dialManager->setAudioMode(network, mode);
    else
    {
        CmdData cmdData;
        cmdData.cmd = "setAudioMode";
        cmdData.arg = mode;
        cmdData.netWork = network;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }
}

void DialInterface::onSetMuteMicrophone(int network, bool on)
{
    qDebug()<<"onSetMuteMicrophone"<<network<<on;
    if (HOSTMACHINE)
        m_dialManager->setMuteMicrophone(network, on);
    else
    {
        CmdData cmdData;
        cmdData.cmd = "setMuteMicrophone";
        cmdData.enable = on;
        cmdData.netWork = network;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }
}

void DialInterface::onCreateConfCall(int network)
{
    if (HOSTMACHINE)
        m_dialManager->createConfCall(network);
    else
    {
        CmdData cmdData;
        cmdData.cmd = "createConfCall";
        cmdData.netWork = network;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }
}

QString DialInterface::onGetAudioMode(int net)
{
    if (HOSTMACHINE)
        return m_dialManager->audioMode(net);
    else{
        qDebug()<<"onGetAudioMode  "<< __LINE__ << m_dialManagerData.audioMode;
        return m_dialManagerData.audioMode;
    }
}

bool DialInterface::onIsMicrophoneMuted(int net)
{
    if (HOSTMACHINE)
        return m_dialManager->isMicrophoneMuted(net);
    else
        return m_dialManagerData.isMicrophoneMuted;
}

void DialInterface::onMicrophoneMuteChanged(int network)
{
    bool on;
    on = m_dialManagerData.isMicrophoneMuted = m_dialManager->isMicrophoneMuted(network);
    emit signalMicrophoneMuteChanged(on);
    for (int i = 0,iend = m_handlerList.count(); i < iend; i++)
    {
        //onCallHandlerIdChanged(m_handlerList.at(i).dialNumber, m_handlerList.at(i).handlerId);
        m_dialManagerData.audioMode = m_dialManager->audioMode(network);
        m_dialManagerData.isMicrophoneMuted = m_dialManager->isMicrophoneMuted(network);
        sendManagerToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , m_dialManagerData);
    }
    return;

//    qDebug()<<"onMicrophoneMuteChanged"<<m_handlerList.count();
//    for (int i = 0,iend = m_handlerList.count(); i < iend; i++)
//    {
//        qDebug()<<"onMicrophoneMuteChanged end";
//        onCallHandlerIdChanged(m_handlerList.at(i).dialNumber, m_handlerList.at(i).handlerId);
//        m_dialManagerData.isMicrophoneMuted = m_dialManager->isMicrophoneMuted(network);
//        m_dialManagerData.audioMode = m_dialManager->audioMode(network);
//        sendManagerToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , m_dialManagerData);
//    }
//    qDebug()<<"onMicrophoneMuteChanged end";
//    return;


}

void DialInterface::onAppAlreadyStart()
{
    if (HOSTMACHINE){
        if (m_handlerList.count() > 0 ){//&& startManager.isIncoming == true){
            emit signalHandlerListChanged(HANDLER_UPDATA,startManager);
            sendHandlerToBlueTooth(HANDLER_UPDATA , startManager);
        }
    }
    else {
        for (int i = 0 ;i < m_handlerList.count() ;i++)
        {
            emit signalHandlerListChanged(HANDLER_UPDATA,m_handlerList.at(i));
        }
    }
    if (m_videoHandler.handlerId != "")
        emit signalVideoHandlerChanged(m_videoHandler);
}

void DialInterface::onHangupMultipartyCalls(int network)
{
    qDebug()<<"onHangupMultipartyCalls"<<network;
    if (HOSTMACHINE)
        m_dialManager->hangupMultipartyCalls(network);
    else
    {
        CmdData cmdData;
        cmdData.cmd = "hangupMultipartyCalls";
        cmdData.netWork = network;
        sendCmdToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , cmdData);
    }
}

void DialInterface::onMultipartyChanged(QString handlerId, bool isMultiparty)
{
    qDebug()<<"onMultipartyChanged in service"<<handlerId<<isMultiparty;
    onFileDebug("onMultipartyChanged");
    for (int i = 0;i < m_handlerList.count(); i++)
    {
        if (handlerId == m_handlerList.at(i).handlerId){
            onCallHandlerIdChanged(m_handlerList.at(i).dialNumber,handlerId);
            HandlerManager handler = m_handlerList.at(i);
            handler.isMultiparty = isMultiparty;
            sendHandlerToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , handler);

            break;
        }
    }
    emit signalMultipartyChanged(handlerId, isMultiparty);
}

void DialInterface::onHandlerReceived(const BluetoothTransData &message)
{
    qDebug()<<"onHandlerReceived in service ";
    QByteArray transData = message.transData;
    QDataStream stream(&transData, QIODevice::ReadOnly);
    if(message.type == -1)
        qDebug() << "sync message received from server";
    HandlerManager handler;
    DialManagerData dialManagerData;
    CmdData cmdData;
    switch (message.type) {
    case BLUETOOTHTRANSDATATYPE_DIALHANDLERDATA:
        stream >> handler;
        receiveBuleToothHandlerData(message.handleType, handler);
        break;
    case BLUETOOTHTRANSDATATYPE_DIALHANDLERCMD:
        stream >> cmdData;
        receiveBuleToothHandlerCmd(cmdData);
        break;
    case BLUETOOTHTRANSDATATYPE_DIALMANAGERDATA:
        stream >> dialManagerData;
        receiveBuleToothManagerData(message.handleType , dialManagerData);
        break;
    default:
        break;
    }
}

void DialInterface::onSetStart(bool on)
{
    m_start = on;
}

void DialInterface::onRecord(int network, bool on)
{
    m_dialManager->record(network , on);
    for (int i = 0,iend = m_handlerList.count(); i < iend; i++)
    {
        m_dialManagerData.audioMode = m_dialManager->audioMode(network);
        m_dialManagerData.isMicrophoneMuted = m_dialManager->isMicrophoneMuted(network);
        m_dialManagerData.record = on;
        sendManagerToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , m_dialManagerData);
    }
}

int DialInterface::onSetIsMini(bool on)
{
    qDebug()<<"onSetIsMini"<<on;
    m_isMini = on;
    return 1;
}

bool DialInterface::onGetIsMini()
{
    return m_isMini;
}

int DialInterface::onShowFullScreen()
{
    m_isMini = false;
    emit signalShowFullScreen();
}

void DialInterface::onVideoCallInComming(QString callId, QString remoteIpAddr)
{
    qDebug()<<"onVideoCallInComming" << callId << remoteIpAddr;
    emit signalVideoCallInComming(callId, remoteIpAddr);
}

void DialInterface::onFileDebug(QString mess)
{
    QFile f("/home/user/PhoneCall_debug");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Open failed.";
        return ;
    }
    QTextStream txtOutput(&f);
    txtOutput << mess << endl;
    f.close();
}

int DialInterface::onGetActiveMode()
{
    return m_dialManager->getActiveMode();
}

int DialInterface::onSetHoldConference(int network, bool on)
{
    return m_dialManager->setHoldConference(network, on);
}

int DialInterface::setVidEnabledChanged(int network, QString callId, bool on)
{
    return m_dialManager->setVidEnabledChanged(network, callId, on);
}

int DialInterface::setVidInputChanged(int network, QString callId)
{
    return m_dialManager->setVidInputChanged(network, callId);
}

int DialInterface::getGetAudOrVideo(int network, QString callId)
{
    qDebug() << "getGetAudOrVideoxxxxxxxxxx" << network << callId;
    return m_dialManager->getGetAudOrVideo(network, callId);
}

void DialInterface::sendhandlerToMiniPhoneCall(const int &type, const HandlerManager &handlerManager)
{
    if (!m_isMini)
        return;
    MiniDialData mini;
    mini.dialNum = handlerManager.dialNumber;
    mini.handlerId = handlerManager.handlerId;
    mini.isIncoming = handlerManager.isIncoming;
    mini.state = handlerManager.state;
    Contacter user;
    QString userid;
    userid = m_contacterAdaptor->searchContacterNumber(3 , mini.dialNum);
    if (userid.isEmpty())
    {
        mini.name = "";
        mini.photoPath = "";
    }
    else
    {
        user = m_contacterAdaptor->getContacter(userid);
        mini.name = user.surname + user.name;
        mini.photoPath = user.photoPath;
    }
    qDebug()<<"signalSendMiniData";
    emit signalSendMiniData(type , mini);

}

int DialInterface::sendHandlerToBlueTooth(const int &type, const HandlerManager &handlerManager)
{
    qDebug()<<"sendHandlerToBlueTooth in service ";
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_DIALHANDLERDATA;
    data.handleType = type;

    QByteArray ba;
    ba.resize(sizeof(HandlerManager));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << handlerManager;

    data.transData = ba;

    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);

    object.insert("key", "dialHandler");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);
    document.setObject(object);

    return m_buluTooth->onSendMessage(document.toBinaryData());
}

int DialInterface::sendManagerToBlueTooth(const int &type, const DialManagerData &dialManagerData)
{
    qDebug()<<"sendManagerToBlueTooth " << dialManagerData.audioMode << dialManagerData.isMicrophoneMuted;
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_DIALMANAGERDATA;
    data.handleType = type;

    QByteArray ba;
    ba.resize(sizeof(DialManagerData));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << dialManagerData;

    data.transData = ba;
    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);
    object.insert("key", "dialManagerData");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);
    document.setObject(object);
    return m_buluTooth->onSendMessage(document.toBinaryData());
}

int DialInterface::sendCmdToBlueTooth(const int &type, const CmdData &cmdData)
{
    BluetoothTransData data;
    data.type = BLUETOOTHTRANSDATATYPE_DIALHANDLERCMD;
    data.handleType = type;

    QByteArray ba;
    ba.resize(sizeof(DialManagerData));
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << cmdData;

    data.transData = ba;
    QByteArray sendDataArray;
    sendDataArray.resize(sizeof(BluetoothTransData));
    QBuffer sendBuffer(&sendDataArray);
    sendBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&sendBuffer);
    out << data;
    QString sendDataArrayString = QString::fromLatin1(sendDataArray.data(), sendDataArray.count());
    //{key:contacts,count:count,state:ok,msg:””,Array:[]}
    QJsonDocument document;
    QJsonObject object;

    QJsonArray array;
    array.append(sendDataArrayString);
    object.insert("key", "cmdData");
    object.insert("count", "1");
    object.insert("state", "ok");
    object.insert("msg", "");
    object.insert("Array", array);
    document.setObject(object);
    return m_buluTooth->onSendMessage(document.toBinaryData());
}

void DialInterface::receiveBuleToothHandlerData(int type, HandlerManager handler)
{
    startManager = handler;
    bool isFind = false;
    int ret = -1;
    qDebug()<<"emit buletooth signalStartApp"<<"m_handlerList.count() = "<<m_handlerList.count()<<"receive type = " << type;

    for (int i = 0 ; i < m_handlerList.count() ; i++)
    {
        qDebug()<<m_handlerList.at(i).handlerId;
        if (handler.handlerId == m_handlerList.at(i).handlerId)
        {
            m_handlerList.replace(i , handler);
            isFind = true;
            ret = i;
            break;
        }
    }
    if (!isFind)
    {
        m_handlerList.append(handler);
        ret = m_handlerList.count() - 1;
    }
    if (!m_start){
        if (m_handlerList.count() == 1 && handler.isIncoming == true && handler.isAudio == false){
            emit signalStartApp(handler.dialNumber , handler.handlerId);
            m_start = true;
        }
        else if (m_handlerList.count() == 1 && handler.isIncoming == false && handler.isAudio == false){
            emit signalStartMake(handler.network,handler.dialNumber);
            m_start = true;
        }
        else if (m_handlerList.count() == 1 && handler.isIncoming == true && handler.isAudio == true){
            emit signalVideoCallInComming(handler.handlerId, handler.dialNumber);
            m_start = true;
        }
        else if (m_handlerList.count() == 1 && handler.isIncoming == false && handler.isAudio == true){
            emit signalStartVideo(handler.network, handler.dialNumber);
            m_start = true;
        }
    }

    if (type == HANDLER_REMOVE)
    {
        if (ret != -1)
            m_handlerList.removeAt(ret);
        else
            m_handlerList.removeAt(m_handlerList.count() - 1);
    }


    /*
    switch (type) {
    case HANDLER_ADD:
        m_handlerList.append(handler);
        if (m_handlerList.count() == 1 && handler.isIncoming == true){
            emit signalStartApp(handler.dialNumber , handler.handlerId);
        }
        else if (m_handlerList.count() == 1 && handler.isIncoming == false){
            emit signalStartMake(handler.network,handler.dialNumber);
        }
        break;
    case HANDLER_UPDATA:
        for (int i = 0; i < m_handlerList.count(); i++)
        {
            if (m_handlerList.at(i).handlerId == handler.handlerId){
                m_handlerList.replace(i , handler);
                isFind = true;
                break;
            }
        }
        break;
    case HANDLER_REMOVE:
        for (int i = 0; i < m_handlerList.count(); i++)
        {
            if (m_handlerList.at(i).handlerId == handler.handlerId){
                m_handlerList.removeAt(i);
                break;
            }
        }
        break;
    case HANDLER_UNDEFINE:
    default:
        break;
    }
    */
    emit signalHandlerListChanged(type, handler);
}

void DialInterface::receiveBuleToothHandlerCmd(CmdData &cmdData)
{
    QString cmd = cmdData.cmd;
    if (cmd == "dial")
        m_dialManager->dial(cmdData.info);
    else if (cmd == "hangup")
        m_dialManager->getHandlerById(cmdData.handlerId)->hangup();
    else if (cmd == "answer")
        m_dialManager->getHandlerById(cmdData.handlerId)->answer();
    else if (cmd == "hold")
        m_dialManager->getHandlerById(cmdData.handlerId)->hold(cmdData.enable);
    else if (cmd == "setAudioMode")
        m_dialManager->setAudioMode(cmdData.netWork,cmdData.arg);
    else if (cmd == "setMuteMicrophone")
        m_dialManager->setMuteMicrophone(cmdData.netWork,cmdData.enable);
    else if (cmd == "createConfCall")
        m_dialManager->createConfCall(cmdData.netWork);
    else if (cmd == "hangupMultipartyCalls")
        m_dialManager->hangupMultipartyCalls(cmdData.netWork);

}

void DialInterface::receiveBuleToothManagerData(int type, DialManagerData dialManagerData)
{
    m_dialManagerData = dialManagerData;
}

void DialInterface::onAudioModeChanged(int network)
{
    bool on;
    m_dialManagerData.audioMode = m_dialManager->audioMode(network);
    if (m_dialManagerData.audioMode == "ihf")
        on = true;
    else
        on = false;
    emit signalAudioModeChanged(on);

    for (int i = 0,iend = m_handlerList.count(); i < iend; i++)
    {
        //onCallHandlerIdChanged(m_handlerList.at(i).dialNumber, m_handlerList.at(i).handlerId);
        m_dialManagerData.audioMode = m_dialManager->audioMode(network);
        m_dialManagerData.isMicrophoneMuted = m_dialManager->isMicrophoneMuted(network);
        sendManagerToBlueTooth(BLUETOOTHTRANSDATAHANDLETYPE_UPDATE , m_dialManagerData);
    }
    return;
}

QDBusVariant DialInterface::onGetHandlerList()
{
    QDBusVariant dbusValue;
    QVariant listVar = QVariant::fromValue(m_handlerList);
    dbusValue.setVariant(listVar);
    return dbusValue;
}

