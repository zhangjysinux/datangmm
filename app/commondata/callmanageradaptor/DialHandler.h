#ifndef DIAL_HANDLER_H_1455851098
#define DIAL_HANDLER_H_1455851098

#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/commondata.h"
#define SWITCH "sinux"
namespace QT_DTT
{
class DialHandler: public QObject
{
    Q_OBJECT

public:
    DialHandler(QString handlerId, QObject *parent = 0);
    ~DialHandler();

public:
	/**
     * @brief ��ȡ�绰����
     * @return ����������ĵ绰����
     */
	QString dialNumber();

	/**
	 * @brief ��ͨ��ҵ�����󣬿�ͨ���˺�����ȡ��ͨ��ҵ���Ӧ��handlerId.
	 * @return ����ͨ��ҵ���handlerId.
	 *
	 */
	QString handlerId();

	/**
	 * @brief ��ǰͨ��ҵ���״̬(�粦���С�����״̬��hold״̬�ȵ�(����״̬���������������))).
	 * @return ���ص�ǰͨ��ҵ���״̬
	 */
	int status();

	/**
	 * @brief ��ǰͨ���Ƿ��Ǻ���绰
	 * @return ture : ��ǰͨ���Ǻ���绰; false : ��ǰ�Ǻ����绰
	 *
	 */
	bool isIncoming();

	/**
	 * @brief ��ǰͨ���Ƿ��Ƕ෽ͨ��
	 * @return ture: ��ǰͨ���Ƕ෽ͨ����\n
	 *         false: ��ǰͨ�����Ƕ෽ͨ��
	 *
     */
	bool isMultiparty();

	/**
	 * @brief ��ǰͨ��ʹ�õ�����
	 * @return ���ص�ǰͨ����Ӧ����������
	 *
	 */
	int network();
	
	/**
	 * @brief �е绰����ʱ������ͨ��
	 *
	 */
	void answer();

	/**
	 * @brief �Ҷϵ�ǰͨ��
	 *
	 */
	void hangup();

	/**
	 * @brief ������߱��ֵ�ǰͨ��
	 * @param on : on = tureʱ����ʾ���ֵ�ǰͨ����on = falseʱ����ʾ���ǰͨ����
	 *
	 */
	void hold(bool on);
	
private Q_SLOTS:
    void onStatusChanged(QString handlerId, int state);
	void onError(const QString &error);
	void onMultipartyChanged();
    void initialize();

Q_SIGNALS:
	/**
	 * @brief �෽ͨ��״̬�����ı䣬���źű�����
	 * 
	 */
	//void multipartyChanged(bool isMultiparty);
	
	void multipartyChanged(QString handlerId, bool isMultiparty);

    /**
    * @brief ͨ��״̬�����ı�ʱ�����źű�����
    *
    */
	void statusChanged(QString handlerId, int status);

	void error(const QString &err);


private:
    //void initialize();

private:
	QDBusInterface *m_interface;
	QString m_handlerId;
	QString m_dialNumber;
	int m_status;
	bool m_isIncoming;
	bool m_isMultiparty;
	NetworkType m_network;
};
}
#endif
