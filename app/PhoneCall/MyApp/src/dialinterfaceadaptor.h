#ifndef DIALINTERFACEADAPTOR_H
#define DIALINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include <QTimer>
#include "../../../commondata/datatype/dialdata.h"
#include "contacterinterfaceadaptor.h"
#include "callhistoryinterfaceadaptor.h"
#include "../../../commondata/datatype/commondata.h"
#include "tablemodel.h"
#include <csystempowermanager.h> //包含头文件
#include "screenshot.h"
#include "SharedMemory.h"
#include <qmutex.h>
#define HOSTMACHINE true
class DialInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit DialInterfaceAdaptor(QObject *parent = 0);
    static DialInterfaceAdaptor *getInstance();

    Q_INVOKABLE void myDebug(QString mess);
    Q_INVOKABLE int makeVideo(int net, QString phoneNum);
    Q_INVOKABLE int dial(int net, QString phoneNum);
    Q_INVOKABLE void dials(QString netstring,QString phoneNum);
    Q_INVOKABLE void hangup(const QString &handId);
    Q_INVOKABLE void answer(const QString &handId);
    Q_INVOKABLE void hold(const QString &handId,const bool &on);
    Q_INVOKABLE int getQmlState();
    Q_INVOKABLE void setAudioMode(int network, const QString &mode);
    Q_INVOKABLE void setMuteMicrophone(int network, bool on = true);
    Q_INVOKABLE void createConfCall(int network);
    Q_INVOKABLE void setIsTalking(bool on);
    Q_INVOKABLE bool getIsTalking();
    Q_INVOKABLE int getHandlerCount();
    Q_INVOKABLE int getMettingCount();
    Q_INVOKABLE QString getHandlerValue(int index, int type);
    Q_INVOKABLE QString getMettingValue(int index, int type);
    Q_INVOKABLE QString getNamebyDialnum(QString dialnum);

    Q_INVOKABLE QString getAudioMode(int net);       //免提"ihf"表示免提， "earpiece"表示听筒
    Q_INVOKABLE bool getIsMicrophoneMuted(int net);  //true: 静音

    Q_INVOKABLE void getAddPersonInfo(QString searchString);
    Q_INVOKABLE void getContacter(QString id);
    Q_INVOKABLE void onHangupMultipartyCalls(int network);
    Q_INVOKABLE QString getLocalNum(int net);
    Q_INVOKABLE void onRecord(int network, bool on);
    Q_INVOKABLE QString onGetContacterValue(int index, QString key);
    Q_INVOKABLE bool getIsMini();

    Q_INVOKABLE int answerVideo();
    Q_INVOKABLE int hangupVideo();
    Q_INVOKABLE int getActiveMode();
    Q_INVOKABLE int setHoldConference(int network, bool on);
    Q_INVOKABLE void initContacterData(QString phoneNum);
    Q_INVOKABLE void setScreenShot(ScreenShot *screenShot);
    Q_INVOKABLE void setScreenShotV4l2(ScreenShot *screenShot);
    Q_INVOKABLE QString getTimeString(QString handler);
    Q_INVOKABLE QString getContacterData(QString searchText);

    Q_INVOKABLE int setVidEnabledChanged(int network , QString callId, bool on); //true : 0 false :1
    Q_INVOKABLE int setVidInputChanged(int network, QString callId);
    Q_INVOKABLE int getGetAudOrVideo(int network, QString callId);//0:audio 1:video
    Q_INVOKABLE bool isVideoExit();
    Q_INVOKABLE bool isAudioExit();
    Q_INVOKABLE bool isHandlerExist(QString ip);
    Q_INVOKABLE bool isHostMachine();
    Q_INVOKABLE QString getVideoValue(QString key);
    Q_INVOKABLE int getVideoNetWork();

    Q_INVOKABLE void setVideoPage(bool on);
    Q_INVOKABLE void setAudioPage(bool on);
    Q_INVOKABLE bool getVideoPage();
    Q_INVOKABLE bool getAudioPage();
    Q_INVOKABLE void clearScreenShot();
    Q_INVOKABLE void initTalking();
    Q_INVOKABLE bool isFileExists(QString filePath);
    // by michael zheng 2017.4.7
