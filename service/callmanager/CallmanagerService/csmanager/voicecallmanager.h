#ifndef VOICECALLMANAGER_H
#define VOICECALLMANAGER_H

#include "voicecallhandler.h"

#include "voicecallmodel.h"
#include "voicecallprovidermodel.h"

#include <QObject>

#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusArgument>


class VoiceCallManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QDBusInterface* interface READ interface)

    Q_PROPERTY(VoiceCallModel* voiceCalls READ voiceCalls NOTIFY voiceCallsChanged)
    Q_PROPERTY(VoiceCallProviderModel* providers READ providers NOTIFY providersChanged)

    Q_PROPERTY(QString defaultProviderId READ defaultProviderId NOTIFY defaultProviderChanged)

    Q_PROPERTY(VoiceCallHandler* activeVoiceCall READ activeVoiceCall NOTIFY activeVoiceCallChanged)
    Q_PROPERTY(VoiceCallHandler* confVoiceCall READ confVoiceCall)
    Q_PROPERTY(VoiceCallHandler* onCallVoiceCall READ onCallVoiceCall NOTIFY onCallVoiceCallChanged)
    Q_PROPERTY(VoiceCallHandler* onHoldVoiceCall READ onHoldVoiceCall NOTIFY onHoldVoiceCallChanged)

    Q_PROPERTY(QString audioMode READ audioMode WRITE setAudioMode NOTIFY audioModeChanged)
    Q_PROPERTY(bool isAudioRouted READ isAudioRouted WRITE setAudioRouted NOTIFY audioRoutedChanged)
    Q_PROPERTY(bool isMicrophoneMuted READ isMicrophoneMuted WRITE setMuteMicrophone NOTIFY microphoneMutedChanged)
    Q_PROPERTY(bool isSpeakerMuted READ isSpeakerMuted WRITE setMuteSpeaker NOTIFY speakerMutedChanged)
    Q_PROPERTY(bool isAnyCallOnCall READ isAnyCallOnCall NOTIFY isAnyCallOnCallChanged)
    Q_PROPERTY(bool isAnyCallOnHold READ isAnyCallOnHold NOTIFY isAnyCallOnHoldChanged)
    Q_PROPERTY(int numberOfMultipartyParticipants READ numberOfMultipartyParticipants NOTIFY numberOfMultipartyParticipantsChanged)

    Q_PROPERTY(bool bluetoothState READ bluetoothState  NOTIFY bluetoothStateChanged)
    Q_PROPERTY(bool headsetState READ headsetState  NOTIFY headsetStateChanged)


public:
    explicit VoiceCallManager(QObject *parent = 0);
            ~VoiceCallManager();

    QDBusInterface* interface() const;
    VoiceCallModel* voiceCalls() const;
    VoiceCallProviderModel* providers() const;

    QString defaultProviderId() const;

    VoiceCallHandler* activeVoiceCall() const;
    VoiceCallHandler* confVoiceCall() const;
    VoiceCallHandler* onCallVoiceCall() const;
    VoiceCallHandler* onHoldVoiceCall() const;
    QString audioMode() const;
    bool isAudioRouted() const;

    bool isMicrophoneMuted() const;
    bool isSpeakerMuted() const;

    bool bluetoothState() const;
    bool headsetState() const;

    Q_INVOKABLE void setVoiceCallToCall(int iIndex);
    Q_INVOKABLE int getCallInstanceIndex(const QString &handlerId);
    Q_INVOKABLE void hangupMultipartyCalls();
    Q_INVOKABLE void hangupActiveCall();

    static void parseQDbusMessage(const QVariantList &list, const QStringList &listModemPath);
Q_SIGNALS:
    void error(const QString &message);
    void dialError(const QString &message);
    void confCallFinished(const QString &message);
    void splitConfFinished(const QString &message);
    void providersChanged();
    void voiceCallsChanged();

    void defaultProviderChanged();

    void activeVoiceCallChanged();

    void audioModeChanged();
    void audioRoutedChanged();
    void microphoneMutedChanged();
    void speakerMutedChanged();
    void startDial(QString dialingNumber,QString name);
    void onCallVoiceCallChanged();

    void isAnyCallOnCallChanged();
    void isAnyCallOnHoldChanged();
    void onHoldVoiceCallChanged();
    void bluetoothStateChanged();
    void headsetStateChanged();
    void numberOfMultipartyParticipantsChanged();
    void setVoicecallEncryptFinished(const QString &message,const QString &number, bool flag);

public Q_SLOTS:
    void dial(const QString &providerId, const QString &msisdn, bool bEmergencyCallAcceptedOnly, bool bLaunchDialer = false);
    void dial(const QString &provider, const QString &msisdn,QString name = "");
    Q_INVOKABLE void createConfCall(const QString &provider);
   // void isAnyCallOnCallChanged();
    //void onCallVoiceCallChanged();
    //void isAnyCallOnHoldChanged();
    // void onHoldVoiceCallChanged();
  //  void numberOfMultipartyParticipantsChanged();

    void silenceRingtone();

    void setAudioMode(const QString &mode);
    void setAudioRouted(bool on);
    void setMuteMicrophone(bool on = true);
    void setMuteSpeaker(bool on = true);

    bool startDtmfTone(const QString &tone);
    bool stopDtmfTone();
    bool isAnyCallOnCall();
    bool isAnyCallOnHold();
    int numberOfMultipartyParticipants();
    void onCallStatusChanged();

    void checkAnyCallOnCall();
    void checkAnyCallOnHold();
    void makeSureOneCallIsOnCall();
    void setActiveVoiceCall(int iIndex);
    //void setVoicecallEncrypt(bool flag = true);
    //void setCallBackGround(bool background);

protected Q_SLOTS:
    void initialize(bool notifyError = false);
    void onDialError(QString errorString);
    void onConfCallFinished(QString string);
    void onSplitConfFinished(QString string);
    void onProvidersChanged();
    void onVoiceCallsChanged();
    void onActiveVoiceCallChanged();

    void onPendingCallFinished(QDBusPendingCallWatcher *call);

    void onPendingGetModemsFinished(QDBusPendingCallWatcher *watcher);
    void onPendingEmergencyNumbersFinished(QDBusPendingCallWatcher *watcher);

private:
    class VoiceCallManagerPrivate *d_ptr;

    Q_DISABLE_COPY(VoiceCallManager)
    Q_DECLARE_PRIVATE(VoiceCallManager)

    static bool argToString(const QDBusArgument &busArg, const QStringList &listModemPath);
    static bool variantToString(const QVariant &arg, const QStringList &listModemPath);
    void getModemPaths();
    bool getNextEmergencyNumber(int iModemListIndex);

    int m_iCurrentModemPathIndex;
    QStringList m_listModemPath;
    QString m_strProvider;
    QString m_strDialNumber;
    bool m_bIsAnyCallOnCall;
    bool m_bIsAnyCallOnHold;
    int m_iNumberOfMultipartyParticipants;

    int getCountOfActiveCalls();
    QList<VoiceCallHandler *>getHandlerPriorityList();
};

#endif // VOICECALLMANAGER_H
