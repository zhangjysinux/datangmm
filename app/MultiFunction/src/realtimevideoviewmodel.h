#ifndef REALTIMEVIDEOVIEWMODEL_H
#define REALTIMEVIDEOVIEWMODEL_H

#include <QObject>
#include "tablemodel.h"
#include "cnotification.h"
#include "cnotificationmanager.h"
class ScreenShot;
class VoipServiceAdaptor;
class SharedMemory;
class RealtimeVideoViewModel : public QObject
{
    Q_OBJECT
public:
    explicit RealtimeVideoViewModel(QObject *parent = 0);
    ~RealtimeVideoViewModel();
    Q_PROPERTY(bool acceptAble READ getAccept WRITE setAccept NOTIFY signalAcceptAbleChanged)
    Q_PROPERTY(QString callIP READ getCallIP WRITE setCallIP NOTIFY signalCallIPChanged)
    Q_PROPERTY(int callId READ getCallId WRITE setCallId NOTIFY signalCallIdChanged)
signals:
    void signalCallInComing();
    void signalCloseCallInComing();
    void signalAcceptAbleChanged(bool accept);
    void signalCallIPChanged(QString callIP);
    void signalCallIdChanged(int callId);
    void signalVideoCallStatusChanged(int callId, bool isDisconnected);
    void signalStartVideoCall();
    void signalIncomingChanged();
    void signalContacterInfo(QString key,QString value);
    void signalSendNameToPersonInfo(QString name,QString photoPath);
public slots:
    void onCallInComming(int callId, QString remoteIpAddr);
    void onCallStatusChanged(int callId, bool isDisconnected);
    void onVoipFramData();
    void onV4l2FramData();
    void onSignalCallState(int callId, int callState);
public:
    void testthread();
    ContacterModel *m_contacterModel;
    Q_INVOKABLE void callAccept();
    Q_INVOKABLE void callRefuse();
    Q_INVOKABLE int callRemote(const QString &ip);
    Q_INVOKABLE void setScreenShot(ScreenShot *screenShot);
    Q_INVOKABLE void getContacter(QString id);
    Q_INVOKABLE void getAddPersonInfo(QString searchString);
    Q_INVOKABLE bool getIsComming();
    bool getAccept();
    void setAccept(bool isAccept);

    QString getCallIP();
    void setCallIP(QString callIP);
    int getCallId();
    void setCallId(int callId);
//    void setInCommingIp(QString InComingIp);
//    QString getInCommingIp();
private:
    ScreenShot *m_screenShot;
    SharedMemory *m_pSharedMemory;
    bool m_isAccept;
    QString m_callIP;
    int m_callID;
    //QString m_InCommingIp;
private:
    VoipServiceAdaptor *m_voip;
    CNotificationManager *notiMgr;
    bool m_bVoipFirst;
    bool m_bV4L2First;
};

#endif // REALTIMEVIDEOVIEWMODEL_H
