#ifndef GROUPINTERFACE_H
#define GROUPINTERFACE_H

#include <QObject>
#include <QSqlTableModel>
#include <QTimer>
#include "../../commondata/datatype/commondata.h"
#include "../../commondata/datatype/groupdata.h"
#include "sqltablemodelwithcache.h"
#include "contacterinterface.h"
#include "adaptor/groupshareadaptor.h"
#include "adaptor/downloadcontactadaptor.h"
#include "adaptor/groupmessagesenderadaptorlte.h"

class GroupInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.group")
public:
    static GroupInterface *getInstance();
    Group getGroup(const QString &id);
    GroupList getGroupList();
    Group findGroupByName(const QString &groupName);
    bool removeGroupById(const QString &id);
    QStringList getGroupAddrsByType(const QString &groupId,
                                    int networkType,
                                    bool includeCreator = true);

signals:
    void signalGroupChanged(int type, QDBusVariant data);
    void signalGroupChangedToChangeRemoteGroup(int handleType, const GroupList &groups);
    void signalGroupExchange();
    void signalPttAllowChanged(int status, QString ip, QString soureceIp);

public slots:
    bool onAddGroup(const Group &group);
    bool onRemoveGroup(const QString &id);
    bool onUpdateGroup(const Group &group);
    QDBusVariant onGetGroup(const QString &id);
    QStringList onGetGroups(const int &type);
    QStringList onGetSelfCreatedGroups(const int &type);

    //for share
    void onGroupReceived(const int &handleType, const GroupList &groups);
    void onDbChanged();
    void onGroupDownload(const GroupList &groups);

    //for remote lte network create group
    void onLteGroupReceived(const Group &group, int operateType);

private:
    explicit GroupInterface(QObject *parent = 0);
    bool checkGroupExist(const QString &id);
    Group getGroup(const int &row);
    Group getGroupFromRowData(const QStringList &rowData);
    bool setGroup(const int &row, const Group &group);
    QString convertMembersToString(const QStringList &members);
    QStringList convertStringToMembers(const QString &value);
    QString convertLeadersToString(const QStringList &leaders);
    QStringList convertStringToLeaders(const QString &value);
//    QString convertNetworkTypeToString(const NetworkType &type);
//    NetworkType convertStringToNetworkType(const QString &value);
    QString convertTypeToString(const GroupType &type);
    GroupType convertStringToType(const QString value);
    void emitChangeSignal(int type, Group changeGroup);
    void groupChangeToChangeGroupPeopleAddQueue(const Group &group);
    GroupList getBtGroup();
    QStringList getBtGroupAddrs();

private slots:
    void onSignalGroupShareConnected(const int &status);
    void onSignalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onGroupChangeToChangeGroupPeople(const Group &group);

    void groupChangeToChangeGroupPeople();

    void onSignalGroupChangedFinished(const Group &group);

private:
    bool addGroup(const Group &group);
    bool addGroup2(const Group &group);
    bool removeGroup(const QString &id);
    bool removeGroups();
    bool updateGroup(const Group &group);

    //add lte group
    void addGroupNetwork(const Group &group);
    void removeGroupNetwork(const Group &group);
    void updateGroupNetwork(const Group &group);

private:
    SqlTableModelWithCache m_groupModel;
    static GroupInterface *m_instance;
    ContacterInterface *m_contacterInterface;
    GroupShareAdaptor *m_groupShareAdaptor;
    GroupList m_sendGroupList;
    QTimer m_timer;
    DownloadContactAdaptor *m_downloadAdaptor;
};

#endif // GROUPINTERFACE_H
