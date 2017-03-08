#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include <QObject>
#include <QDBusInterface>
#include "../../../app/commondata/datatype/callhistorydata.h"
#include "../../../app/commondata/datatype/commondata.h"
#include "../../voipService/voipcallmanager.h"
#include "callhandler.h"
#include "../../voipService/sinuxvoicecallmanager.h"
#include "QSound"
#include <csystemprofilemanager.h>
class CallManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.CommTransService.CallManager")

public:
    explicit CallManager(QObject *parent = 0);
    static CallManager *instance(QObject *parent = 0);
    ~CallManager();
    QList<CallHandler*> m_mettingCalls;
    int voipInitialize(NetworkType network, int sipPort, int rtpPort, int rtpPortRange);

public slots:
    /**
     * @brief 根据user信息，拨打电话.
     * @param user : 通话对象具体信息
     */
    void dial(UserInfo user);
    int onMakeVideo(UserInfo user);
    void onDialError(int handlerId);
    void onCsDialError(QString mess);

    void record(int network, bool on);

    void onHangupConference(int network);
    void onMultipartyChanged(QString handlerId, bool isMultiparty, int network);

    int onGetActiveMode();
    int onSetHoldConference(int network, bool on);
    QString onGetAudioMode(int network);
    void setAudioMode(int network, QString mode);
    bool isMicrophoneMuted(int network);
    void setMuteMicrophone(int network, bool on);
    void onCreateConfCall(int network);

    int setVidEnabledChanged(int network , QString callId, bool on);
    int setVidInputChanged(int network, QString callId);
    int getGetAudOrVideo(int network, QString callId);//0:audio 1:video
    int getGetAudOrVideoInt(int network, int callId);

    unsigned joinGroup(int network, QString groupAddr, unsigned port); // returns groupId
    int leaveGroup(int network, unsigned groupId);
    int getPttState(int network, unsigned groupId);    // return type is PttState
    int setPttState(int network, unsigned groupId, int state); // state is of type PttState 2:speak 1:listen
    void onPttRecordFileFinished(unsigned groupId, const QString &fileName);

    void baoTongDial(QString phoneNum);
    void baoTongHangUp();
    void baoTongSet(bool on);
    bool baoTongGet();
    QString baoTongStatus();
    bool baoTongPttAllow();

signals:

    /**
     * @brief 拨打电话后，当电话业务建立后，会触发此信号，上报电话业务的handlerId, 上层使用者可通过此handlerId,绑定到具体的电话业务上进行通信。
     * @param dialingNumber: 拨打的电话号码
     * @param handlerId: 具体电话业务的id
     */
    void callHandlerIdChanged(QString dialingNumber, QString handlerId);
    void signalVideoCallHandlerIdChanged(QString dialNum, QString callId);

    /**
     * @brief 拨打过程中出现异常，此信号会被触发。
     * @param err : 异常信息。
     */
    void dialError(int err);

    /**
     * @brief 通话状态发生变化，此信号会被触发
     */
    void statusChanged(QString handlerId, int status);

    /**
     * @brief audioMode发生变化时，此信号会被触发
     */
    void audioModeChanged(int network);

    /**
     * @brief 麦克风静音状态发生变化时，此信号会被触发
     */
    void microphoneMutedChanged(int network);

    /**
     * @brief 拨打电话时，所使用的拨打网络发生变化时，此信号会被触发
     */
    void networkChanged(int network);

    /**
     * @brief network对应的通话服务断开时，此信号被触发
     **/
    void serviceDisconnect(int network);

    /**
     * @brief 合并通话完成后，触发此信号
     */
     void confCallFinished(int network, QString message);

     /**
      * @brief 电话会议参会人数发生变化时，此信号被触发
      * @param network: 对应的网络类型
      */
     void numberOfMultipartyParticipantsChanged(int network);
     void multipartyChanged(QString handlerId, bool isMultiparty);

    void signalHandlerEnd(HandlerData data);
    void signalMettingEnd(HandlerData data);
    void signalMettingHandlerDown(QString handlerId, QString dialNum);
    void signalVoipFrameData();
    void signalV4l2FrameData();
    void signalVidChanged(bool isVideoHold); //fase : video   true: audio

    //ptt
    void signalPttStateChanged(unsigned groupId, int state);
    void signalPttRecordFileFinished(unsigned groupId, const QString &fileName);

    void PttGettedChanged(bool isGetted);

private:
    int m_activeMode; //通话状态　０：err １：单通　２：多方　３：会议　４：会议＋多方
    QDBusInterface *m_interface;
    static CallManager *m_instance;

    VoipCallManager *m_wifiManager;
    VoipCallManager *m_adhocManager;
    VoipCallManager *m_lteManager;
    VoipCallManager *m_battleManager;
    SinuxVoicecallManager *m_voiceManager;
    BaotongDial *m_baotongManager;
    QString m_baoTongNum;

    QList<CallHandler*> m_wifiCalls;
    QList<CallHandler*> m_adCalls;
    QList<CallHandler*> m_lteCalls;
    QList<TiantongDial*> m_csCalls;
    QList<CallHandler*> m_battleCalls;


    CallHandler *m_videoHandler;

    QDateTime mettingStart;
    QDateTime mettingEnd;
    QStringList m_mettingTarget;
    QSound *dialSound;
    QSound *listenSound;
    QSound *errSound;

private Q_SLOTS:
    void initialize();
    void setActiveMode();
    int readActiveMode();
    void onCallHandlerIdChanged(int handlerId);// voip calls
    void onCsCallHandlerIdChanged(QString handlerId);// cs calls
    void onStatusChanged(QString handlerId, int status);
    void onAudioModeChanged();
    void onMicrophoneMuteChanged();
    void onNetworkChanged(int network);
    void onServiceDisconnect(int network);
    void onConfCallFinished();
    void onNumberOfMultipartyParticipantsChanged();
};

#endif // CALLMANAGER_H
