#ifndef CONTACTERINTERFACEADAPTOR_H
#define CONTACTERINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/contacterdata.h"

class ContacterInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    static ContacterInterfaceAdaptor *getInstance();
    QStringList getContacters(const int &contacterType = CONTACTERTYPE_ORGANIZATION);
    Contacter getContacter(const QString &id);
    Contacter getLocalInfo();
    bool updateLocalInfo(const Contacter &localInfo);
    bool addContacter(const Contacter &contacter);
    bool updateContacter(const Contacter &contacter);
    bool removeContacter(const QString &id);

    QStringList searchContactors(const int &contacterType, const QString& searchText);
    QString searchContacterNumber(const int &contacterType, const QString& searchText);

signals:
    void signalContacterChanged(int type, Contacter contacter);



public slots:
    void onDbusSignalContacterChanged(int type, QDBusVariant data);



private:
    explicit ContacterInterfaceAdaptor(QObject *parent = 0);

private:
    QDBusInterface m_interface;
    static ContacterInterfaceAdaptor *m_instance;
};

#endif // CONTACTERINTERFACEADAPTOR_H
