#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>
#include "dialinterfaceadaptor.h"
DialInterfaceAdaptor *DialInterfaceAdaptor::m_instance = NULL;
DialInterfaceAdaptor *DialInterfaceAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new DialInterfaceAdaptor();
    return m_instance;
}

void DialInterfaceAdaptor::myDebug(QString mess)
{
    m_interface.call("onMyDebug",mess);
}

QString DialInterfaceAdaptor::getTimeString(QString handler)
{
    QDateTime startTime;
//    for (int i = 0; i < m_handlerList.size(); i++)
//    {
//        if (handler == m_handlerList.at(i).handlerId)
//            startTime = m_handlerList.at(i).startTime;
//    }
//    if (handler == m_videoManager.handlerId)
    startTime = m_videoManager.startTime;
    return timeToString(QDateTime::currentDateTime().toTime_t() - startTime.toTime_t());
}

void DialInterfaceAdaptor::initContacterData(QString phoneNum)
{
    userId = m_contacterAdaptor->searchContacterNumber(3,phoneNum);
    if (userId != "")
        m_contacter = m_contacterAdaptor->getContacter(userId);
    m_remoteIpAddr = phoneNum;
}

QString DialInterfaceAdaptor::getContacterData(QString searchText)
{
    if (userId == "")
        return "";
    if (searchText == "name")
        return m_contacter.surname + m_contacter.name;
    else if (searchText == "photo") {
        if (m_contacter.photoPath != "null")
            return m_contacter.photoPath;
        else
            return "";
    }
    else if (searchText == "title"){
        QString titleName;
        switch(m_contacter.title){
        case BATTLION:
            titleName = "营长";
            break;
        case COMPANY:
            titleName = "连长";
            break;
        case PLATOON:
            titleName = "排长";
            break;
        case SQUAD:
            titleName = "班长";
            break;
        case SOLIDER:
            titleName = "战士";
            break;
        default:
            titleName = "未知";
            break;
        }
        return titleName;
    }

}

int DialInterfaceAdaptor::setVidEnabledChanged(int network, QString callId, bool on)
{
    qDebug() << "setVidEnabledChanged" << network << callId << on;

    int ret = -1;
    QDBusReply<int> reply = m_interface.call("setVidEnabledChanged",m_videoManager.network, m_videoManager.handlerId, on);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}

int DialInterfaceAdaptor::setVidInputChanged(int network, QString callId)
{
    qDebug() << "setVidInputChanged" << network << callId<<m_videoManager.network<<m_videoManager.handlerId;
    int ret = -1;
    QDBusReply<int> reply = m_interface.call("setVidInputChanged",m_videoManager.network, m_videoManager.handlerId);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}

int DialInterfaceAdaptor::getGetAudOrVideo(int network, QString callId)
{
    int ret = -1;
    QDBusReply<int> reply = m_interface.call("getGetAudOrVideo",m_videoManager.network, m_videoManager.handlerId);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}

bool DialInterfaceAdaptor::isVideoExit()
{
    if (m_videoManager.handlerId != "")
        return true;
    return false;
}

bool DialInterfaceAdaptor::isAudioExit()
{
    if (m_handlerList.size() > 0)
        return true;
    return false;
}

bool DialInterfaceAdaptor::isHandlerExist(QString ip)
{
    for (int i = 0; i < m_handlerList.count(); i++)
    {
        if (m_handlerList.at(i).dialNumber == ip)
            return true;
    }
    return false;
}

bool DialInterfaceAdaptor::isHostMachine()
{
    if (HOSTMACHINE)
        return true;
    else
        return false;

}

QString DialInterfaceAdaptor::getVideoValue(QString key)
{
    if (m_videoManager.handlerId == "")
        return "";
    if (key == "handlerId")
        return m_videoManager.handlerId;
    else if (key == "phoneNum")
        return m_videoManager.dialNumber;
}

int DialInterfaceAdaptor::getVideoNetWork()
{
    if (m_videoManager.handlerId == "")
        return 0;
    return m_videoManager.network;
}

void DialInterfaceAdaptor::setVideoPage(bool on)
{
    videoPage = on;
}

void DialInterfaceAdaptor::setAudioPage(bool on)
{
    audioPage = on;
}

bool DialInterfaceAdaptor::getVideoPage()
{
    return videoPage;
}

bool DialInterfaceAdaptor::getAudioPage()
{
    return audioPage;
}

void DialInterfaceAdaptor::clearScreenShot()
{
    m_screenShot = NULL;
    m_screenShotV4l2 = NULL;
}

void DialInterfaceAdaptor::initTalking()
{
    emit signalHandlerListChanged();
}

