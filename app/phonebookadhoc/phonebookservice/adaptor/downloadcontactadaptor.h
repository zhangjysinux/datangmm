#ifndef DOWNLOADCONTACTADAPTOR_H
#define DOWNLOADCONTACTADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/contacterdata.h"
#include "../../../commondata/datatype/groupdata.h"

class DownloadContactAdaptor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.downloadcontactadaptor")
public:
    static DownloadContactAdaptor *getInstance();

signals:
    void signalContactersDownloadFinished(const ContacterList &contacterList);
    void signalGroupsDownloadFinished(const GroupList &groupList);
    void signalGroupBattlesDownloadFinished(const GroupList &groupList);
    void signalGetFinished(int status);
    void signalPostContactFinished(int status, const Contacter &contacter);
    void signalPostGroupFinished(int status, const Group &group);
    void signalPostGroupBattleFinished(int status, const Group &group);

public slots:
    void onStartGet();
    void onPostContact(const Contacter &contacter);
    void onPostGroup(const Group &group);
    void onPostGroupBattle(const Group &group);

private slots:
    void onSignalContactersDownloadFinished(const ContacterList &contacterList);
    void onSignalGroupsDownloadFinished(const GroupList &groupList);
    void onSignalGroupBattlesDownloadFinished(const GroupList &groupList);
    void onSignalGetFinished(int status);
    void onSignalPostContactFinished(int status, const Contacter &contacter);
    void onSignalPostGroupFinished(int status, const Group &group);
    void onSignalPostGroupBattleFinished(int status, const Group &group);

private:
    explicit DownloadContactAdaptor(QObject *parent = 0);
    static DownloadContactAdaptor *instance;
    QDBusInterface m_interface;
};

#endif // DOWNLOADCONTACTADAPTOR_H
