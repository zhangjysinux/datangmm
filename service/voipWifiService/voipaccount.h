#ifndef VOIPACCOUNT_H
#define VOIPACCOUNT_H

#include "voipcalllistmanager.h"
#include <pjsua2.hpp>
#include <QObject>

using namespace pj;

class VoipAccount : public QObject, public Account
{
    Q_OBJECT
public:
    VoipAccount();
    ~VoipAccount();

    void onRegState(OnRegStateParam &prm);
    void onIncomingCall(OnIncomingCallParam &iprm);

signals:
    void signalIncommingCallHandle(const OnIncomingCallParam &iprm);

private slots:
    void onIncommingCallHandle(const OnIncomingCallParam &iprm);

public:
    VoipCallListManager m_callListMgr;
};

#endif // VOIPACCOUNT_H
