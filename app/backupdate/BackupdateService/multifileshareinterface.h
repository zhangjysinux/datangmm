#ifndef _MULTI_FILESHAREINTERFACE_H
#define _MULTI_FILESHAREINTERFACE_H

#include <QObject>
#include "fileshare.h"
class MultiFileShareInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.multifileshare")
public:
    static MultiFileShareInterface *getInstance();
    ~MultiFileShareInterface();
signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
public slots:
    void onSetPort(const int &port);

    void onSetReceivePath(const QString &path);
    void onSetAddresses(const QStringList &addresses);
    int onConnect();
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &fileName);

private slots:
    void onFileShareConnectFinished(const int &status);
    void onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
private:
    explicit MultiFileShareInterface(QObject *parent = 0);

private:
    static MultiFileShareInterface *instance;
    FileShare *m_share;
};


#endif // FILESHAREINTERFACE_H
