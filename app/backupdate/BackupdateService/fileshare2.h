#ifndef FILESHARE2_H
#define FILESHARE2_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFuture>
#include <QDebug>
#include "../../commondata/datatype/commondata.h"

class FileShare2 : public QObject
{
    Q_OBJECT
public:
    static FileShare2 *newInstance();
    ~FileShare2();

signals:
    void signalSetConnectType(const int &netType);
    void signalSetPort(const int &port);
    void signalInitServer();

    void signalSetReceivePath(const QString &path);
    void signalConnect(const QString &id, const QString &address);
    void signalDisconnect();
    void signalSendFile(const QString &id,
                        const QString &msgId,
                        const QString &fileName,
                        const int &msgType);
    void signalSetLocalId(const QString &id);

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


private slots:
    void onSetConnectType(const int &netType);
    void onSetPort(const int &port);
    void onInitServer();

    void onSetReceivePath(const QString &path);
    int onConnect(const QString &id, const QString &address);

    int onDisconnect();
    int onSendFile(const QString &id,
                   const QString &msgId,
                   const QString &fileName,
                   const int &msgType);

    void onSetLocalId(const QString &id);

public slots:
    bool onIsConnected();

private slots:
    void onConnectServerFinished();
    void onDisconnectServerFinished();
    void onConnectServerError(QAbstractSocket::SocketError error);
    void onConnectedClientError(QAbstractSocket::SocketError error);

    void onNewConnection();
    void onNewMessage();

    void onSendFiles();


private:
    explicit FileShare2(QObject *parent = 0);

private:
    static FileShare2 *instance;
    QList<QTcpSocket*> m_wantConnectClientList;
    QList<QTcpSocket*> m_connectedClientList;
    QTcpServer *m_server;
    QStringList m_addresses;
    int m_port;
    QString m_ifaceName;//nic name
    QList<QFile*> m_sendFiles;
    QList<QPair<QString,QFile*>> m_sendFiles2;
    QMap<QString, QFile*> m_receiveFilesMap;
    QFuture<void> m_future;
    QMutex m_mutex;
    QString m_receiveFilePath;
    QMap<QString,QTcpSocket*> m_idSocketMap;
    QSet<QString> m_connectedIds;
    QString m_id;
};

#endif // FILESHARE2_H
