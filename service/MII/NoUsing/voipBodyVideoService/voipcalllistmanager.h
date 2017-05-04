#ifndef VOIPCALLLISTMANAGER_H
#define VOIPCALLLISTMANAGER_H

#include <pjsua2.hpp>
#include <QString>
#include <QObject>
#include <QTimer>
#include <list>
#include <pthread.h>
#include <audiomanager.h>

using namespace pj;
using namespace std;

class VoipCall;

class VoipCallListManager : public QObject
{
    Q_OBJECT
public:
    static VoipCallListManager &instance();

    VoipCallListManager();
    ~VoipCallListManager();

    void addCall(VoipCall *call);
    void removeCall(VoipCall *call);
    int answerCall(int callId);
    int hangupCall(int callId);
    int setHold(int callId, bool on);
    int setHoldConference(bool on);
    int getCallStatus(int callId);
    QString getCallIpAddr(int callId);
    bool isCallIncoming(int callId);
    bool isCallInConference(int callId);
    int hangupConference();
    bool mergeIntoConference();
    void setMute(bool on);
    void record();
    int getConferenceParticipants();
    bool isAudioUsed();
    static bool setPort(int type);
    static int getPort();
    int setVidChanged(int callId, bool on);
    int getCallError(int callId);
    bool addToConference(VoipCall *call, const AudioMedia *audioMedia);

private slots:
    void onHeartTimeout();

private:
//    enum VoipCallStatus
//    {
//        STATUS_NULL,
//        STATUS_ACTIVE,
//        STATUS_HELD,
//        STATUS_DIALING,
//        STATUS_ALERTING,
//        STATUS_INCOMING,
//        STATUS_WAITING,
//        STATUS_DISCONNECTED
//    };

    VoipCall *findCall(int callId);
    static VoipCallListManager *m_instance;
    list<VoipCall *> m_calls;
    pthread_rwlock_t m_lock;
    QTimer m_heartTimer;
};

#endif // VOIPCALLLISTMANAGER_H
