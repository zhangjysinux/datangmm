#include "voipcallhandler.h"
#include "voipserviceadaptor.h"
#include "../../../app/commondata/datatype/callhistorydata.h"
#include "../../../app/commondata/datatype/commondata.h"
#include "voipcallmanager.h"
VoipCallHandler::VoipCallHandler(bool isLte, int callId, bool isIncoming, const QString &lineId,NetworkType netWork, QObject *parent) :
    m_isLte(isLte), m_callId(callId), m_isIncoming(isIncoming), m_lineId(lineId), m_netWork(netWork), QObject(parent)
{
    m_isVideo = false;
}

QString VoipCallHandler::handlerId() const
{
    return QString::number(m_callId);
}

int VoipCallHandler::status() const
{
    if (m_netWork == LTE_NETWORK)
        return VoipCallManager::instanceLte()->m_voipServiceAdaptor->getCallStatus(m_callId);
    else if (m_netWork == AD_HOC_NETWORK)
        return VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->getCallStatus(m_callId);
    else if (m_netWork == WIFI_NETWORK)
        return VoipCallManager::instanceWifi()->m_voipServiceAdaptor->getCallStatus(m_callId);
    else if (m_netWork == BATTLE_NETWORK)
        return VoipCallManager::instanceBattle()->m_voipServiceAdaptor->getCallStatus(m_callId);

    //    if (m_isLte)
    //    {
    //        return WifiCallManager::instance()->m_voipServiceAdaptor->getCallStatus(m_callId);
    //    }
    //    else
    //    {
    //        return WifiCallManager::instance()->m_voipServiceAdaptor->getCallStatus(m_callId);
    //    }
}

QString VoipCallHandler::lineId() const
{
    if (!m_lineId.isEmpty())
        return m_lineId;

    if (m_netWork == LTE_NETWORK)
        return VoipCallManager::instanceLte()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    else if (m_netWork == AD_HOC_NETWORK)
        return VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    else if (m_netWork == WIFI_NETWORK)
        return VoipCallManager::instanceWifi()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    else if (m_netWork == BATTLE_NETWORK)
        return VoipCallManager::instanceBattle()->m_voipServiceAdaptor->getCallIpAddr(m_callId);

    //    if (m_isLte)
    //    {
    //        return WifiCallManager::instance()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    //    }
    //    else
    //    {
    //        return WifiCallManager::instance()->m_voipServiceAdaptor->getCallIpAddr(m_callId);
    //    }
}

bool VoipCallHandler::isIncoming() const
{
    return m_isIncoming;
    /*
    if (m_isLte)
    {
        return WifiCallManager::instance()->m_voipServiceAdaptor->isCallIncoming(m_callId);
    }
    else
    {
        return WifiCallManager::instance()->m_voipServiceAdaptor->isCallIncoming(m_callId);
    }
*/
}

bool VoipCallHandler::isMultiparty() const
{

    if (m_netWork == LTE_NETWORK)
        return VoipCallManager::instanceLte()->m_voipServiceAdaptor->isCallInConference(m_callId);
    else if (m_netWork == AD_HOC_NETWORK)
        return VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->isCallInConference(m_callId);
    else if (m_netWork == WIFI_NETWORK)
        return VoipCallManager::instanceWifi()->m_voipServiceAdaptor->isCallInConference(m_callId);
    else if (m_netWork == BATTLE_NETWORK)
        return VoipCallManager::instanceBattle()->m_voipServiceAdaptor->isCallInConference(m_callId);


    //    if (m_isLte)
    //    {
    //        return WifiCallManager::instance()->m_voipServiceAdaptor->isCallInConference(m_callId);
    //    }
    //    else
    //    {
    //        return WifiCallManager::instance()->m_voipServiceAdaptor->isCallInConference(m_callId);
    //    }
}

bool VoipCallHandler::isLte() const
{
    return m_isLte;
}

NetworkType VoipCallHandler::netWork() const
{

    return m_netWork;
}

