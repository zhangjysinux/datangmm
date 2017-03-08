#ifndef DIALMANAGER_H_1455851056
#define DIALMANAGER_H_1455851056

#include <QList>
#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/commondata.h"
#include "../../commondata/callmanageradaptor/audiomanager.h"
namespace QT_DTT
{
class DialHandler;

class DialManager : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief ���캯��.
     *
     */
    DialManager(QObject* parent = 0);

public:
    /**
     * @brief ��������.
     *
     */
	~DialManager();
	
    static DialManager *instance(QObject *parent = 0);
	DialHandler *getHandlerById(QString handlerId);
    /**
     * @brief ��ȡĳһ��������ͨ��.
     * @param network : ��������.
     * @return ����network��Ӧ����������ͨ��ҵ��.
     */
	QList<DialHandler*> * voiceCalls(NetworkType network);
	
	void addDialHandler(QString handlerId);
	
	/**
     * @brief ��������\��Ͳ
     * @param mode: ihf��ʾ���ᣬ earpiece��ʾ��Ͳ
     */
	void setAudioMode(int network, const QString &mode); 

    /**
     * @brief ���þ���
     * @param on = ture��ʾ����
     **/
	void setMuteMicrophone(int network, bool on = true);
	
	/**
	 * @brief �ϲ�ͬһ�����ͨ���������绰����
	 * @param network : ��������
	 **/
	void createConfCall(int network);
	
	/**
	 * @brief ��ȡaudio mode
	 * @return "ihf"��ʾ���ᣬ "earpiece"��ʾ��Ͳ
	 */
	QString audioMode(int network) const;
	
	/**
	 * @brief ��ȡ��˷��Ƿ��Ǿ���
	 * @return true: ����
	 */
	bool isMicrophoneMuted(int network) const;
	
	/**
	 * @brief ��ȡnetwork��Ӧ������绰����Ĳ�������
	 * @param network:��������
	 */
	int numberOfMultipartyParticipants(int network);

	void hangupMultipartyCalls(int network);

	void record(int network, bool on);

    int getActiveMode();
    int setHoldConference(int network, bool on);

    int setVidEnabledChanged(int network , QString callId, bool on);
    int setVidInputChanged(int network, QString callId);
    int getGetAudOrVideo(int network, QString callId);//0:audio 1:video

    unsigned joinGroup(int network, QString groupAddr, unsigned port); // returns groupId
    int leaveGroup(int network, unsigned groupId);
    int getPttState(int network, unsigned groupId);    // return type is PttState
    int setPttState(int network, unsigned groupId, int state); // state is of type PttState 2:speak 1:listen

    //voip�˿ڳ�ʼ��  retrun 1: succ 0: fail
    int voipInitialize(NetworkType network, int sipPort, int rtpPort, int rtpPortRange);

    bool g_headphoneState;
    bool g_blueHead;
    AudioManager audioManager;
    AudioManager::AM_TT_AUDIO_ROUTE m_trans;
    AudioManager::AM_PORT_TYPE m_port;
    AudioManager::AM_AUDIO_MODE m_model;
public Q_SLOTS:
	/**
	 * @brief ���user��Ϣ������绰.
	 * @param user : ͨ�����������Ϣ
	 */
	void dial(UserInfo user);
    int onMakeVideo(UserInfo user);
	void onDialError(int err);
    void onSignalHandlerEnd(HandlerData data);
    void onSignalMettingEnd(HandlerData data);
    void onSignalMettingHandlerDown(QString handlerId, QString dialNum);

    void baoTongDial(QString phoneNum);
    void baoTongHangUp();
    void baoTongSet(bool on);
    bool baoTongGet();
    bool baoTongPttAllow();
    void info(QString state);//, QStringList list);
    void headphoneState(bool state);


private Q_SLOTS:
    void onCallHandlerIdChanged(QString number, QString handlerId);
    void onVideoCallHandlerIdChanged(QString dialNum, QString callId);
    void onStatusChanged(QString handlerId, int status);
    void onClearAudioModel();
    void onAudioModeChanged(int network);
	void onMicrophoneMuteChanged(int network);
	void onNetworkChanged(int network);
	void onServiceDisconnect(int network);
	void onConfCallFinished(int network, QString message);
	void onNumberOfMultipartyParticipantsChanged(int network);
    void initialize();
    void onSignalVoipFrameData();
Q_SIGNALS:

	/**
	 * @brief ����绰�󣬵��绰ҵ�����󣬻ᴥ�����źţ��ϱ��绰ҵ���handlerId, �ϲ�ʹ���߿�ͨ���handlerId,�󶨵�����ĵ绰ҵ���Ͻ���ͨ�š�
	 * @param dialingNumber: ����ĵ绰����
	 * @param handlerId: ����绰ҵ���id
	 */
	void callHandlerIdChanged(QString dialingNumber, QString handlerId);
    void signalVideoCallHandlerIdChanged(QString dialingNumber, QString handlerId);
	/**
	 * @brief �������г����쳣�����źŻᱻ������
	 * @param err : �쳣��Ϣ��
	 */
	void dialError(int err);
	
	/**
	 * @brief ͨ��״̬����仯�����źŻᱻ����
	 *
	 */
	void statusChanged(QString handlerId, int status);
	
    /**
	 * @brief audioMode����仯ʱ�����źŻᱻ����
	 *
	 */
	void audioModeChanged(int network);
	
	/**
	 * @brief ��˷羲��״̬����仯ʱ�����źŻᱻ����
	 *
	 */
	void microphoneMuteChanged(int network);
	
	/**
	 * @brief ����绰ʱ����ʹ�õĲ������緢��仯ʱ�����źŻᱻ����
	 *
	 */
	void networkChanged(int network);
	
	/**
	 * @brief network��Ӧ��ͨ������Ͽ�ʱ�����źű�����
	 *
	 **/
    void serviceDisconnect(int network);
	
	/**
	 * @brief �ϲ�ͨ����ɺ󣬴������ź�
	 *
	 */
	 void confCallFinished(int network, QString message);
	 
	 /**
	  * @brief �绰����λ�������仯ʱ�����źű�����
	  * @param network: ��Ӧ����������
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

	
private:
	QDBusInterface *m_interface;
	QList<DialHandler*> m_adCalls;
	QList<DialHandler*> m_lteCalls;
    QList<DialHandler*> m_wifiCalls;
    QList<DialHandler*> m_batCalls;
    QList<DialHandler*> m_satelliteCalls;
	QList<DialHandler*> m_digitalCalls;
	
    DialHandler *m_videoHandler;
	static DialManager *m_instance;

};

}
#endif
