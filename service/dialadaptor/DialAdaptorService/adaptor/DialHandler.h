#ifndef DIAL_HANDLER_H_1455851098
#define DIAL_HANDLER_H_1455851098

#include <QObject>
#include <QDBusInterface>
#include "common.h"
#include "control/voipcallhandler.h"



class DialHandler: public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "com.sinux.CommTransService.Dial")

public:
    DialHandler(VoipCallHandler *handler, QObject *parent = 0);
    ~DialHandler();

public Q_SLOTS:
	/**
     * @brief 获取电话号码
     * @return 返回所拨打的电话号码
     */
	QString dialNumber();

	/**
	 * @brief 当通话业务建立后，可通过此函数获取此通话业务对应的handlerId.
	 * @return 返回通话业务的handlerId.
	 *
	 */
	QString handlerId();

	/**
	 * @brief 当前通话业务的状态(如拨打中、激活状态、hold状态等等(具体状态仍需继续讨论完善))).
	 * @return 返回当前通话业务的状态
	 */
	int status();

	/**
	 * @brief 当前通话是否是呼入电话
	 * @return ture : 当前通话是呼入电话; false : 当前是呼出电话
	 *
	 */
	bool isIncoming();

	/**
	 * @brief 当前通话是否是多方通话
	 * @return ture: 当前通话是多方通话。\n
	 *         false: 当前通话不是多方通话
	 *
	 */
	bool isMultiparty();

	/**
	 * @brief 当前通话使用的网络
	 * @return 返回当前通话对应的网络类型
	 *
	 */
	int network();
	
	/**
	 * @brief 有电话呼入时，接听通话
	 *
	 */
	void answer();

	/**
	 * @brief 挂断当前通话
	 *
	 */
	void hangup();

	/**
	 * @brief 激活或者保持当前通话
	 * @param on : on = ture时，表示保持当前通话；on = false时，表示激活当前通话。
	 *
	 */
	void hold(bool on);
	
private Q_SLOTS:
    void onStatusChanged();
	void onError(const QString &error);
	void onMultipartyChanged();
	
Q_SIGNALS:
	/**
	 * @brief 多方通话状态发生改变，此信号被触发
	 * 
	 */
	//void multipartyChanged(bool isMultiparty);
	
	void multipartyChanged(QString handlerId, bool isMultiparty);

    /**
    * @brief 通话状态发生改变时，此信号被触发
    *
    */
	void statusChanged(QString handlerId, int status);

	void error(const QString &err);


private Q_SLOTS:
	void initialize();

private:
//	QDBusInterface *m_interface;
    VoipCallHandler *m_callHandler;
	QString m_handlerId;
	QString m_dialNumber;
	int m_status;
	bool m_isIncoming;
	bool m_isMultiparty;
	NetworkType m_network;
};

#endif