int DialInterfaceAdaptor::makeVideo(int net, QString phoneNum)
{
    int ret = -1;
    if (phoneNum == "")
        return -1;
    for (int i = 0;i < m_handlerList.count(); i++)
    {
        if (m_handlerList.at(i).dialNumber == phoneNum)
            return -1;
    }
    dialTimer->start(24000);
    qDebug()<<"-------------------makevideo michael --------------------";
    QDBusReply<int> reply = m_interface.call("onMakeVideo", net, phoneNum);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}

int DialInterfaceAdaptor::dial(int net, QString phoneNum)
{
    if (phoneNum == "")
        return -1;
    for (int i = 0;i < m_handlerList.count(); i++)
    {
        if (m_handlerList.at(i).dialNumber == phoneNum)
            return -1;
    }
    dialTimer->start(24000);
    qDebug() << "dial in app" << net << phoneNum;
    m_interface.call("onDial", net, phoneNum);
    return 1;
}

void DialInterfaceAdaptor::dials(QString netstring, QString phoneNum)
{
    int net = 0;
    if (netstring == "LTE")
        net = LTE_NETWORK;
    else if (netstring == "自组网")
        net = AD_HOC_NETWORK;
    else if (netstring == "天通网")
        net = SATELLITE_NETWORK;
    else if (netstring == "WIFI")
        net = WIFI_NETWORK;
    else
        net = UNKNOW_NETWORK;

    if (phoneNum == "")
        return;
    if (isHandlerExist(phoneNum))
        return;
    dialTimer->start(24000);

    /*
    for (int i = 0;i < m_handlerList.count(); i++)
    {
//        if (m_handlerList.at(i).state == 1  && m_qmlState == 1 && net != m_handlerList.at(i).network)
//            hold(m_handlerList.at(i).handlerId, true);
//        else if (m_qmlState != 1 && m_handlerList.at(i).state == 1)
//            hold(m_handlerList.at(i).handlerId, true);

        if (m_qmlState == 1)
        {
            if (net == AD_HOC_NETWORK && m_handlerList.at(i).network == 1)
                break;
            else {
                if (m_handlerList.at(i).state == 1)
                    hold(m_handlerList.at(i).handlerId, true);
            }
        }
        else if (m_qmlState == 3)
        {
            if (net == AD_HOC_NETWORK && m_handlerList.at(i).network == 1)
                break;
            else if (net != AD_HOC_NETWORK || (net == AD_HOC_NETWORK && m_handlerList.at(i).network != 1))
            {
                if (m_handlerList.at(i).state == 1)
                    hold(m_handlerList.at(i).handlerId, true);
            }
        }
    }
    */
    m_interface.call("onDial", net, phoneNum);
}

void DialInterfaceAdaptor::hangup(const QString &handId)
{
    if (handId.isEmpty() || handId == "null" || handId == ""){
        m_handlerList.clear();
        m_meetingList.clear();
        emit signalQuitApp();
        return;
    }
    m_interface.call("onHangup", handId);
}

void DialInterfaceAdaptor::answer(const QString &handId)
{
    m_interface.call("onAnswer", handId);
}

void DialInterfaceAdaptor::hold(const QString &handId, const bool &on)
{
    m_interface.call("onHold", handId, on);
}

int DialInterfaceAdaptor::getQmlState()
{
    int activeMode = getActiveMode();
    m_qmlState = activeMode;
    return activeMode;
    //    myDebug(QString("qml ~~~~~~~~~~qmlstate == %1").arg(m_qmlState));
    //    return m_qmlState;
}

void DialInterfaceAdaptor::setAudioMode(int network, const QString &mode)
{
    m_interface.call("onSetAudioMode", network, mode);
}

void DialInterfaceAdaptor::setMuteMicrophone(int network, bool on)
{
    m_interface.call("onSetMuteMicrophone", network, on);
}

void DialInterfaceAdaptor::createConfCall(int network)
{
    m_interface.call("onCreateConfCall", network);
}

void DialInterfaceAdaptor::setIsTalking(bool on)
{
    isTalking = on;
    m_start = on;
    m_interface.call("onSetStart", on);
}

bool DialInterfaceAdaptor::getIsTalking()
{
    return isTalking;
}

int DialInterfaceAdaptor::getHandlerCount()
{
    return m_handlerList.count();
}

int DialInterfaceAdaptor::getMettingCount()
{
    return m_meetingList.count();
}

