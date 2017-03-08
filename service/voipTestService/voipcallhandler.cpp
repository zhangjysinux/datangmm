#include "voipcallhandler.h"
#include "voipcallmanageradhoc.h"
#include "voipcallmanagerlte.h"
#include "voipserviceadaptor.h"

VoipCallHandler::VoipCallHandler(bool isLte, int callId, bool isIncoming, const QString &lineId, QObject *parent) :
    QObject(parent), m_isLte(isLte), m_callId(callId), m_isIncoming(isIncoming), m_lineId(lineId)
{
}

QString VoipCallHandler::handlerId() const
{
    return QString::number(m_callId);
}

int VoipCallHandler::status() const
{
    if (m_isLte)
    {
        return VoipCallManagerLte::instance()->m_voipServiceAdaptor->getCallStatus(m_callId);
    }
    else
    {
        return VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->getCallStatus(m_callId);
    }
}

QString VoipCallHandler::lineId() const
{
    if (!m_lineId.isEmpty())
        return m_lineId;

    if (m_isLte)
    {
        return VoipCallManagerLte::instance()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    }
    else
    {
        return VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    }
}

bool VoipCallHandler::isIncoming() const
{
    return m_isIncoming;
/*
    if (m_isLte)
    {
        return VoipCallManagerLte::instance()->m_voipServiceAdaptor->isCallIncoming(m_callId);
    }
    else
    {
        return VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->isCallIncoming(m_callId);
    }
*/
}

bool VoipCallHandler::isMultiparty() const
{
    if (m_isLte)
    {
        return VoipCallManagerLte::instance()->m_voipServiceAdaptor->isCallInConference(m_callId);
    }
    else
    {
        return VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->isCallInConference(m_callId);
    }
}

void VoipCallHandler::answer()
{
    if (m_isLte)
    {
        VoipCallManagerLte::instance()->m_voipServiceAdaptor->answer(m_callId);
    }
    else
    {
        VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->answer(m_callId);
    }
}

void VoipCallHandler::hangup()
{
    if (m_isLte)
    {
        VoipCallManagerLte::instance()->m_voipServiceAdaptor->hangup(m_callId);
        VoipCallManagerLte::instance()->onCallStatusChanged(m_callId, true);
    }
    else
    {
        VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->hangup(m_callId);
        VoipCallManagerAdHoc::instance()->onCallStatusChanged(m_callId, true);
    }
}

void VoipCallHandler::hold(bool on)
{
    if (m_isLte)
    {
        VoipCallManagerLte::instance()->m_voipServiceAdaptor->setHold(m_callId, on);
    }
    else
    {
        VoipCallManagerAdHoc::instance()->m_voipServiceAdaptor->setHold(m_callId, on);
    }
}

void VoipCallHandler::emitStatusChangedSignal()
{
    emit statusChanged();
}

void VoipCallHandler::emitMultipartyChanged()
{
    emit multipartyChanged();
}
