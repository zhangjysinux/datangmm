#include "voipcalllistmanager.h"
#include "voipcall.h"
#include "voipinterface.h"
#include <QDebug>
#include <QTimer>
#include <unistd.h>

extern AudioMediaRecorder *recorder;
extern VoipInterface *service;
extern AudioMediaPlayer *player;
extern AudioMediaPlayer *outgoingPlayer;

class ScopedRWLock
{
public:
    ScopedRWLock(pthread_rwlock_t &rwl, bool write = false) :
        m_rwl(rwl)
    {
        if (write)
            pthread_rwlock_wrlock(&m_rwl);
        else
            pthread_rwlock_rdlock(&m_rwl);
    }

    ~ScopedRWLock()
    {
        pthread_rwlock_unlock(&m_rwl);
    }

private:
    pthread_rwlock_t &m_rwl;
};

VoipCallListManager *VoipCallListManager::m_instance = 0;

VoipCallListManager &VoipCallListManager::instance()
{
    return *m_instance;
}

VoipCallListManager::VoipCallListManager()
{
    pthread_rwlock_init(&m_lock, NULL);
    m_instance = this;
}

VoipCallListManager::~VoipCallListManager()
{
    m_instance = 0;
    pthread_rwlock_destroy(&m_lock);
}

void VoipCallListManager::addCall(VoipCall *call)
{
    pthread_rwlock_wrlock(&m_lock);
    m_calls.push_back(call);
    pthread_rwlock_unlock(&m_lock);
}

void VoipCallListManager::removeCall(VoipCall *call)
{
    pthread_rwlock_wrlock(&m_lock);

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        if (*it == call)
        {
//            delete call;
            call->deleteLater();
            m_calls.erase(it);
            break;
        }
    }

    pthread_rwlock_unlock(&m_lock);
}

