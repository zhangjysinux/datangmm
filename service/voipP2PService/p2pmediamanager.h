#ifndef P2PMEDIAMANAGER_H
#define P2PMEDIAMANAGER_H

#include <QString>
#include <pjsua2.hpp>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMap>

using namespace pj;
using namespace std;

class P2PAccount : public QObject, public Account
{
    Q_OBJECT
public:
    P2PAccount();
    ~P2PAccount();

    void onRegState(OnRegStateParam &prm);
    void onIncomingCall(OnIncomingCallParam &iprm);
};


class P2PCall : public QObject, public Call
{
    Q_OBJECT
public:
    P2PCall(Account &acc, int call_id = PJSUA_INVALID_ID);
    ~P2PCall();

    void onCallState(OnCallStateParam &prm);
    void onCallMediaState(OnCallMediaStateParam &prm);
    int setPttState(int state);
    int getPttState();
    int record(bool on);

signals:
    void signalRecordFinished(const QString &addr, const QString &fileName);

private:
    P2PAccount *m_myAcc;
    int m_state;
    AudioMediaRecorder *m_recorder;
    bool m_recordOn;
    QString m_recordFileName;
    bool m_connected;

    QString getRemoteAddr();
};

class P2PManager : public QObject
{
    Q_OBJECT
public:
    static P2PManager *getInstance();
    ~P2PManager();

    void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);

    int makeCall(const QString &addr);
    int answerCall(const QString &addr);
    int hangupCall(const QString &addr);
    int setPttState(const QString &addr, int state);
    int getPttState(const QString &addr);
    int record(const QString& addr, bool on);

    bool removeAddr(const QString &addr);
    bool addAddr(const QString &addr, P2PCall* call);
    QString getIp();

private slots:
    void onSignalRecordFinished(const QString &addr, const QString &fileName);

signals:
    void signalCallP2PFinished(const QString &addr, int state);
    void signalRecordFinished(const QString &addr, const QString &fileName);

private:
    P2PManager();

    static P2PManager *instance;
    QMap<QString, P2PCall*> m_addrCallMap;
    Endpoint *m_endpoint;
    unsigned m_sipPort;
    P2PAccount *m_account;
};

#endif // P2PMEDIAMANAGER_H
