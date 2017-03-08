#ifndef VOIPINTERFACE_H
#define VOIPINTERFACE_H

#include <QDBusConnection>
#include <QDBusVariant>
#include <QObject>
#include <QString>
#include "p2pmediamanager.h"

enum VoipCallStatus {
    STATUS_NULL,
    STATUS_ACTIVE,
    STATUS_HELD,
    STATUS_DIALING,
    STATUS_ALERTING,
    STATUS_INCOMING,
    STATUS_WAITING,
    STATUS_DISCONNECTED
};

enum VoipCallErrorCode
{
    SOCKET_CONNECT_SUCCESS = 1,
    SOCKET_CONNECT_FAIL,
    PORT_NEGO_SUCCESS,
    PORT_NEGO_FAIL,
    DIAL_SUCCESS,
    DIAL_FAIL,
};

enum PttState
{
    PttStatePaused      = 0,
    PttStateListening   = 1,
    PttStateSpeaking    = 2
};

class VoipInterface: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.voip")

public:
    VoipInterface(QObject* parent = 0);
    ~VoipInterface();

public slots:
    int onInitialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);

    //p2p
    int onCallP2P(QString addr); // returns groupId
    int onHangupP2P(QString addr);
    int onGetP2PState(QString addr);    // return type is PttState
    int onSetP2PState(QString addr, int state); // state is of type PttState
    int onRecord(QString addr, bool on);

signals:
    //p2p

    void signalCallP2PFinished(const QString &addr, int state);
    void signalRecordFinished(const QString &addr, const QString &fileName);

private slots:

private:
    bool m_bIsRegisted;
    QDBusConnection m_dbusConnection;
    bool m_isInitialized;
    unsigned m_sipPort;
    QString m_filename;
    P2PManager *m_callManager;
};

#endif // VOIPINTERFACE_H
