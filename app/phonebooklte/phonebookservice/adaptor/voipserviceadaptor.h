#ifndef VOIPSERVICEADAPTOR_H
#define VOIPSERVICEADAPTOR_H

#include <QObject>
#include <QDBusInterface>

class VoipServiceAdaptor : public QObject
{
    Q_OBJECT

public:
    static VoipServiceAdaptor* getInstance(const QString &serviceName);
       //ptt
    unsigned joinGroup(QString groupAddr, unsigned port); // returns groupId
    int leaveGroup(unsigned groupId);
    int getPttState(unsigned groupId);    // return type is PttState
    int setPttState(unsigned groupId, int state); // state is of type PttState

signals:
    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private slots:
    void onSignalPttStateChanged(unsigned groupId, int state);
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

private:
    QDBusInterface m_interface;
    static VoipServiceAdaptor* instance;
    explicit VoipServiceAdaptor(const QString serviceName, QObject *parent = 0);

};

#endif // VOIPSERVICEADAPTOR_H
