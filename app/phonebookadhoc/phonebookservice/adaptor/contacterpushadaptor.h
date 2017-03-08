#ifndef CONTACTERPUSHADAPTOR_H
#define CONTACTERPUSHADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/contacterdata.h"

class ContacterPushAdaptor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.phonebookcontactershare")
public:
    static ContacterPushAdaptor *getInstance();
    ~ContacterPushAdaptor();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

    void signalContacterReceived(const Contacter &contacter);

public slots:
    void onSetPort(const int &port);

    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);

    int onSendContacter(const Contacter &contacter);

private slots:
    void onSignalFileShareConnectFinished(const int &status);
    void onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit ContacterPushAdaptor(QObject *parent = 0);

private:
    static ContacterPushAdaptor *instance;
    QDBusInterface m_interface;

};

#endif // CONTACTERPUSHADAPTOR_H