//QString handlerId;               ///< user ID
//QString dialNumber;              ///< 所拨打的电话号码
//int network;                     ///< 网络类型  取自 common.h::NetworkType
//int state;                       ///< 通话业务的状态  取自 common.h::DialTransStatus
//bool isIncoming;                 ///< ture : 当前通话是呼入电话; false : 当前是呼出电话
//bool isMultiparty;               ///< ture : 当前通话是电话会议; false : 当前通话不是电话会议
//bool isMuted;                    ///< true : 当前是静音 ;       false : 当前不是静音
//bool isHandsfree ;               ///< true : 当前是免提 ;       false : 当前不是免提
//bool isRecord ;                  ///< true : 当前是录音 ;       false : 当前不是录音

QString DialInterfaceAdaptor::getHandlerValue(int index, int type)
{
    QString rets = "";
    switch (type) {
    case 1:
        rets = m_handlerList.at(index).handlerId;
        break;
    case 2:
        rets = m_handlerList.at(index).dialNumber;
        break;
    case 3:
        rets = QString("%1").arg(m_handlerList.at(index).network);
        break;
    case 4:
        rets = QString("%1").arg(m_handlerList.at(index).state);
        break;
    case 5:
        rets = QString("%1").arg(m_handlerList.at(index).isIncoming);
        break;
    case 6:
        rets = QString("%1").arg(m_handlerList.at(index).isMultiparty);
        break;
    case 7:
        rets = QString("%1").arg(m_handlerList.at(index).isMuted);
        break;
    case 8:
        rets = QString("%1").arg(m_handlerList.at(index).isHandsfree);
        break;
    case 9:
        rets = QString("%1").arg(m_handlerList.at(index).isRecord);
        break;
    default:
        break;
    }
    return rets;
}

QString DialInterfaceAdaptor::getMettingValue(int index, int type)
{
    QString rets = "";
    switch (type) {
    case 1:
        rets = m_meetingList.at(index).handlerId;
        break;
    case 2:
        rets = m_meetingList.at(index).dialNumber;
        break;
    case 3:
        rets = QString("%1").arg(m_meetingList.at(index).network);
        break;
    case 4:
        rets = QString("%1").arg(m_meetingList.at(index).state);
        break;
    case 5:
        rets = QString("%1").arg(m_meetingList.at(index).isIncoming);
        break;
    case 6:
        rets = QString("%1").arg(m_meetingList.at(index).isMultiparty);
        break;
    case 7:
        rets = QString("%1").arg(m_meetingList.at(index).isMuted);
        break;
    case 8:
        rets = QString("%1").arg(m_meetingList.at(index).isHandsfree);
        break;
    case 9:
        rets = QString("%1").arg(m_meetingList.at(index).isRecord);
        break;
    case 10:
        rets = QString("%1").arg(m_meetingList.at(index).isOnline);
        break;
    default:
        break;
    }
    return rets;
}

QString DialInterfaceAdaptor::getNamebyDialnum(QString dialnum)
{
    QString name;
    QString titleName;
    QString id;
    QString photoPath;
    QString list;
    Contacter contacter;
    //QString id,QString name,QString title,QString dialnum,QString photoPath
    list = m_contacterAdaptor->searchContacterNumber(3,dialnum);
    if (list.isEmpty()){
        emit signalUpdateContacterInfo("","",dialnum,dialnum,"");
        return list;
    }
    contacter = m_contacterAdaptor->getContacter(list);
    name = contacter.surname + contacter.name;
    id = contacter.id;
    photoPath = contacter.photoPath;
    if (photoPath.isEmpty() || photoPath.isNull() || photoPath == "null"){
        photoPath = "";
    }
    switch(contacter.title){
    case BATTLION:
        titleName = "营长";
        break;
    case COMPANY:
        titleName = "连长";
        break;
    case PLATOON:
        titleName = "排长";
        break;
    case SQUAD:
        titleName = "班长";
        break;
    case SOLIDER:
        titleName = "战士";
        break;
    default:
        titleName = "未知";
        break;
    }
    emit signalUpdateContacterInfo(id,name,titleName,dialnum,photoPath);
    return list;
}

QString DialInterfaceAdaptor::getAudioMode(int net)
{
    QString values;
    QDBusReply<QString> reply = m_interface.call("onGetAudioMode" , net);
    if(reply.isValid())
        values = reply.value();
    else
        qDebug() << "onGetAudioMode reply error:" << reply.error();
    return values;
}

bool DialInterfaceAdaptor::getIsMicrophoneMuted(int net)
{
    bool values;
    QDBusReply<bool> reply = m_interface.call("onIsMicrophoneMuted" , net);
    if(reply.isValid())
        values = reply.value();
    else
        qDebug() << "onIsMicrophoneMuted reply error:" << reply.error();
    return values;
}

