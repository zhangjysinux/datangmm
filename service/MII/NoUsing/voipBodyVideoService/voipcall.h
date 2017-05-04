#ifndef VOIPCALL_H
#define VOIPCALL_H

#include <pjsua2.hpp>
#include <QObject>
#include <QTimer>
#include <QMutex>

using namespace pj;

class VoipAccount;

class VoipCall : public QObject, public Call
{
    Q_OBJECT
public:
    VoipCall(Account &acc, int call_id = PJSUA_INVALID_ID);
    ~VoipCall();

    void onCallState(OnCallStateParam &prm);
    void onCallMediaState(OnCallMediaStateParam &prm);
    void onInstantMessage(OnInstantMessageParam &prm);
//    void onInstantMessageStatus(OnInstantMessageStatusParam &prm);

    bool isInConference();
    void setInConference();

    bool isDisconnected();
    void setDisconnected();
  

    bool isAudioUsed();
    void setAudioUsed(bool use);

    void setWillBeConference(int willBe);

signals:

public slots:
    void onSendHeart();
private:
    VoipAccount *m_myAcc;
    bool m_isInConference;
    bool m_isDisconnected;
    bool m_isAudioUsed;
    bool m_startRecordHeart;
    int m_currentTimeD;//time over
    int m_willBeConference;
//    pthread_rwlock_t m_lock;
//    QTimer m_heartTimer;
};

#endif // VOIPCALL_H
