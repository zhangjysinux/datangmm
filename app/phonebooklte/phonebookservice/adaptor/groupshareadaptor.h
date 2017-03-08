#ifndef GROUPSHAREADAPTOR_H
#define GROUPSHAREADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/groupdata.h"

class GroupShareAdaptor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.phonebookfileshare")
public:
    static GroupShareAdaptor *getInstance();
    ~GroupShareAdaptor();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

    void signalGroupReceived(const Group &group);

public slots:
    void onSetPort(const int &port);

    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);

    int onSendGroup(const Group &group);

private slots:
    void onSignalFileShareConnectFinished(const int &status);
    void onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit GroupShareAdaptor(QObject *parent = 0);

private:
    static GroupShareAdaptor *instance;
    QDBusInterface m_interface;

};

#endif // GROUPSHAREADAPTOR_H
