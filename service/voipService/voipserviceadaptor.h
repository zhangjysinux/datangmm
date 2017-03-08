#ifndef VOIPSERVICEADAPTOR_H
#define VOIPSERVICEADAPTOR_H

#include "voipdbusdata.h"
#include <QObject>
#include <QDBusInterface>

class VoipServiceAdaptor : public QObject
{
    Q_OBJECT

public:
    explicit VoipServiceAdaptor(const QString serviceName, QObject *parent = 0);
    Q_INVOKABLE int initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
    // returns callId
    Q_INVOKABLE int makeCall(QString ipAddr);
    Q_INVOKABLE int makeVideoCall(QString ipAddr);
    Q_INVOKABLE int answer(int callId);
    Q_INVOKABLE int hangup(int callId);
    Q_INVOKABLE int setHold(int callId, bool on);
    Q_INVOKABLE int setHoldConference(bool on);
    Q_INVOKABLE VoipCallInfo getCallInfo(int callId);
    Q_INVOKABLE int getCallStatus(int callId);
    Q_INVOKABLE QString getCallIpAddr(int callId);
    Q_INVOKABLE bool isCallIncoming(int callId);
    Q_INVOKABLE bool isCallInConference(int callId);
    Q_INVOKABLE int hangupConference();
    Q_INVOKABLE int hangupAllCalls();
    Q_INVOKABLE int setMute(bool on);
    Q_INVOKABLE bool isMute();
    Q_INVOKABLE int setHandsFree(bool on);
    Q_INVOKABLE bool isHandsFree();
    Q_INVOKABLE int record(bool on);
    Q_INVOKABLE int mergeIntoConference();
    Q_INVOKABLE int getConferenceParticipants();
    Q_INVOKABLE int VidEnabledChanged(int callId, bool on);
    Q_INVOKABLE int VidInputChanged(int callId);
    Q_INVOKABLE int GetAudOrVideo(int callId);//0:audio 1:video
    Q_INVOKABLE int getCallError(int callId); // VoipCallErrorCode

    //ptt
    unsigned joinGroup(QString groupAddr, unsigned port); // returns groupId
    int leaveGroup(unsigned groupId);
    int getPttState(unsigned groupId);    // return type is PttState
    int setPttState(unsigned groupId, int state); // state is of type PttState

signals:
    void signalIncomingCall(int callId, QString remoteIpAddr);
    void signalCallState(int callId, int callState);
    void signalCallStatusChanged(int callId, bool isDisconnected);
    void signalMuteChanged();
    void signalHandsFreeChanged();
    void signalCallIsInConference(int callId);
    void signalConferenceEstablished();
    void signalConferenceParticipantsChanged();
    void signalVoipFrameData();
    void signalV4l2FrameData();
    void signalVidChanged(bool isVideoHold);
    void signalCallError(int callId);

    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private slots:
    void onSignalPttStateChanged(unsigned groupId, int state);
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

public slots:
    void onIncomingCall(int callId, QString remoteIpAddr);
    void onCallState(int callId, int callState);
    void onCallStatusChanged(int callId, bool isDisconnected);
    void onMuteChanged();
    void onHandsFreeChanged();
    void onCallIsInConference(int callId);
    void onConferenceEstablished();
    void onConferenceParticipantsChanged();
    void onVoipFrameData();
    void onV4l2FrameData();
    void onsignalVidChanged(bool isVideoHold); //true: audio　false: vdeio+audio

private:
    QDBusInterface m_interface;
};

#endif // VOIPSERVICEADAPTOR_H
