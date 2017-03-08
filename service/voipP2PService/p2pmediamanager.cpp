#include "p2pmediamanager.h"
#include "voipinterface.h"
#include "voipdbusdata.h"
#include <QDebug>
#include <audiomanager.h>
#include <sstream>
#include <QDateTime>
#include <QNetworkInterface>

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
    P2PCall *call = new P2PCall(*this, iprm.callId);
    CallInfo ci = call->getInfo();
    qDebug() << "ci.remoteUri.data()" << ci.remoteContact.data();

    QString addr = QString::fromStdString(ci.remoteContact);
    addr = addr.section(":", 1, 1);

    P2PManager::getInstance()->addAddr(addr, call);
    qDebug() << "P2PManager::addAddr " << addr;

    P2PManager::getInstance()->answerCall(addr);
}

P2PCall::P2PCall(Account &acc, int call_id)
    : Call(acc, call_id),
      m_state(0),
      m_recordOn(false),
      m_recorder(NULL),
      m_connected(false)
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
             << "remote uri:"
             <<  ci.remoteUri.c_str()
             << "remote contact: "
             <<  ci.remoteContact.data()
             << " [" << ci.stateText.c_str()
             << "]" << "wifi"
             << "lastReason"
             << ci.lastReason.data()
             <<"lastStatusCode:"
             <<ci.lastStatusCode<<endl;

    //remote contact <sip:192.168.36.236:35789;ob> sip:192.168.3.108:30000
    QString addr = QString::fromStdString(ci.remoteContact);
    if(addr.isEmpty())
    {
        addr = QString::fromStdString(ci.remoteUri);
    }

    qDebug() << "addr" << addr;
    addr = addr.section(":", 1, 1);
    qDebug() << "remote addr: " << addr;

    //send time error for ui to call next call
    QString reason = QString::fromStdString(ci.lastReason);
    if(reason.contains("timeout", Qt::CaseInsensitive))
    {
        qDebug() << "call timeout";
        P2PManager::getInstance()->removeAddr(addr);
        emit P2PManager::getInstance()->signalCallP2PFinished(addr, CallStateTimeOut);
    }
    else if(ci.lastStatusCode == PJSIP_SC_SERVICE_UNAVAILABLE)
    {
        qDebug() << "call PJSIP_SC_SERVICE_UNAVAILABLE";
        P2PManager::getInstance()->removeAddr(addr);
        emit P2PManager::getInstance()->signalCallP2PFinished(addr, CallStateTimeOut);
    }
    else if(ci.state == PJSIP_INV_STATE_DISCONNECTED)
    {
        qDebug() << "call PJSIP_INV_STATE_DISCONNECTED";
        P2PManager::getInstance()->removeAddr(addr);
        emit P2PManager::getInstance()->signalCallP2PFinished(addr, CallStateDisconnected);
    }
    else if(ci.state == PJSIP_INV_STATE_CONFIRMED)
    {
        qDebug() << "call PJSIP_INV_STATE_CONFIRMED";
        emit P2PManager::getInstance()->signalCallP2PFinished(addr, CallStateConnected);
    }


    if(ci.state == PJSIP_INV_STATE_CONFIRMED)
    {
        m_connected = true;
    }
    else
    {
        m_connected = false;
    }
}

void P2PCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    setPttState(1);
}