int VoipCallListManager::answerCall(int callId)
{
    int result = 0;

    pthread_rwlock_rdlock(&m_lock);

    VoipCall *call = findCall(callId);
    if (call)
    {
        CallOpParam prm;
        prm.opt.audioCount = 1;
        prm.opt.videoCount = 1;
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

    pthread_rwlock_unlock(&m_lock);

    return result;
}

int VoipCallListManager::hangupCall(int callId)
{
    int result = 0;

    pthread_rwlock_rdlock(&m_lock);
    VoipCall *call = findCall(callId);
    pthread_rwlock_unlock(&m_lock);

    if (call)
    {
        try
        {
            CallOpParam prm;
            call->setDisconnected();
            call->setAudioUsed(false);
            call->hangup(prm);

            //when user hangup, stop all the ringing first.
            AudioMedia& playMed = Endpoint::instance().audDevManager().getPlaybackDevMedia();
            player->stopTransmit(playMed);
            outgoingPlayer->stopTransmit(playMed);
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

int VoipCallListManager::setHold(int callId, bool on)
{
    int result = 0;

    ScopedRWLock srwl(m_lock);

    VoipCall *call = findCall(callId);
    if (call)
    {
        if (on)
        {
            CallOpParam prm;
            prm.opt.videoCount=0;
            try
            {
                call->setHold(prm);
            }
            catch(pj::Error &error)
            {
                qDebug() << "setHold error: " << error.reason.data();
            }
        }
        else
        {
            CallOpParam prm(true);
            prm.opt.flag = PJSUA_CALL_UNHOLD;
            prm.opt.videoCount=0;
            try
            {
                call->reinvite(prm);
            }
            catch(pj::Error &error)
            {
                qDebug() << "reinvite error: " << error.reason.data();
            }
        }
    }
    else
        result = -1;

    return result;
}

int VoipCallListManager::setHoldConference(bool on)
{
    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        setHold((*it)->getId(), on);
    }
    return 0;
}

int VoipCallListManager::getCallStatus(int callId)
{
    int result = STATUS_NULL;

    pthread_rwlock_rdlock(&m_lock);

    VoipCall *call = findCall(callId);
    if (call)
    {
        if (call->isDisconnected())
            result = STATUS_DISCONNECTED;
        else
        {
            CallInfo ci = call->getInfo();
            if (ci.state == PJSIP_INV_STATE_NULL)
                result = STATUS_NULL;
            else if (ci.state == PJSIP_INV_STATE_DISCONNECTED)
                result = STATUS_DISCONNECTED;
            else if (ci.state == PJSIP_INV_STATE_CONFIRMED)
            {
                bool isActive = false;
                for (unsigned i = 0; i < ci.media.size(); i++)
                {
                    if (ci.media[i].type == PJMEDIA_TYPE_AUDIO)
                    {
                        if (ci.media[i].status == PJSUA_CALL_MEDIA_ACTIVE)
                            isActive = true;
                        break;
                    }
                }
                if (isActive)
                    result = STATUS_ACTIVE;
                else
                    result = STATUS_HELD;
            }
            else if (ci.state == PJSIP_INV_STATE_CALLING)
                result = STATUS_DIALING;
            else if (ci.state == PJSIP_INV_STATE_INCOMING)
                result = STATUS_INCOMING;
            else if (ci.state == PJSIP_INV_STATE_EARLY ||
                     ci.state == PJSIP_INV_STATE_CONNECTING)
            {
                if (ci.role == PJSIP_ROLE_UAC)
                    result = STATUS_DIALING;
                else
                    result = STATUS_INCOMING;
            }
        }
    }

    pthread_rwlock_unlock(&m_lock);

    return result;
}

QString VoipCallListManager::getCallIpAddr(int callId)
{
    QString result;

    pthread_rwlock_rdlock(&m_lock);

    VoipCall *call = findCall(callId);
    if (call)
    {
        CallInfo ci = call->getInfo();
        string remIpAddr = ci.remoteContact;
        size_t pos1;
        if ((pos1 = ci.remoteContact.find(':')) != string::npos)
        {
            size_t pos2;
            if ((pos2 = ci.remoteContact.find(':', pos1 + 1)) != string::npos)
            {

                remIpAddr = ci.remoteContact.substr(pos1 + 1, pos2 - pos1 - 1);
            }
        }
        result = QString::fromStdString(remIpAddr);
    }

    pthread_rwlock_unlock(&m_lock);

    return result;
}

bool VoipCallListManager::isCallIncoming(int callId)
{
    bool result = false;

    pthread_rwlock_rdlock(&m_lock);

    VoipCall *call = findCall(callId);
    if (call)
    {
        CallInfo ci = call->getInfo();
        result = ((ci.role == PJSIP_ROLE_UAS) ? true : false);
    }

    pthread_rwlock_unlock(&m_lock);

    return result;
}

bool VoipCallListManager::isCallInConference(int callId)
{
    bool result = false;

    pthread_rwlock_rdlock(&m_lock);

    VoipCall *call = findCall(callId);
    if (call)
    {
        result = call->isInConference();
    }

    pthread_rwlock_unlock(&m_lock);

    return result;
}

int VoipCallListManager::hangupConference()
{
    int result = -1;

    pthread_rwlock_rdlock(&m_lock);

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        VoipCall *call = *it;
        if(call->isInConference())
        {
            CallOpParam prm;
            call->setDisconnected();
            call->hangup(prm);
        }

        result = 0;
    }

    pthread_rwlock_unlock(&m_lock);

    return result;
}

bool VoipCallListManager::mergeIntoConference()
{
    bool result = false;

    pthread_rwlock_rdlock(&m_lock);

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        bool isUnhold = false;
        CallInfo ci = (*it)->getInfo();

        for (unsigned j = 0; j < ci.media.size(); j++)
        {
            if (ci.media[j].type == PJMEDIA_TYPE_AUDIO && (*it)->getMedia(j)
                    && ci.media[j].status == PJSUA_CALL_MEDIA_LOCAL_HOLD)
            {
                CallOpParam prm(true);
                prm.opt.flag = PJSUA_CALL_UNHOLD;
                (*it)->reinvite(prm);
                isUnhold = true;
            }
        }

        if((*it)->isInConference())
            continue;

        for (unsigned j = 0; j < ci.media.size(); j++)
        {
            if (ci.media[j].type == PJMEDIA_TYPE_AUDIO && (*it)->getMedia(j)
                        && (ci.media[j].status == PJSUA_CALL_MEDIA_ACTIVE
                            || ci.media[j].status == PJSUA_CALL_MEDIA_REMOTE_HOLD
                            || isUnhold))
            {
                AudioMedia* audioMedia = static_cast<AudioMedia*>((*it)->getMedia(j));
                if (addToConference((*it), audioMedia))
                    result = true;
            }
        }
    }

    pthread_rwlock_unlock(&m_lock);

    return result;
}

void VoipCallListManager::setMute(bool on)
{
    AudioMedia& capMed = Endpoint::instance().audDevManager().getCaptureDevMedia();

    pthread_rwlock_rdlock(&m_lock);

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        CallInfo ci = (*it)->getInfo();
        for (unsigned j = 0; j < ci.media.size(); j++)
        {
            if (ci.media[j].type == PJMEDIA_TYPE_AUDIO && (*it)->getMedia(j)
                && (ci.media[j].status == PJSUA_CALL_MEDIA_ACTIVE
                    || ci.media[j].status == PJSUA_CALL_MEDIA_REMOTE_HOLD))
            {
                AudioMedia *audMed = (AudioMedia *)(*it)->getMedia(j);
                if (on)
                    capMed.stopTransmit(*audMed);
                else
                    capMed.startTransmit(*audMed);
            }
        }
    }

    pthread_rwlock_unlock(&m_lock);
}

