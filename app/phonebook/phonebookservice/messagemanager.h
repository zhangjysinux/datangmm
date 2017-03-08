#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <QObject>
#include <QSqlTableModel>
#include "receiver.h"
#include "groupmessagemanager.h"
#include "../../commondata/datatype/messagedata.h"

class MessageManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.msgmgr")
public:
    static MessageManager *getInstance();
    Message getMessage(const QString &id, const QString &groupId);

public slots:
    int onRemoveMessageGroup(const QString &groupId);
    int onRemoveMessageGroupData(const QString &groupId);
    QStringList onGetMessageList(const QString &groupId);
    QStringList onGetMessageList(const QDateTime &startTime, const QDateTime &endTime, const QString &groupId);
    //startIndex from close start
    QStringList onGetMessageList(const int &startIndex, const int &endIndex, const QString &groupId);
    QDBusVariant onGetMessage(const QString &id, const QString &groupId);
    void onGetMessageDownload(const QString &id, const QString &groupId);
    bool onSetMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId);

    quint64 onSendMessage(const Message &msg);
    quint64 onResendMessage(const Message &msg);
    quint64 onSendTextMessage(const QString &groupId, const QString &text);
    quint64 onSendOtherMessage(const QString &groupId, const QString &fileName, const int &messageType);

    QString onGetGroupNameByContacter(const QString &contactId);

    //schedul for remote message changed
    void onMessageReceived(const int &handleType, const MessageList &msgs);

    //sync count from phone
    void onClearSyncCount(const QString &groupId);
    void onClearSyncCount();
    int onGetSyncCount(const QString &groupId);
    int onGetSyncCount();

    //voip ptt
    unsigned onJoinGroup(int network, const QString &groupId); // returns groupId
    int onLeaveGroup(int network, const QString &groupId);
    int onGetPttState(int network, const QString &groupId);    // return type is PttState
    int onSetPttState(int network, const QString &groupId, int state); // state is of type PttState 2:speak 1:listen

    //baotong ptt
    void onJoinBtGroup(const QString &groupId);
    void onSetBtPttState(const QString &groupId, int state);//2:speak 1:listen
    bool onGetBtPttAllow(const QString &groupId);

    //ui join ptt
    void onInGroupUi(const QString &groupId);
    void onOutGroupUi(const QString &groupId);

    void onGetPosition(int type, QString groupid);
    //ui join p2p ptt
    int onCallP2P(int network, const QString &groupId); // returns groupId
    int onHangupP2P(int network, const QString &groupId);
    int onGetP2PState(int network, const QString &groupId);    // return type is PttState
    int onSetP2PState(int network, const QString &groupId, int state); // state is of type PttState

    void onSetTts(QString groupId, bool on);
    bool onGetTts(QString groupId);

signals:
    void signalGetPositon(int type,QString groupid);
    void signalMessageChanged(int type, QDBusVariant msg);
    void signalMessageChangedToChangeRemoteMessage(int type, const MessageList &msgs);
    void signalMessageDownload(QDBusVariant msg);

    //ptt
    void signalPttStateChanged(int network, const QString &groupId, int state);
    void signalPttGroupStateChanged(int network, const QString &grooupId, int state);

    //send recv percent
    void signalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void signalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    //inform ui to update message count
    void signalUpdateMessageCount(const QString &groupId, const int &messageCount);

    //baotong signal
    void signalCreateDeleteGroup(const QString &groupId, int success);
    void signalJoinBtGroup(const QString &groupId, int success);
    void signalSetBtPttState(const QString &groupId, int success);
    void signalAllowSetPttState(const QString &groupId, int allow);

    //p2p
    void signalCallP2PStateChanged(int network, const QString &groupId, int state);

    //p2p connected finished
    void signalP2PConnected(const QString &groupId, int state);


private:
    explicit MessageManager(QObject *parent = 0);
    GroupMessageManager *getGroupMessageManager(const QString &groupId);
    int removeGroupMessageManager(const QString &groupId);
    int removeGroupMessageManagerData(const QString &groupId);

    QString convertUuidToString(QString uuid);
    QString convertStringToUuid(QString string);

    //get the watch or phone
    bool isWatch();

    //sync count from phone
    void clearSyncCount(const QString &groupId);
    void clearSyncCount();
    int getSyncCount(const QString &groupId);
    int getSyncCount();
    void setSyncCountToAppIcon(const int &count);

private slots:
    void onGroupMessageChanged(int type, Message msg);
    void onSignalMessageDownload(const Message &msg);
    void onSignalPttStateChanged(int network, const QString &groupId, int state);

    //
    void onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    //for point to point
    void onSignalPointToPointNewMessae(const Message &message);

private:
    static MessageManager *m_instance;
    QHash<QString, GroupMessageManager*> m_msgManagerMap;
};

#endif // MESSAGEMANAGER_H