int P2PCall::setPttState(int state)
{
    if(state != 0 && state != 1 && state != 2)
        return -1;

    if(!m_connected)
    {
        qDebug() << "did not connected, can not set ptt state";
        return -1;
    }

    if(m_state != state)
    {
        CallInfo ci;
        try
        {
            ci = getInfo();
        }
        catch (pj::Error &error)
        {
            qDebug() << "get info error: " << error.reason.data();
            return -1;
        }

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
                qDebug() << "getMedia" << audMed;

                AudDevManager& mgr = Endpoint::instance().audDevManager();

                m_state = state;

                if(m_state == 1)
                {
                    try
                    {
                        qDebug() << "start listening";
                        audMed->startTransmit(mgr.getPlaybackDevMedia());
                        mgr.getCaptureDevMedia().stopTransmit(*audMed);
                    }
                    catch(pj::Error &error)
                    {
                        qDebug() << error.reason.data();
                    }
                }
                else if(m_state == 2)
                {
                    try
                    {
                        qDebug() << "start speak";
                        mgr.getCaptureDevMedia().startTransmit(*audMed);
                        audMed->stopTransmit(mgr.getPlaybackDevMedia());
                    }
                    catch(pj::Error &error)
                    {
                        qDebug() << error.reason.data();
                    }
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

int P2PCall::record(bool on)
{
    if(m_recordOn == on)
        return -1;

    m_recordOn = on;

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
            qDebug() << "getMedia" << audMed;
            try
            {
                if(m_recordOn)
                {
                    //create recorder
                    if(m_recorder == NULL)
                        m_recorder = new AudioMediaRecorder();

                    QString timeStr = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz");
                    m_recordFileName = "/home/user/sinux/voicerecords/" + timeStr + ".wav";
                    qDebug() << "createRecorder" << m_recordFileName;
                    m_recorder->createRecorder(m_recordFileName.toStdString());

                    //start record
                    AudioMedia& cap_med = Endpoint::instance().audDevManager().getCaptureDevMedia();
                    qDebug() << "startTransmit";
                    cap_med.startTransmit(*m_recorder);
                    qDebug() << "startTransmit end";
                }
                else
                {
                    //stop record
                    AudioMedia& cap_med = Endpoint::instance().audDevManager().getCaptureDevMedia();
                    cap_med.stopTransmit(*m_recorder);
                    delete m_recorder;
                    m_recorder = NULL;

                    emit signalRecordFinished(getRemoteAddr(), m_recordFileName);
                }
            }
            catch(pj::Error &error)
            {
                qDebug() << error.reason.data();
            }
        }
    }

    return 0;
}

QString P2PCall::getRemoteAddr()
{
    CallInfo ci = getInfo();

    QString addr = QString::fromStdString(ci.remoteUri);
    addr = addr.section(":", 1, 1);

    return addr;
}

P2PManager *P2PManager::instance = NULL;

P2PManager *P2PManager::getInstance()
{
    if(instance == NULL)
        instance = new P2PManager();

    return instance;
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

#ifdef voipAdHocP2PService
    m_endpoint->codecSetPriority("speex/8000/1", 200);
    qDebug() << "ep->codecSetPriority(\"speex/8000/1\", 200)";
#else
    m_endpoint->codecSetPriority("iLBC/8000/1", 200);
    qDebug() << "ep->codecSetPriority(\"iLBC/8000/1\", 200)";
#endif

    TransportConfig tcfg;
    tcfg.port = sipPort;
    m_endpoint->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    m_endpoint->libStart();

    AccountConfig acc_config;
    acc_config.idUri = "sip:192.168.11.99";
    acc_config.mediaConfig.transportConfig.port = rtpPort;
    acc_config.mediaConfig.transportConfig.portRange = rtpPortRange;
    acc_config.videoConfig.autoShowIncoming = true;
    acc_config.videoConfig.autoTransmitOutgoing = true;
    m_account = new P2PAccount;
    m_account->create(acc_config);

    m_sipPort = sipPort;
}

int P2PManager::makeCall(const QString &addr)
{
    qDebug() << "P2PManager::onMakeCall ipAddress=" << addr << endl;

    if(addr.isEmpty())
        return -1;
    if(m_addrCallMap.contains(addr))
    {
        qDebug() << "call exists: " << addr;
        return -1;
    }

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

int P2PManager::answerCall(const QString &addr)
{
    qDebug() << "P2PManager::answerCall ipAddress=" << addr << endl;

    if(addr.isEmpty())
        return -1;
    if(!m_addrCallMap.contains(addr))
        return -1;

    int result = 0;

    P2PCall *call = m_addrCallMap.value(addr);

    if (call)
    {
        CallOpParam prm;
        prm.opt.audioCount = 1;
        prm.opt.videoCount = 0;
        prm.statusCode = PJSIP_SC_OK;

        try
        {
            call->answer(prm);
        }
        catch(pj::Error &error)
        {
            qDebug() << "answerCall error: " << error.reason.data();
        }
    }
    else
        result = -1;

    return result;
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

int P2PManager::record(const QString &addr, bool on)
{
    if(!m_addrCallMap.contains(addr))
        return -1;

    P2PCall *call = m_addrCallMap.value(addr);

    int status = -1;
    if(call)
    {
        status = call->record(on);
    }

    return status;
}

bool P2PManager::removeAddr(const QString &addr)
{
    qDebug() << "P2PManager::removeAddr: " << addr;
    if(m_addrCallMap.contains(addr))
    {
        qDebug() << "remove addr: " << addr;
        P2PCall *call = m_addrCallMap.value(addr);
        m_addrCallMap.remove(addr);

        call->deleteLater();
    }

    return true;
}

bool P2PManager::addAddr(const QString &addr, P2PCall *call)
{
    if(!m_addrCallMap.contains(addr))
    {
        connect(call, SIGNAL(signalRecordFinished(QString,QString)),
                this, SLOT(onSignalRecordFinished(QString,QString)));

        m_addrCallMap.insert(addr, call);
        qDebug() << "add addr" << addr;
        return true;
    }
    else
    {
        qDebug() << "already has addr" << addr;
        return false;
    }
}

void P2PManager::onSignalRecordFinished(const QString &addr, const QString &fileName)
{
    qDebug() << "P2PManager::onSignalRecordFinished";
    emit signalRecordFinished(addr, fileName);
}

QString P2PManager::getIp()
{
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
        qDebug() << "Address:" << address << address.protocol();
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                !address.toString().contains("100.100") &&
                !address.isLoopback())
        {
            return address.toString();
        }
    }
    return "";
}
