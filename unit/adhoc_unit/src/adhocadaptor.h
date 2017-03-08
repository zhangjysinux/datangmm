#ifndef ADHOCADAPTOR_H
#define ADHOCADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "commondata.h"
#define CTS_PRINT (qDebug() <<"adhoc_unit:" << __FILE__ << __FUNCTION__ << __LINE__ << ": ")

using namespace QT_DTT;
class AdHocAdaptor : public QObject
{
    Q_OBJECT
public:
    static AdHocAdaptor *getInstance();
    ~AdHocAdaptor();
    void testGroup();
    void createGroupInfo(const GroupInfo &info);
    void removeGroupInfo(const GroupInfo &info);
    void updateGroupInfo(const GroupInfo &info);
    void sendMessage(const GroupTransMsg &msg);

signals:
    //to do add type
    void signalGroupUpdate(int type, GroupInfo info);
    //to do add type
    void signalAdHocCreateGroupResult(int type, GroupInfo info);
    void signalMessageChanged(GroupTransMsg msg);
    void signalPttStateChanged(int status, QString ip);
    void signalPttAllowChanged(int status, QString ip);
public slots:
    void onAdHocCreateGroupResult(QDBusVariant infoVar, int reason);
    void onGroupUpdate(QDBusVariant infoVar, int type);
    //to do
    void onGroupTransSignalToGroupIntercomApp(QDBusVariant data);
    void onSetPtt(QString ip, bool on);
    void onPttStateChanged(int status, QString ip);
    void onPttAllowChanged(int status, QString ip);
private:
    explicit AdHocAdaptor(QObject *parent = 0);

    static AdHocAdaptor *m_instance;
    QDBusInterface m_groupInterface;
    QDBusInterface m_groupCommuInterface;
    //QDBusInterface m_groupTransactionInterface;

};

#endif // ADHOCADAPTOR_H
