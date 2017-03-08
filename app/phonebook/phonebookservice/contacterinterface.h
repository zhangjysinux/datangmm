#ifndef CONTACTERINTERFACE_H
#define CONTACTERINTERFACE_H

#include <QObject>
#include <QSqlTableModel>
#include <QTimer>
#include "../../commondata/datatype/commondata.h"
#include "../../commondata/datatype/contacterdata.h"
#include "sqltablemodelwithcache.h"
#include "errorhandle.h"
#include "../../commondata/datatype/groupdata.h"
#include "adaptor/contacterpushadaptor.h"
#include "adaptor/downloadcontactadaptor.h"

struct IdIpMap
{
    Contacter contacter;
    QStringList sendToContacterList;
};

class ContacterInterface : public QObject, public ErrorHandle
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.contacter")
public:
    static ContacterInterface *getInstance();
    Contacter getLocalInfo();
    void setLocalInfo(Contacter data);
    void addUpdateLocalInfo(Contacter data);
    bool isIdNumberAdapt(const QString &id, const QString &number);//judge the contact id and number adapt
    Contacter getContacterByPocId(const QString &pocid);

    static QString converNumbersToString(const QList<PhoneNumber> &numbers);
    static QList<PhoneNumber> converNumbersToStruct(const QString &value);
signals:
    void signalContacterChanged(int type, QDBusVariant data);
    void signalContacterChangedToChangeRemoteContacter(int type, const ContacterList &contacters);
    void signalContacterChangedToChangeHistory(int type, const Contacter &data);

public slots:
    bool onAddContacter(const Contacter &contacter);
    bool onRemoveContacter(const QString &id);
    bool onUpdateContacter(const Contacter &contacter);
    bool onUpdateLocalInfo(const Contacter &contacter);

    QDBusVariant onGetLocalInfo();
    QDBusVariant onGetContacter(const QString &id, const bool &containsPhotoData = false);
    Contacter getContacter(const QString &id, const bool &containsPhotoData = false);
    Contacter onGetContacter2(const QString &id, const bool &containsPhotoData = false);
    QStringList onGetContacters(const int &contacterType);
    QStringList onGetContactersExcept(const int &contacterType, const int &netType, const QStringList &exceptContacters);
    QStringList onGetContactersExcept(const int &contacterType, const QStringList &exceptContacters);
    int onGetContactersCount(const int &contacterType);

    QStringList onSearchContactors(const int &contacterType, const QString& searchText);
    //if netType is -1 indicate that alltype
    QStringList onSearchContactors(const int &contacterType, const int &netType, const QString& searchText);
    ContacterList onSearchContactors2(const int &contacterType, const QString& searchText);
    QStringList onSearchContactorsExcept(const int &contacterType, const QString& searchText, const QStringList &exceptContacters);
    QStringList onSearchContactorsExcept(const int &contacterType, const int &netType, const QString& searchText, const QStringList &exceptContacters);
    QString onSearchContacterNumber(const int &contacterType, const QString& searchText);
    QString onSearchContacterBluetooth(const QString& bluetoothAddress);
    void onSetAllSelectedStatus(const int &status);

    //contacter schedul to delete or add or update
    void onContacterReceived(const int &handleType, const ContacterList &contacters);
    void onContacterOneReceived(const int &handleType, const Contacter &ct);//for contact share
    void onDbChanged();
    void onContacterDownload(const ContacterList &contacters);

    void onContacterPostFinished(int status, const Contacter &contacter);

    //local acount can edit contacter

    bool onCanEditContacter(const QString &id);
    QStringList onGetLowLevelContacters(const QString &id);

private:
    explicit ContacterInterface(QObject *parent = 0);
    bool checkUserExist(const QString &userId);
    Contacter getContacter(const int &row);
    Contacter getContacterFromRowData(const QStringList &rowData, const bool &containsPhotoData = false);

    bool setContacter(const int &row, const Contacter &contacter);
    static QString converNetworkTypeToString(const NetworkType &type);
    static NetworkType converStringToNetworkType(const QString &value);
    QString convertLeadersToString(const QStringList &leaders);
    QStringList convertStringToLeaders(const QString &value);
    void emitChangeSignal(int type, Contacter changeContacter);
    bool removeOrganizationContacters();
    void contacterChangeToChangeLowLevelPeopleAddQueue(const IdIpMap &idIpMap);

private slots:
    void contacterChangeToChangeLowLevelPeople();
    void onContacterChangeToChangeLowLevelPeople(const Contacter &contacter);

    void onSignalContacterPushConnected(const int &status);
    void onSignalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    int addContacter(const Contacter &contacter, Contacter &addedContacter);
    int addOrganizationContacter(const Contacter &contacter, Contacter &addedContacter);
    int removeContacter(const QString &id, Contacter &removedContacter);
    int updateContacter(const Contacter &contacter, Contacter &updatedContacter);
    QStringList getLowLevel(const QString &id, const QStringList &alreadyHasLeaders,const GroupList &groupList);
    QString getUseableContacterId();

private:
    SqlTableModelWithCache m_contacterModel;
    SqlTableModel m_localModel;
    static ContacterInterface *m_instance;
    QList<IdIpMap> m_sendContacterToGroupContacters;
    ContacterPushAdaptor *m_contacterPushAdaptor;
    QTimer m_timer;
    DownloadContactAdaptor *m_downloadAdaptor;
};

#endif // CONTACTERINTERFACE_H
