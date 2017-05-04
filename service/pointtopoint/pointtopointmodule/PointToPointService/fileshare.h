#ifndef FILESHARE_H
#define FILESHARE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFuture>
#include <QDebug>
//#include "../../commondata/datatype/commondata.h"
#include "./datatype/commondata.h"

//class FileShareDelegate
//{
//public:
//    explicit FileShareDelegate(){}
//    virtual void onFileShareConnectFinished(const int &status){}
//    virtual void onFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize){}
//    virtual void onFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize){}
//};

class FileShare : public QObject
{
    Q_OBJECT
public:
    static FileShare *newInstance();
    ~FileShare();
//    FileShareDelegate *delegate;
signals:
    void signalSetConnectType(const int &netType);
    void signalSetPort(const int &port);
    void signalInitServer();

    void signalSetReceivePath(const QString &path);
    void signalSetAddresses(const IdNetworkMap &addresses);
    void signalConnect();
    void signalDisconnect();
    void signalSendFile(const QString &fileName);

    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);


private slots:
    void onSetConnectType(const int &netType);
    void onSetPort(const int &port);
    void onInitServer();

    void onSetReceivePath(const QString &path);
    void onSetAddresses(const IdNetworkMap &idNetworkMap);
    int onConnect();

    int onDisconnect();
    int onSendFile(const QString &fileName);


public slots:
    bool onIsConnected();

private slots:
    void onConnectServerFinished();
    void onDisconnectServerFinished();
    void onConnectServerError(QAbstractSocket::SocketError error);

    void onNewConnection();
    void onNewMessage();

    void onSendFiles();


private:
    explicit FileShare(QObject *parent = 0);

private:
    static FileShare *instance;
    QList<QTcpSocket*> m_wantConnectClientList;
    QList<QTcpSocket*> m_connectedClientList;
    QTcpServer *m_server;
    QStringList m_addresses;
    int m_port;
    QString m_ifaceName;//nic name
    QList<QFile*> m_sendFiles;
    QList<QFile*> m_sendFiles2;
    QMap<QString, QFile*> m_receiveFilesMap;
    QFuture<void> m_future;
    QMutex m_mutex;
    QString m_receiveFilePath;
    IdNetworkMap m_idNetworkMap;
    QSet<QString> m_connectedIds;
//    FileShareDelegate *m_defaultDelegate;
};

#endif // FILESHARE_H
