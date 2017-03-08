#ifndef VOIPCALLMANAGER_H
#define VOIPCALLMANAGER_H

#include <QObject>
#include "../../../app/commondata/datatype/callhistorydata.h"
#include "../../../app/commondata/datatype/commondata.h"
class VoipServiceAdaptor;
class VoipCallHandler;

class VoipCallManager : public QObject
{
    Q_OBJECT

public:
    static VoipCallManager *instanceWifi();
    static VoipCallManager *instanceAdHoc();
    static VoipCallManager *instanceLte();
    static VoipCallManager *instanceBattle();

    // returns 0 if not found
    VoipCallHandler *lookupHandler(int callId);

    bool isMute();
    bool isHandsFree();

Q_SIGNALS:
    void signalNewCall(int callId);
    void signalMuteChanged();
    void signalHandsFreeChanged();
    void signalConferenceEstablished();
    void numberOfMultipartyParticipantsChanged();

    void signalIncomingCall(int callId, QString remoteIpAddr);
    void signalCallState(int callId, int callState);
    void signalCallStatusChanged(int callId, bool isDisconnected);
    void signalCallIsInConference(int callId);
    void signalConferenceParticipantsChanged();
    void signalVoipFrameData();
    void signalV4l2FrameData();
    void signalVidChanged(bool isVideoHold); //fase : video   true: audio
    void signalCallError(int callId);

    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

public :
    // This function should be called only once at the beginning
    // @rtpPort should be even number
    void printArgs(QString arg0, QString arg1);
    void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
    int makeCall(const QString &ipAddr);
    int makeVideoCall(QString ipAddr);
    void hangupConference();
    void hangupAllCalls();
    void setMute(bool on);
    void setHandsFree(bool on);
    void record(bool on);
    void mergeIntoConference();
    int numberOfMultipartyParticipants();

    int answer(int callId);
    int hangup(int callId);
    int setHold(int callId, bool on);
    int setHoldConference(bool on);
    int getCallStatus(int callId);
    QString getCallIpAddr(int callId);
    bool isCallIncoming(int callId);
    bool isCallInConference(int callId);
    int getConferenceParticipants();
    int setVidEnabledChanged(int callId, bool on);
    int setVidInputChanged(int callId);
    int getGetAudOrVideo(int callId);//0:audio 1:video

    //ptt
    unsigned joinGroup(QString groupAddr, unsigned port); // returns groupId
    int leaveGroup(unsigned groupId);
    int getPttState(unsigned groupId);    // return type is PttState
    int setPttState(unsigned groupId, int state); // state is of type PttState 2:speak 1:listen

    //network type
    void setNetworkType(NetworkType netType);

private Q_SLOTS:
    void onIncomingCall(int callId, QString remoteIpAddr);
    void onCallStatusChanged(int callId, bool isDisconnected);
    void onMuteChanged();
    void onHandsFreeChanged();
    void onCallIsInConference(int callId);
    void onConferenceEstablished();
    void onConferenceParticipantsChanged();

    void onCallState(int callId, int callState);
    void onVoipFrameData();
    void onV4l2FrameData();
    void onsignalVidChanged(bool isVideoHold); //true: audio　false: vdeio+audio
    void onSignalCallError(int callId);

private:
    explicit VoipCallManager(const QString &serviceName, QObject *parent = 0);

    void removeHandler(VoipCallHandler *handler);

    static VoipCallManager *m_instanceWifi;
    static VoipCallManager *m_instanceAdHoc;
    static VoipCallManager *m_instanceLte;
    static VoipCallManager *m_instanceBattle;


    VoipServiceAdaptor *m_voipServiceAdaptor;
    QList<VoipCallHandler *> m_handlers;
    NetworkType m_netType;

    friend class VoipCallHandler;
};

#endif // VOIPCALLMANAGER_H
