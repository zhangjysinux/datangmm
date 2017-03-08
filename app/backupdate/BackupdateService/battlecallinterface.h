#ifndef BATTLECALLINTERFACE_H
#define BATTLECALLINTERFACE_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QtNetwork>
#include <QDBusInterface>
#include <QTimerEvent>
#include <QDBusReply>

class BattleCall : public QObject
{
    Q_OBJECT
public:
    static BattleCall *getInstance();
    int call(const QString &addr, int port);
    int hangup();
    int setPttState(int pttState);
    int getPttState();

private:
    void audioInputInit();
    void audioInputDestory();

    void audioOutputInit();
    void audioOutputDestroy();

    void transportInit();
    void transportDestroy();

private slots:
    //终端采集语音数据发送到电台
    void onCaptureDataFromDevice();
    //电台发送语音到终端播放
    void onReadyRead();

    void onServerError(QAbstractSocket::SocketError error);

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void signalBattleCallStateChanged(const QString &ip, int state);

private:
    explicit BattleCall(QObject *parent = 0);
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_audioInputIODevice;
    QIODevice *m_audioOutputIODevice;
    QUdpSocket *m_serverUdp;
    QUdpSocket *m_sendUdp;
    QByteArray m_header;//终端发送给电台的报头
    QString m_serverIp;
    int m_port;
    bool m_isFirst;
    QDBusInterface m_battleInterface;
    static BattleCall *m_instance;
    bool m_bindFlag;
};

enum BattleState
{
    BattleState_CallStateUnknown,
    BattleState_CallStateConnecting,
    BattleState_CallStateConnected,
    BattleState_CallStateDisconnected,
    BattleState_CallStateTimeOut
};

class BattleCallInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.battlecall")
public:
    static BattleCallInterface *getInstance();

public slots:
    int onCallBattle(const QString &addr);
    int onHangupBattle(const QString &addr);
    int onSetBattlePttState(const QString &addr, int pttState);
    int onGetBattlePttState(const QString &addr);
    int onSetPort(int port);

signals:
    void signalBattleCallStateChanged(const QString &ip, int state);

private:
    explicit BattleCallInterface(QObject *parent = 0);

private:
    static BattleCallInterface *instance;
    QMap<QString,BattleCall*> m_addrCallMap;
    int m_port;
};

#endif // BATTLECALLINTERFACE_H
