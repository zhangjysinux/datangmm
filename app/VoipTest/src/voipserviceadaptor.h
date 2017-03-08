#ifndef VOIPSERVICEADAPTOR_H
#define VOIPSERVICEADAPTOR_H

#include <QObject>
#include <QDBusInterface>

class VoipServiceAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit VoipServiceAdaptor(QObject *parent = 0);
    Q_INVOKABLE int makeCall(QString ip);
    Q_INVOKABLE int answer(int callId);
    Q_INVOKABLE int hangup(int callId);
    Q_INVOKABLE int hangupAllCalls();

    Q_PROPERTY(int callId READ getCallId WRITE setCallId)



signals:
    void signalIncomingCall(int callId);

public slots:
    void onIncomingCall(int callId);
    int getCallId() const;
    void setCallId(int callId);
private:
    QDBusInterface m_interface;
    int m_callId;
};

#endif // VOIPSERVICEADAPTOR_H
