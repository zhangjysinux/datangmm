#ifndef VOIPINTERFACE_H
#define VOIPINTERFACE_H

#include <QDBusConnection>
#include <QDBusVariant>
#include <QObject>
#include <QString>
#include <QMutex>
#include "pttmediamanager.h"

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

enum VoipCallErrorCode
{
    SOCKET_CONNECT_SUCCESS = 1,
    SOCKET_CONNECT_FAIL,
    PORT_NEGO_SUCCESS,
    PORT_NEGO_FAIL,
    DIAL_SUCCESS,
    DIAL_FAIL,
};

class VoipInterface: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.voip")

public:
//    enum VoipCallStatus {
//        STATUS_NULL,
//        STATUS_ACTIVE,
//        STATUS_HELD,
//        STATUS_DIALING,
//        STATUS_ALERTING,
//        STATUS_INCOMING,
//        STATUS_WAITING,
//        STATUS_DISCONNECTED
//    };

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
    void emitCallError(int callId);
    
    //ptt
    void emitPttStateChangedSignal(unsigned groupId, int state);
    void emitSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

    //p2p
    void emitSignalP2PStateChanged(QString addr, int state);
    void emitSignalP2PRecordFileFinished(QString addr, QString fileName);

    //set lte adhoc network
    void setInterface(int networkType);

    QString getAdHocIpAddress();
    int getAdHocPort();

public slots:
    void printArgs(QString arg0, QString arg1);
    int onInitialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
    int onDelayInitialize();
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
    int onGetCallError(int callId); // VoipCallErrorCode

    
	//ptt
    unsigned onJoinGroup(QString groupAddr, unsigned port); // returns groupId
    int onLeaveGroup(unsigned groupId);
    int onGetPttState(unsigned groupId);    // return type is PttState
    int onSetPttState(unsigned groupId, int state); // state is of type PttState
    
    //p2p
    int onCallP2P(QString addr); // returns groupId
    int onHangupP2P(QString addr);
    int onGetP2PState(QString addr);    // return type is PttState
    int onSetP2PState(QString addr, int state); // state is of type PttState

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
    void signalCallError(int callId);

    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

    //p2p
    void signalP2PStateChanged(QString addr, int state);
    void signalP2PRecordFileFinished(QString addr, const QString &fileName);

private slots:
    //ptt
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private:
    //ptt
    int toPttState(PttMediaStream::MediaDirection dir);

    //p2p
//    int toP2PState(P2PMediaStream::MediaDirection dir);

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
    unsigned m_rtpPort;
    unsigned m_rtpPortRange;
    QMutex m_mutex ;
};

#endif // VOIPINTERFACE_H
