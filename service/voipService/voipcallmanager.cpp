#include "voipcallmanager.h"
#include "voipserviceadaptor.h"
#include "voipcallhandler.h"
#include <QTimer>

VoipCallManager *VoipCallManager::m_instanceWifi = NULL;
VoipCallManager *VoipCallManager::m_instanceAdHoc = NULL;
VoipCallManager *VoipCallManager::m_instanceLte = NULL;
VoipCallManager *VoipCallManager::m_instanceBattle = NULL;

VoipCallManager *VoipCallManager::instanceWifi()
{
    if (m_instanceWifi == 0)
    {
        m_instanceWifi = new VoipCallManager("com.sinux.voipWifi");
        m_instanceWifi->setNetworkType(WIFI_NETWORK);
    }

    return m_instanceWifi;
}

VoipCallManager *VoipCallManager::instanceAdHoc()
{
    if (m_instanceAdHoc == 0)
    {
        m_instanceAdHoc = new VoipCallManager("com.sinux.voipAdHoc");
        m_instanceAdHoc->setNetworkType(AD_HOC_NETWORK);
    }

    return m_instanceAdHoc;
}

VoipCallManager *VoipCallManager::instanceLte()
{
    if (m_instanceLte == 0)
    {
        m_instanceLte = new VoipCallManager("com.sinux.voipLte");
        m_instanceLte->setNetworkType(LTE_NETWORK);
    }

    return m_instanceLte;
}

VoipCallManager *VoipCallManager::instanceBattle()
{
    if (m_instanceBattle == 0)
    {
        m_instanceBattle = new VoipCallManager("com.sinux.voipBattle");
        m_instanceBattle->setNetworkType(BATTLE_NETWORK);
    }
    return m_instanceBattle;
}

VoipCallHandler *VoipCallManager::lookupHandler(int callId)
{
    VoipCallHandler *result = 0;
    for (QList<VoipCallHandler *>::iterator it = m_handlers.begin();
         it != m_handlers.end(); ++it)
    {
        if ((*it)->m_callId == callId)
        {
            result = *it;
            break;
        }
    }

    return result;
}

bool VoipCallManager::isMute()
{
    CTS_PRINT << endl;

    return m_voipServiceAdaptor->isMute();
}

bool VoipCallManager::isHandsFree()
{
    CTS_PRINT << endl;

    return m_voipServiceAdaptor->isHandsFree();
}

void VoipCallManager::initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    m_voipServiceAdaptor->initialize(sipPort, rtpPort, rtpPortRange);
}

int VoipCallManager::makeCall(const QString &ipAddr)
{
    CTS_PRINT << endl;

    int callId = -1;

    if ((callId = m_voipServiceAdaptor->makeCall(ipAddr)) >= 0)
    {
        VoipCallHandler *handler = new VoipCallHandler(true, callId, false, ipAddr ,m_netType);
        m_handlers.push_back(handler);
        emit signalNewCall(callId);
    }

    return callId;
}

int VoipCallManager::makeVideoCall(QString ipAddr)
{
    CTS_PRINT << endl;

    int callId = -1;
    if ((callId = m_voipServiceAdaptor->makeVideoCall(ipAddr)) >= 0)
    {
        VoipCallHandler *handler = new VoipCallHandler(true, callId, false, ipAddr, m_netType);
        handler->SetVideo(true);
        m_handlers.push_back(handler);
        emit signalNewCall(callId);
    }

    return callId;
}

int VoipCallManager::answer(int callId)
{
    CTS_PRINT << endl;

    int error = -1;
    error = m_voipServiceAdaptor->answer(callId);
    return error;
}

int VoipCallManager::hangup(int callId)
{
    CTS_PRINT << endl;

    int error = -1;
    error = m_voipServiceAdaptor->hangup(callId);
    return error;
}
int VoipCallManager::setHold(int callId, bool on)
{
    int error = -1;
    error = m_voipServiceAdaptor->setHold(callId,on);
    return error;
}

int VoipCallManager::setHoldConference(bool on)
{
    CTS_PRINT << endl;

    int error = -1;
    error = m_voipServiceAdaptor->setHoldConference(on);
    return error;
}

int VoipCallManager::getCallStatus(int callId)
{
    CTS_PRINT << endl;

}

QString VoipCallManager::getCallIpAddr(int callId)
{
    CTS_PRINT << endl;

    QString result;
    result = m_voipServiceAdaptor->getCallIpAddr(callId);
    return result;
}

bool VoipCallManager::isCallIncoming(int callId)
{
    bool result = false;
    result = m_voipServiceAdaptor->isCallIncoming(callId);
    return result;
}

bool VoipCallManager::isCallInConference(int callId)
{
    bool result = false;
    result = m_voipServiceAdaptor->isCallInConference(callId);
    return result;
}

void VoipCallManager::hangupConference()
{
    m_voipServiceAdaptor->hangupConference();
}

void VoipCallManager::hangupAllCalls()
{
    m_voipServiceAdaptor->hangupAllCalls();
}

void VoipCallManager::setMute(bool on)
{
    CTS_PRINT << endl;

    m_voipServiceAdaptor->setMute(on);
}

void VoipCallManager::setHandsFree(bool on)
{
    CTS_PRINT << endl;

    m_voipServiceAdaptor->setHandsFree(on);
}

void VoipCallManager::record(bool on)
{
    CTS_PRINT << endl;

    m_voipServiceAdaptor->record(on);
}

void VoipCallManager::mergeIntoConference()
{
    CTS_PRINT << "xxxx" << endl;

    m_voipServiceAdaptor->mergeIntoConference();
}

int VoipCallManager::numberOfMultipartyParticipants()
{
    CTS_PRINT << endl;

    return m_voipServiceAdaptor->getConferenceParticipants();
}

