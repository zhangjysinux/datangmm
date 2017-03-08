#ifndef GROUPINTERFACE_H
#define GROUPINTERFACE_H

#include <QObject>
#include <QSqlTableModel>
#include "commondata.h"
#include "groupdata.h"

class GroupInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.group")
public:
    explicit GroupInterface(QObject *parent = 0);

signals:
    void signalGroupChanged(int type, QDBusVariant data);

public slots:
    bool onAddGroup(const Group &group);
    bool onRemoveGroup(const quint32 &id);
    bool onUpdateGroup(const Group &group);
    QDBusVariant onGetGroup(const quint32 &id);
    QDBusVariant onGetGroups(const int &type);

private:
    bool checkGroupExist(const quint32 &id);
    Group getGroup(const int &row);
    bool setGroup(const int &row, const Group &group);
    QString convertMembersToString(const QList<quint32> &members);
    QList<quint32> convertStringToMembers(const QString &value);
    QString convertLeadersToString(const QList<quint32> &leaders);
    QList<quint32> convertStringToLeaders(const QString &value);
    QString convertNetworkTypeToString(const NetworkType &type);
    NetworkType convertStringToNetworkType(const QString &value);
    QString convertTypeToString(const GroupType &type);
    GroupType convertStringToType(const QString value);
    void emitChangeSignal(int type, Group changeGroup);

private:
    QSqlTableModel m_groupModel;
};

#endif // GROUPINTERFACE_H
