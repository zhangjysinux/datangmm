#ifndef GETDATA_H
#define GETDATA_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QQuickView>
#include <QQuickItem>
#include <QRegExp>
#include <QFile>
#include "contacterinterfaceadaptor.h"
#include "groupinterfaceadaptor.h"
#include "phonebookapp_workspace.h"
#include "messagemanageradaptor.h"
#include "callhistoryinterfaceadaptor.h"
#include "bluetoothinterfaceadaptor.h"
#include "tablemodel.h"
#include "fileshareadaptor.h"
#include "contactershareadaptor.h"
#include "../../../commondata/callmanageradaptor/DialManager.h"

class CachePage
{
public:
    QString groupName;
    QString groupId;
    bool bIsPtt;
    QString groupnum;
    QString netinfo;
    int syncCount;
};
typedef QList<CachePage> CachePages;


class GetData : public QObject
{
    Q_OBJECT
    Q_ENUMS(SETTINGFLAG)
public:
    explicit GetData(QObject *parent = 0);
    Q_INVOKABLE void testHistory();
    Q_INVOKABLE QString getData();
    Q_INVOKABLE QString digitalDial(QString ip);
    Q_INVOKABLE void hangUp();
    Q_INVOKABLE void accept();
    Q_INVOKABLE void decline();
    Q_INVOKABLE void updateLocalInfo(QString name, QString title,QString snNum, QString bluetooth, QString pocid);
    Q_INVOKABLE void localPersonInfoInit();
    Q_INVOKABLE void getContacter(QString id);
    Q_INVOKABLE void addContacter(QString name, QString title, QString photo);
    Q_INVOKABLE void updateContacter(QString name, QString title, QString num, QString photopath, QString pocid);
    Q_INVOKABLE void setPhoneList(QString key, QString value);
    Q_INVOKABLE bool getCurrentNetworkIsOnLine(const QString id,const QString net);
    Q_INVOKABLE bool removeContacter(const QString id);
    Q_INVOKABLE void addGroup(QString name, QString networkType, QString iconPath,int groupType);
    Q_INVOKABLE bool removeGroup(const QString id);
    Q_INVOKABLE void updateGroup(QString id, QString name);
    Q_INVOKABLE void getGroup(QString id);
    Q_INVOKABLE void setView(QQuickView *pview);
    Q_INVOKABLE bool getGroups(bool bIsDelete);
    Q_INVOKABLE void onGetGroups(int groupType);
    Q_INVOKABLE int getGroupNetType(const QString netinfo);
    Q_INVOKABLE quint64 sendMessage(const int type ,const QString &msg , const QString &userId, const QString &groupID);
    Q_INVOKABLE quint64 sendTextMessage(const QString &msg , const QString &userId, const QString &groupID);
    Q_INVOKABLE QString getLocalId();
    Q_INVOKABLE int getLocalTitle();
    Q_INVOKABLE QString getContacterPhoto(QString userId);
    Q_INVOKABLE QString getLocalPhoto();
    Q_INVOKABLE int getMessageList(const QString &groupId);
    Q_INVOKABLE void onGetMessageList(const QString &groupId);
    Q_INVOKABLE QString getAdhocNetwork(QString id);
    Q_INVOKABLE QString getFirstNetwork(QString id);
    Q_INVOKABLE QString getFirstNum(QString id);
    Q_INVOKABLE void getAddGroupPersonInfo(const int &netType, QString searchinfo, QString groupId, QStringList list);
    Q_INVOKABLE QString getContacterNum();
    Q_INVOKABLE void getCallHistoryList();
    Q_INVOKABLE void getCallHistoryBytarget(const QString userid,bool bIsStranger);
    Q_INVOKABLE void removeCallHistory(const QString historyid,bool bIsStranger);
    Q_INVOKABLE void getSearchContactors(const int &nettype, const int &contacterType, const QString& searchText);
    Q_INVOKABLE void getSearchContactors2(const int &nettype, const int &contacterType, const QString& searchText);
    Q_INVOKABLE bool bIsOrganization(const QString id);
    Q_INVOKABLE void openblueToothdevice();
    Q_INVOKABLE void findblueToothDevice();
    Q_INVOKABLE void getSearchCallHistory(int type, const QString &searchText);
    Q_INVOKABLE void getCurrentNetworkInfo(const QString net);
    Q_INVOKABLE void clearGroupMembers(void);
    Q_INVOKABLE void getGroupMembers(QString groupId);
    Q_INVOKABLE void setGroupMembers(QString id);
    Q_INVOKABLE void clearGroupLeaders(void);
    Q_INVOKABLE void getGroupLeaders(QString groupId);
    Q_INVOKABLE void setGroupLeaders(QString id);
    Q_INVOKABLE QString getLocalInGroupIdentify(QString id);
    Q_INVOKABLE QString getLocalPersonName();
    Q_INVOKABLE void getMessageDownload(const QString &id, const QString &groupId);
    Q_INVOKABLE bool getContacterLeader(QString userId);
    //获取本地蓝牙号,称号,名字
    Q_INVOKABLE void getLocalDeviceInfo();
    //开始配对
    Q_INVOKABLE void startPairing(const QString &name, const QString &address);
    //确认配对
    Q_INVOKABLE void pairingConfirmation(bool confirm);
    //connect device
    Q_INVOKABLE bool connectDevice(const QString &name, const QString &address);
    //send contact
    Q_INVOKABLE bool sendContact(QString id);
    Q_INVOKABLE void disconnectDevice();
    Q_INVOKABLE bool isBlutoothOpened();
    Q_INVOKABLE void getImageMessageList();

