#ifndef DOWNLOADCONTACT_H
#define DOWNLOADCONTACT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QQueue>
//#include "../../commondata/datatype/contacterdata.h"
//#include "../../commondata/datatype/groupdata.h"
#include "./datatype/contacterdata.h"
#include "./datatype/groupdata.h"
#include <QProcess>
//#include <serverdata.h>
#include "./datatype/serverdata.h"
//#include "request.h"
#include "request2.h"

enum ParseType
{
    ParseType_Contacts = 0 ,
    ParseType_Groups = 1,
    ParseType_None = 2
};

enum AfterLoginToDo
{
    AfterLoginToDo_Download = 0,
    AfterLoginToDo_PostContacter,
    AfterLoginToDo_PostGroup,
    AfterLoginToDo_PostGroupBattle
};

typedef QMap<QString, QString> ServerErrorInfo;

class DownloadContact : public QObject
{
    Q_OBJECT
//    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.downloadcontact")

public:
    static DownloadContact* getInstance();

public slots:
    void onStart();

private:
    explicit DownloadContact(QObject *parent = 0);

private:
    ContacterServer convertJsonToContacter(const QString& content);
    ContacterServerList convertJsonToContacters(const QString &content);
    QString convertContacterToJson(const ContacterServer &contacter);

    GroupOrganizationServer convertJsonToGroup(const QString& content);
    GroupOrganizationServerList convertJsonToGroups(const QString &content);
    QString convertGroupToJson(const GroupOrganizationServer &group);

    ContacterPhotoServer convertJsonToPhoto(const QString &content);
    QString convertPhotoToJson(const ContacterPhotoServer &photo);

    ContacterNetworkServer convertJsonToNetwork(const QString &content);
    IdContacterNetworkServerMap convertJsonToNetworks(const QString &content);
    QString convertNetworkToJson(const ContacterNetworkServer &contacterNetwork);

    ContacterPasswordServer convertJsonToPassword(const QString &content);
    QString convertPasswordToJson(const ContacterPasswordServer &contacterPassword);

    GroupBattleServer convertJsonToBattle(const QString &content);
    QString convertBattleToJson(const GroupBattleServer &group);

    ServerErrorInfo convertJsonToLoginError(const QString &content);
    QString removeSpecialWord(const QString &content);
    ContacterServer findContacter(const QString &id);

    void combineServerDataToLocal();

public:

    void login(const LoginRequestData &loginRequestData);

    void getContacts();
    void postContact(const ContacterServer &contact);

    void getGroups();
    void postGroup(const GroupOrganizationServer &group);

    void getContacterPassword(const QString &id);
    void postContacterPassword(const ContacterPasswordServer &password);

    void getContacterNetworks();
    void postContacterNetwork(const ContacterNetworkServer &network);

    void getContacterPhoto(const QString &id);
    void postContacterPhoto(const ContacterPhotoServer &photo);

    void getBattleGroup();
    void postBattleGroup(const GroupBattleServer &groupBattle);

    bool isRequestFinished();

private slots:
    void onDownloadRequestFinished(const QString &content);

signals:
    void signalContactersDownload(const ContacterServerList &cList);
    void signalContacterNetworksDownload(const IdContacterNetworkServerMap &idNetworkMap);
    void signalContacterPhotoDownload(const IdContacterPhotoNameMap &idPhotoMap);
    void signalGroupsDownload(const GroupOrganizationServerList &gList);
    void signalGroupBattlesDownload(const GroupBattleServerList &gList);
    void signalDownloadPercent(qint32 size,
                               qint32 maxSize,
                               qint32 statusCode,
                               const QString &statusString);

    void signalPostFinished(int type, int status);

private:
    static DownloadContact* instance;
//    QNetworkAccessManager *m_network;
//    ContacterServerList m_contacterServerList;
//    IdContacterNetworkServerMap m_idContacterNetworkServerMap;
//    IdContacterPhotoNameMap m_idContacterPhotoNameMap;
//    GroupOrganizationServerList m_groupOrganizationServerList;
//    GroupBattleServerList m_groupBattleServerList;
    QString m_contacterUpdateFileName;
    bool m_loginSuccess;
    ContacterList m_contacterLocalList;
    int m_afterLoginToDo;
    QQueue<Request2*> m_queue;
    bool m_requestFinished;


public:
//    ContacterServerList getContacterServer();
//    IdContacterNetworkServerMap getContacterNetworkServer();
//    IdContacterPhotoNameMap getContacterPhotoServer();
//    GroupOrganizationServerList getGroupOrganizationServer();
//    GroupBattleServerList getGroupBattleServer();
};

#endif // DOWNLOADCONTACT_H
