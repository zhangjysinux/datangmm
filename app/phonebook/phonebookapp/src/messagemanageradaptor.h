#ifndef MESSAGEMANAGERADAPTOR_H
#define MESSAGEMANAGERADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/messagedata.h"

class MessageManagerAdaptor : public QObject
{
    Q_OBJECT
public:
    static MessageManagerAdaptor *getInstance();
    int removeMessageGroupData(const QString &groupId);
    QStringList getMessageList(const QString &groupId);
    QStringList onGetMessageList(const QDateTime &startTime, const QDateTime &endTime, const QString &groupId);
    //startIndex from close start
    QStringList onGetMessageList(const int &startIndex, const int &endIndex, const QString &groupId);
    quint64 sendMessage(const Message &msg);
    quint64 resendMessage(const Message &msg);
    quint64 sendTextMessage(const QString &groupId, const QString &text);
    Message getMessage(const QString &id, const QString &groupId);
    QString getGroupNameByContacter(const QString &contactId);
    void getMessageDownload(const QString &id, const QString &groupId);
    bool setMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId);
    quint64 sendOtherMessage(const QString &groupId, const QString &fileName, const int &messageType);
    int getSpeakChance();
    int startSpeak();
    void onSetTts(QString groupId, bool on);
    bool onGetTts(QString groupId);

    // set read/unread
    void clearSyncCount(const QString &groupId);
    void clearSyncCount();
    int getSyncCount(const QString &groupId);
    int getSyncCount();

    //ptt
    unsigned joinGroup(int network, const QString &groupId); // returns groupId
    int leaveGroup(int network, const QString &groupId);
    int getPttState(int network, const QString &groupId);    // return type is PttState
    int setPttState(int network, const QString &groupId, int state); // state is of type PttState 2:speak 1:listen

    //baotong ptt group
    void joinBtGroup(const QString &groupId);
    void setBtPttState(const QString &groupId, int state);//2:speak 1:listen
    bool getBtPttAllow(const QString &groupId);

    //ui in out group
    void onInGroupUi(const QString &groupId);
    void onOutGroupUi(const QString &groupId);

    void onGetPosition(int type, QString groupid);

    //ui join p2p ptt
    int onCallP2P(int network, const QString &groupId); // returns groupId
    int onHangupP2P(int network, const QString &groupId);
    int onGetP2PState(int network, const QString &groupId);    // return type is PttState
    int onSetP2PState(int network, const QString &groupId, int state); // 2:speak  1:listen state is of type PttState



signals:
    void signalGetPosition(int type,QString groupId); // 0--group 1--p2p
    void signalMessageChanged(int type, Message msg);
    void signalMessageDownload(const Message &msg);

    //ptt
    void signalPttStateChanged(int network, const QString &groupId, int state);
    void signalPttGroupStateChanged(int network, const QString &grooupId, int state);

    //send recv percent
    void signalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void signalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    //inform ui to update the message count
    void signalUpdateMessageCount(const QString &groupId, const int &messageCount);

    //baotong ptt group
    void signalCreateDeleteGroup(const QString &groupId, int success);
    void signalJoinBtGroup(const QString &groupId, int success);
    void signalSetBtPttState(const QString &groupId, int success);
    void signalAllowSetPttState(const QString &groupId, int allow);

    //p2p voice
    void signalCallP2PStateChanged(int network, const QString &groupId, int state);

    //p2p connected finished
    void signalP2PConnected(const QString &groupId, int state);

public slots:
    void onDbusSignalMessageChanged(int type, QDBusVariant data);
    void onDbusSignalMessageDownload(QDBusVariant data);

    void onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

private slots:
    //baotong ptt group
    void onSignalCreateDeleteGroup(const QString &groupId, int success);
    void onSignalJoinBtGroup(const QString &groupId, int success);
    void onSignalSetBtPttState(const QString &groupId, int success);
    void onSignalAllowSetPttState(const QString &groupId, int allow);

    void onSignalCallP2PStateChanged(int network, const QString &groupId, int state);

private:
    explicit MessageManagerAdaptor(QObject *parent = 0);

private:
    QDBusInterface m_interface;
    static MessageManagerAdaptor *m_instance;
};

#endif // MESSAGEMANAGERADAPTOR_H
