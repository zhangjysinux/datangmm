#ifndef _MULTI_FILESHAREADAPTOR_H
#define _MULTI_FILESHAREADAPTOR_H

#include <QObject>
#include <QDBusInterface>

class MultiFileShareAdaptor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.multifileshare")
public:
    static MultiFileShareAdaptor *getInstance();
    ~MultiFileShareAdaptor();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

public slots:
    void onSetConnectType(const int &netType);
    void onSetPort(const int &port);
    void onInitServer();

    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    int onDisconnect();
    bool onIsConnected();
    int onSendFile(const QString &fileName);
    void onSetReceivePath(const QString &path);
private slots:
    void onSignalFileShareConnectFinished(const int &status);
    void onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit MultiFileShareAdaptor(QObject *parent = 0);

private:
    static MultiFileShareAdaptor *instance;
    QDBusInterface m_interface;

};

#endif // FILESHAREADAPTOR_H
