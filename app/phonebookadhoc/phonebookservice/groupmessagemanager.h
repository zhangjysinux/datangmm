#ifndef GROUPMESSAGEMANAGER_H
#define GROUPMESSAGEMANAGER_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QTimer>
#include "receiver.h"
#include "sender.h"
#include "sqltablemodelwithcache.h"
#include "groupmessagecontroller2.h"
#include "../../commondata/callmanageradaptor/DialManager.h"
#include "adaptor/groupmessagesenderadaptorbt.h"
#include "adaptor/p2pvoiceadaptor.h"
#include "adaptor/battlecalladator.h"
#include "linyun/lingyun.h"
#include "adaptor/voipserviceadaptor.h"

//using namespace QT_DTT;

class GroupMessageManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupMessageManager(const QString &groupId, QObject *parent = 0);
    ~GroupMessageManager();
    quint64 sendMessage(const Message &msg);
    quint64 resendMessage(const QString &msgId);
    QStringList getMessageList();
    QStringList getMessageList(const QDateTime &startTime, const QDateTime &endTime);
    QStringList getMessageList(const int &startIndex, const int &endIndex);
    Message getMessage(const QString &id);
    void getMessageDownload(const QString &id);
    bool setMessageIsread(const bool &isReadFlag, const QString &id);
    bool clearData();

    //baotong ptt
    void onSetBtPttState(int state);
    void onJoinBtGroup();
    bool onGetBtPttAllow();

public slots:
    //new message
    void onNewMessage(Message msg);

    //message schedul to delete or add or update
    void onMessageReceived(const int &handleType, const MessageList &msgs);
//    void onMessageReceived(const int &handleType, const Message &msg);
    void onDbChanged();

    //sync count from phone
    void onAddSyncCount(const int &count);
    void onClearSyncCount();
    int onGetSyncCount();

    //ptt
    unsigned onJoinGroup(int network); // returns groupId
    int onLeaveGroup(int network);
    int onGetPttState(int network);    // return type is PttState
    int onSetPttState(int network, int state); // state is of type PttState 2:speak 1:listen

    //ui join ptt
    void onInGroupUi();
    void onOutGroupUi();

    //p2p ptt
    int onCallP2P(int network, QString addr); // returns groupId
    int onHangupP2P(int network, QString addr);
    int onGetP2PState(int network, QString addr);    // return type is PttState
    int onSetP2PState(int network, QString addr, int state); // state is of type PttState

    void onSetTts(QString groupId, bool on);
    bool onGetTts();
private slots:

    //ptt
    void onAdhocPttStateChanged(int status, QString ip);
    void onSignalPttStateChanged(unsigned groupId, int state);
    void onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName);

    //ptt lte slots
    void onSignalNotifyChannelConnect(QString groupId, int connectStatus);
    void onSignalNotifyChannelDisconnnect(QString groupId);
    void onSignalNotifyTalkBegin(QString groupId);
    void onSignalNotifyTalkEnd(QString groupId, int errCode);

    //
    void onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    //for lte login
    void onSignalSendLoginStatus(int status);
    void onSignalSendLoginStatusToApp(int status);

    //for baotong get ptt group
    void onNotifyStatusSignal(int status);
    void onNotifyPTTGroupSignal(QStringList groupList);
    void onIncommMessageSignal(QT_DTT::GroupTransMsg msg);
    void onPttGroupMSGSignal(QString msg);

    void onSignalCreateDeleteGroup(int success);
    void onSignalJoinBtGroup(int success);
    void onSignalSetBtPttState(int success);
    void onSignalAllowSetPttState(int allow);

    //for download finished
    void onGetMessageDownload(const QString &id, const QString &fileName);

    //p2p
    void onSignalRecordFinished(int network, const QString &addr, const QString &fileName);
    //P2P VOICE
    void onSignalCallP2PStateChanged(int network, const QString &addr, int state);

    void onBattleSignalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onBattleSignalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

signals:
    void signalMessageChanged(int type, Message msg);
    void signalMessageChangedToChangeRemoteMessage(int type, const MessageList &msgs);
    void signalMessageDownload(const Message &msg);

    //ptt
    void signalPttStateChanged(int network, const QString &groupId, int state);
    void signalPttGroupStateChanged(int network, const QString &grooupId, int state);
//    void signalPttRecordFileFinished(int network, const QString &groupId, const QString &fileName);

    //send recv percent
    void signalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void signalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    //inform ui to update message count
    void signalUpdateMessageCount(const QString &groupId, const int &messageCount);

    //for baotong ptt
    void signalCreateDeleteGroup(const QString &groupId, int success);
    void signalJoinBtGroup(const QString &groupId, int success);
    void signalSetBtPttState(const QString &groupId, int success);
    void signalAllowSetPttState(const QString &groupId, int allow);
//    void signal

    //p2p ptt
    void signalRecordFinished(int network, const QString &addr, const QString &fileName);
    void signalCallP2PStateChanged(int network, const QString &groupId, int state);

private:
    QString m_msgid;
    quint64 sendTextMessage(const QVariant &data);
//    bool recordReceiveMessage(Message &message);//change message to which contans file path
//    bool recordSendMessage(Message &message);//change mesaage to which not contans file path
    bool recordMessage(Message &message);
    Message messageHostToNet(const Message &msg);
    Message messageNetToHost(const Message &msg);

    bool updateMessage(const Message &message);
    bool removeMessage(const QString &msgId);
    Message getMessage(const int &row);
    Message getMessage(const QStringList &rowData);
    bool findMessage(const QString &id);
    int insertLastRow(const QDateTime &time);
    void insertData(const QString& tableName);

    void scaleMessageImage(const Message &message);

    //sync count from phone
    void addSyncCount(const int &count);
    void clearSyncCount();
    int getSyncCount();
    void setCurrentSyncId();

private:
    bool m_ttsEnable;
    QString m_groupId;
    QString m_groupIdP2P;
    SqlTableModel m_sqlModel;
    Receiver m_receiver;
    Sender m_sender;
    GroupMessageSenderAdaptor *m_groupMessageSender;
//    GroupMessageController2 *m_controller;
    int m_currentSyncCount;
    QString m_currentSyncId;
    QTimer *m_timer;
    int m_netType;
    QString m_tableName;
    QString m_groupDataPath;

    QString m_addressAdhoc;

    //ptt dbus adaptor
    QT_DTT::DialManager *m_dialManager;
    unsigned m_pttGroupId;

    //static
    static int UpdPort;

    //point to point networktype
    int m_p2pNetworkType;
    P2PVoiceAdaptor m_p2pVoiceAdaptor;
    BattleCallAdator *m_battleCallAdaptor;
    VoipServiceAdaptor *m_voipServiceAdaptor;

    LingYun *m_lingyun;
};

#endif // GROUPMESSAGEMANAGER_H