int DialInterfaceAdaptor::findHandler(QString handlerId)
{
    for (int i = 0, iend = m_handlerList.count(); i < iend; i++ )
    {
        if (handlerId == m_handlerList.at(i).handlerId)
            return i;
    }
    return -1;
}

int DialInterfaceAdaptor::findMettingHandler(QString handlerId)
{
    for (int i = 0, iend = m_meetingList.count(); i < iend; i++ )
    {
        if (handlerId == m_meetingList.at(i).handlerId)
            return i;
    }
    return -1;
}

int DialInterfaceAdaptor::setMettingHandler(QString handlerId)
{
    for (int i = 0, iend = m_handlerList.count(); i < iend; i++ )
    {
        if (handlerId == m_handlerList.at(i).handlerId){
            m_handlerList.removeAt(i);
            break;
        }
    }
    for (int i = 0, iend = m_meetingList.count(); i < iend; i++ )
    {
        if (handlerId == m_meetingList.at(i).handlerId)
            return i;
    }
    return -1;
}

void DialInterfaceAdaptor::getHandlerList()
{
    QDBusReply<QDBusVariant> handlerReply = m_interface.call("onGetHandlerList");
    if(handlerReply.isValid())
    {
        QDBusVariant dbusVariant = handlerReply.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        m_handlerList = qdbus_cast<HandlerManagerList>(argument);
    }
    else
    {
        qDebug() << "onGetContacter reply error:" << handlerReply.error();
    }

}

void DialInterfaceAdaptor::onSignalHandlerListChanged(int type, HandlerManager handler)
{
    //视频挂断信号，视频切换信号
    //判断视频还是语音
    m_mutex.lock();
    if (dialTimer->isActive())
        dialTimer->stop();
    qDebug() << " onSignalHandlerListChanged in app";
    getQmlState();
    if (!timer->isActive())
    {
        timer->start(1000);
    }
    int index;
    index = findHandler(handler.handlerId);
    if (index == -1){
        m_handlerList.append(handler);
        index = m_handlerList.count() - 1;
    }
    switch (type) {
    case HANDLER_ADD:
        emit signalSetHandler(handler.handlerId,handler.dialNumber,handler.network,handler.state,handler.isIncoming,handler.isMultiparty);
        break;
    case HANDLER_UPDATA:
        m_handlerList.replace(index, handler);
        emit signalUpdateHandler(handler.handlerId,handler.dialNumber,handler.network,handler.state,handler.isIncoming,handler.isMultiparty);
        break;
    case HANDLER_REMOVE:
        if (m_handlerList.at(index).handlerId == anotherHandler && m_handlerList.at(index).state == STATUS_INCOMING)
            emit signalCloseAnother(m_handlerList.at(index).handlerId);

        if (m_handlerList.at(index).isOnline)
            m_handlerList.removeAt(index);

        break;
    case HANDLER_UNDEFINE:
        break;
    default:
        break;
    }

    if (m_handlerList.count() == 0 && m_meetingList.count() == 0){
        if (timer->isActive())
            timer->stop();
        if (m_videoManager.handlerId == "")
            emit signalQuitApp();
        else {
            emit signalAudioReturnPage();
        }
        return;
    }
    qDebug() << "XXX" << m_qmlState << handler.state << isTalking << handler.isAudio << handler.isIncoming;
//    if (m_qmlState == 1 && handler.state == 1 && isTalking == false && handler.isAudio == false && handler.isIncoming == true){

    if (m_qmlState == 1 && handler.state == 1 && handler.isAudio == false && handler.isIncoming == true){
        emit signalAlreadyAnswer(handler.handlerId);
    }
    else if (m_qmlState == 1 && handler.state == 1 && isTalking == false && handler.isAudio == true && handler.isIncoming == true)
    {
        //emit signalVideoAlreadyAnswer(handler.handlerId);
    }
    emit signalHandlerListChanged();
    if ((m_handlerList.count() >= 1 || m_meetingList.count() >= 1) && handler.isIncoming == true && (handler.state == STATUS_INCOMING)){
        anotherHandler = handler.handlerId;
        emit signalAnotherIncoming(handler.handlerId);
    }
    m_mutex.unlock();
}

