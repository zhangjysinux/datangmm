#ifndef MESSAGESENDERINTERFACE_H
#define MESSAGESENDERINTERFACE_H

#include <QObject>
#include "fileshare2.h"

class MessageSenderInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.messagesenderinterface")
public:
    static MessageSenderInterface *getInstance();
    ~MessageSenderInterface();

signals:
    void signalConnectFinished(const QString &id, const int &status);
    void signalReceivePercent(const QString &id,
                              const QString &msgId,
                              const int &msgType,
                              const QString &fileName,
                              const qint64 &size,
                              const qint64 &maxSize);
    void signalSendPercent(const QString &id,
                           const QString &msgId,
                           const int &msgType,
                           const QString &fileName,
                           const qint64 &size,
                           const qint64 &maxSize);

public slots:
    void onSetPort(const int &port);

    void onSetReceivePath(const QString &path);
    int onConnect(const QString &id, const QString &address);
    bool onIsConnected();
    int onDisconnect();
    int onSendFile(const QString &id,
                   const QString &msgId,
                   const QString &fileName,
                   const int &msgType);

    void onSetLocalId(const QString &id);

private slots:
    void onSignalConnectFinished(const QString &id, const int &status);
    void onSignalReceivePercent(const QString &id,
                                const QString &msgId,
                                const int &msgType,
                                const QString &fileName,
                                const qint64 &size,
                                const qint64 &maxSize);
    void onSignalSendPercent(const QString &id,
                             const QString &msgId,
                             const int &msgType,
                             const QString &fileName,
                             const qint64 &size,
                             const qint64 &maxSize);

private:
    explicit MessageSenderInterface(QObject *parent = 0);

private:
    static MessageSenderInterface *instance;
    FileShare2 *m_share;
};

#endif // MESSAGESENDERINTERFACE_H
