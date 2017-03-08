#ifndef CONTACTERINTERFACEADAPTOR_H
#define CONTACTERINTERFACEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "contacterdata.h"

class ContacterInterfaceAdaptor : public QObject
{
    Q_OBJECT
public:
    static ContacterInterfaceAdaptor *getInstance();
    QList<quint32> getContacters();
    Contacter getContacter(const quint32 &id);
    Contacter getLocalInfo();
    bool updateLocalInfo(const Contacter &localInfo);
    bool addContacter(const Contacter &contacter);
    bool updateContacter(const Contacter &contacter);
    bool removeContacter(const quint32 &id);

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
