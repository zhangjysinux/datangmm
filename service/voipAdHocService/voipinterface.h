#ifndef VOIPINTERFACE_H
#define VOIPINTERFACE_H

#include <QDBusConnection>
#include <QDBusVariant>
#include <QObject>
#include <QString>
#include "pttmediamanager.h"

class VoipInterface: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.voip")

public:
    enum VoipCallStatus {
        STATUS_NULL,
        STATUS_ACTIVE,
        STATUS_HELD,
        STATUS_DIALING,
        STATUS_ALERTING,
        STATUS_INCOMING,
        STATUS_WAITING,
        STATUS_DISCONNECTED
    };

    //ptt
    enum PttState
    {
        PttStatePaused      = 0,
        PttStateListening   = 1,
        PttStateSpeaking    = 2
    };

    VoipInterface(QObject* parent = 0);
    ~VoipInterface();

    void emitIncomingCallSignal(int callId, const QString &remoteIpAddr);
    void emitCallStateSignal(int callId, int callState);
    void emitCallStatusChanged(int callId, bool isDisconnected = false);
    void emitMuteChanged();
    void emitHandsFreeChanged();
    void emitCallIsInConference(int callId);
    void emitConferenceEstablished();
    void emitConferenceParticipantsChanged();
    void emitVoipFrameData();
    void emitV4l2FrameData();
    void emitVidChanged(bool isVideoHold=true);
	void emitAudOrVideo(int iAudOrVideo);
    
    //ptt
    void emitPttStateChangedSignal(unsigned groupId, int state);
    void emitSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

public slots:
    void printArgs(QString arg0, QString arg1);
    int onInitialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
    int onMakeCall(QString ipAddress);
    int onMakeVideoCall(QString ipAddress);
    int onAnswer(int callId);
    int onHangup(int callId);
    int onSetHold(int callId, bool on);
    int onSetHoldConference(bool on);
    QDBusVariant onGetCallInfo(int callId);
    int onGetCallStatus(int callId);
    QString onGetCallIpAddr(int callId);
    bool onIsCallIncoming(int callId);
    bool onIsCallInConference(int callId);
    int onHangupConference();
    int onHangupAllCalls();
    int onSetMute(bool on);
    bool onIsMute();
    int onSetHandsFree(bool on);
    bool onIsHandsFree();
    int onRecord(bool on);
    int onMergeIntoConference();
    int onGetConferenceParticipants();
    int onVidEnabledChanged(int callId, bool on);
    int onVidInputChanged(int callId);
    int onGetAudOrVideo(int callId);//0:audio 1:video
    
	//ptt
    unsigned onJoinGroup(QString groupAddr, unsigned port); // returns groupId
    int onLeaveGroup(unsigned groupId);
    int onGetPttState(unsigned groupId);    // return type is PttState
    int onSetPttState(unsigned groupId, int state); // state is of type PttState
    
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
    void signalVidChanged(bool bVidIsHold);

    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private slots:
    //ptt
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private:
    //ptt
    int toPttState(PttMediaStream::MediaDirection dir);

private:
    bool m_bIsRegisted;
    QDBusConnection m_dbusConnection;
    bool m_isInitialized;
    unsigned m_sipPort;
    bool m_isMute;
    QString m_filename;
    bool m_bIsVideoHold;
    bool m_bIsStart;
    int m_iAudOrVideo;
};

#endif // VOIPINTERFACE_H
