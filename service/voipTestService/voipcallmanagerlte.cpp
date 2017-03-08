#include "voipcallmanagerlte.h"
#include "voipserviceadaptor.h"
#include "voipcallhandler.h"

VoipCallManagerLte *VoipCallManagerLte::m_instance;

VoipCallManagerLte *VoipCallManagerLte::instance()
{
    if (m_instance == 0)
    {
        m_instance = new VoipCallManagerLte;
    }

    return m_instance;
}

VoipCallHandler *VoipCallManagerLte::lookupHandler(int callId)
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

bool VoipCallManagerLte::isMute()
{
    return m_voipServiceAdaptor->isMute();
}

bool VoipCallManagerLte::isHandsFree()
{
    return m_voipServiceAdaptor->isHandsFree();
}

void VoipCallManagerLte::initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    m_voipServiceAdaptor->initialize(sipPort, rtpPort, rtpPortRange);
}

int VoipCallManagerLte::makeCall(const QString &ipAddr)
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

void VoipCallManagerLte::hangupConference()
{
    m_voipServiceAdaptor->hangupConference();
}

void VoipCallManagerLte::hangupAllCalls()
{
    m_voipServiceAdaptor->hangupAllCalls();
}

void VoipCallManagerLte::setMute(bool on)
{
    m_voipServiceAdaptor->setMute(on);
}

void VoipCallManagerLte::setHandsFree(bool on)
{
    m_voipServiceAdaptor->setHandsFree(on);
}

void VoipCallManagerLte::record(bool on)
{
    m_voipServiceAdaptor->record(on);
}

void VoipCallManagerLte::mergeIntoConference()
{
    m_voipServiceAdaptor->mergeIntoConference();
}

int VoipCallManagerLte::numberOfMultipartyParticipants()
{
    return m_voipServiceAdaptor->getConferenceParticipants();
}

void VoipCallManagerLte::onIncomingCall(int callId, QString remoteIpAddr)
{
    Q_UNUSED(remoteIpAddr)

    VoipCallHandler *handler = new VoipCallHandler(true, callId, true);
    m_handlers.push_back(handler);
    emit signalNewCall(callId);
}

void VoipCallManagerLte::onCallStatusChanged(int callId, bool isDisconnected)
{
    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitStatusChangedSignal();
        if (isDisconnected)
        {
            removeHandler(handler);
            delete handler;
        }
    }
}

void VoipCallManagerLte::onMuteChanged()
{
    emit signalMuteChanged();
}

void VoipCallManagerLte::onHandsFreeChanged()
{
    emit signalHandsFreeChanged();
}

void VoipCallManagerLte::onCallIsInConference(int callId)
{
    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitMultipartyChanged();
    }
}

void VoipCallManagerLte::onConferenceEstablished()
{
    emit signalConferenceEstablished();
}

void VoipCallManagerLte::onConferenceParticipantsChanged()
{
    emit numberOfMultipartyParticipantsChanged();
}

VoipCallManagerLte::VoipCallManagerLte(QObject *parent) :
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

void VoipCallManagerLte::removeHandler(VoipCallHandler *handler)
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
