#include "callmanager.h"
#include <QtDBus/QDBusMetaType>
#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QTimer>
#include <QDBusReply>
#include "callhandler.h"
#include "QDateTime"

#define DIAL_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_INTERFACE QLatin1String("com.sinux.CommTransService.CallManager")
#define DIAL_PATH QLatin1String("/CallManager")

CallManager * CallManager::m_instance = NULL;
CallManager::CallManager(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType<UserInfo>();
    qDBusRegisterMetaType<HandlerData>();
    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService(DIAL_SERVICE))
        qDebug() << "bus error:" << bus.lastError().message() << bus.lastError().type();

    bool contacterReg = bus.registerObject(DIAL_PATH, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterReg)
        qDebug() << "bus error:" << "register /CallManager false";

    m_activeMode = 0;
    m_videoHandler = NULL;
    initialize();
    CSystemProfileManager *profileManager = new CSystemProfileManager();

    dialSound = new QSound("/usr/share/rings/dial.wav");
    listenSound = new QSound(profileManager->defaultTone());
    errSound = new QSound("/usr/share/rings/err.wav");
    dialSound->setLoops(10);
    listenSound->setLoops(5);
}

CallManager::~CallManager()
{
    for(int i = 0; i < m_adCalls.size(); i++)
    {
        CallHandler *pHandler = m_adCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_adCalls.clear();

    for(int i = 0; i < m_lteCalls.size(); i++)
    {
        CallHandler *pHandler = m_lteCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_lteCalls.clear();

    for(int i = 0; i < m_wifiCalls.size(); i++)
    {
        CallHandler *pHandler = m_wifiCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    for(int i = 0; i < m_battleCalls.size(); i++)
    {
        CallHandler *pHandler = m_battleCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_battleCalls.clear();
    for(int i = 0; i < m_csCalls.size(); i++)
    {
        TiantongDial *pHandler = m_csCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_csCalls.clear();
}

CallManager *CallManager::instance(QObject *parent)
{
    if(m_instance == NULL)
    {
        m_instance = new CallManager();
    }
    return m_instance;
}

int CallManager::voipInitialize(NetworkType network, int sipPort, int rtpPort, int rtpPortRange)
{
    switch (network) {
    case WIFI_NETWORK:
        m_wifiManager->initialize(sipPort, rtpPort, rtpPortRange);
        break;
    case AD_HOC_NETWORK:
        m_adhocManager->initialize(sipPort, rtpPort, rtpPortRange);
        break;
    case LTE_NETWORK:
        m_lteManager->initialize(sipPort, rtpPort, rtpPortRange);
        break;
    default:
        break;
    }
    return 1;
}

void CallManager::initialize()
{
    m_wifiManager = VoipCallManager::instanceWifi();
    m_wifiManager->setObjectName("wifi");
    connect(m_wifiManager, SIGNAL(signalNewCall(int)), this, SLOT(onCallHandlerIdChanged(int)));
    connect(m_wifiManager, SIGNAL(signalHandsFreeChanged()), this, SLOT(onAudioModeChanged()));
    connect(m_wifiManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    connect(m_wifiManager, SIGNAL(signalConferenceEstablished()), this, SLOT(onConfCallFinished()));
    connect(m_wifiManager, SIGNAL(numberOfMultipartyParticipantsChanged()), this, SLOT(onNumberOfMultipartyParticipantsChanged()));
    connect(m_wifiManager, SIGNAL(signalVoipFrameData()), this, SIGNAL(signalVoipFrameData()));
    connect(m_wifiManager, SIGNAL(signalV4l2FrameData()), this, SIGNAL(signalV4l2FrameData()));
    connect(m_wifiManager, SIGNAL(signalVidChanged(bool)), this, SIGNAL(signalVidChanged(bool)));
    connect(m_wifiManager, SIGNAL(signalPttStateChanged(uint,int)), this, SIGNAL(signalPttStateChanged(uint,int)));
    connect(m_wifiManager, SIGNAL(signalPttRecordFileFinished(uint,QString)), this, SLOT(onPttRecordFileFinished(uint,QString)));
    connect(m_wifiManager, SIGNAL(signalCallError(int)), this, SLOT(onDialError(int)));
    m_wifiManager->initialize(5080, 33333, 10);

    m_adhocManager = VoipCallManager::instanceAdHoc();
    m_adhocManager->setObjectName("adhoc");
    QObject::connect(m_adhocManager, SIGNAL(signalHandsFreeChanged()), this, SLOT(onAudioModeChanged()));
    QObject::connect(m_adhocManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    QObject::connect(m_adhocManager, SIGNAL(numberOfMultipartyParticipantsChanged()),
                     this, SLOT(onNumberOfMultipartyParticipantsChanged()));
    QObject::connect(m_adhocManager, SIGNAL(signalNewCall(int)), this, SLOT(onCallHandlerIdChanged(int)));
    connect(m_adhocManager, SIGNAL(signalConferenceEstablished()), this, SLOT(onConfCallFinished()));

    connect(m_adhocManager, SIGNAL(signalVoipFrameData()), this, SIGNAL(signalVoipFrameData()));
    connect(m_adhocManager, SIGNAL(signalV4l2FrameData()), this, SIGNAL(signalV4l2FrameData()));
    connect(m_adhocManager, SIGNAL(signalVidChanged(bool)), this, SIGNAL(signalVidChanged(bool)));
    connect(m_adhocManager, SIGNAL(signalPttStateChanged(uint,int)), this, SIGNAL(signalPttStateChanged(uint,int)));
    connect(m_adhocManager, SIGNAL(signalPttRecordFileFinished(uint,QString)), this, SLOT(onPttRecordFileFinished(uint,QString)));
    connect(m_adhocManager, SIGNAL(signalCallError(int)), this, SLOT(onDialError(int)));
    m_adhocManager->initialize(7010, 6010, 14);

    m_lteManager = VoipCallManager::instanceLte();
    m_lteManager->setObjectName("lte");
    QObject::connect(m_lteManager, SIGNAL(signalHandsFreeChanged()), this, SLOT(onAudioModeChanged()));
    QObject::connect(m_lteManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    QObject::connect(m_lteManager, SIGNAL(numberOfMultipartyParticipantsChanged()),
                     this, SLOT(onNumberOfMultipartyParticipantsChanged()));
    QObject::connect(m_lteManager, SIGNAL(signalConferenceEstablished()),
                     this, SLOT(onConfCallFinished()));
    QObject::connect(m_lteManager, SIGNAL(signalNewCall(int)), this, SLOT(onCallHandlerIdChanged(int)));
    connect(m_lteManager, SIGNAL(signalVoipFrameData()), this, SIGNAL(signalVoipFrameData()));
    connect(m_lteManager, SIGNAL(signalV4l2FrameData()), this, SIGNAL(signalV4l2FrameData()));
    connect(m_lteManager, SIGNAL(signalVidChanged(bool)), this, SIGNAL(signalVidChanged(bool)));
    connect(m_lteManager, SIGNAL(signalPttStateChanged(uint,int)), this, SIGNAL(signalPttStateChanged(uint,int)));
    connect(m_lteManager, SIGNAL(signalPttRecordFileFinished(uint,QString)), this, SLOT(onPttRecordFileFinished(uint,QString)));
    connect(m_lteManager, SIGNAL(signalCallError(int)), this, SLOT(onDialError(int)));
    m_lteManager->initialize(5160, 5161, 20);

    m_battleManager = VoipCallManager::instanceBattle();
    m_battleManager->setObjectName("battle");
    QObject::connect(m_battleManager, SIGNAL(signalHandsFreeChanged()), this, SLOT(onAudioModeChanged()));
    QObject::connect(m_battleManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    QObject::connect(m_battleManager, SIGNAL(numberOfMultipartyParticipantsChanged()),
                     this, SLOT(onNumberOfMultipartyParticipantsChanged()));
    QObject::connect(m_battleManager, SIGNAL(signalConferenceEstablished()),
                     this, SLOT(onConfCallFinished()));
    QObject::connect(m_battleManager, SIGNAL(signalNewCall(int)), this, SLOT(onCallHandlerIdChanged(int)));
    connect(m_battleManager, SIGNAL(signalVoipFrameData()), this, SIGNAL(signalVoipFrameData()));
    connect(m_battleManager, SIGNAL(signalV4l2FrameData()), this, SIGNAL(signalV4l2FrameData()));
    connect(m_battleManager, SIGNAL(signalVidChanged(bool)), this, SIGNAL(signalVidChanged(bool)));
    connect(m_battleManager, SIGNAL(signalPttStateChanged(uint,int)), this, SIGNAL(signalPttStateChanged(uint,int)));
    connect(m_battleManager, SIGNAL(signalPttRecordFileFinished(uint,QString)), this, SLOT(onPttRecordFileFinished(uint,QString)));
    connect(m_battleManager, SIGNAL(signalCallError(int)), this, SLOT(onDialError(int)));
    m_battleManager->initialize(5260, 5261, 20);

    m_voiceManager = SinuxVoicecallManager::instance();
    m_voiceManager->setObjectName("cs");
    QObject::connect(m_voiceManager, SIGNAL(signalNewCall(QString)), this, SLOT(onCsCallHandlerIdChanged(QString)));
    QObject::connect(m_voiceManager, SIGNAL(audioModeChanged()), this, SLOT(onAudioModeChanged()));
    QObject::connect(m_voiceManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    QObject::connect(m_voiceManager, SIGNAL(signalConferenceEstablished()),
                     this, SLOT(onConfCallFinished()));
    QObject::connect(m_voiceManager, SIGNAL(signalDialError(QString)), this, SLOT(onCsDialError(QString)));

    m_baotongManager = new BaotongDial(this);
    m_baotongManager->setObjectName("baotong");
    connect(m_baotongManager,SIGNAL(PttGettedChanged(bool)),this,SIGNAL(PttGettedChanged(bool)));
}

//多方等状态判断
void CallManager::setActiveMode()
{
    int count;
    count = m_adCalls.size() + m_lteCalls.size() + m_wifiCalls.size() + m_csCalls.size() + m_battleCalls.size();

    if (m_mettingCalls.size() == 0){
        if (count == 1)
            m_activeMode = 1;
        else if (count > 1)
            m_activeMode = 2;
    }
    else if (m_mettingCalls.size() > 0){
        if (count == m_mettingCalls.size())
            m_activeMode = 3;
        else if (count > m_mettingCalls.size())
            m_activeMode = 4;
    }
}

int CallManager::readActiveMode()
{
    return m_activeMode;
}

void CallManager::onCallHandlerIdChanged(int handlerId)
{
    CTS_PRINT << handlerId;
    QString number;
    VoipCallHandler * handler = NULL;
    CallHandler *pHandler = NULL;
    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        handler = m_adhocManager->lookupHandler(handlerId);
        if (getGetAudOrVideoInt(AD_HOC_NETWORK, handlerId) == 1)
        {
            if (m_videoHandler == NULL){
                m_videoHandler = new CallHandler(handler);
                number = m_videoHandler->dialNumber();
                QObject::connect(m_videoHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
                if (m_videoHandler->status() == STATUS_INCOMING)
                    listenSound->play();
                else if (m_videoHandler->status() == STATUS_DIALING || pHandler->status() == STATUS_ALERTING)
                    dialSound->play();
                emit signalVideoCallHandlerIdChanged(number, m_videoHandler->handlerId());
            }
            else
            {
                CallHandler *handtemp = new CallHandler(handler);
                handtemp->hangup();
            }
            return;
        }
    }
    else if(sender->objectName() == "lte")
    {
        handler = m_lteManager->lookupHandler(handlerId);
        if (getGetAudOrVideoInt(LTE_NETWORK, handlerId) == 1)
        {
            if (m_videoHandler == NULL){
                m_videoHandler = new CallHandler(handler);
                number = m_videoHandler->dialNumber();
                QObject::connect(m_videoHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
                if (m_videoHandler->status() == STATUS_INCOMING)
                    listenSound->play();
                else if (m_videoHandler->status() == STATUS_DIALING || pHandler->status() == STATUS_ALERTING)
                    dialSound->play();
                emit signalVideoCallHandlerIdChanged(number, m_videoHandler->handlerId());

            }
            else
            {
                CallHandler *handtemp = new CallHandler(handler);
                handtemp->hangup();
            }
            return;
        }
    }
    else if (sender->objectName() == "wifi")
    {
        handler = m_wifiManager->lookupHandler(handlerId);
        if (getGetAudOrVideoInt(WIFI_NETWORK, handlerId) == 1)
        {
            if (m_videoHandler == NULL){
                m_videoHandler = new CallHandler(handler);
                number = m_videoHandler->dialNumber();
                QObject::connect(m_videoHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
                if (m_videoHandler->status() == STATUS_INCOMING)
                    listenSound->play();
                else if (m_videoHandler->status() == STATUS_DIALING || pHandler->status() == STATUS_ALERTING)
                    dialSound->play();
                emit signalVideoCallHandlerIdChanged(number, m_videoHandler->handlerId());
            }
            else
            {
                CallHandler *handtemp = new CallHandler(handler);
                handtemp->hangup();
            }
            return;
        }
    }
    else if (sender->objectName() == "battle")
    {
        handler = m_battleManager->lookupHandler(handlerId);
        if (getGetAudOrVideoInt(BATTLE_NETWORK, handlerId) == 1)
        {
            if (m_videoHandler == NULL){
                m_videoHandler = new CallHandler(handler);
                number = m_videoHandler->dialNumber();
                QObject::connect(m_videoHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
                if (m_videoHandler->status() == STATUS_INCOMING)
                    listenSound->play();
                else if (m_videoHandler->status() == STATUS_DIALING || pHandler->status() == STATUS_ALERTING)
                    dialSound->play();
                emit signalVideoCallHandlerIdChanged(number, m_videoHandler->handlerId());

            }
            else
            {
                CallHandler *handtemp = new CallHandler(handler);
                handtemp->hangup();
            }
            return;
        }
    }
    if(pHandler == NULL)
        pHandler = new CallHandler(handler);
    number = pHandler->dialNumber();
    QObject::connect(pHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
    QObject::connect(pHandler, SIGNAL(multipartyChanged(QString,bool,int)), this, SLOT(onMultipartyChanged(QString, bool, int)));

    int i = 0;
    for(i = 0; i < m_adCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_adCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2 || number == m_adCalls.at(i)->dialNumber())
        {
            CTS_PRINT << "AdHoc voip call : handlerId = " << m_adCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    for(i = 0; i < m_lteCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_lteCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2 || number == m_lteCalls.at(i)->dialNumber())
        {
            CTS_PRINT << "Lte voip call : handlerId = " << m_lteCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    for(i = 0; i < m_wifiCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_wifiCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2 || number == m_wifiCalls.at(i)->dialNumber())
        {
            CTS_PRINT << "wifi voip call : handlerId = " << m_wifiCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    for(i = 0; i < m_csCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_csCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "Lte voip call : handlerId = " << m_csCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }
    for(i = 0; i < m_battleCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_battleCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "Lte voip call : handlerId = " << m_battleCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    switch(pHandler->network())
    {
    case AD_HOC_NETWORK:
        m_adCalls.append(pHandler);
        break;
    case LTE_NETWORK:
        m_lteCalls.append(pHandler);
        break;
    case WIFI_NETWORK:
        m_wifiCalls.append(pHandler);
        break;
    case BATTLE_NETWORK:
        m_battleCalls.append(pHandler);
        break;
    default:
        break;
    }
    if (pHandler->status() == STATUS_INCOMING)
        listenSound->play();
    else if (pHandler->status() == STATUS_DIALING || pHandler->status() == STATUS_ALERTING)
        dialSound->play();
    setActiveMode();
    Q_EMIT callHandlerIdChanged(number, pHandler->handlerId());
}

void CallManager::onCsCallHandlerIdChanged(QString handlerId)
{
    CTS_PRINT << handlerId;
    QString number;
    VoiceCallHandler * handler = NULL;
    TiantongDial *pHandler = NULL;
    QObject *sender = this->sender();
    if(sender->objectName() == "cs")
    {
        handler = m_voiceManager->lookupHandler(handlerId);
    }

    if(pHandler == NULL)
        pHandler = new TiantongDial(handler);
    number = pHandler->dialNumber();

    int i = 0;
    for(i = 0; i < m_adCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_adCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "AdHoc voip call : handlerId = " << m_adCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    for(i = 0; i < m_lteCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_lteCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "Lte voip call : handlerId = " << m_lteCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    for(i = 0; i < m_wifiCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_wifiCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "wifi voip call : handlerId = " << m_wifiCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }
    for(i = 0; i < m_battleCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_battleCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "bat voip call : handlerId = " << m_battleCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    for(i = 0; i < m_csCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_csCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING || m_activeMode == 2)
        {
            CTS_PRINT << "cs voip call : handlerId = " << m_csCalls.at(i)->handlerId() << ", status = " << status;
            pHandler->hangup();
            return;
        }
    }

    switch(pHandler->network())
    {
    case SATELLITE_NETWORK:
        m_csCalls.append(pHandler);
        break;
    default:
        break;
    }


    QObject::connect(pHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
    QObject::connect(pHandler, SIGNAL(multipartyChanged(QString,bool,int)), this, SLOT(onMultipartyChanged(QString, bool, int)));
    setActiveMode();
    Q_EMIT callHandlerIdChanged(number, handlerId);
}

//这里挂断需要等２０秒再释放handler  发信号添加通话记录 号码，时间，网络，通话状态
void CallManager::onStatusChanged(QString handlerId, int status)
{
    CTS_PRINT << "handlerId : " << handlerId << ", status = " << status << endl;

    if (status == STATUS_ACTIVE || status == STATUS_DISCONNECTED || status == STATUS_NULL){
        listenSound->stop();
        dialSound->stop();
    }

    int i = 0;
    bool isMetting = false;
    if(status == STATUS_NULL || status == STATUS_DISCONNECTED)
    {
        if (m_videoHandler && m_videoHandler->handlerId() == handlerId)
        {
            HandlerData data;
            data.dialNum = m_videoHandler->dialNumber();
            data.end = m_videoHandler->endTime;
            data.start = m_videoHandler->startTime;
            data.isMetting = false;
            switch (m_videoHandler->network()) {
            case AD_HOC_NETWORK:
                data.network = ADC_NETWORK_VIDEO_DIAL;
                break;
            case LTE_NETWORK:
                data.network = LTE_NETWORK_VIDEO_DIAL;
                break;
            case BATTLE_NETWORK:
                data.network = BATTLE_NETWORK_VIDEO_DIAL;
                break;
            case WIFI_NETWORK:
                data.network = WIFI_NETWORK_VIDEO_DIAL;
                break;
            default:
                break;
            }
            data.dialState = m_videoHandler->m_dialState;
            Q_EMIT signalHandlerEnd(data);

            m_videoHandler->deleteLater();
            m_videoHandler = NULL;
            Q_EMIT statusChanged(handlerId, status);
            return;
        }

        for(i = 0; i < m_mettingCalls.size(); i++)
        {
            if(m_mettingCalls.at(i)->handlerId() == handlerId)
            {
                isMetting = true;
                CallHandler *pHandler = m_mettingCalls.takeAt(i);
                if (m_mettingCalls.size() > 1){
                    //会议成员掉线信号 号码，时间，网络，通话状态
                    Q_EMIT signalMettingHandlerDown(pHandler->handlerId(), pHandler->dialNumber());
                    pHandler->deleteLater();
                    pHandler = NULL;
                }
                else if (m_mettingCalls.size() == 1){
                    //会议结束信号 号码，时间，网络，通话状态
                    HandlerData data;
                    data.dialNum = pHandler->dialNumber();
                    mettingEnd = QDateTime::currentDateTime();
                    data.end = mettingEnd;
                    data.start = mettingStart;
                    data.isMetting = true;
                    data.mettingTarget = m_mettingTarget;
                    if (pHandler->network() == LTE_NETWORK)
                        data.network = LTE_NETWORK_METTING_DIAL;
                    else if (pHandler->network() == AD_HOC_NETWORK)
                        data.network = ADC_NETWORK_METTING_DIAL;
                    else if (pHandler->network() == WIFI_NETWORK)
                        data.network = WIFI_NETWORK_METTING_DIAL;
                    else if (pHandler->network() == SATELLITE_NETWORK)
                        data.network = SATELLITE_NETWORK;
                    else if (pHandler->network() == BATTLE_NETWORK)
                        data.network = BATTLE_NETWORK;

                    Q_EMIT signalMettingEnd(data);
                    pHandler->deleteLater();
                    pHandler = NULL;
                    m_mettingCalls.clear();
                    m_mettingTarget.clear();
                }

                break;
            }
        }

        bool bRemoved = false;
        for(i = 0; i < m_adCalls.size(); i++)
        {
            if(m_adCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                CallHandler *pHandler = m_adCalls.takeAt(i);
                QTimer::singleShot(20000, pHandler, SLOT(deleteLater()));
                if (!isMetting){
                    HandlerData data;
                    data.dialNum = pHandler->dialNumber();
                    data.end = pHandler->endTime;
                    data.start = pHandler->startTime;
                    data.isMetting = false;
                    data.network = ADC_NETWORK_VOICE_DIAL;
                    data.dialState = pHandler->m_dialState;
                    Q_EMIT signalHandlerEnd(data);
                }
                break;
            }
        }

        for(i = 0; i < m_lteCalls.size() && bRemoved == false; i++)
        {
            if(m_lteCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                CallHandler *pHandler = m_lteCalls.takeAt(i);
                QTimer::singleShot(20000, pHandler, SLOT(deleteLater()));
                if (!isMetting){
                    HandlerData data;
                    data.dialNum = pHandler->dialNumber();
                    data.end = pHandler->endTime;
                    data.start = pHandler->startTime;
                    data.isMetting = false;
                    data.network = LTE_NETWORK_VOICE_DIAL;
                    data.dialState = pHandler->m_dialState;
                    Q_EMIT signalHandlerEnd(data);
                }
                break;
            }
        }

        for(i = 0; i < m_wifiCalls.size() && bRemoved == false; i++)
        {
            if(m_wifiCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                CallHandler *pHandler = m_wifiCalls.takeAt(i);
                QTimer::singleShot(20000, pHandler, SLOT(deleteLater()));
                if (!isMetting){
                    HandlerData data;
                    data.dialNum = pHandler->dialNumber();
                    data.end = pHandler->endTime;
                    data.start = pHandler->startTime;
                    data.isMetting = false;
                    data.network = WIFI_NETWORK_VOICE_DIAL;
                    data.dialState = pHandler->m_dialState;
                    Q_EMIT signalHandlerEnd(data);
                }
                break;
            }
        }
        for(i = 0; i < m_battleCalls.size() && bRemoved == false; i++)
        {
            if(m_battleCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                CallHandler *pHandler = m_battleCalls.takeAt(i);
                QTimer::singleShot(20000, pHandler, SLOT(deleteLater()));
                if (!isMetting){
                    HandlerData data;
                    data.dialNum = pHandler->dialNumber();
                    data.end = pHandler->endTime;
                    data.start = pHandler->startTime;
                    data.isMetting = false;
                    data.network = BATTLE_NETWORK_VOICE_DIAL;
                    data.dialState = pHandler->m_dialState;
                    Q_EMIT signalHandlerEnd(data);
                }
                break;
            }
        }
        for(i = 0; i < m_csCalls.size() && bRemoved == false; i ++)
        {
            if(m_csCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                TiantongDial *pHandler = m_csCalls.takeAt(i);
                QTimer::singleShot(20000, pHandler, SLOT(deleteLater()));
                if (!isMetting){
                    HandlerData data;
                    data.dialNum = pHandler->dialNumber();
                    data.end = pHandler->endTime;
                    data.start = pHandler->startTime;
                    data.isMetting = false;
                    data.network = CS_NETWORK_VOICE_DIAL;
                    data.dialState = pHandler->m_dialState;
                    Q_EMIT signalHandlerEnd(data);
                }
                break;
            }
        }
        setActiveMode();



//        for(i = 0; i < m_digitalCalls.size() && bRemoved == false; i++)
//        {
//            if(m_digitalCalls.at(i)->handlerId() == handlerId)
//            {
//                bRemoved = true;
//                DialHandler *pHandler = m_digitalCalls.takeAt(i);
//                pHandler->deleteLater();
//                //pHandler = NULL;
//                break;
//            }
//        }
        //挂断后只剩一路电话，自动激活
        if (m_adCalls.size() + m_lteCalls.size() + m_wifiCalls.size() + m_csCalls.size() + m_battleCalls.size() == 1)
        {
            if (m_adCalls.size() == 1)
            {
                m_adCalls.at(0)->hold(false);
            }
            else if (m_lteCalls.size() == 1)
            {
                m_lteCalls.at(0)->hold(false);
            }
            else if (m_wifiCalls.size() == 1)
            {
                m_wifiCalls.at(0)->hold(false);
            }
            else if (m_csCalls.size() == 1)
            {
                m_csCalls.at(0)->hold(false);
            }
            else if (m_battleCalls.size() == 1)
            {
                m_battleCalls.at(0)->hold(false);
            }
        }
        else if (m_adCalls.size() + m_lteCalls.size() + m_wifiCalls.size() + m_csCalls.size() + m_battleCalls.size() == m_mettingCalls.size() && m_mettingCalls.size() != 0)
        {
            //挂断后只剩会议，自动激活
            if (m_mettingCalls.at(0)->network() == WIFI_NETWORK)
                m_wifiManager->setHoldConference(false);
            else if (m_mettingCalls.at(i)->network() == LTE_NETWORK)
                m_lteManager->setHoldConference(false);
            else if (m_mettingCalls.at(i)->network() == AD_HOC_NETWORK)
                m_adhocManager->setHoldConference(false);
            else if (m_mettingCalls.at(i)->network() == BATTLE_NETWORK)
                m_battleManager->setHoldConference(false);
        }
    }
    //自组网自动形成会议
    else if (status == STATUS_ACTIVE)
    {
//        if (m_activeMode == 2 && m_adCalls.size() == 2)
//        {
//            m_adhocManager->mergeIntoConference();
//        }
//        else if (m_activeMode == 4 && m_adCalls.size() == (m_mettingCalls.size() + 1))
//        {
//            m_adhocManager->mergeIntoConference();
//        }


        if (m_activeMode == 2)
        {
            int firstNet = 0;
            int secondNet = UNKNOW_NETWORK;
            bool isMetting = false;
            for (i = 0; i < m_adCalls.size(); i++)
            {
                if (handlerId == m_adCalls.at(i)->handlerId())
                {
                    secondNet = AD_HOC_NETWORK;
                }
                if (m_adCalls.at(i)->status() == STATUS_HELD)
                {
                    firstNet = m_adCalls.at(i)->network();
                }
                if (m_adCalls.at(i)->status() == STATUS_ACTIVE && m_adCalls.at(i)->handlerId() != handlerId)
                {
                    bool ok;
                    m_adhocManager->setHold(m_adCalls.at(i)->handlerId().section("/",-1).toInt(&ok, 10), true);
                }
            }
            for (i = 0; i < m_lteCalls.size(); i++)
            {
                if (handlerId == m_lteCalls.at(i)->handlerId())
                {
                    secondNet = LTE_NETWORK;
                }
                if (m_lteCalls.at(i)->status() == STATUS_HELD)
                {
                    firstNet = m_lteCalls.at(i)->network();
                }
                if (m_lteCalls.at(i)->status() == STATUS_ACTIVE && m_lteCalls.at(i)->handlerId() != handlerId)
                {
                    bool ok;
                    m_lteManager->setHold(m_lteCalls.at(i)->handlerId().section("/",-1).toInt(&ok, 10), true);                }
            }
            for (i = 0; i < m_wifiCalls.size(); i++)
            {
                if (handlerId == m_wifiCalls.at(i)->handlerId())
                {
                    secondNet = WIFI_NETWORK;
                }
                if (m_wifiCalls.at(i)->status() == STATUS_HELD)
                {
                    firstNet = m_wifiCalls.at(i)->network();
                }
                if (m_wifiCalls.at(i)->status() == STATUS_ACTIVE && m_wifiCalls.at(i)->handlerId() != handlerId)
                {

                    bool ok;
                    m_wifiManager->setHold(m_wifiCalls.at(i)->handlerId().section("/",-1).toInt(&ok, 10), true);
                }
            }

            for (i = 0; i < m_battleCalls.size(); i++)
            {
                if (handlerId == m_battleCalls.at(i)->handlerId())
                {
                    secondNet = BATTLE_NETWORK;
                }
                if (m_battleCalls.at(i)->status() == STATUS_HELD)
                {
                    firstNet = m_battleCalls.at(i)->network();
                }
                if (m_battleCalls.at(i)->status() == STATUS_ACTIVE && m_battleCalls.at(i)->handlerId() != handlerId)
                {

                    bool ok;
                    m_battleManager->setHold(m_battleCalls.at(i)->handlerId().section("/",-1).toInt(&ok, 10), true);
                }
            }

            if (m_mettingCalls.size() > 0)
            {
                isMetting = true;
                firstNet = m_mettingCalls.at(0)->network();
            }
            if (firstNet == secondNet == AD_HOC_NETWORK)
            {
                m_adhocManager->mergeIntoConference();
            }
        }

    }

    setActiveMode();
    Q_EMIT statusChanged(handlerId, status);
}

void CallManager::onAudioModeChanged()
{
    int network = UNKNOW_NETWORK;

    QObject *sender = this->sender();
    qDebug() << "onAudioModeChanged in service" << sender->objectName();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }
    else if (sender->objectName() == "wifi")
    {
        network = WIFI_NETWORK;
    }
    else if (sender->objectName() == "battle")
    {
        network = BATTLE_NETWORK;
    }
    else if (sender->objectName() == "cs")
    {
        network = SATELLITE_NETWORK;
    }

    CTS_PRINT << "network = " << network << endl;

    Q_EMIT audioModeChanged(network);
}

void CallManager::onMicrophoneMuteChanged()
{
    int network = UNKNOW_NETWORK;

    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }
    else if (sender->objectName() == "wifi")
    {
        network = WIFI_NETWORK;
    }
    else if (sender->objectName() == "battle")
    {
        network = BATTLE_NETWORK;
    }
    else if (sender->objectName() == "cs")
    {
        network = SATELLITE_NETWORK;
    }
    qDebug() << "onMicrophoneMuteChanged" << network;

    Q_EMIT microphoneMutedChanged(network);
}

void CallManager::onNetworkChanged(int network)
{
    CTS_PRINT << "network = " << network << endl;
    Q_EMIT networkChanged(network);
}

void CallManager::onServiceDisconnect(int network)
{
    CTS_PRINT << "network = " << network << endl;
    NetworkType type = (NetworkType)network;
    if(type == AD_HOC_NETWORK)
    {
        for(int i = 0; i < m_adCalls.size(); i++)
        {
            CallHandler *pHandler = m_adCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_adCalls.clear();
    }
    else if(type == LTE_NETWORK)
    {
        for(int i = 0; i < m_lteCalls.size(); i++)
        {
            CallHandler *pHandler = m_lteCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_lteCalls.clear();
    }
    else if(type == WIFI_NETWORK)
    {
        for(int i = 0; i < m_wifiCalls.size(); i++)
        {
            CallHandler *pHandler = m_wifiCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_wifiCalls.clear();
    }
    else if(type == BATTLE_NETWORK)
    {
        for(int i = 0; i < m_battleCalls.size(); i++)
        {
            CallHandler *pHandler = m_battleCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_battleCalls.clear();
    }

    Q_EMIT serviceDisconnect(network);
}

void CallManager::onConfCallFinished()
{
    int network = UNKNOW_NETWORK;

    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }
    else if (sender->objectName() == "wifi")
    {
        network = WIFI_NETWORK;
    }
    else if (sender->objectName() == "battle")
    {
        network = BATTLE_NETWORK;
    }
    else if (sender->objectName() == "cs")
    {
        network = SATELLITE_NETWORK;
    }

    m_activeMode = 3;
    QString message = "success";
    CTS_PRINT << "network = " << network << ", message = " << message << endl;
    Q_EMIT confCallFinished(network, message);
}

void CallManager::onNumberOfMultipartyParticipantsChanged()
{
    int network = UNKNOW_NETWORK;
    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }
    else if (sender->objectName() == "wifi")
    {
        network = WIFI_NETWORK;
    }
    else if (sender->objectName() == "battle")
    {
        network = BATTLE_NETWORK;
    }

    CTS_PRINT << "network = " << network << endl;

    Q_EMIT numberOfMultipartyParticipantsChanged(network);
}

void CallManager::onCsDialError(QString mess)
{
    CTS_PRINT << mess << endl;
}

//* @return "ihf"表示免提， "earpiece"表示听筒
QString CallManager::onGetAudioMode(int network)
{
    QString res;
    if (network == WIFI_NETWORK)
    {
        if (m_wifiManager->isHandsFree())
        {
            res = "ihf";
        }
        else
        {
            res = "earpiece";
        }
    }
    else if (network == LTE_NETWORK)
    {
        if (m_lteManager->isHandsFree())
        {
            res = "ihf";
        }
        else
        {
            res = "earpiece";
        }
    }
    else if (network == AD_HOC_NETWORK)
    {
        if (m_adhocManager->isHandsFree())
        {
            res = "ihf";
        }
        else
        {
            res = "earpiece";
        }
    }
    else if (network == BATTLE_NETWORK)
    {
        if (m_battleManager->isHandsFree())
        {
            res = "ihf";
        }
        else
        {
            res = "earpiece";
        }
    }
    else if (network == SATELLITE_NETWORK)
    {
        res = m_voiceManager->getAudioModeChanged();
    }
    return res;

}

void CallManager::setAudioMode(int network, QString mode)
{
    bool on;
    if (mode == "ihf")
        on = true;
    else
        on = false;
    if (network == WIFI_NETWORK)
    {
        m_wifiManager->setHandsFree(on);
    }
    else if (network == LTE_NETWORK)
    {
        m_lteManager->setHandsFree(on);
    }
    else if (network == AD_HOC_NETWORK)
    {
        m_adhocManager->setHandsFree(on);
    }
    else if (network == BATTLE_NETWORK)
    {
        m_battleManager->setHandsFree(on);
    }
    else if (network == SATELLITE_NETWORK)
    {
        m_voiceManager->setHandsFree(on);
    }
}

bool CallManager::isMicrophoneMuted(int network)
{
    bool on;
    if (network == WIFI_NETWORK)
    {
        on = m_wifiManager->isMute();
    }
    else if (network == LTE_NETWORK)
    {
        on = m_lteManager->isMute();
    }
    else if (network == AD_HOC_NETWORK)
    {
        on = m_adhocManager->isMute();
    }
    else if (network == BATTLE_NETWORK)
    {
        on = m_battleManager->isMute();
    }
    else if (network == SATELLITE_NETWORK)
    {
        on = m_voiceManager->isMute();
    }
    return on;
}

void CallManager::setMuteMicrophone(int network, bool on)
{
    if (network == WIFI_NETWORK)
    {
        m_wifiManager->setMute(on);
    }
    else if (network == LTE_NETWORK)
    {
        m_lteManager->setMute(on);
    }
    else if (network == AD_HOC_NETWORK)
    {
        m_adhocManager->setMute(on);
    }
    else if (network == BATTLE_NETWORK)
    {
        m_battleManager->setMute(on);
    }
    else if (network == SATELLITE_NETWORK)
    {
        m_voiceManager->setMute(on);
    }
}

void CallManager::onCreateConfCall(int network)
{
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->mergeIntoConference();
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->mergeIntoConference();
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->mergeIntoConference();
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->mergeIntoConference();
    }
    else if (network == SATELLITE_NETWORK)
    {
        return m_voiceManager->mergeIntoConference();
    }
}

int CallManager::setVidEnabledChanged(int network, QString callId, bool on)
{
    bool ok;
    int handlerId = callId.section("/",-1).toInt(&ok, 10);

    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->setVidEnabledChanged(handlerId, on);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->setVidEnabledChanged(handlerId, on);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->setVidEnabledChanged(handlerId, on);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->setVidEnabledChanged(handlerId, on);
    }
}

int CallManager::setVidInputChanged(int network, QString callId)
{
    bool ok;
    int handlerId = callId.section("/",-1).toInt(&ok, 10);

    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->setVidInputChanged(handlerId);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->setVidInputChanged(handlerId);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->setVidInputChanged(handlerId);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->setVidInputChanged(handlerId);
    }
}

int CallManager::getGetAudOrVideoInt(int network, int callId)
{
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->getGetAudOrVideo(callId);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->getGetAudOrVideo(callId);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->getGetAudOrVideo(callId);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->getGetAudOrVideo(callId);
    }
    else if (network == SATELLITE_NETWORK)
    {
        return 0;
    }
}

int CallManager::getGetAudOrVideo(int network, QString callId)
{
    bool ok;
    int handlerId = callId.section("/",-1).toInt(&ok, 10);
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->getGetAudOrVideo(handlerId);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->getGetAudOrVideo(handlerId);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->getGetAudOrVideo(handlerId);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->getGetAudOrVideo(handlerId);
    }
    else if (network == SATELLITE_NETWORK)
    {
        return 0;
    }
}

unsigned CallManager::joinGroup(int network, QString groupAddr, unsigned port)
{
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->joinGroup(groupAddr, port);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->joinGroup(groupAddr, port);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->joinGroup(groupAddr, port);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->joinGroup(groupAddr, port);
    }
}

int CallManager::leaveGroup(int network, unsigned groupId)
{
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->leaveGroup(groupId);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->leaveGroup(groupId);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->leaveGroup(groupId);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->leaveGroup(groupId);
    }
}

int CallManager::getPttState(int network, unsigned groupId)
{
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->getPttState(groupId);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->getPttState(groupId);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->getPttState(groupId);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->getPttState(groupId);
    }
}

int CallManager::setPttState(int network, unsigned groupId, int state)
{
    if (network == WIFI_NETWORK)
    {
        return m_wifiManager->setPttState(groupId, state);
    }
    else if (network == LTE_NETWORK)
    {
        return m_lteManager->setPttState(groupId, state);
    }
    else if (network == AD_HOC_NETWORK)
    {
        return m_adhocManager->setPttState(groupId, state);
    }
    else if (network == BATTLE_NETWORK)
    {
        return m_battleManager->setPttState(groupId, state);
    }
}

void CallManager::onPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    qDebug("onPttRecordFileFinished");
    emit signalPttRecordFileFinished(groupId, fileName);
}

void CallManager::baoTongDial(QString phoneNum)
{
    qDebug() << "baoTongDialxx";
    m_baotongManager->dial(phoneNum, 1, "single", "normal");
    m_baoTongNum = phoneNum;
}

void CallManager::baoTongHangUp()
{
    m_baotongManager->hangup();
}

void CallManager::baoTongSet(bool on)
{
    qDebug() << "baoTongSetxx ";
    if (on == true && m_baotongManager->status() == "disconnected")
        baoTongDial(m_baoTongNum);
    m_baotongManager->pttSet(on);
}

bool CallManager::baoTongGet()
{
    return m_baotongManager->pttGetted();
}

QString CallManager::baoTongStatus()
{
    return m_baotongManager->status();
}

bool CallManager::baoTongPttAllow()
{
    return m_baotongManager->pttAllow();
}

//添加通话时的逻辑,保持原通话，再拨打电话 down
void CallManager::dial(UserInfo user)
{

    CTS_PRINT << "CallManager::dial" << "userId = " << user.userId << "network = " << user.network
              << "addr = " << user.addr << "appId = " << user.appId << "optId = " << user.optId
              << "userName = " << user.userName << "lte_ip = " << user.lte_ip << "ad_hoc_ip = "
              << user.ad_hoc_ip << "satellite_ip = " << user.satellite_ip << "satellite_phone_number = "
              << user.satellite_phone_number << "digital_number = " << user.digital_number
              << "battle_ip = " << user.battle_ip << user.wifi_ip << endl;


    int i;
    int count = m_adCalls.size() + m_lteCalls.size() + m_wifiCalls.size() + m_csCalls.size();
    if (m_mettingCalls.size() == 0 && count > 0)
    {
        for (i = 0; i < m_adCalls.size(); i++)
        {
            if (m_adCalls.at(i)->status() == STATUS_ACTIVE)
            {
                m_adCalls.at(i)->hold(true);
            }
        }
        for (i = 0; i < m_lteCalls.size(); i++)
        {
            if (m_lteCalls.at(i)->status() == STATUS_ACTIVE)
            {
                m_lteCalls.at(i)->hold(true);
            }
        }
        for (i = 0; i < m_wifiCalls.size(); i++)
        {
            if (m_wifiCalls.at(i)->status() == STATUS_ACTIVE)
            {
                m_wifiCalls.at(i)->hold(true);
            }
        }
        for (i = 0; i < m_battleCalls.size(); i++)
        {
            if (m_battleCalls.at(i)->status() == STATUS_ACTIVE)
            {
                m_battleCalls.at(i)->hold(true);
            }
        }
        for (i = 0; i < m_csCalls.size(); i++)
        {
            if (m_csCalls.at(i)->status() == STATUS_ACTIVE)
            {
                m_csCalls.at(i)->hold(true);
            }
        }
    }
    else if (m_mettingCalls.size() > 0)
    {
        if (m_mettingCalls.at(0)->network() == AD_HOC_NETWORK)
        {
            m_adhocManager->hangupConference();
        }
        else if(m_mettingCalls.at(0)->network() == LTE_NETWORK)
        {
            m_lteManager->hangupConference();
        }
        else if(m_mettingCalls.at(0)->network() == WIFI_NETWORK)
        {
            m_wifiManager->setHoldConference(true);
        }
        else if(m_mettingCalls.at(0)->network() == BATTLE_NETWORK)
        {
            m_battleManager->setHoldConference(true);
        }
        else if(m_mettingCalls.at(0)->network() == SATELLITE_NETWORK)
        {
            m_voiceManager->setHoldConference(true);
        }
    }

    if(user.network == AD_HOC_NETWORK)
    {
        m_adhocManager->makeCall(user.addr);
    }
    else if(user.network == LTE_NETWORK)
    {
        m_lteManager->makeCall(user.addr);
    }
    else if(user.network == WIFI_NETWORK)
    {
        m_wifiManager->makeCall(user.addr);
    }
    else if(user.network == BATTLE_NETWORK)
    {
        m_battleManager->makeCall(user.addr);
    }
    else if(user.network == SATELLITE_NETWORK)
    {
        m_voiceManager->makeCall(user.addr);
    }
}

int CallManager::onMakeVideo(UserInfo user)
{
    CTS_PRINT << "onMakeVideo" << user.network;
    if(user.network == AD_HOC_NETWORK)
    {
        return m_adhocManager->makeVideoCall(user.addr);
    }
    else if(user.network == LTE_NETWORK)
    {
        return m_lteManager->makeVideoCall(user.addr);
    }
    else if(user.network == WIFI_NETWORK)
    {
        return m_wifiManager->makeVideoCall(user.addr);
    }
    else if(user.network == BATTLE_NETWORK)
    {
        return m_battleManager->makeVideoCall(user.addr);
    }
}

void CallManager::onDialError(int handlerId)
{
    CTS_PRINT << "error = " << handlerId << endl;
    Q_EMIT dialError(handlerId);
}

void CallManager::record(int network, bool on)
{
    if(network == AD_HOC_NETWORK)
    {
        m_adhocManager->record(on);
    }
    else if(network == LTE_NETWORK)
    {
        m_lteManager->record(on);
    }
    else if (network == WIFI_NETWORK)
    {
        m_wifiManager->record(on);
    }
    else if (network == BATTLE_NETWORK)
    {
        m_battleManager->record(on);
    }
//    else if (network == SATELLITE_NETWORK)
//    {
//        m_voiceManager->record(on);
//    }
}

void CallManager::onHangupConference(int network)
{
    CTS_PRINT <<network<< endl;

    if(network == AD_HOC_NETWORK)
    {
        m_adhocManager->hangupConference();
    }
    else if(network == LTE_NETWORK)
    {
        m_lteManager->hangupConference();
    }
    else if (network == WIFI_NETWORK)
    {
        m_wifiManager->hangupConference();
    }
    else if (network == BATTLE_NETWORK)
    {
        m_battleManager->hangupConference();
    }
    else if (network == SATELLITE_NETWORK)
    {
        m_voiceManager->hangupConference();
    }
}

//添加到m_metting，并发送信号到界面 down
void CallManager::onMultipartyChanged(QString handlerId, bool isMultiparty, int network)
{
    CTS_PRINT << handlerId << isMultiparty << network;
    int i;
    if (isMultiparty)
    {
        if (m_mettingCalls.size() == 0)
        {
            mettingStart = QDateTime::currentDateTime();
        }
        switch (network) {
        case AD_HOC_NETWORK:
            for (i = 0 ;i < m_adCalls.size();i++)
            {
                if (handlerId == m_adCalls.at(i)->handlerId()){
                    m_mettingTarget.append(m_adCalls.at(i)->dialNumber());
                    m_mettingCalls.append(m_adCalls.at(i));
                    break;
                }
            }
            break;
        case LTE_NETWORK:
            for (i = 0 ;i < m_lteCalls.size();i++)
            {
                if (handlerId == m_lteCalls.at(i)->handlerId()){
                    m_mettingTarget.append(m_adCalls.at(i)->dialNumber());
                    m_mettingCalls.append(m_lteCalls.at(i));
                    break;
                }
            }
            break;
        case WIFI_NETWORK:
            for (i = 0 ;i < m_wifiCalls.size();i++)
            {
                if (handlerId == m_wifiCalls.at(i)->handlerId()){
                    m_mettingTarget.append(m_wifiCalls.at(i)->dialNumber());
                    m_mettingCalls.append(m_wifiCalls.at(i));
                    break;
                }
            }
            break;
        case BATTLE_NETWORK:
            for (i = 0 ;i < m_battleCalls.size();i++)
            {
                if (handlerId == m_battleCalls.at(i)->handlerId()){
                    m_mettingTarget.append(m_battleCalls.at(i)->dialNumber());
                    m_mettingCalls.append(m_battleCalls.at(i));
                    break;
                }
            }
            break;
        case SATELLITE_NETWORK:
            for (i = 0 ;i < m_csCalls.size();i++)
            {
                if (handlerId == m_csCalls.at(i)->handlerId()){
                    m_mettingTarget.append(m_wifiCalls.at(i)->dialNumber());
                    m_mettingCalls.append(m_wifiCalls.at(i));
                    break;
                }
            }
            break;
        default:
            break;
        }
    }

    Q_EMIT multipartyChanged(handlerId,isMultiparty);
}

int CallManager::onGetActiveMode()
{
    CTS_PRINT << m_activeMode;
    return m_activeMode;
}

int CallManager::onSetHoldConference(int network, bool on)
{
    CTS_PRINT << endl;
    if (network == WIFI_NETWORK)
        m_wifiManager->setHoldConference(on);
    else if (network == AD_HOC_NETWORK)
        m_adhocManager->setHoldConference(on);
    else if (network == LTE_NETWORK)
        m_lteManager->setHoldConference(on);
    else if (network == BATTLE_NETWORK)
        m_battleManager->setHoldConference(on);
}