void DialInterfaceAdaptor::onVideoHandlerChanged(HandlerManager handler)
{
    if (dialTimer->isActive())
        dialTimer->stop();
    qDebug() << "onVideoHandlerChanged" << m_videoManager.state << handler.state;
    if (m_videoManager.state == STATUS_INCOMING && handler.state == STATUS_ACTIVE)
    {
        emit signalVideoAlreadyAnswer(handler.handlerId, handler.network);
    }

    m_videoManager = handler;
    if (m_videoManager.state == 1)
        m_videoManager.startTime = QDateTime::currentDateTime();
    timer->start(1000);
    if ((m_videoManager.state == STATUS_DISCONNECTED || m_videoManager.state == STATUS_NULL) && m_handlerList.count() == 0 ){
        m_videoManager.handlerId = "";
        emit signalQuitApp();
    }
    else if ((m_videoManager.state == STATUS_DISCONNECTED || m_videoManager.state == STATUS_NULL) && m_handlerList.count() > 0 ){
        m_videoManager.handlerId = "";
        emit signalVideoReturnPage();
    }
}

void DialInterfaceAdaptor::setScreenShot(ScreenShot *screenShot)
{
    m_screenShot = screenShot;
    QImage images;
    images.load("qrc:/main/images/init_photo.png");
    m_screenShot->setImage(images);
}

void DialInterfaceAdaptor::setScreenShotV4l2(ScreenShot *screenShot)
{
    m_screenShotV4l2 = screenShot;
}

void DialInterfaceAdaptor::onVoipFrameData()
{
    QImage image;
    bool ret = m_pSharedMemory->GetImageFromSharedMemory(image);
    if(ret)
    {
        if (m_screenShot != NULL)
            m_screenShot->setImage(image);
    }
    //emit signalVoipFrameData();
}

void DialInterfaceAdaptor::onV4l2FrameData()
{
    QImage image;
    bool ret = m_pSharedMemory->GetImageFromV4l2SharedMemory(image);
    if(ret)
    {
        if (m_screenShotV4l2 != NULL)
            m_screenShotV4l2->setImage(image);
    }
    //emit signalV4l2FrameData();
}

void DialInterfaceAdaptor::onDialTimerOut()
{
    if (m_handlerList.size() == 0 && m_videoManager.handlerId == ""){
        emit signalQuitApp();
        if (dialTimer->isActive())
            dialTimer->stop();
    }
}
// by michael zheng 2017.4.7
void DialInterfaceAdaptor::onAgainLinkVideo(QString ipAddr)
{
    QDBusInterface interface("com.sinux.voipAdHoc", "/", "com.sinux.DBus.voip");
    interface.call("onHangup",0);
    makeVideo(0x00000001,ipAddr);
}

void DialInterfaceAdaptor::onIsAgainLink(bool flag)
{
    isAGain = flag;
    signalIsAgainLink(isAGain);
}
// end by michael zheng
void DialInterfaceAdaptor::onConfCallFinished(int network, QString message)
{

}

void DialInterfaceAdaptor::onTimerout()
{
    if (m_handlerList.count() == 0 && m_meetingList.count() == 0)
        return;

    for (int i = 0; i < m_handlerList.size(); i++)
    {
        if (m_handlerList.at(i).state == 1 || m_handlerList.at(i).state == 3 || m_handlerList.at(i).state == 4)
        {
            int time = 0;
            time = QDateTime::currentDateTime().toTime_t() - m_handlerList.at(i).startTime.toTime_t();
            emit signalUpdateTimer(m_handlerList.at(i).handlerId, timeToString(time));
        }
        else if (m_handlerList.at(i).state == 2)
        {
            emit signalUpdateTimer(m_handlerList.at(i).handlerId, "保持");
        }
    }

    /*
    if (m_qmlState == 1 || m_qmlState == 2 ){
        for (int i = 0, iend = m_handlerList.count(); i < iend; i++)
        {
            if (m_handlerList.at(i).state == 1 || m_handlerList.at(i).state == 3 || m_handlerList.at(i).state == 4)
            {
                for (int j =0,jend = m_callHistoryList.count(); j < jend ; j++)
                {
                    if (m_handlerList.at(i).handlerId == m_callHistoryList.at(j).dialHistoryId)
                    {
                        int time = 0;
                        time = QDateTime::currentDateTime().toTime_t() - m_callHistoryList.at(j).dialDateStart.toTime_t();
                        emit signalUpdateTimer(m_handlerList.at(i).handlerId, timeToString(time));
                    }
                }
            }
            else if (m_handlerList.at(i).state == 2)
            {
                emit signalUpdateTimer(m_handlerList.at(i).handlerId, "保持");
            }
        }
    }
    else if (m_qmlState == 3){
        for (int i = 0, iend = m_meetingList.count(); i < iend; i++)
        {
            if (m_meetingList.at(i).state == 1 || m_meetingList.at(i).state == 3 || m_meetingList.at(i).state == 4)
            {
                for (int j =0,jend = m_callHistoryList.count(); j < jend ; j++)
                {
                    if (m_meetingList.at(i).handlerId == m_callHistoryList.at(j).dialHistoryId)
                    {
                        int time = 0;
                        time = QDateTime::currentDateTime().toTime_t() - m_callHistoryList.at(j).dialDateStart.toTime_t();
                        emit signalUpdateTimer(m_meetingList.at(i).handlerId, timeToString(time));
                    }
                }
            }
            else if (m_meetingList.at(i).state == 2)
            {
                emit signalUpdateTimer(m_meetingList.at(i).handlerId, "保持");
            }
        }
    }
    */
}

