#ifndef _SMSSENDER_INTERFACE_H_
#define _SMSSENDER_INTERFACE_H_
#include <QDBusConnection>
#include <QDBusVariant>
#include <QObject>
#include "fileshare.h"

class SmsSenderInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.groupshare")
public:
    static SmsSenderInterface *getInstance();
    ~SmsSenderInterface();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalDataReceived(const QByteArray &data);

public slots:
    void onSetPort(const int &port);

    void onSetReceivePath(const QString &path);
    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);
    int onSendData(const QByteArray &data);

private slots:
    void onFileShareConnectFinished(const int &status);
    void onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit SmsSenderInterface(QObject *parent = 0);

private:
    static SmsSenderInterface *instance;
    FileShare *m_share;
};

#endif
