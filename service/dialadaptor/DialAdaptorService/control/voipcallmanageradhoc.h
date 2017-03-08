#ifndef VOIPCALLMANAGERADHOC_H
#define VOIPCALLMANAGERADHOC_H

#include <QObject>

class VoipServiceAdaptor;
class VoipCallHandler;

class VoipCallManagerAdHoc : public QObject
{
    Q_OBJECT

public:
    static VoipCallManagerAdHoc *instance();

    // returns 0 if not found
    VoipCallHandler *lookupHandler(int callId);

    bool isMute();
    bool isHandsFree();

Q_SIGNALS:
    void signalNewCall(int callId);
    void signalMuteChanged();
    void signalHandsFreeChanged();
    void numberOfMultipartyParticipantsChanged();

public Q_SLOTS:
    // This function should be called only once at the beginning
    // @rtpPort should be even number
    void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
    int makeCall(const QString &ipAddr);
    void hangupConference();
    void hangupAllCalls();
    void setMute(bool on);
    void setHandsFree(bool on);
    void record(bool on);
    int numberOfMultipartyParticipants();

private Q_SLOTS:
    void onIncomingCall(int callId, QString remoteIpAddr);
    void onCallStatusChanged(int callId, bool isDisconnected);
    void onMuteChanged();
    void onHandsFreeChanged();
    void onCallIsInConference(int callId);
    void onConferenceParticipantsChanged();

private:
    explicit VoipCallManagerAdHoc(QObject *parent = 0);

    void removeHandler(VoipCallHandler *handler);

    static VoipCallManagerAdHoc *m_instance;

    VoipServiceAdaptor *m_voipServiceAdaptor;
    QList<VoipCallHandler *> m_handlers;

    friend class VoipCallHandler;
};

#endif // VOIPCALLMANAGERADHOC_H
