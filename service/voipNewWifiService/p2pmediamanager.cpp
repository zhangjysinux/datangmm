#include "p2pmediamanager.h"
#include "voipaccount.h"
#include "voipcalllistmanager.h"
#include "voipinterface.h"
#include "voipdbusdata.h"
#include <QDebug>
#include <audiomanager.h>
#include <sstream>

P2PAccount::P2PAccount()
{
}

P2PAccount::~P2PAccount()
{
}

void P2PAccount::onRegState(OnRegStateParam &prm)
{
    AccountInfo ai = getInfo();
    qDebug() << (ai.regIsActive ? "*** Register: code=" : "*** Unregister: code=")
             << prm.code << endl;
}

void P2PAccount::onIncomingCall(OnIncomingCallParam &iprm)
{
//    emit signalIncommingCallHandle(iprm);
//    onIncommingCallHandle(iprm);
}

P2PCall::P2PCall(Account &acc, int call_id)
    : Call(acc, call_id), m_state(0)
{
    m_myAcc = (P2PAccount *)&acc;
}

P2PCall::~P2PCall()
{
}

void P2PCall::onCallState(OnCallStateParam &prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
    qDebug() << "*** Call: "
             <<  ci.remoteUri.c_str()
             << " [" << ci.stateText.c_str()
             << "]" << "wifi"
             << "lastReason"
             << ci.lastReason.data()
             <<"lastStatusCode:"
             <<ci.lastStatusCode<<endl;

}

void P2PCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    CallInfo ci = getInfo();

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
            AudioMedia *audMed = (AudioMedia *)getMedia(i);

            AudDevManager& mgr = Endpoint::instance().audDevManager();
            audMed->startTransmit(mgr.getPlaybackDevMedia());
            mgr.getCaptureDevMedia().startTransmit(*audMed);
        }
    }
}

int P2PCall::setPttState(int state)
{
    if(state != 0 || state != 1 || state != 2)
        return -1;

    if(m_state != state)
    {
        m_state = state;

        CallInfo ci = getInfo();

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
                AudioMedia *audMed = (AudioMedia *)getMedia(i);

                AudDevManager& mgr = Endpoint::instance().audDevManager();
                if(m_state == 1)
                {
                    audMed->startTransmit(mgr.getPlaybackDevMedia());
                    mgr.getCaptureDevMedia().stopTransmit(*audMed);
                }
                else if(m_state == 2)
                {
                    mgr.getCaptureDevMedia().startTransmit(*audMed);
                    audMed->stopTransmit(mgr.getPlaybackDevMedia());
                }
            }
        }
    }
    return 0;
}

int P2PCall::getPttState()
{
    return m_state;
}


P2PManager::P2PManager()
{

}

P2PManager::~P2PManager()
{

}

void P2PManager::initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    m_endpoint = new Endpoint;
    m_endpoint->libCreate();

    EpConfig ep_cfg;
    ep_cfg.logConfig.level = 0;
    m_endpoint->libInit(ep_cfg);
    //    ep->codecSetPriority("speex/8000", 200);
    TransportConfig tcfg;
    tcfg.port = sipPort;
    m_endpoint->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    m_endpoint->libStart();

    AccountConfig acc_cfg;
    acc_cfg.idUri = "sip:192.168.1.99";
    acc_cfg.mediaConfig.transportConfig.port = rtpPort;
    acc_cfg.mediaConfig.transportConfig.portRange = rtpPortRange;
    acc_cfg.videoConfig.autoShowIncoming = true;
    acc_cfg.videoConfig.autoTransmitOutgoing = true;
    m_account = new P2PAccount;
    m_account->create(acc_cfg);

    m_sipPort = sipPort;
}

int P2PManager::makeCall(const QString &addr)
{
    qDebug() << "P2PManager::onMakeCall ipAddress=" << addr << endl;

    if(addr.isEmpty())
        return -1;
    if(m_addrCallMap.contains(addr))
        return -1;

    P2PCall *call = new P2PCall(*m_account);
    m_addrCallMap.insert(addr, call);

    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;

    stringstream ss;
    ss << m_sipPort;
    string dstUri = "sip:" + addr.toStdString() + ":" + ss.str();
    try
    {
        call->makeCall(dstUri, prm);
    }
    catch(pj::Error &error)
    {
        qDebug() << "P2PManager::onMakeCall error: " << error.reason.data();
    }

    return call->getId();
}

int P2PManager::hangupCall(const QString &addr)
{
    qDebug() << "P2PManager::hangupCall ipAddress=" << addr << endl;

    if(addr.isEmpty())
        return -1;
    if(!m_addrCallMap.contains(addr))
        return -1;

    int result = 0;

    P2PCall *call = m_addrCallMap.value(addr);

    if (call)
    {
        try
        {
            CallOpParam prm;
            call->hangup(prm);
        }
        catch(pj::Error &error)
        {
            qDebug() << "hangupCall error: " << error.reason.data();
        }
    }
    else
        result = -1;

    return result;
}

int P2PManager::setPttState(const QString &addr, int state)
{
    if(!m_addrCallMap.contains(addr))
        return -1;

    P2PCall *call = m_addrCallMap.value(addr);

    int status = -1;
    if(call)
    {
        status = call->setPttState(state);
    }

    return status;
}

int P2PManager::getPttState(const QString &addr)
{
    if(!m_addrCallMap.contains(addr))
        return -1;

    P2PCall *call = m_addrCallMap.value(addr);

    int status = -1;
    if(call)
    {
        status = call->getPttState();
    }

    return status;
}
