#ifndef CALLHISTORYINTERFACEADAPTOR_H
#define CALLHISTORYINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/callhistorydata.h"

class CallHistoryInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit CallHistoryInterfaceAdaptor(QObject *parent = 0);
    static CallHistoryInterfaceAdaptor *getInstance();

    QStringList getCallHistoryList();
    CallHistory getCallHistory(const QString &historyId);
    bool addCallHistory(CallHistory &callHistory);
    int getCallHistoryCount();
    bool removeCallHistory(const QString &historyId);
    QStringList getCallHistoryBytarget(QString target);
    bool clearCallHistory();
    QStringList searchCallHistory(const QString &searchText);
signals:
    void signalCallHistoryChanged(QString historyId,int type);

public slots:
    void onCallHistoryChanged(int type,QDBusVariant callHistory);
private:
    QDBusInterface m_interface;
    static CallHistoryInterfaceAdaptor *m_instance;

};

#endif // CALLHISTORYINTERFACEADAPTOR_H
