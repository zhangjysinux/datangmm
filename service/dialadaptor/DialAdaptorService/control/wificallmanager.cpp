#include "wificallmanager.h"
#include "voipserviceadaptor.h"
#include "voipcallhandler.h"
#include <QTimer>

WifiCallManager *WifiCallManager::m_instance;

WifiCallManager *WifiCallManager::instance()
{
    if (m_instance == 0)
    {
        m_instance = new WifiCallManager;
    }

    return m_instance;
}

VoipCallHandler *WifiCallManager::lookupHandler(int callId)
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

bool WifiCallManager::isMute()
{
    return m_voipServiceAdaptor->isMute();
}

bool WifiCallManager::isHandsFree()
{
    return m_voipServiceAdaptor->isHandsFree();
}

void WifiCallManager::initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    m_voipServiceAdaptor->initialize(sipPort, rtpPort, rtpPortRange);
}

int WifiCallManager::makeCall(const QString &ipAddr)
{
    int callId = -1;

    if ((callId = m_voipServiceAdaptor->makeCall(ipAddr)) >= 0)
    {
        VoipCallHandler *handler = new VoipCallHandler(true, callId, false, ipAddr);
        m_handlers.push_back(handler);
        emit signalNewCall(callId);
    }

    return callId;
}


int WifiCallManager::answer(int callId)
{
    int error = -1;
    error = m_voipServiceAdaptor->answer(callId);
    return error;
}

int WifiCallManager::hangup(int callId)
{
    int error = -1;
    error = m_voipServiceAdaptor->hangup(callId);
    return error;
}
int WifiCallManager::setHold(int callId, bool on)
{
    int error = -1;
    error = m_voipServiceAdaptor->setHold(callId,on);
    return error;
}

int WifiCallManager::getCallStatus(int callId)
{

}

QString WifiCallManager::getCallIpAddr(int callId)
{
    QString result;
    result = m_voipServiceAdaptor->getCallIpAddr(callId);
    return result;

}
bool WifiCallManager::isCallIncoming(int callId)
{
    bool result = false;
    result = m_voipServiceAdaptor->isCallIncoming(callId);
    return result;
}
bool WifiCallManager::isCallInConference(int callId)
{
    bool result = false;
    result = m_voipServiceAdaptor->isCallInConference(callId);
    return result;
}
void WifiCallManager::hangupConference()
{
    m_voipServiceAdaptor->hangupConference();
}

void WifiCallManager::hangupAllCalls()
{
    m_voipServiceAdaptor->hangupAllCalls();
}

void WifiCallManager::setMute(bool on)
{
    m_voipServiceAdaptor->setMute(on);
}

void WifiCallManager::setHandsFree(bool on)
{
    m_voipServiceAdaptor->setHandsFree(on);
}

void WifiCallManager::record(bool on)
{
    m_voipServiceAdaptor->record(on);
}

void WifiCallManager::mergeIntoConference()
{
    m_voipServiceAdaptor->mergeIntoConference();
}

int WifiCallManager::numberOfMultipartyParticipants()
{
    return m_voipServiceAdaptor->getConferenceParticipants();
}

void WifiCallManager::onIncomingCall(int callId, QString remoteIpAddr)
{
    Q_UNUSED(remoteIpAddr)

    VoipCallHandler *handler = new VoipCallHandler(true, callId, true);
    m_handlers.push_back(handler);
    emit signalNewCall(callId);
}

void WifiCallManager::onCallStatusChanged(int callId, bool isDisconnected)
{
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

void WifiCallManager::onMuteChanged()
{
    emit signalMuteChanged();
}



void WifiCallManager::onCallIsInConference(int callId)
{
    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitMultipartyChanged();
    }
}

void WifiCallManager::onConferenceEstablished()
{
    emit signalConferenceEstablished();
}

void WifiCallManager::onConferenceParticipantsChanged()
{
    emit numberOfMultipartyParticipantsChanged();
}

void WifiCallManager::onCallState(int callId, int callState)
{

}



void WifiCallManager::onHandsFreeChanged()
{
    emit signalHandsFreeChanged();
}

void WifiCallManager::onVoipFrameData()
{

}
void WifiCallManager::onV4l2FrameData()
{

}


WifiCallManager::WifiCallManager(QObject *parent) :
    QObject(parent), m_voipServiceAdaptor(new VoipServiceAdaptor("com.sinux.voipLte", this))
{
    connect(m_voipServiceAdaptor, SIGNAL(signalIncomingCall(int,QString)), this, SLOT(onIncomingCall(int,QString)));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallStatusChanged(int,bool)), this, SLOT(onCallStatusChanged(int,bool)));
    connect(m_voipServiceAdaptor, SIGNAL(signalMuteChanged()), this, SLOT(onMuteChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalHandsFreeChanged()), this, SLOT(onHandsFreeChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallIsInConference(int)), this, SLOT(onCallIsInConference(int)));
    connect(m_voipServiceAdaptor, SIGNAL(signalConferenceEstablished()), this, SLOT(onConferenceEstablished()));
    connect(m_voipServiceAdaptor, SIGNAL(signalConferenceParticipantsChanged()), this, SLOT(onConferenceParticipantsChanged()));
}

void WifiCallManager::removeHandler(VoipCallHandler *handler)
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
