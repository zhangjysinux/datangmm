#ifndef CONTACTERSHAREINTERFACE_H
#define CONTACTERSHAREINTERFACE_H

#include <QObject>
#include "fileshare.h"

class ContacterShareInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.contactershare")
public:
    static ContacterShareInterface *getInstance();
    ~ContacterShareInterface();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

public slots:
    void onSetPort(const int &port);

    void onSetReceivePath(const QString &path);
    void onSetAddresses(const QStringList &addresses);
    void onSetAddresses(const IdNetworkMap &idNetworkMap);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);

private slots:
    void onFileShareConnectFinished(const int &status);
    void onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit ContacterShareInterface(QObject *parent = 0);

private:
    static ContacterShareInterface *instance;
    FileShare *m_share;
};

#endif // CONTACTERSHAREINTERFACE_H
