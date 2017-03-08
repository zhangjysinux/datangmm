#ifndef FILESHAREADAPTOR_H
#define FILESHAREADAPTOR_H

#include <QObject>
#include <QDBusInterface>

class FileShareAdaptor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.phonebookfileshare")
public:
    static FileShareAdaptor *getInstance();
    ~FileShareAdaptor();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

public slots:
    void onSetPort(const int &port);

    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);

private slots:
    void onSignalFileShareConnectFinished(const int &status);
    void onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

private:
    explicit FileShareAdaptor(QObject *parent = 0);

private:
    static FileShareAdaptor *instance;
    QDBusInterface m_interface;

};

#endif // FILESHAREADAPTOR_H