    Q_INVOKABLE void clearSyncCount(const QString &groupId);
    Q_INVOKABLE int getSyncCount(const QString &groupId);
    Q_INVOKABLE int getMessageCount();
    Q_INVOKABLE bool isFileExists(QString filePath);
    Q_INVOKABLE int getCallHistoryNet(const QString id);
    Q_INVOKABLE void pttSet(bool on);

    Q_INVOKABLE unsigned joinGroup(int network, const QString &groupId); // returns groupId
    Q_INVOKABLE int leaveGroup(int network, const QString &groupId);
    Q_INVOKABLE int getPttState(int network, const QString &groupId);    // return type is PttState
    Q_INVOKABLE int setPttState(int network, const QString &groupId, int state); // state is of type PttState 2:speak 1:listen

    Q_INVOKABLE bool setMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId);
    Q_INVOKABLE void setSlect(QString id, int isSlect);
    Q_INVOKABLE QString getMessageDate(QDateTime lastTime, QDateTime dateTime);
    Q_INVOKABLE QString getSmallUrl(QString url);
    Q_INVOKABLE QString getFileNmae(QString url);

    Q_INVOKABLE int fileConnect();
    Q_INVOKABLE int fileDisConnect();
    Q_INVOKABLE int sendFile(QString url);
    Q_INVOKABLE void setAddresses(const QStringList &addresses);
    Q_INVOKABLE QString getAddress(QString userId, QString net);
    Q_INVOKABLE int getLocalNumCount();
    Q_INVOKABLE QString getLocalNum(int index);
    Q_INVOKABLE QString transtoIp(QString text);
    Q_INVOKABLE void clearGroupMessage(QString groupId);
    Q_INVOKABLE void saveAsFile(const QString &sourceFileName);

    Q_INVOKABLE int getCachePageCount();
    Q_INVOKABLE QString getCachePageValue(int index, QString key);
    Q_INVOKABLE void addCachePage(QString groupName, QString groupId, bool bIsPtt, QString groupnum, QString netinfo);
    Q_INVOKABLE void removeCachePage(QString groupId);
    Q_INVOKABLE void setTabIndex(QString groupId);
    Q_INVOKABLE int getTabIndex(QString groupId);
    Q_INVOKABLE int getTabCurrentIndex();

    //contacter share network
    Q_INVOKABLE int contacterShareStartConnectNetwork(const QString &contacterId);
    Q_INVOKABLE int contacterShareDisconnectNetwork();
    Q_INVOKABLE int contacterShareStartShare(const QString &contacterId);
    Q_INVOKABLE int intToNetworkType(int val);
    Q_INVOKABLE quint64 resendMessage(const QString &msgId, QString groupId);

    Q_INVOKABLE void baoTongDial(QString phoneNum);
    Q_INVOKABLE void baoTongHangUp();
    Q_INVOKABLE void baoTongSet(bool on);
    Q_INVOKABLE bool baoTongGet();
    Q_INVOKABLE bool baoTongPttAllow();

    Q_INVOKABLE void joinBtGroup(const QString &groupId);
    Q_INVOKABLE void setBtPttState(const QString &groupId, int state);//2:speak 1:listen
    Q_INVOKABLE bool getBtPttAllow(const QString &groupId);

    Q_INVOKABLE void sendBlueToothFile(QString fileName);

    Q_INVOKABLE void onInGroupUi(const QString &groupId);
    Q_INVOKABLE void onOutGroupUi(const QString &groupId);
    Q_INVOKABLE QString sortPhoneNumbers(const QString &userId, QString key);

    Q_INVOKABLE QStringList getGroupValue(QString groupId);
    Q_INVOKABLE QStringList getLocation(QString msgId, QString groupId);
    Q_INVOKABLE void emitGetPosition(int type ,QString groupid);

    //P2P
    Q_INVOKABLE int onCallP2P(int network, const QString &groupId); // returns groupId
    Q_INVOKABLE int onHangupP2P(int network, const QString &groupId);
    Q_INVOKABLE int onGetP2PState(int network, const QString &groupId);    // return type is PttState
    Q_INVOKABLE int onSetP2PState(int network, const QString &groupId, int state); // 2:speak  1:listen state is of type PttState

    Q_INVOKABLE void onSetPropertyType(int type);

    Q_INVOKABLE void onSetTts(QString groupId, bool on);
    Q_INVOKABLE bool onGetTts(QString groupId);

    enum SETTINGFLAG
    {
        SET_DELETE = 1,
        SET_UPDATA = 2,
    };

    QT_DTT::DialManager *m_dialManager;
    CachePages m_cachePages;
    ContacterInterfaceAdaptor *m_adaptor;
    GroupInterfaceAdaptor *m_groupAdaptor;
    MessageManagerAdaptor *m_messageAdaptor;
    CallHistoryInterfaceAdaptor *m_historyInterface;
    BluetoothInterfaceAdaptor *m_bluetoothAdaptor;
    QStringList m_contacterInfoList;
    QDBusConnection m_sessionBus;
    QDBusInterface *m_dataInterface;
    QDBusInterface *m_voipInterface;
    QDBusInterface *m_sosInterface;
    ContacterModel *m_contacterModel;
    CallHistoryModel *m_callHistoryModel;
    MessageModel *m_messageModel;
    FileShareAdaptor *m_fileShareAdaptor;
    ContacterShareAdaptor *m_contacterShareAdaptor;

    int m_groupTabIndex;
    int m_groupCurrentIndex;
    PhoneNumbers m_phoneNumbers;
    QStringList m_leaders;
    QStringList m_members;
    QStringList m_groupLeaders;
    QString m_id;
    QString m_name;
    int m_title;
    QString m_photoPath;
    QString m_let;
    QString m_localId;
    QString m_groupId;
    QDateTime m_time;
