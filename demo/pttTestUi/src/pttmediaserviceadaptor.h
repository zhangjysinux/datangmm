#ifndef PTTSERVICEADAPTOR_H
#define PTTSERVICEADAPTOR_H

#include <QObject>
#include <QString>
#include <QDBusInterface>

class PttMediaServiceAdaptor : public QObject
{
    Q_OBJECT

public:
    enum PttState
    {
        PttStatePaused      = 0,
        PttStateListening   = 1,
        PttStateSpeaking    = 2
    };

    explicit PttMediaServiceAdaptor(QObject *parent = 0);

    // returns groupId on success, returns 0 on error
    Q_INVOKABLE unsigned joinGroup(QString groupAddr, unsigned port);

    // leave group specified by groupId
    Q_INVOKABLE int leaveGroup(unsigned groupId);

    // returns state of type PttState
    Q_INVOKABLE int getPttState(unsigned groupId);

    // state is of type PttState
    Q_INVOKABLE int setPttState(unsigned groupId, int state);

signals:
    void signalPttStateChanged(unsigned groupId, int state);

public slots:
    // state is of type PttState
    void onPttStateChanged(unsigned groupId, int state);

private:
    QDBusInterface m_interface;
};

#endif // PTTSERVICEADAPTOR_H
