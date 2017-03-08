#ifndef FILESHAREADAPTOR_H
#define FILESHAREADAPTOR_H

#include <QObject>
#include <QDBusInterface>

class FileShareAdaptor : public QObject
{
    Q_OBJECT
public:
    static FileShareAdaptor *getInstance();
    ~FileShareAdaptor();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);

public:
    void setPort(const int &port);

    void setAddresses(const QStringList &addresses);
    int connect();
    bool onIsConnected();
    int disconnect();
    int sendFile(const QString &fileName);

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
