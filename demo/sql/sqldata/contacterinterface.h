#ifndef CONTACTERINTERFACE_H
#define CONTACTERINTERFACE_H

#include <QObject>
#include <QSqlTableModel>
#include "commondata.h"
#include "contacterdata.h"

class ContacterInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.contacter")
public:
    explicit ContacterInterface(QObject *parent = 0);

signals:
    void signalContacterChanged(int type, QDBusVariant data);

public slots:
    bool onAddContacter(const Contacter &contacter);
    bool onRemoveContacter(const quint32 &id);
    bool onUpdateContacter(const Contacter &contacter);
    bool onUpdateLocalInfo(const Contacter &localInfo);
    QDBusVariant onGetLocalInfo();
    QDBusVariant onGetContacter(const quint32 &id);
    QDBusVariant onGetContacters();
    int onGetContactersCount();

private:
    bool checkUserExist(const quint32 userId);
    Contacter getContacter(const int &row);
    Contacter getLocalInfo();
    bool setContacter(const int &row, const Contacter &contacter);
    QString converNumbersToString(const QList<PhoneNumber> &numbers);
    QList<PhoneNumber> converNumbersToStruct(const QString &value);
    QString converNetworkTypeToString(const NetworkType &type);
    NetworkType converStringToNetworkType(const QString &value);
    QString convertLeadersToString(const QList<quint32> &leaders);
    QList<quint32> convertStringToLeaders(const QString &value);
    void emitChangeSignal(int type, Contacter changeContacter);

private:
    QSqlTableModel m_contacterModel;
    QSqlTableModel m_localModel;
};

#endif // CONTACTERINTERFACE_H
