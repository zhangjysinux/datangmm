#ifndef VOIPINTERFACE_H
#define VOIPINTERFACE_H

#include <QDBusConnection>
#include <QDBusVariant>
#include <QObject>
#include <QString>
#include "pttmediamanager.h"

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

class VoipInterface: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.voip")

public:
    //ptt
    enum PttState
    {
        PttStatePaused      = 0,
        PttStateListening   = 1,
        PttStateSpeaking    = 2
    };

    VoipInterface(QObject* parent = 0);
    ~VoipInterface();
    
    //ptt
    void emitPttStateChangedSignal(unsigned groupId, int state);
    void emitSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

public slots:
	//ptt
    unsigned onJoinGroup(QString groupAddr, unsigned port); // returns groupId
    int onLeaveGroup(unsigned groupId);
    int onGetPttState(unsigned groupId);    // return type is PttState
    int onSetPttState(unsigned groupId, int state); // state is of type PttState
    
signals:
    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private slots:
    //ptt
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private:
    //ptt
    int toPttState(PttMediaStream::MediaDirection dir);

private:
    bool m_bIsRegisted;
    QDBusConnection m_dbusConnection;
    bool m_isInitialized;
    unsigned m_sipPort;
    bool m_isMute;
    QString m_filename;
    bool m_bIsVideoHold;
    bool m_bIsStart;
    int m_iAudOrVideo;
};

#endif // VOIPINTERFACE_H
