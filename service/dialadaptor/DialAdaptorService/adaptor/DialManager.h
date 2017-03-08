#ifndef DIALMANAGER_H_1455851056
#define DIALMANAGER_H_1455851056

#include <QList>
#include <QObject>
#include <QDBusInterface>
#include "common.h"
#include "control/voipcallmanageradhoc.h"
#include "control/voipcallmanagerlte.h"

class DialHandler;

class DialManager : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "com.sinux.CommTransService.DialManager")

private:
    /**
     * @brief 构造函数.
     *
     */
    DialManager(QObject* parent = 0);

public:
    /**
     * @brief 析构函数.
     *
     */
	~DialManager();
	
    static DialManager *instance(QObject *parent = 0);

public Q_SLOTS:

	DialHandler *getHandlerById(QString handlerId);
    /**
     * @brief 获取某一网络所有通话.
     * @param network : 网络类型.
     * @return 返回network对应的网络所有通话业务.
     */
	QList<DialHandler*> * voiceCalls(NetworkType network);
	
	void addDialHandler(QString handlerId);
	
	/**
     * @brief 设置免提\听筒
     * @param mode: ihf表示免提， earpiece表示听筒
     */
	void setAudioMode(int network, const QString &mode); 

    /**
     * @brief 设置静音
     * @param on = ture表示静音
     **/
	void setMuteMicrophone(int network, bool on = true);
	
	/**
	 * @brief 合并同一网络的通话，创建电话会议
	 * @param network : 网络类型
	 **/
	void createConfCall(int network);
	
	/**
	 * @brief 获取audio mode
	 * @return "ihf"表示免提， "earpiece"表示听筒
	 */
	QString audioMode(int network) const;
	
	/**
	 * @brief 获取麦克风是否是静音
	 * @return true: 静音
	 */
	bool isMicrophoneMuted(int network) const;
	
	/**
	 * @brief 获取network对应的网络电话会议的参与人数
	 * @param network:网络类型
	 */
	int numberOfMultipartyParticipants(int network);

	void hangupMultipartyCalls(int network);

	void record(int network, bool on);

public Q_SLOTS:
	/**
	 * @brief 根据user信息，拨打电话.
	 * @param user : 通话对象具体信息
	 */
	void dial(UserInfo user);
	void onDialError(int err);
	

private Q_SLOTS:
    void onCallHandlerIdChanged(int handlerId);
	void onStatusChanged(QString handlerId, int status);
    void onAudioModeChanged();
    void onMicrophoneMuteChanged();
	void onNetworkChanged(int network);
	void onServiceDisconnect(int network);
    void onConfCallFinished();
    void onNumberOfMultipartyParticipantsChanged();
	
Q_SIGNALS:

	/**
	 * @brief 拨打电话后，当电话业务建立后，会触发此信号，上报电话业务的handlerId, 上层使用者可通过此handlerId,绑定到具体的电话业务上进行通信。
	 * @param dialingNumber: 拨打的电话号码
	 * @param handlerId: 具体电话业务的id
	 */
	void callHandlerIdChanged(QString dialingNumber, QString handlerId);

	/**
	 * @brief 拨打过程中出现异常，此信号会被触发。
	 * @param err : 异常信息。
	 */
	void dialError(int err);
	
	/**
	 * @brief 通话状态发生变化，此信号会被触发
	 *
	 */
	void statusChanged(QString handlerId, int status);
	
    /**
	 * @brief audioMode发生变化时，此信号会被触发
	 *
	 */
	void audioModeChanged(int network);
	
	/**
	 * @brief 麦克风静音状态发生变化时，此信号会被触发
	 *
	 */
    void microphoneMutedChanged(int network);
	
	/**
	 * @brief 拨打电话时，所使用的拨打网络发生变化时，此信号会被触发
	 *
	 */
	void networkChanged(int network);
	
	/**
	 * @brief network对应的通话服务断开时，此信号被触发
	 *
	 **/
    void serviceDisconnect(int network);
	
	/**
	 * @brief 合并通话完成后，触发此信号
	 *
	 */
	 void confCallFinished(int network, QString message);
	 
	 /**
	  * @brief 电话会议参会人数发生变化时，此信号被触发
	  * @param network: 对应的网络类型
	  */
	 void numberOfMultipartyParticipantsChanged(int network);
	 
	 void multipartyChanged(QString handlerId, bool isMultiparty);
	 
private Q_SLOTS:
	void initialize();
	
private:
    VoipCallManagerAdHoc *m_adhocManager;
    VoipCallManagerLte *m_lteManager;
	QList<DialHandler*> m_adCalls;
	QList<DialHandler*> m_lteCalls;
//	QList<DialHandler*> m_satelliteCalls;
//	QList<DialHandler*> m_digitalCalls;
	
	static DialManager *m_instance;

};


#endif
