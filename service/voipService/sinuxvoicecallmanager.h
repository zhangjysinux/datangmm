#ifndef SINUXVOICECALLMANAGER_H
#define SINUXVOICECALLMANAGER_H

#include <QObject>
#include <QDBusInterface>
#include "csmanager/voicecallhandler.h"
#include "csmanager/voicecallmanager.h"
#include "csmanager/voicecallmodel.h"
#include "csmanager/voicecallprovidermodel.h"
#include "csmanager/BaotongDial.h"
#include "csmanager/TiantongDial.h"

class SinuxVoicecallManager : public QObject
{
    Q_OBJECT
public:
    explicit SinuxVoicecallManager(QObject *parent = 0);
    static SinuxVoicecallManager *instance(QObject *parent = 0);

    int makeCall(const QString &phoneNum);
    void setMute(bool on);
    void setHandsFree(bool on);
    void mergeIntoConference();
    void hangupConference();
    QString getAudioModeChanged();
    bool isMute();
signals:
    void audioModeChanged();
    void signalMuteChanged();
    void signalConferenceEstablished();
    void signalNewCall(QString callId);
    void signalDialError(QString mess);
public slots:
    void init();
    void onCallHandlerChanged();
    void setHoldConference(bool on);
    VoiceCallHandler *lookupHandler(QString handlerId);
private:
    VoiceCallManager *m_voiceCallManager;
    VoiceCallProviderModel *m_voiceCallProviderModel;
    QString m_provider;
    QDBusInterface *m_interface;
    static SinuxVoicecallManager *m_instance;
    QList<VoiceCallHandler *> m_handlers;

};

#endif // SINUXVOICECALLMANAGER_H
