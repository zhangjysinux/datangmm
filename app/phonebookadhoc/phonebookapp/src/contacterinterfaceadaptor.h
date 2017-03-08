#ifndef CONTACTERINTERFACEADAPTOR_H
#define CONTACTERINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/contacterdata.h"

class ContacterInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    static ContacterInterfaceAdaptor *getInstance();
    QStringList getContacters(const int &contacterType = CONTACTERTYPE_ORGANIZATION);
    QStringList getContactersExcept(const int &contacterType, const QStringList &exceptContacters);
    QStringList getContactersExcept(const int &contacterType, const int &netType, const QStringList &exceptContacters);
    Contacter getContacter(const QString &id, const bool &containsPhotoData = false);
    Contacter getLocalInfo();
    bool updateLocalInfo(const Contacter &localInfo);
    bool addContacter(const Contacter &contacter);
    bool updateContacter(const Contacter &contacter);
    bool removeContacter(const QString &id);

    QStringList searchContactors(const int &contacterType, const QString& searchText);
    ContacterList onSearchContactors2(const int &contacterType, const QString& searchText);
    QStringList searchContactorsExcept(const int &contacterType, const QString& searchText, const QStringList &exceptContacters);
    QStringList searchContactorsExcept(const int &contacterType, const int &netType, const QString& searchText, const QStringList &exceptContacters);
    QString searchContacterNumber(const int &contacterType, const QString& searchText);
    QString searchContacterBluetooth(const QString& bluetoothAddress);
    void setAllSelectedStatus(const int &status);

    bool canEditContacter(const QString &id);

signals:
    void signalContacterChanged(int type, Contacter contacter);

public slots:
    void onDbusSignalContacterChanged(int type, QDBusVariant data);
    void onContacterReceived(const int &handleType, const Contacter &contacter);

private:
    explicit ContacterInterfaceAdaptor(QObject *parent = 0);

private:
    QDBusInterface m_interface;
    static ContacterInterfaceAdaptor *m_instance;
};

#endif // CONTACTERINTERFACEADAPTOR_H