//    Q_INVOKABLE void isAgainLink(QString ipAddr);
    // end by michael zheng


    ContacterModel *m_contacterModel;
    void appAlreadyStart();
    int findHandler(QString handlerId);
    int findMettingHandler(QString handlerId);
    int setMettingHandler(QString handlerId);
    HandlerManager *handlerManager;
    QList<HandlerManager> m_handlerList;
    QList<HandlerManager> m_meetingList;
    HandlerManager m_videoManager;
    QTimer *timer;
    QMutex m_mutex;

    bool videoPage;
    bool audioPage;
signals:
    void signalQuitApp();
    void signalVideoReturnPage();
    void signalAudioReturnPage();
    void signalCallHandlerIdChanged(QString handlerId);
    void signalAlreadyAnswer(QString handlerId);
    void signalVideoAlreadyAnswer(QString handlerId, int netWork);
    void signalDialing(QString handlerId);
    void signalAddPersonInfo(QString id, QString name , QString title, QString namePinyin,int isNormal);
    void signalSetHandler(QString handlerId,QString dialNumber,int network,int state,bool isIncoming,bool isMultiparty);
    void signalUpdateHandler(QString handlerId,QString dialNumber,int network,int state,bool isIncoming,bool isMultiparty);
    void signalRemoveHandler(QString handlerId);
    void signalHandlerListChanged();
    void signalUpdateContacterInfo(QString id,QString name,QString title,QString dialnum,QString photoPath);
    void signalAnotherIncoming(QString handlerId);
    void signalContacterInfo(QString key,QString value);
    void signalSendNameToPersonInfo(QString name,QString photoPath);
    void signalUpdateTimer(QString handlerId, QString timeString);
    void signalCloseAnother(QString colseHandlerId);
    void signalMakeStart(int net , QString phoneNum);
    void signalShowFullScreen();
    void signalScreen_on();
    void signalAudioModeChanged(bool isHandsFree);
    void signalReturnPage();
    void signalVidChanged(bool isVideoHold); //fase : video   true: audio
    void videoHangup();
    void signalMicrophoneMuteChanged(bool on);
    // by michael zheng 2017.4.7
    void signalIsAgainLink(bool);
    //end by michael zheng

public slots:
    void getHandlerList();
    void onSignalHandlerListChanged(int type, HandlerManager handler);
    void onVideoHandlerChanged(HandlerManager handler);
    void onConfCallFinished(int network , QString message);
    void onTimerout();
    void onDialError(int err);
    void setDisplayState(bool state);//添加函数供qml中调用
    void onSignalAudioModeChanged(bool isHandsFree);
    void onMultipartyChanged(QString handlerId, bool isMultiparty);
    void onSignalHandlerEnd(HandlerData data);
    void onSignalMettingEnd(HandlerData data);
    void onSignalMettingHandlerDown(QString handlerId, QString dialNum);
    void onVoipFrameData();
    void onV4l2FrameData();
    void onDialTimerOut();

    // by michael zheng 2017.4.7
    void onAgainLinkVideo(QString ipAddr);
    void onIsAgainLink(bool flag);
    // end by michael zheng


private:
    QString timeToString(int time);
    QTimer *dialTimer;
    int m_qmlState; ///0:初始 1:单通 2:三方  3:会议 4:会议+三方
    bool m_start;
    bool isTalking;
    QString anotherHandler;
    QDBusInterface m_interface;
    static DialInterfaceAdaptor *m_instance;
    ContacterInterfaceAdaptor *m_contacterAdaptor;
    CallHistoryInterfaceAdaptor * m_callhistoryAdaptor;
    CallHistoryList m_callHistoryList;
    bool isanother;
    CSystemPowerManager powerManager; //定义变量

    Contacter m_contacter;
    ScreenShot *m_screenShot;
    ScreenShot *m_screenShotV4l2;
    SharedMemory *m_pSharedMemory;
    QString userId;
    QString m_remoteIpAddr;
    bool isVideoing;
    bool m_isVideoHold;//true 语音　false 视频+语音
    // by michael zheng 2017.4.7
    bool isAGain;
    // end by michael zheng

};

#endif // DIALINTERFACEADAPTOR_H
