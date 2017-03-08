#ifndef PTTMEDIAINTERFACE_H
#define PTTMEDIAINTERFACE_H

#include "pttmediamanager.h"
#include <QObject>
#include <QDBusConnection>

class PttMediaInterface: public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.pttMedia")

public:
    enum PttState
    {
        PttStatePaused      = 0,
        PttStateListening   = 1,
        PttStateSpeaking    = 2
    };

    PttMediaInterface(QObject* parent = 0);
    ~PttMediaInterface();

public slots:
    void printArgs(QString arg0, QString arg1);

    unsigned onJoinGroup(QString groupAddr, unsigned port); // returns groupId
    int onLeaveGroup(unsigned groupId);
    int onGetPttState(unsigned groupId);    // return type is PttState
    int onSetPttState(unsigned groupId, int state); // state is of type PttState

private slots:
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

signals:
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private:
    int toPttState(PttMediaStream::MediaDirection dir);
    void emitPttStateChangedSignal(unsigned groupId, int state);
    void emitSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private:
    bool m_bIsRegisted;
    QDBusConnection m_dbusConnection;
};

#endif // PTTMEDIAINTERFACE_H
