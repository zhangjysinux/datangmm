#ifndef VOIPCALLMANAGERLTE_H
#define VOIPCALLMANAGERLTE_H

#include <QObject>

class VoipServiceAdaptor;
class VoipCallHandler;

class VoipCallManagerLte : public QObject
{
    Q_OBJECT

public:
    static VoipCallManagerLte *instance();

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
    void mergeIntoConference();
    int numberOfMultipartyParticipants();

private Q_SLOTS:
    void onIncomingCall(int callId, QString remoteIpAddr);
    void onCallStatusChanged(int callId, bool isDisconnected);
    void onMuteChanged();
    void onHandsFreeChanged();
    void onCallIsInConference(int callId);
    void onConferenceEstablished();
    void onConferenceParticipantsChanged();

private:
    explicit VoipCallManagerLte(QObject *parent = 0);

    void removeHandler(VoipCallHandler *handler);

    static VoipCallManagerLte *m_instance;

    VoipServiceAdaptor *m_voipServiceAdaptor;
    QList<VoipCallHandler *> m_handlers;

    friend class VoipCallHandler;
};

#endif // VOIPCALLMANAGERLTE_H
