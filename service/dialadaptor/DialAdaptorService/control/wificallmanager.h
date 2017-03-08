#ifndef WIFICALLMANAGER_H
#define WIFICALLMANAGER_H

#include <QObject>

class VoipServiceAdaptor;
class VoipCallHandler;

class WifiCallManager : public QObject
{
    Q_OBJECT

public:
    static WifiCallManager *instance();

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

public :
    // This function should be called only once at the beginning
    // @rtpPort should be even number
    void printArgs(QString arg0, QString arg1);
    void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
    int makeCall(const QString &ipAddr);
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
    int getCallStatus(int callId);
    QString getCallIpAddr(int callId);
    bool isCallIncoming(int callId);
    bool isCallInConference(int callId);
    int getConferenceParticipants();

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
private:
    explicit WifiCallManager(QObject *parent = 0);

    void removeHandler(VoipCallHandler *handler);

    static WifiCallManager *m_instance;

    VoipServiceAdaptor *m_voipServiceAdaptor;
    QList<VoipCallHandler *> m_handlers;

    friend class VoipCallHandler;
};

#endif // WIFICALLMANAGER_H
