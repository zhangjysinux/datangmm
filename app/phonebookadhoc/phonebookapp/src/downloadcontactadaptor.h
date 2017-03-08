#ifndef DOWNLOADCONTACTADAPTOR_H
#define DOWNLOADCONTACTADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/contacterdata.h"
#include "../../../commondata/datatype/groupdata.h"

class DownloadContactAdaptor : public QObject
{
    Q_OBJECT
public:
    static DownloadContactAdaptor *getInstance();
    void startDownload();
signals:
    void signalContacterDownload(const ContacterList& contacterList);
    void signalGroupsDownload(const GroupList &groupList);
    void signalDownloadPercent(const QString &percent, const QString &type);

private slots:
    void onDbusSignalContactersDownload(const ContacterList& contacterList);
    void onDbusSignalGroupsDownload(const GroupList &groupList);
    void onDbusSignalDownloadPercent(const QString &percent, const QString &type);

private:
    explicit DownloadContactAdaptor(QObject *parent = 0);
    static DownloadContactAdaptor *instance;
    QDBusInterface m_interface;
};

#endif // DOWNLOADCONTACTADAPTOR_H
