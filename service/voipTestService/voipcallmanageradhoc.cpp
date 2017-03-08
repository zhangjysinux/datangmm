#include "voipcallmanageradhoc.h"
#include "voipserviceadaptor.h"
#include "voipcallhandler.h"

VoipCallManagerAdHoc *VoipCallManagerAdHoc::m_instance;

VoipCallManagerAdHoc *VoipCallManagerAdHoc::instance()
{
    if (m_instance == 0)
    {
        m_instance = new VoipCallManagerAdHoc;
    }

    return m_instance;
}

VoipCallHandler *VoipCallManagerAdHoc::lookupHandler(int callId)
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

bool VoipCallManagerAdHoc::isMute()
{
    return m_voipServiceAdaptor->isMute();
}

bool VoipCallManagerAdHoc::isHandsFree()
{
    return m_voipServiceAdaptor->isHandsFree();
}

void VoipCallManagerAdHoc::initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    m_voipServiceAdaptor->initialize(sipPort, rtpPort, rtpPortRange);
}

int VoipCallManagerAdHoc::makeCall(const QString &ipAddr)
{
    int callId = -1;

    if ((callId = m_voipServiceAdaptor->makeCall(ipAddr)) >= 0)
    {
        VoipCallHandler *handler = new VoipCallHandler(false, callId, false, ipAddr);
        m_handlers.push_back(handler);
        emit signalNewCall(callId);
    }

    return callId;
}

void VoipCallManagerAdHoc::hangupConference()
{
    m_voipServiceAdaptor->hangupConference();
}

void VoipCallManagerAdHoc::hangupAllCalls()
{
    m_voipServiceAdaptor->hangupAllCalls();
}

void VoipCallManagerAdHoc::setMute(bool on)
{
    m_voipServiceAdaptor->setMute(on);
}

void VoipCallManagerAdHoc::setHandsFree(bool on)
{
    m_voipServiceAdaptor->setHandsFree(on);
}

void VoipCallManagerAdHoc::record(bool on)
{
    m_voipServiceAdaptor->record(on);
}

int VoipCallManagerAdHoc::numberOfMultipartyParticipants()
{
    return m_voipServiceAdaptor->getConferenceParticipants();
}

void VoipCallManagerAdHoc::onIncomingCall(int callId, QString remoteIpAddr)
{
    Q_UNUSED(remoteIpAddr)

    VoipCallHandler *handler = new VoipCallHandler(false, callId, true);
    m_handlers.push_back(handler);
    emit signalNewCall(callId);
}

void VoipCallManagerAdHoc::onCallStatusChanged(int callId, bool isDisconnected)
{
    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitStatusChangedSignal();
        if (isDisconnected)
        {
            removeHandler(handler);
            handler->deleteLater();
        }
    }
}

void VoipCallManagerAdHoc::onMuteChanged()
{
    emit signalMuteChanged();
}

void VoipCallManagerAdHoc::onHandsFreeChanged()
{
    emit signalHandsFreeChanged();
}

void VoipCallManagerAdHoc::onCallIsInConference(int callId)
{
    VoipCallHandler *handler = lookupHandler(callId);
    if (handler)
    {
        handler->emitMultipartyChanged();
    }
}

void VoipCallManagerAdHoc::onConferenceParticipantsChanged()
{
    emit numberOfMultipartyParticipantsChanged();
}

VoipCallManagerAdHoc::VoipCallManagerAdHoc(QObject *parent) :
    QObject(parent), m_voipServiceAdaptor(new VoipServiceAdaptor("com.sinux.voipAdHoc", this))
{
    connect(m_voipServiceAdaptor, SIGNAL(signalIncomingCall(int,QString)), this, SLOT(onIncomingCall(int,QString)));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallStatusChanged(int,bool)), this, SLOT(onCallStatusChanged(int,bool)));
    connect(m_voipServiceAdaptor, SIGNAL(signalMuteChanged()), this, SLOT(onMuteChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalHandsFreeChanged()), this, SLOT(onHandsFreeChanged()));
    connect(m_voipServiceAdaptor, SIGNAL(signalCallIsInConference(int)), this, SLOT(onCallIsInConference(int)));
    connect(m_voipServiceAdaptor, SIGNAL(signalConferenceParticipantsChanged()), this, SLOT(onConferenceParticipantsChanged()));
}

void VoipCallManagerAdHoc::removeHandler(VoipCallHandler *handler)
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