signals:
    void signalComing();
    void signalContacterChanged(int type,QString id,QString name,QString title,QString size, QString contactertype);
    void signalContacterInfo(QString key,QString value);
    void signalSendNameToPersonInfo(QString name,QString photoPath);
    void signalSendLocalInfoToPersonInfo(QString key, QString value);
    void signalSendOrganizationGroupInfoToGroup(QString id,QString parentId, QString name, QString network, QString num, int title,QString nameInfo, int syncCount);
    void signalSendGroupInfoToGroup(QString id,QString name, QString network, QString num, int syncCount);
    void signalSendMemberInfoToGroup(QString id,QString bIsLeader,QString name, QString value, QString type);
    void signalGroupChanged(int type,QString id,QString name,QString network, QString num, int syncCount);
    void signalMessageChanged(int messageHandle, int messageType, int messageState, QString fileName, QString msg, QString userId,QString id);
    void signalAddGroupPerson(QString id, QString name , QString title, QString size);
    //    void signalCallHistoryChanged(int type, QString id, QString post, int dialtype,
    //                                  QString name, QString title,QString phonenum,QString time,QString net);
    void signalCallHistoryChanged(int type, QString id);
    void signalSendCallHistory(QString id, QString post,int dialtype,QString name,
                               QString title,QString phonenum,QString time, QString net);
    void signalSendHistoryInfo(QString id,int type,int network, QString time,QString netinfo,
                               QString phonenum, QString phonetime);
    void signalSendDeleteHistoryInfo(int type, QString id);
    void signalDeviceDiscovered(const BluetoothAddressData &data);
    void signalSendBlueToothInfo(QString name, QString address, QString contactTitle, QString contactName);
    void signalSendNetworkInfo(QString netinfo);
    void signalSendLocalDeviceInfo(QString bluetoothName, QString title, QString name);

    void signalPairConfirmation(QString name, QString address, QString pin);
    void signalConnectServerFinished(QString name, QString address);
    void signalConnectServerFailed(const int &error);
    void signalPairFinished(QString name, QString address, int pairing);
    void signalContacterReceived(int type);
    void signalContacterSended(const int &sendStatus);

    void signalFreshMessage();
    void signalSendGroupLeaders(QString id, QString name, QString position);
    void signalSendGroupMembers(QString id, QString name, QString position);

    void signalPttGettedChanged(bool isGetted);
    //ptt
    void signalPttStateChanged(const QString &groupId, int state);

    void signalSendPercent(const QString &sendGroupId, const QString &sendMessageid, int recvNum, int allNum);
    void signalRecvPercent(const QString &sendGroupId, const QString &sendMessageid, int recvNum, int allNum);

    void signalFileConnectFinished(const int &status);
    void signalFileReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalFileSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

    //contacter share network
    void signalContacterShareConnectNetworkFinished(const int &contactConnectStatus);
    void signalContacterShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

    void signalUpdateMessageCount(const QString &groupId, const int &messageCount);

    void pttGettedChanged(bool isGetted);
    void signalGroupExchange(int type);

    void signalSosReturn(int sosret);

    //p2p connected finished
    void signalP2PConnected(const QString &groupId, int state);

