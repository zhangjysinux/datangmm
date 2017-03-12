#ifndef ADHOCADAPTOR_H
#define ADHOCADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/commondata.h"
#include "commondefadhoc.h"
#include "groupmessagesenderadaptor.h"
#include "groupmessagecontroller2.h"
#include "../../commondata/datatype/groupdata.h"
#define CTS_PRINT (qDebug() <<"adhoc_unit:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")

class AdHocAdaptor : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    static AdHocAdaptor *getInstance();
    ~AdHocAdaptor();
    void testGroup();
    void createGroupInfo(const QT_DTT_ADHOC::GroupInfo &info);
    void removeGroupInfo(const QT_DTT_ADHOC::GroupInfo &info);
    void updateGroupInfo(const QT_DTT_ADHOC::GroupInfo &info);
    void sendMessage(const QT_DTT_ADHOC::GroupTransMsg &msg);
    qint64 sendMessage(const Message& message);
    void onSetPtt(QString ip, bool on);
    void adHocCreateMsg(QString muticastAddr);

    void setGroupIpsAndPath(const QString &groupId,
                            const QString &address,
                            const int &port,
                            const QStringList &ips,
                            const QString &path);


signals:
    //to do add type MEMBER
    void signalGroupUpdate(int type, QT_DTT_ADHOC::GroupInfo info);
    //to do add type OWNER
    void signalAdHocCreateGroupResult(int type, QT_DTT_ADHOC::GroupInfo info);
    void signalMessageChanged(QT_DTT_ADHOC::GroupTransMsg msg);
    void signalPttStateChanged(int status, QString ip);
    void signalPttAllowChanged(int status, QString ip, QString soureceIp);

    //signal send percent adhoc
    void signalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void signalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    //remote group
    void signalRecvGroupFromRemote(const Group &group, int operateType);


public slots:
    void onAdHocCreateGroupResult(QDBusVariant infoVar, int reason);
    void onGroupUpdate(QDBusVariant infoVar, int type);
    //to do
    void onGroupTransSignalToGroupIntercomApp(QDBusVariant data);
    //0, SUCCESS 1,FAIL
    void onPttStateChanged(int status, QString ip);
    void onPttAllowChanged(int status, QString ip, QString sourceIp);

private:
    explicit AdHocAdaptor(QObject *parent = 0);

    static AdHocAdaptor *m_instance;
    QDBusInterface m_groupInterface;
    QDBusInterface m_groupCommuInterface;
    QMap<QString,GroupMessageController2*> m_groupIdMessageSenderMap;
    //QDBusInterface m_groupTransactionInterface;

    Message converTransMessageToLocalMessage(const QT_DTT_ADHOC::GroupTransMsg &transMessage);
    QT_DTT_ADHOC::GroupTransMsg converLocalMessageToTransMessage(const Message &message);
};

#endif // ADHOCADAPTOR_H
