#ifndef DIALINTERFACE_H
#define DIALINTERFACE_H

#include <QObject>
#include "../../commondata/datatype/dialdata.h"
#include "../../commondata/callmanageradaptor/DialHandler.h"
#include "../../commondata/callmanageradaptor/DialManager.h"
#include "../../commondata/datatype/commondata.h"
#include "contacterinterfaceadaptor.h"
#include "../../commondata/bluetoothbackgroundtransferinterface.h"
#include "../../commondata/bluetoothdata.h"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include "miniphonecalldata.h"
#include "voipserviceadaptor.h"
#include <qmutex.h>
#define HOSTMACHINE true

class DialInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.dial")
public:
    explicit DialInterface(QObject *parent = 0);
    static DialInterface *getInstance();
    HandlerManagerList m_handlerList;
    HandlerManager startManager;
    HandlerManager m_videoHandler;
signals:
    void signalHandlerListChanged(int type, HandlerManager handlerList);
    void signalStartApp(QString dialingNumber,QString handlerId);
    void signalStartMake(int net , QString phoneNum);
    void signalVideoCallInComming(QString callId, QString remoteIpAddr);
    void signalStartVideo(int net, QString phoneNum);
    void dialError(int err);
    void signalConfCallFinished(int network , QString message);
    void signalShowFullScreen();
    void signalSendMiniData(int type , MiniDialData mini);
    void signalAudioModeChanged(bool isHandsFree);
    void signalMicrophoneMuteChanged(bool on);
    void signalHandlerEnd(HandlerData data);
    void signalMettingEnd(HandlerData data);
    void signalMettingHandlerDown(QString handlerId, QString dialNum);
    void signalMultipartyChanged(QString handlerId, bool isMultiparty);
    void signalVoipFrameData();
    void signalV4l2FrameData();
    void signalVidChanged(bool isVideoHold); //fase : video   true: audio

    void signalVideoHandlerChanged(HandlerManager videoHandler);
public slots:
    void onMyDebug(QString mess);
    QDBusVariant onGetHandlerList();
    void onCallHandlerIdChanged(QString dialingNumber, QString handlerId);
    void onVideoCallHandlerIdChanged(QString dialingNumber, QString handlerId);
    void onStatusChanged(QString handlerId, int status);
    void onDial(const int &net, QString phoneNum);
    int onMakeVideo(int net, QString phoneNum);
    void onHangup(const QString &handId);
    void onAnswer(const QString &handId);
    void onHold(const QString &handId,const bool &on);
    void onConfCallFinished(int network , QString message);

    void onSetAudioMode(int network, const QString &mode);
    void onSetMuteMicrophone(int network, bool on = true);
    void onCreateConfCall(int network);
    QString onGetAudioMode(int net);
    bool onIsMicrophoneMuted(int net);

    void onAudioModeChanged(int network);
    void onMicrophoneMuteChanged(int network);

    void onAppAlreadyStart();
    //void updateHandler();
    void onHangupMultipartyCalls(int network);
    void onMultipartyChanged(QString handlerId, bool isMultiparty);
    void onHandlerReceived(const BluetoothTransData &message);
    void onSetStart(bool on);
    void onRecord(int network, bool on);
    int onSetIsMini(bool on);
    bool onGetIsMini();
    int onShowFullScreen();
    void onVideoCallInComming(QString callId, QString remoteIpAddr);
    void onFileDebug(QString mess);
    int onGetActiveMode();
    int onSetHoldConference(int network, bool on);

    int setVidEnabledChanged(int network , QString callId, bool on);
    int setVidInputChanged(int network, QString callId);
    int getGetAudOrVideo(int network, QString callId);//0:audio 1:video
private:
    static DialInterface *m_instance;
    QT_DTT::DialManager *m_dialManager;
    ContacterInterfaceAdaptor *m_contacterAdaptor;
    BluetoothBackgroundTransferInterface * m_buluTooth;
    DialManagerData m_dialManagerData;
    bool m_isMini;
    bool m_start;
    VoipServiceAdaptor *m_voip;
    QMutex m_mutex;
private:
    void sendhandlerToMiniPhoneCall(const int &type , const HandlerManager &handlerManager);
    int sendHandlerToBlueTooth(const int &type , const HandlerManager &handlerManager);
    int sendManagerToBlueTooth(const int &type , const DialManagerData &dialManagerData);
    int sendCmdToBlueTooth(const int & type , const CmdData &cmdData);
    void receiveBuleToothHandlerData(int type , HandlerManager handler);
    void receiveBuleToothHandlerCmd(CmdData &cmdData);
    void receiveBuleToothManagerData(int type , DialManagerData dialManagerData);

};

#endif // DIALINTERFACE_H