int VoipCallManager::setVidEnabledChanged(int callId, bool on)
{
    CTS_PRINT << endl;
    return m_voipServiceAdaptor->VidEnabledChanged(callId,on);
}

int VoipCallManager::setVidInputChanged(int callId)
{
    CTS_PRINT << endl;
    return m_voipServiceAdaptor->VidInputChanged(callId);
}

int VoipCallManager::getGetAudOrVideo(int callId)//0:audio 1:video
{
    CTS_PRINT << m_voipServiceAdaptor->GetAudOrVideo(callId) << "xxxxxxxxxxx" << endl;
    return m_voipServiceAdaptor->GetAudOrVideo(callId);
}

unsigned VoipCallManager::joinGroup(QString groupAddr, unsigned port)
{
    CTS_PRINT << endl;
    return m_voipServiceAdaptor->joinGroup(groupAddr, port);
}

int VoipCallManager::leaveGroup(unsigned groupId)
{
    CTS_PRINT << endl;
    return m_voipServiceAdaptor->leaveGroup(groupId);
}

int VoipCallManager::getPttState(unsigned groupId)
{
    CTS_PRINT << endl;

    return m_voipServiceAdaptor->getPttState(groupId);
}

int VoipCallManager::setPttState(unsigned groupId, int state)
{
    CTS_PRINT << endl;

    return m_voipServiceAdaptor->setPttState(groupId, state);
}

void VoipCallManager::onIncomingCall(int callId, QString remoteIpAddr)
{
    CTS_PRINT << endl;

    Q_UNUSED(remoteIpAddr)
    VoipCallHandler *handler = new VoipCallHandler(true, callId, true, remoteIpAddr, m_netType);
    m_handlers.push_back(handler);
    emit signalNewCall(callId);
}

void VoipCallManager::onCallStatusChanged(int callId, bool isDisconnected)
{
    CTS_PRINT << endl;

    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitStatusChangedSignal();
        if (isDisconnected)
        {
            removeHandler(handler);
            QTimer::singleShot(30000, handler, SLOT(deleteLater()));
        }
    }
}

void VoipCallManager::onMuteChanged()
{
    CTS_PRINT << endl;

    emit signalMuteChanged();
}



void VoipCallManager::onCallIsInConference(int callId)
{
    CTS_PRINT << endl;

    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitMultipartyChanged();
    }
}

void VoipCallManager::onConferenceEstablished()
{
    CTS_PRINT << endl;

    emit signalConferenceEstablished();
}

void VoipCallManager::onConferenceParticipantsChanged()
{
    CTS_PRINT << endl;

    emit numberOfMultipartyParticipantsChanged();
}

void VoipCallManager::onCallState(int callId, int callState)
{
    CTS_PRINT << endl;

}



void VoipCallManager::onHandsFreeChanged()
{
    emit signalHandsFreeChanged();
}

void VoipCallManager::onVoipFrameData()
{
    emit signalVoipFrameData();
}
void VoipCallManager::onV4l2FrameData()
{
    emit signalV4l2FrameData();
}
void VoipCallManager::onsignalVidChanged(bool isVideoHold)
{
    CTS_PRINT << isVideoHold << endl;

    emit signalVidChanged(isVideoHold);
//    VoipCallHandler *handler = lookupHandler(callId);
//    if (handler)
//    {
//        handler->SetVideo(!isVideoHold);
    //    }
}

void VoipCallManager::onSignalCallError(int callId)
{
    CTS_PRINT << "onSignalCallError: " << callId;
    emit signalCallError(callId);
}

VoipCallManager::VoipCallManager(const QString &serviceName, QObject *parent) :
    QObject(parent), m_voipServiceAdaptor(new VoipServiceAdaptor(serviceName, this))
{
    connect(m_voipServiceAdaptor, SIGNAL(signalIncomingCall(int,QString)), this, SLOT(onIncomingCall(int,QString)));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallStatusChanged(int,bool)), this, SLOT(onCallStatusChanged(int,bool)));
    connect(m_voipServiceAdaptor, SIGNAL(signalMuteChanged()), this, SLOT(onMuteChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalHandsFreeChanged()), this, SLOT(onHandsFreeChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallIsInConference(int)), this, SLOT(onCallIsInConference(int)));
    connect(m_voipServiceAdaptor, SIGNAL(signalConferenceEstablished()), this, SLOT(onConferenceEstablished()));
    connect(m_voipServiceAdaptor, SIGNAL(signalConferenceParticipantsChanged()), this, SLOT(onConferenceParticipantsChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalVoipFrameData()), this, SLOT(onVoipFrameData()));
    connect(m_voipServiceAdaptor, SIGNAL(signalV4l2FrameData()), this, SLOT(onV4l2FrameData()));
    connect(m_voipServiceAdaptor, SIGNAL(signalVidChanged(bool)), this, SLOT(onsignalVidChanged(bool)));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallError(int)), this, SLOT(onSignalCallError(int)));
    //ptt
    connect(m_voipServiceAdaptor, SIGNAL(signalPttStateChanged(uint,int)), this, SIGNAL(signalPttStateChanged(uint,int)));
    connect(m_voipServiceAdaptor, SIGNAL(signalPttRecordFileFinished(uint,QString)), this, SIGNAL(signalPttRecordFileFinished(uint,QString)));
}

void VoipCallManager::removeHandler(VoipCallHandler *handler)
{
    for (QList<VoipCallHandler *>::iterator it = m_handlers.begin();
         it != m_handlers.end(); ++it)
    {
        if (handler == *it)
        {
            m_handlers.erase(it);
            break;
        }
    }
}

void VoipCallManager::setNetworkType(NetworkType netType)
{
    m_netType = netType;
}
