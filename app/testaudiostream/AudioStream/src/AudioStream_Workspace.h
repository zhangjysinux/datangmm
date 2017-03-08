#ifndef __AUDIOSTREAM_WORKSPACE__
#define __AUDIOSTREAM_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QtNetwork>
#include <QDBusInterface>


using namespace SYBEROS;

class AudioStream_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;

    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_audioInputIODevice;
    QIODevice *m_audioOutputIODevice;
    QUdpSocket *m_serverUdp;
    QUdpSocket *m_sendUdp;
    QByteArray m_header;//终端发送给电台的报头
    bool m_isFirst;
    QString m_zhwIp;//战互网ip
    int m_udpPort;//端口
    bool m_talkFlag;
    QDBusInterface m_battleInterface;

public:
    AudioStream_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

    void initAudioInput();
    void destoryAudioOutput();

    void initAudioOutput();
    void userInit();

    Q_INVOKABLE void onStartTalkClick();
    Q_INVOKABLE void onEndTalkClick();

signals:
    void signalText(QVariant text);

public slots:
    //终端采集语音数据发送到电台
    void captureDataFromDevice();
    //电台发送语音到终端播放
    void onReadyRead();

};


#endif //__AUDIOSTREAM_WORKSPACE__