void DialInterfaceAdaptor::onDialError(int err)
{
    m_handlerList.clear();
    m_meetingList.clear();
    emit signalQuitApp();
}

void DialInterfaceAdaptor::setDisplayState(bool state)
{
    //    if (state) {
    //        powerManager.setDisplayState(powerManager.DisplayOn); //控制点亮屏幕
    //    } else {
    //        powerManager.setDisplayState(powerManager.DisplayOff); //关闭屏幕
    //    }
}

void DialInterfaceAdaptor::onSignalAudioModeChanged(bool isHandsFree)
{
    emit signalAudioModeChanged(isHandsFree);
}

//添加通话记录
void DialInterfaceAdaptor::onSignalHandlerEnd(HandlerData data)
{
    CallHistory history;
    history.dialDateEnd = data.end;
    history.dialDateStart = data.start;
    history.dialNetworkType = data.network;

    history.dialTarget.append(m_contacterAdaptor->searchContacterNumber(3,data.dialNum));
    history.dialTargetNumber.append(data.dialNum);
    history.dialType = data.dialState;
    history.dialTime = timeToString(data.end.toTime_t() - data.start.toTime_t());
    m_callhistoryAdaptor->addCallHistory(history);
}

void DialInterfaceAdaptor::onSignalMettingEnd(HandlerData data)
{
    CallHistory history;
    QString num;
    history.dialDateEnd = data.end;
    history.dialDateStart = data.start;
    history.dialNetworkType = (HistoryNetwork)data.network;
    for (int i = 0; i < data.mettingTarget.size(); i++)
    {
        num = data.mettingTarget.at(i);
        history.dialTargetNumber.append(num);
        history.dialTarget.append(m_contacterAdaptor->searchContacterNumber(3,num));
    }
    history.dialType = MEETING_DIAL;
    history.dialTime = timeToString(data.end.toTime_t() - data.start.toTime_t());
    m_callhistoryAdaptor->addCallHistory(history);
    m_meetingList.clear();
    if (m_handlerList.count() == 0 && m_videoManager.handlerId == "")
        emit signalQuitApp();
    else if (m_handlerList.count() == 0 && m_videoManager.handlerId != "")
        emit signalReturnPage();
}

void DialInterfaceAdaptor::onSignalMettingHandlerDown(QString handlerId, QString dialNum)
{
    for (int i = 0; i < m_meetingList.size(); i++)
    {
        if (handlerId == m_meetingList.at(i).handlerId)
        {
            HandlerManager temp = m_meetingList.at(i);
            temp.isOnline = false;
            m_meetingList.replace(i, temp);
            break;
        }
    }
    for (int i = 0; i < m_handlerList.size(); i++)
    {
        if (handlerId == m_handlerList.at(i).handlerId)
        {
            HandlerManager temp = m_meetingList.at(i);
            temp.isOnline = false;
            m_meetingList.replace(i, temp);
            break;
        }
    }
}

//添加到 m_meetingList
void DialInterfaceAdaptor::onMultipartyChanged(QString handlerId, bool isMultiparty)
{
    for (int i = 0; i < m_handlerList.size(); i++)
    {
        if (handlerId == m_handlerList.at(i).handlerId && isMultiparty == true)
        {
            m_meetingList.append(m_handlerList.at(i));
            break;
        }
    }
}

QString DialInterfaceAdaptor::timeToString(int time)
{
    int minute;
    int second;
    int hour;
    QString minutes;
    QString seconds;
    QString timeString = "";
    if (time >= 3600)
    {
        hour = time / 3600;
        minute = time % 3600 / 60;
        second = time % 60;
        if (minute < 10)
            minutes = QString("0%1").arg(minute);
        else
            minutes = QString("%1").arg(minute);

        if (second < 10)
            seconds = QString("0%1").arg(second);
        else
            seconds = QString("%1").arg(second);
        timeString = QString("%1:%2:%3").arg(hour).arg(minutes).arg(seconds);
    }
    else if (time < 3600 && time > 0)
    {
        minute = time / 60;
        second = time % 60;
        if (minute < 10)
            minutes = QString("0%1").arg(minute);
        else
            minutes = QString("%1").arg(minute);

        if (second < 10)
            seconds = QString("0%1").arg(second);
        else
            seconds = QString("%1").arg(second);
        timeString = QString("%1:%2").arg(minutes).arg(seconds);
    }
    return timeString;
}

