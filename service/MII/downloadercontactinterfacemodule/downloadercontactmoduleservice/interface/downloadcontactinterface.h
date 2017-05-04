#ifndef DOWNLOADCONTACTINTERFACE_H
#define DOWNLOADCONTACTINTERFACE_H

#include <QObject>
#include "downloadcontact.h"

class DownloadContactInterface : public QObject
{
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.downloadcontact")
    Q_OBJECT
public:
    static DownloadContactInterface* getInstance();

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
    void onSignalContactersDownload(const ContacterServerList &cList);
    void onSignalContacterNetworksDownload(const IdContacterNetworkServerMap &idNetworkMap);
    void onSignalContacterPhotoDownload(const IdContacterPhotoNameMap &idPhotoMap);
    void onSignalGroupsDownload(const GroupOrganizationServerList &gList);
    void onSignalGroupBattlesDownload(const GroupBattleServerList &gList);

    void onSignalPostFinished(int type, int status);

private:
    explicit DownloadContactInterface(QObject *parent = 0);
    static DownloadContactInterface* instance;
    DownloadContact *m_downloadContact;

    ContacterServerList m_contacterServerList;
    IdContacterNetworkServerMap m_idContacterNetworkServerMap;
    IdContacterPhotoNameMap m_idContacterPhotoNameMap;
    GroupOrganizationServerList m_groupOrganizationServerList;
    GroupBattleServerList m_groupBattleServerList;
    LoginRequestData m_loginData;

    Contacter m_postContacter;
    Group m_postGroup;
    Group m_postGroupBattle;
};

#endif // DOWNLOADCONTACTINTERFACE_H
