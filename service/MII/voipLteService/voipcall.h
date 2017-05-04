#ifndef VOIPCALL_H
#define VOIPCALL_H

#include <pjsua2.hpp>
#include <QObject>
#include <QTimer>

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
    void onInstantMessageStatus(OnInstantMessageStatusParam &prm);

    bool isInConference() const;
    void setInConference();

    bool isDisconnected() const;
    void setDisconnected();
  

    bool isAudioUsed();
    void setAudioUsed(bool use);

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
    QTimer m_heartTimer;
};

#endif // VOIPCALL_H
