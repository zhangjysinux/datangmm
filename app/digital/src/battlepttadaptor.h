#ifndef BATTLEPTTADAPTOR_H
#define BATTLEPTTADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include <QAudioInput>
#include <QAudioOutput>
#include <QUdpSocket>

class BattlePTTAdaptor : public QObject
{
    Q_OBJECT
public:
    static BattlePTTAdaptor* getInstane();
    virtual ~BattlePTTAdaptor();

    Q_INVOKABLE void pttStart();
    Q_INVOKABLE void pttEnd();
    Q_INVOKABLE void setTestMode();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void onBoradcastIn();
    void onBoradcastInError(QAbstractSocket::SocketError error);
    void onAudioIn();

private:
    void initAudioInput();
    void initAudioOutput();
    void initBattleBoradcastIn();

    explicit BattlePTTAdaptor(QObject *parent = 0);
    static BattlePTTAdaptor *m_instance;
    QDBusInterface m_battleInterface;
    QAudioInput *m_audioInput;
    QIODevice *m_audioInDevice;
    QAudioOutput *m_audioOutput;
    QIODevice *m_audioOutDevice;
    QUdpSocket *m_inSocket;
    bool m_pttState;
    bool m_testMode;
};

#endif // BATTLEPTTADAPTOR_H
