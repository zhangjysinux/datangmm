#ifndef POINTTOPOINTADAPTOR_H
#define POINTTOPOINTADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/messagedata.h"
#include "groupmessagesenderadaptor.h"

class PointToPointAdaptor : public GroupMessageSenderAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.phonebookcontactershare")
public:
    static PointToPointAdaptor *getInstance();
    ~PointToPointAdaptor();
    qint64 sendMessage(const Message &message);

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

//    void signalGroupMessageReceived(const Message &message);

public slots:
    void onSetPort(const int &port);

    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);

    int onSendMessage(const Message &message);

private slots:
    void onSignalFileShareConnectFinished(const int &status);
    void onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit PointToPointAdaptor(QObject *parent = 0);

private:
    static PointToPointAdaptor *instance;
    QDBusInterface m_interface;

};

#endif // POINTTOPOINTADAPTOR_H
