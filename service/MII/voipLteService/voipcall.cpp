#include "voipcall.h"
#include "voipaccount.h"
#include "voipcalllistmanager.h"
#include "voipinterface.h"
#include "voipdbusdata.h"
#include <QDebug>
#include <audiomanager.h>

extern AudioMediaPlayer *player;
extern AudioMediaPlayer *outgoingPlayer;
extern VoipInterface *service;

VoipCall::VoipCall(Account &acc, int call_id)
    : Call(acc, call_id),
      m_isInConference(false),
      m_isDisconnected(false),
      m_currentTimeD(0),
      m_startRecordHeart(false)
{
    m_myAcc = (VoipAccount *)&acc;
    QObject::connect(&m_heartTimer, SIGNAL(timeout()),
                         this, SLOT(onSendHeart()));
    m_heartTimer.setInterval(1000);
//    m_heartTimer.start();
}

VoipCall::~VoipCall()
{
}

void VoipCall::onCallState(OnCallStateParam &prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
    qDebug() << "*** Call: " <<  ci.remoteUri.c_str() << " [" << ci.stateText.c_str()
             << "]" << "wifi" << "lastReason" << ci.lastReason.data() << ci.lastStatusCode;

    AudioMedia& playMed = Endpoint::instance().audDevManager().getPlaybackDevMedia();
//    player->stopTransmit(playMed);
//    outgoingPlayer->stopTransmit(playMed);

    if (service)
    {
        int callState = CallStateUnknown;

        switch (ci.state)
        {
        case PJSIP_INV_STATE_DISCONNECTED:
            callState = CallStateDisconnected;
            break;
        case PJSIP_INV_STATE_CONFIRMED:
            callState = CallStateConnected;
            break;
        case PJSIP_INV_STATE_NULL:
            callState = CallStateUnknown;
            break;
        default:
            callState = CallStateConnecting;
            break;
        }

        service->emitCallStateSignal(getId(), callState);
        if (ci.state != PJSIP_INV_STATE_EARLY && ci.state != PJSIP_INV_STATE_CONNECTING)
        {
            //incomming call
            if(VoipCallListManager::instance().getCallStatus(getId()) != 5)
            {
                service->emitCallStatusChanged(getId(),
                                               ci.state == PJSIP_INV_STATE_DISCONNECTED ? true : false);
            }
        }

        if(ci.state != PJSIP_INV_STATE_CALLING && ci.state != PJSIP_INV_STATE_INCOMING)
        {
            if(isAudioUsed())
            {
                qDebug() << "audio should not used, ci.state " << ci.state;
                setAudioUsed(false);
            }
        }

        if(ci.state == PJSIP_INV_STATE_CONFIRMED)
        {
            if(!isAudioUsed())
            {
                qDebug() << "audio should used, ci.state " << ci.state;
                setAudioUsed(true);
            }

            //stop play music
            player->stopTransmit(playMed);
            outgoingPlayer->stopTransmit(playMed);

            //start heart timer
            m_startRecordHeart = true;
        }

        if (ci.state == PJSIP_INV_STATE_CALLING)
        {
//            AudioManager audMgr;
//            audMgr.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
            VoipCallListManager::setPort(AudioManager::AM_PORT_CALLON_EARPIECE);

            //play music for outgoing call
            AudioMedia& playMed = Endpoint::instance().audDevManager().getPlaybackDevMedia();
            outgoingPlayer->setPos(0);
            outgoingPlayer->startTransmit(playMed);
            setAudioUsed(true);

            if(service)
                service->emitHandsFreeChanged();
        }
        else if (ci.state == PJSIP_INV_STATE_DISCONNECTED)
        {
            if(!VoipCallListManager::instance().isAudioUsed())
            {
//                AudioManager audMgr;
//                audMgr.setPort(AudioManager::AM_PORT_OUTPUT_IHF);
                VoipCallListManager::setPort(AudioManager::AM_PORT_OUTPUT_IHF);

                player->stopTransmit(playMed);
                outgoingPlayer->stopTransmit(playMed);
            }

            //stop heart timer
            m_startRecordHeart = false;

            VoipCallListManager::instance().removeCall(this);

            if(service)
                service->emitConferenceParticipantsChanged();
        }
    }
}

void VoipCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();

    if(ci.remVideoCount==0 && ci.connectDuration.sec!=0)
    {

      service->emitVidChanged(true);
      qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$#################media state:"<<ci.lastStatusCode<<endl;
qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$#################ci.connectDuration.msec:"<<ci.connectDuration.msec
       <<"sec:"<<ci.connectDuration.sec<<endl;

      qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ emit vid changed"<<endl;
    }

    // Iterate all the call medias
    for (unsigned i = 0; i < ci.media.size(); i++)
    {
        if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i)
            && (ci.media[i].status == PJSUA_CALL_MEDIA_ACTIVE
                || ci.media[i].status == PJSUA_CALL_MEDIA_REMOTE_HOLD))
        {
//            AudioManager audMgr;
//            audMgr.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
            VoipCallListManager::setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
            if(service)
                service->emitHandsFreeChanged();

            AudioMedia *audMed = (AudioMedia *)getMedia(i);

            // Connect the call audio media to sound devices
            qDebug() << "--------------------Connect the call audio media to sound device" << endl;
            AudDevManager& mgr = Endpoint::instance().audDevManager();
            audMed->startTransmit(mgr.getPlaybackDevMedia());
            mgr.getCaptureDevMedia().startTransmit(*audMed);

//            m_myAcc->addToConference(this, audMed);
        }
    }

    if(VoipCallListManager::instance().getCallStatus(getId()) != 5)
    {
        if (service)
            service->emitCallStatusChanged(getId());
    }
}

bool VoipCall::isInConference() const
{
    return m_isInConference;
}

void VoipCall::setInConference()
{
    if (m_isInConference)
        return;

    m_isInConference = true;

    if(service)
        service->emitCallIsInConference(getId());
}

bool VoipCall::isDisconnected() const
{
    return m_isDisconnected;
}

void VoipCall::setDisconnected()
{
    m_isDisconnected = true;
}

bool VoipCall::isAudioUsed()
{
    return m_isAudioUsed;
}

void VoipCall::setAudioUsed(bool use)
{
    m_isAudioUsed = use;
}

void VoipCall::onInstantMessage(OnInstantMessageParam &prm)
{
//    qDebug() << "onInstantMessage" << prm.msgBody.data();
    if(prm.msgBody == "online")
    {
        m_currentTimeD = 0;
    }
}

void VoipCall::onInstantMessageStatus(OnInstantMessageStatusParam &prm)
{
    qDebug() << "onInstantMessageStatus"
             << prm.code
             << prm.msgBody.data()
             << prm.reason.data()
             << prm.toUri.data();
}

void VoipCall::onSendHeart()
{
    if(m_startRecordHeart)
    {
//        qDebug() << "onSendHeart";
        SendInstantMessageParam param;
        param.content = "online";
        sendInstantMessage(param);
        m_currentTimeD ++;
        if(abs(m_currentTimeD) > 10)
        {
            qDebug() << "it is time to hangup";
            service->onHangup(getId());
        }
    }
    else
    {
//        qDebug() << "onSendHeart timer running";
    }
}

