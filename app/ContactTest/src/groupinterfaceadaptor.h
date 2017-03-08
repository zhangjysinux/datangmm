#ifndef GROUPINTERFACEADAPTOR_H
#define GROUPINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "groupdata.h"

class GroupInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    static GroupInterfaceAdaptor *getInstance();
    bool addGroup(const Group &group);
    bool removeGroup(const QString &id);
    bool updateGroup(const Group &group);
    Group getGroup(const QString &id);
    QStringList getGroups(const GroupType &type);

signals:
    void signalGroupChanged(int type, Group contacter);

public slots:
    void onDbusSignalGroupChanged(int type, QDBusVariant data);

private:
    explicit GroupInterfaceAdaptor(QObject *parent = 0);

private:
    QDBusInterface m_interface;
    static GroupInterfaceAdaptor *m_instance;
};

#endif // GROUPINTERFACEADAPTOR_H
