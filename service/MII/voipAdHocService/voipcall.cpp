#include "voipcall.h"
#include "voipaccount.h"
#include "voipcalllistmanager.h"
#include "voipinterface.h"
#include "voipdbusdata.h"
#include <QDebug>
#include <audiomanager.h>
#include "adhocbus/adhocconnectadaptor.h"

//extern AudioMediaPlayer *player;
//extern AudioMediaPlayer *outgoingPlayer;
extern VoipInterface *service;

VoipCall::VoipCall(Account &acc, int call_id)
    : Call(acc, call_id),
      m_isInConference(false),
      m_isDisconnected(false),
      m_currentTimeD(0),
      m_startRecordHeart(false),
      m_willBeConference(0)
{
    m_myAcc = (VoipAccount *)&acc;
    //    pthread_rwlock_init(&m_lock, NULL);
    //    QObject::connect(&m_heartTimer, SIGNAL(timeout()),
    //                         this, SLOT(onSendHeart()));
    //    m_heartTimer.setInterval(1000);
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
             << "]" << "wifi"
             << "lastReason" << ci.lastReason.data() <<"lastStatusCode:"<<ci.lastStatusCode<<endl;
    qDebug() << "call time : " << endl;

    if(ci.lastStatusCode==PJSIP_SC_SERVICE_UNAVAILABLE)
    {
        int callState = CallStateTimeOut;
        if (service)
        {
            setDisconnected();
            service->emitCallStatusChanged(getId(), true);
//            service->emitCallStateSignal(getId(), callState);
            return;
        }
    }

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

        //send time error for ui to call next call
        QString reason = QString::fromStdString(ci.lastReason);
        if(reason.contains("timeout", Qt::CaseInsensitive))
        {
            service->emitCallError(getId());
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
            //zhangjy cancel setport 3.21

//            if (VoipCallListManager::getPort() != 8)
//                VoipCallListManager::setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
//            if(service)
//                service->emitHandsFreeChanged();

            //stop play music
            //            player->stopTransmit(playMed);
            //            outgoingPlayer->stopTransmit(playMed);

            //start heart timer
            m_startRecordHeart = true;
        }

        if (ci.state == PJSIP_INV_STATE_CALLING)
        {
            //            AudioManager audMgr;
            //            audMgr.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
            //zhangjy cancel setport 3.21

//            if (VoipCallListManager::getPort() != 8)
//                VoipCallListManager::setPort(AudioManager::AM_PORT_CALLON_EARPIECE);

            //play music for outgoing call
            AudioMedia& playMed = Endpoint::instance().audDevManager().getPlaybackDevMedia();
            //            outgoingPlayer->setPos(0);
            //            outgoingPlayer->startTransmit(playMed);
            setAudioUsed(true);

//            if(service)
//                service->emitHandsFreeChanged();
        }
        else if (ci.state == PJSIP_INV_STATE_DISCONNECTED)
        {
            //zhangjy cancel setport 3.21

//            if(!VoipCallListManager::instance().isAudioUsed())
//            {
//                //                AudioManager audMgr;
//                //                audMgr.setPort(AudioManager::AM_PORT_OUTPUT_IHF);
//                VoipCallListManager::setPort(AudioManager::AM_PORT_OUTPUT_IHF);

//                //                player->stopTransmit(playMed);
//                //                outgoingPlayer->stopTransmit(playMed);
//            }

            //stop heart timer
            m_startRecordHeart = false;

            VoipCallListManager::instance().removeCall(this);

            if(service)
                service->emitConferenceParticipantsChanged();

#ifdef voipAdHocService
            AdhocConnectAdaptor().deleteConnect();
            AdhocConnectAdaptor().deleteVideoConnect();

#endif
        }
    }
}

void VoipCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$#################media state:"<<ci.lastStatusCode<<endl;
    if(ci.remVideoCount==0 && ci.connectDuration.sec!=0)
    {

        service->emitVidChanged(true);
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$#################ci.connectDuration.msec:"<<ci.connectDuration.msec
               <<"sec:"<<ci.connectDuration.sec<<endl;

        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ emit vid changed"<<endl;
    }

    // Iterate all the call medias
    for (unsigned i = 0; i < ci.media.size(); i++)
    {
        qDebug() << "VoipCall::onCallMediaState"
                 << ci.media[i].index
                 << ci.media[i].status
                 << "type: " << ci.media[i].type;
        if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i)
                && (ci.media[i].status == PJSUA_CALL_MEDIA_ACTIVE
                    || ci.media[i].status == PJSUA_CALL_MEDIA_REMOTE_HOLD))
        {
            //            AudioManager audMgr;
            //            audMgr.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
            //zhangjy cancel setport 3.21

//            if (VoipCallListManager::getPort() != 8)
//                VoipCallListManager::setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
//            if(service)
//                service->emitHandsFreeChanged();

            AudioMedia *audMed = (AudioMedia *)getMedia(i);

            // Connect the call audio media to sound devices
            qDebug() << "--------------------Connect the call audio media to sound device" << endl;
            AudDevManager& mgr = Endpoint::instance().audDevManager();
            audMed->startTransmit(mgr.getPlaybackDevMedia());
            mgr.getCaptureDevMedia().startTransmit(*audMed);

            //            m_myAcc->addToConference(this, audMed);
            if(m_willBeConference == 1)
            {
                qDebug() << "m_willBeConference == 0 VoipCallListManager::instance().addToConference(this, audMed)"
                         << endl;
                m_willBeConference = 0;
                if(!m_isInConference)
                {
                    bool success = VoipCallListManager::instance().addToConference(this, audMed);
                    if (success)
                    {
                        service->emitConferenceEstablished();
                        service->emitConferenceParticipantsChanged();
                    }
                }
            }
        }
    }

    if(VoipCallListManager::instance().getCallStatus(getId()) != 5)
    {
        if (service)
            service->emitCallStatusChanged(getId());
    }
}

bool VoipCall::isInConference()
{
    return m_isInConference;
}

void VoipCall::setInConference()
{
    if (m_isInConference)
    {
        return;
    }

    m_isInConference = true;

    if(service)
        service->emitCallIsInConference(getId());
}

bool VoipCall::isDisconnected()
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

void VoipCall::setWillBeConference(int willBe)
{
    m_willBeConference = willBe;
}

void VoipCall::onInstantMessage(OnInstantMessageParam &prm)
{
    //    qDebug() << "onInstantMessage" << prm.msgBody.data();
    if(prm.msgBody == "online")
    {
        m_currentTimeD = 0;
    }
}

//void VoipCall::onInstantMessageStatus(OnInstantMessageStatusParam &prm)
//{
//    qDebug() << "onInstantMessageStatus"
//             << prm.code
//             << prm.msgBody.data()
//             << prm.reason.data()
//             << prm.toUri.data();
//}

void VoipCall::onSendHeart()
{
    if(m_startRecordHeart)
    {
        //        qDebug() << "onSendHeart";
//        SendInstantMessageParam param;
//        param.content = "online";
//        try
//        {
//            CallInfo ci = getInfo();
//            if ( ci.state !=  PJSIP_INV_STATE_DISCONNECTED)
//            {
//                sendInstantMessage(param);
//                m_currentTimeD ++;
//                if(abs(m_currentTimeD) > 500)
//                {
//                    qDebug() << "it is time to hangup";
//                    service->onHangup(getId());
//                }
//            }
//        }
//        catch(pj::Error &error)
//        {
//            qDebug() << "onSendHeart error: " << error.reason.data();
//        }
    }
    else
    {
//        qDebug() << "onSendHeart timer running";
    }
}