void VoipCallListManager::record()
{
    pthread_rwlock_rdlock(&m_lock);

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        CallInfo ci = (*it)->getInfo();
        for (unsigned j = 0; j < ci.media.size(); j++)
        {
            if (ci.media[j].type == PJMEDIA_TYPE_AUDIO && (*it)->getMedia(j)
                && (ci.media[j].status == PJSUA_CALL_MEDIA_ACTIVE
                    || ci.media[j].status == PJSUA_CALL_MEDIA_REMOTE_HOLD))
            {
                AudioMedia *audMed = (AudioMedia *)(*it)->getMedia(j);
                audMed->startTransmit(*recorder);
            }
        }
    }

    pthread_rwlock_unlock(&m_lock);
}

int VoipCallListManager::setVidChanged(int callId, bool on)
{
    int result = 0;
    ScopedRWLock srwl(m_lock);
    VoipCall *call = findCall(callId);
    if (call)
    {
        if (on)
        {
            CallOpParam prm;
            prm.opt.videoCount = 0;
            prm.opt.audioCount = 1;
            call->setHold(prm);
        }
        else
        {
            CallOpParam prm(true);
            prm.opt.videoCount = 1;
            prm.opt.audioCount = 1;
            prm.opt.flag = PJSUA_CALL_UNHOLD;
            call->reinvite(prm);
        }
    }
    else
        result = -1;

    return result;
}

int VoipCallListManager::getConferenceParticipants()
{
    int count = 0;

    pthread_rwlock_rdlock(&m_lock);

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        if ((*it)->isInConference())
            ++count;
    }

    pthread_rwlock_unlock(&m_lock);

    if(count < 2)
        count = 0;

    return count;
}

VoipCall *VoipCallListManager::findCall(int callId)
{
    VoipCall *result = 0;

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        if (callId == (*it)->getId())
        {
            result = *it;
            break;
        }
    }

    return result;
}

bool VoipCallListManager::addToConference(VoipCall *call, const AudioMedia *audioMedia)
{
    bool result = false;

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        if (call == *it)
            continue;

        CallInfo ci = (*it)->getInfo();
        for (unsigned j = 0; j < ci.media.size(); j++)
        {
            if (ci.media[j].type == PJMEDIA_TYPE_AUDIO && (*it)->getMedia(j)
                    && (ci.media[j].status == PJSUA_CALL_MEDIA_ACTIVE
                        || ci.media[j].status == PJSUA_CALL_MEDIA_REMOTE_HOLD))
            {
                AudioMedia* audMed = static_cast<AudioMedia*>((*it)->getMedia(j));
                audioMedia->startTransmit(*audMed);
                audMed->startTransmit(*audioMedia);
                (*it)->setInConference();
                call->setInConference();
                result = true;
            }
        }
    }

    return result;
}

bool VoipCallListManager::isAudioUsed()
{
    VoipCall *result = 0;

    for (list<VoipCall *>::iterator it = m_calls.begin(); it != m_calls.end(); ++it)
    {
        result = *it;
        if(result->isAudioUsed())
            return true;
    }

    return false;
}

bool VoipCallListManager::setPort(int type)
{
    AudioManager audMgr;
    audMgr.setPort(AudioManager::AM_PORT_TYPE(type));
    qDebug() << "setPort: " << type;
    usleep(200000);
    return true;
}