void VoipCallHandler::answer()
{
    if (m_netWork == LTE_NETWORK)
        VoipCallManager::instanceLte()->m_voipServiceAdaptor->answer(m_callId);
    else if (m_netWork == AD_HOC_NETWORK)
        VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->answer(m_callId);
    else if (m_netWork == WIFI_NETWORK)
        VoipCallManager::instanceWifi()->m_voipServiceAdaptor->answer(m_callId);
    else if (m_netWork == BATTLE_NETWORK)
        VoipCallManager::instanceBattle()->m_voipServiceAdaptor->answer(m_callId);
}

void VoipCallHandler::hangup()
{
    qDebug() << " hangup in callhandler";
    if (m_netWork == LTE_NETWORK)
    {
        VoipCallManager::instanceLte()->m_voipServiceAdaptor->hangup(m_callId);
        VoipCallManager::instanceLte()->m_voipServiceAdaptor->onCallStatusChanged(m_callId, true);
    }
    else if (m_netWork == AD_HOC_NETWORK)
    {
        VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->hangup(m_callId);
        VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->onCallStatusChanged(m_callId, true);
    }
    else if (m_netWork == WIFI_NETWORK)
    {
        VoipCallManager::instanceWifi()->m_voipServiceAdaptor->hangup(m_callId);
        VoipCallManager::instanceWifi()->m_voipServiceAdaptor->onCallStatusChanged(m_callId, true);
    }
    else if (m_netWork == BATTLE_NETWORK)
    {
        VoipCallManager::instanceBattle()->m_voipServiceAdaptor->hangup(m_callId);
        VoipCallManager::instanceBattle()->m_voipServiceAdaptor->onCallStatusChanged(m_callId, true);
    }

    //    if (m_isLte)
    //    {
    //        WifiCallManager::instance()->m_voipServiceAdaptor->hangup(m_callId);
    //        WifiCallManager::instance()->onCallStatusChanged(m_callId, true);
    //    }
    //    else
    //    {
    //        WifiCallManager::instance()->m_voipServiceAdaptor->hangup(m_callId);
    //        WifiCallManager::instance()->onCallStatusChanged(m_callId, true);
    //    }
}

void VoipCallHandler::hold(bool on)
{
    if (m_netWork == LTE_NETWORK)
        VoipCallManager::instanceLte()->m_voipServiceAdaptor->setHold(m_callId, on);
    else if (m_netWork == AD_HOC_NETWORK)
        VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->setHold(m_callId, on);
    else if (m_netWork == WIFI_NETWORK)
        VoipCallManager::instanceWifi()->m_voipServiceAdaptor->setHold(m_callId, on);
    else if (m_netWork == BATTLE_NETWORK)
        VoipCallManager::instanceBattle()->m_voipServiceAdaptor->setHold(m_callId, on);
}

void VoipCallHandler::emitStatusChangedSignal()
{
    emit statusChanged();
}

void VoipCallHandler::emitMultipartyChanged()
{
    emit multipartyChanged();
}

void VoipCallHandler::SetVideo(bool bIsVideo)
{
    m_isVideo = bIsVideo;
}

bool VoipCallHandler::isVideo()
{
    return m_isVideo;
}

int VoipCallHandler::getCallError()
{
    int error = 0;

    if (m_netWork == LTE_NETWORK)
        error = VoipCallManager::instanceLte()->m_voipServiceAdaptor->getCallError(m_callId);
    else if (m_netWork == AD_HOC_NETWORK)
        error = VoipCallManager::instanceAdHoc()->m_voipServiceAdaptor->getCallError(m_callId);
    else if (m_netWork == WIFI_NETWORK)
        error = VoipCallManager::instanceWifi()->m_voipServiceAdaptor->getCallError(m_callId);
    else if (m_netWork == BATTLE_NETWORK)
        error = VoipCallManager::instanceBattle()->m_voipServiceAdaptor->getCallError(m_callId);

    return error;
}
