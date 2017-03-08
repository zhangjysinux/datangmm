#ifndef BTGROUPMESSAGESENDER_H
#define BTGROUPMESSAGESENDER_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/commondata.h"
#include "groupadaptordata.h"
#include "groupmessagesenderadaptor.h"
#include "commondef.h"

//baotong group comunication
struct BtGroupInfo
{
    QString pttId;
    int pttMode;
    BtGroupInfo()
    {
        pttId = QString();
        pttMode = 0;
    }
};

class GroupMessageSenderAdaptorBt : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    static GroupMessageSenderAdaptorBt *getInstance();

    //宝通群组jianli
    void createGroup(QT_DTT::GroupInfo info);
    void removeGroup(QT_DTT::GroupInfo info);
    void updateGroup(QT_DTT::GroupInfo info, int groupOpt);
    void getPTTGroup(int workMode);

    //call
    void setWorkMode(const int &workMode);
    void addGroup(const int &index, const QString &pttId);
    void addGroupMap(const QString &pttGroupId, const QString &uiGroupId);
    void removeGroup(const int &index);
    void updateGroup(const int &index, const QString &pttId);
    int findPttGroupIndex(const QString &pttId);
    int getUseableGroupIndex();

    //宝通群组通信
    qint64 sendMessage(const Message& message);
    void GroupPTTDial(QString groupId, int mode);
    void GroupPTTAnswer();
    void GroupPTTHangup();
    void GroupPTTSet(bool set);
    bool getPttSetAllow();

private:
    QString sendMessage(QT_DTT::GroupTransMsg msgData);

signals:
    void notifyStatusSignal(int status);
    void notifyPTTGroupSignal(QStringList groupList);

    void incommMessageSignal(QT_DTT::GroupTransMsg msg);
    void PttGroupMSGSignal(QString msg);

    //signal to inform ui
    void signalCreateDeleteGroup(int success);
    void signalJoinBtGroup(int success);
    void signalSetBtPttState(int success);
    void signalAllowSetPttState(int allow);

public slots:
    void onNotifyStatusSignal(int status);
    void onNotifyPTTGroupSignal(QStringList groupList);

    void onIncommMessageSignal(QT_DTT::GroupTransMsg msg);
    void onPttGroupMSGSignal(QString msgState);

private:
    explicit GroupMessageSenderAdaptorBt(QObject *parent = 0);
    static GroupMessageSenderAdaptorBt *instance;
    QDBusInterface m_interfaceGroup;
    QDBusInterface m_interfaceMessage;
    QT_DTT::GroupInfo m_groupInfo;
    QT_DTT::GroupTransMsg converLocalMessageToTransMessage(const Message &message);
    Message converTransMessageToLocalMessage(const QT_DTT::GroupTransMsg &transMessage);
    bool m_pttSetAllow;
    QMap<QString,QString> m_pttGroupUiGroupMap;
    QMap<QString,QString> m_uiGroupPttGroupMap;
};

#endif // BTGROUPMESSAGESENDER_H
