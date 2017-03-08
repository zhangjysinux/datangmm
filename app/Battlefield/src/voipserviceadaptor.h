#ifndef VOIPSERVICEADAPTOR_H
#define VOIPSERVICEADAPTOR_H

//#include "voipdbusdata.h"
#include <QObject>
#include <QDBusInterface>

class VoipServiceAdaptor : public QObject
{
    Q_OBJECT

public:
    explicit VoipServiceAdaptor(const QString serviceName = "com.sinux.voipVideo", QObject *parent = 0);
    Q_INVOKABLE int initialize(unsigned sipPort = 5080, unsigned rtpPort = 33333, unsigned rtpPortRange = 10);
    // returns callId
    Q_INVOKABLE int makeCall(QString ipAddr);
    Q_INVOKABLE int answer(int callId);
    Q_INVOKABLE int hangup(int callId);
    Q_INVOKABLE int setHold(int callId, bool on);
//    Q_INVOKABLE VoipCallInfo getCallInfo(int callId);
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

signals:
    void signalIncomingCall(int callId, QString remoteIpAddr);
    void signalCallState(int callId, int callState);
    void signalCallStatusChanged(int callId, bool isDisconnected);
    void signalMuteChanged();
    void signalHandsFreeChanged();
    void signalCallIsInConference(int callId);
    void signalConferenceEstablished();
    void signalConferenceParticipantsChanged();

public slots:
    void onIncomingCall(int callId, QString remoteIpAddr);
    void onCallState(int callId, int callState);
    void onCallStatusChanged(int callId, bool isDisconnected);
    void onMuteChanged();
    void onHandsFreeChanged();
    void onCallIsInConference(int callId);
    void onConferenceEstablished();
    void onConferenceParticipantsChanged();

private:
    QDBusInterface m_interface;
};

#endif // VOIPSERVICEADAPTOR_H