void DialInterfaceAdaptor::getAddPersonInfo(QString searchString)
{
    QStringList role;
    role << "personName" << "position" << "size" << "id" << "contacterType";
    m_contacterModel->setRoles(role);
    m_contacterModel->setModelList(3 , searchString);

    /*
    QStringList contacters;
    if (searchString == ""){
        contacters = m_contacterAdaptor->getContacters(CONTACTERTYPE_UNDEFINE);
    }
    else {
        contacters = m_contacterAdaptor->searchContactors(CONTACTERTYPE_UNDEFINE, searchString);
    }
    for (int i = 0; i < contacters.count(); i++) {
        Contacter contacter = m_contacterAdaptor->getContacter(contacters[i]);
        if (contacter.id != 0) {
            QString titleName;
            switch (contacter.title) {
            case BATTLION:
                titleName = "营长";
                break;
            case COMPANY:
                titleName = "连长";
                break;
            case PLATOON:
                titleName = "排长";
                break;
            case SQUAD:
                titleName = "班长";
                break;
            case SOLIDER:
                titleName = "战士";
                break;
            default:
                titleName = "未知";
                break;
            }
            emit signalAddPersonInfo(contacter.id, contacter.surname + contacter.name, titleName, contacter.namePinyin, contacter.type);
        }
    }
    */
}

void DialInterfaceAdaptor::getContacter(QString id)
{

    bool ok;
    Contacter contacter;
    QStringList qLocalList;
    int dec = id.toInt(&ok,10);
    if (dec == -1) {
        contacter = m_contacterAdaptor->getLocalInfo();
    } else {
        contacter = m_contacterAdaptor->getContacter(id);
    }
    if(contacter.id.isEmpty())
    {
        qDebug() << "unvalid userid:" << id;
    }
    QString phoneName;
    QString titleName;
    QString name;
    switch (contacter.title) {
    case BATTLION:
        titleName = "营长";
        break;
    case COMPANY:
        titleName = "连长";
        break;
    case PLATOON:
        titleName = "排长";
        break;
    case SQUAD:
        titleName = "班长";
        break;
    case SOLIDER:
        titleName = "战士";
        break;
    default:
        titleName = "未知";
        break;
    }
    name = contacter.surname + " " + contacter.name;

    emit signalSendNameToPersonInfo(name,contacter.photoPath);
    emit signalContacterInfo("职位",titleName);
    emit signalContacterInfo("编号",id);

    for (int i = 0;i < contacter.numbers.count();i++){
        switch (contacter.numbers[i].netType) {
        case LTE_NETWORK:
            phoneName = "LTE";
            break;
        case AD_HOC_NETWORK:
            phoneName = "自组网";
            break;
        case SATELLITE_NETWORK:
            phoneName = "天通网";
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            phoneName = "数字对讲";
            break;
        case BATTLE_NETWORK:
            phoneName = "战互网";
            break;
        case WIFI_NETWORK:
            phoneName = "WIFI";
            break;
        case UNKNOW_NETWORK:
            //break;
        default:
            phoneName = "未知网络";
            break;
        }
        emit signalContacterInfo(phoneName,contacter.numbers[i].number);
    }
    emit signalContacterInfo("设备序列号",contacter.snNumber);
}

void DialInterfaceAdaptor::onHangupMultipartyCalls(int network)
{
    m_interface.call("onHangupMultipartyCalls", network);
}

QString DialInterfaceAdaptor::getLocalNum(int net)
{
    qDebug() << "getLocalNum 1" << m_contacterAdaptor;
    Contacter contacter = m_contacterAdaptor->getLocalInfo();
    if (contacter.id.isEmpty())
        return "null";
    for (int i = 0;i < contacter.numbers.count();i++){
        if (contacter.numbers[i].netType == net) {
            return contacter.numbers[i].number;
        }
        else
            return "null";
    }
}

void DialInterfaceAdaptor::onRecord(int network, bool on)
{
    m_interface.call("onRecord" , network , on);
}

QString DialInterfaceAdaptor::onGetContacterValue(int index, QString key)
{
    m_contacterModel->getContacterValue(index , key);
}