public slots:
    void onNumberTest(int number,int index);
    void onContacterChanged(int type, Contacter index);
    void onGroupChanged(int type, Group group);
    void onMessageChanged(int messageHandle, Message msg);
    void onCallHistoryChanged(QString historyId, int type);
    void onSignalDeviceDiscovered(const BluetoothAddressData &data);

    void onSignalPairConfirmation(const BluetoothAddressData &address, QString pin);
    void onSignalConnectFinished(const BluetoothAddressData &address);
    void onSignalPairFinished(const BluetoothAddressData &address, int pairing);
    void onSignalContacterReceived(const int &handleType, const Contacter &contacter);
    void onMessageDownload(const Message &msg);
    void onDigitalStatusChanged(QString handlerId, int status);
    void onPttGettedChanged(bool isGetted);

    void onSignalBlueToothFileSendPercent(const QString &fileName, const int &dataType, const int &size, const int &maxSize);

    void onSignalSendPercent(const QString &groupId, const QString &id, int recvNum, int allNum);
    void onSignalReceivePercent(const QString &groupId, const QString &id, int recvNum, int allNum);

    void onSignalContacterShareContacterReceived(const Contacter &contacter);

    //sos
    void onSetProperty();
    void onSetPropertyValue(int type);

    void onSignalGroupExchange();

    //p2p connected finished
    void onSignalP2PConnected(const QString &groupId, int state);
    int onGetP2PConnectedStatus(const QString &groupId);


private:
    QString getTitleStringFromTitleType(int titleType);

private:
    QString m_myData;
    QQuickItem *m_qmlRootItem;
    QObject *m_qmlRootObject;
    QQuickView *m_view;
    QMap<QString,int> m_groupIdStatusMap;
};






#endif // GETDATA_H
