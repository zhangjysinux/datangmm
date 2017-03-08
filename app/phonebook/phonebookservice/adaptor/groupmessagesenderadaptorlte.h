#ifndef LTEGROUPMESSAGESENDER_H
#define LTEGROUPMESSAGESENDER_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/commondata.h"
#include "groupadaptordata.h"
#include "groupmessagesenderadaptor.h"
#include "../../commondata/datatype/groupdata.h"
#include <QTimer>

class GroupMessageSenderAdaptorLte : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    enum LTEPttError
    {
        PTT_RELEASE = 0,
        PTT_GRAPED = 1,
        PTT_REQUEST_TIME_OUT = 2,
        PTT_TIME_UP = 3,
        PTT_VOICE_LESS = 4,
        PTT_PERMISSION = 5,
        PTT_BUSY = 6,
        PTT_UNDEFINE_ERROR,
    };

    enum LTEConnectStatus
    {
        Connect_Success = 1,
        Connect_Fail = 0
    };

    static GroupMessageSenderAdaptorLte *getInstance();
    explicit GroupMessageSenderAdaptorLte(QObject *parent = 0);
    ~GroupMessageSenderAdaptorLte();
    void login(QString userId, QString userPwd, int networkType, QString serverIp, int serverPort);
    void logout(int network);
    void logout();
    void createGroup(QT_DTT_JJF::GroupInfo info);
    void removeGroup(QT_DTT_JJF::GroupInfo info);
    void updateGroup(QT_DTT_JJF::GroupInfo info);
    qint64 sendMessage(const Message& message);
    void getGroupMember(int network, QString groupId);    //获取群组成员

    //jude the service is login
    bool isLogin();

    //lte speak interface
    void enterChart(const QString &groupId);
    void exitChart(const QString &groupId);

    void talkRequest(QString groupId);
    void talkRelease();

    void groupLteResetTalkFile();

    void setGroupForeground(const QString &groupId, bool foreground);

private:
    QString sendMessage(QT_DTT_JJF::GroupTransMsg msgData);
    QT_DTT_JJF::GroupTransMsg converLocalMessageToTransMessage(const Message &message);
    Message converTransMessageToLocalMessage(const QT_DTT_JJF::GroupTransMsg &transMessage);
    QString getGroupId(int sessionIndex);

signals:
    void signalSendLoginStatus(int loginStatus);
    void signalSendLoginStatusToApp(int status);

    void sendPocStatusToApp(int status);
//    void sentMessageInfoToApp(GroupTransMsg mInfo);
//    void recvMessageInfoToApp(GroupTransMsg mInfo);
    void sentMessageInfoToApp(QT_DTT_JJF::GroupTransMsg mInfo, int operateType);
    void recvMessageInfoToApp(QT_DTT_JJF::GroupTransMsg mInfo, int operateType);
    void recvGroupFromRemote(const Group &group, int operateType);

    //lte speak interface
    void signalNotifyTalkBegin(QString groupId);
    void signalNotifyTalkEnd(QString groupId, int errCode);

    void signalNotifyChannelConnect(QString groupId, int connectStatus);
    void signalNotifyChannelDisconnnect(QString groupId);

    void signalNotifyListenBegin(QString groupId);
    void signalNotifyListenEnd(QString groupId);

    void signalReceiveRecord(QString path);
    void signalSendRecord(QString path);

    void signalGroupChangedFinished(const Group &group);

private slots:
    void onSendLoginStatus(int loginStatus);
    void onSendLoginStatusToApp(int status);
//    void onReceivedMessage(GroupTransMsg msgData);

    void onSendPocStatusToApp(int status);
    void onSentMessageInfoToApp(QT_DTT_JJF::GroupTransMsg mInfo, int operateType);
    void onRecvMessageInfoToApp(QT_DTT_JJF::GroupTransMsg mInfo, int operateType);

    //lte speak interface
    void onNotifyTalkBegin(int sessionIndex);
    void onNotifyTalkEnd(int sessionIndex, int errCode);
    void onNotifyChannelConnect(int sessionIndex, int connectStatus);
    void onNotifyChannelDisconnect(int sessionIndex, int connectStatus);

    void onNotifyListenBegin(int sessionIndex, QString pocId, QString userName);
    void onNotifyListenEnd(int sessionIndex);

    void onNotifyRecordStop(int mode);
    void onNotifyPlayerStop(int mode);

    //lte login
    void onLogin();

private:
    static GroupMessageSenderAdaptorLte *instance;
    QDBusInterface m_interface;
    QDBusInterface m_communInterface;
    bool m_isLogin;
    QT_DTT_JJF::GroupTransMsg m_groupTransMsgGroup;
    QHash<QString, int> m_sessionIndexs;

    //lte login
    QTimer m_timer;

    QString m_userId;
    QString m_userPwd;
    int m_networkType;
    QString m_serverIp;
    int m_serverPort;
};

#endif // LTEGROUPMESSAGESENDER_H
