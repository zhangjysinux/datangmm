#ifndef GROUPMESSAGESENDERADAPTORADHOC_H
#define GROUPMESSAGESENDERADAPTORADHOC_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/commondata.h"
#include "groupadaptordata.h"
#include "groupmessagesenderadaptor.h"
#include "commondef.h"

//baotong group comunication

class GroupMessageSenderAdaptorAdhoc : public GroupMessageSenderAdaptor
{
    Q_OBJECT
public:
    static GroupMessageSenderAdaptorAdhoc *getInstance();

    //群组jianli
    void createGroup(QT_DTT::GroupInfo info);
    void removeGroup(QT_DTT::GroupInfo info);
    void updateGroup(QT_DTT::GroupInfo info);

private:
    explicit GroupMessageSenderAdaptorAdhoc(QObject *parent = 0);
    static GroupMessageSenderAdaptorAdhoc *instance;
    QDBusInterface m_interfaceGroup;
    QDBusInterface m_interfaceMessage;
};

#endif // GROUPMESSAGESENDERADAPTORADHOC_H