bool DialInterfaceAdaptor::getIsMini()
{
    int flag;
    QDBusReply<bool> addReply =  m_interface.call("onGetIsMini");
    if(addReply.isValid())
        flag = addReply.value();
    else {
        qDebug() << "onSetIsMini reply error:" << addReply.error();
        flag = false;
    }
    return flag;
}

int DialInterfaceAdaptor::answerVideo()
{
    m_interface.call("onAnswer", m_videoManager.handlerId);
}

int DialInterfaceAdaptor::hangupVideo()
{
    m_interface.call("onHangup", m_videoManager.handlerId);
    if (m_handlerList.count() + m_meetingList.count() == 1 && m_videoManager.handlerId == "")
        emit signalQuitApp();
    if (m_handlerList.count() + m_meetingList.count() == 0 && m_videoManager.handlerId != "")
        emit signalQuitApp();
}

int DialInterfaceAdaptor::getActiveMode()
{
    int activeMode = 0;
    QDBusReply<int> reply =  m_interface.call("onGetActiveMode");
    if(reply.isValid())
    {
        activeMode = reply.value();
    }
    return activeMode;
}

int DialInterfaceAdaptor::setHoldConference(int network, bool on)
{
    int ret = 0;
    QDBusReply<int> reply =  m_interface.call("onSetHoldConference",network, on);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}

void DialInterfaceAdaptor::appAlreadyStart()
{
    emit signalScreen_on();
    m_interface.call("onAppAlreadyStart");
}

bool DialInterfaceAdaptor::isFileExists(QString filePath)
{
    if (filePath.left(5) == "file:"){
        filePath = filePath.mid(7,filePath.count()-7);
        QFile file(filePath);
        return file.exists();
    }else {
        return true;
    }
}

DialInterfaceAdaptor::DialInterfaceAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.dial", "/dial", "com.sinux.DBus.dial", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<HandlerManager>();
    qDBusRegisterMetaType<HandlerManagerList>();
    qDBusRegisterMetaType<HandlerData>();


    m_qmlState = -1;
    m_start = false;
    isTalking = false;
    anotherHandler = "";
    isanother = false;
    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalHandlerListChanged",
                       this,SLOT(onSignalHandlerListChanged(int,HandlerManager)));

    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalConfCallFinished",
                       this,SLOT(onConfCallFinished(int,QString)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalShowFullScreen",
                       this,SIGNAL(signalShowFullScreen()));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalAudioModeChanged",
                       this,SLOT(onSignalAudioModeChanged(bool)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalHandlerEnd",
                       this,SLOT(onSignalHandlerEnd(HandlerData)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalMettingEnd",
                       this,SLOT(onSignalMettingEnd(HandlerData)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalMettingHandlerDown",
                       this,SLOT(onSignalMettingHandlerDown(QString,QString)));                                                                          
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalMultipartyChanged",
                       this,SLOT(onMultipartyChanged(QString,bool)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalVoipFrameData",
                       this,SLOT(onVoipFrameData()));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalV4l2FrameData",
                       this,SLOT(onV4l2FrameData()));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalVidChanged",
                       this,SIGNAL(signalVidChanged(bool)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalMicrophoneMuteChanged",
                       this,SIGNAL(signalMicrophoneMuteChanged(bool)));
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalVideoHandlerChanged",
                       this,SLOT(onVideoHandlerChanged(HandlerManager)));
    sessionBus.connect("com.sinxu.dial", "/dial", "com.sinux.DBus.dial", "dialError",
                       this, SLOT(onDialError(int)));
    // by michael zheng2017.4.6
    sessionBus.connect("com.sinux.myListenHeart", "/myHeart", "com.sinux.DBus.MyHeartBag","signalSendAgainLinkRequs",
                       this,SLOT(onAgainLinkVideo(QString)));
    sessionBus.connect("com.sinux.myListenHeart", "/myHeart", "com.sinux.DBus.MyHeartBag","signalAgainLinkState",
                       this,SLOT(onIsAgainLink(bool)));
    isAGain = false;
    // end by michael zheng
    isVideoing = false;
    m_isVideoHold = false;
    m_screenShot = NULL;
    m_screenShotV4l2 = NULL;
    m_pSharedMemory = new SharedMemory();
    m_videoManager.handlerId = "";
    m_contacterAdaptor = ContacterInterfaceAdaptor::getInstance();
    m_callhistoryAdaptor = CallHistoryInterfaceAdaptor::getInstance();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
    dialTimer = new QTimer(this);
    connect(dialTimer, SIGNAL(timeout()), this, SLOT(onDialTimerOut()));


}


