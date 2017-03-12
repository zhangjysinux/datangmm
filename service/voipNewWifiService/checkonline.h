#ifndef CHECKONLINE_H
#define CHECKONLINE_H

#include <QObject>
#include <QTcpSocket>

typedef QMap<QString, bool> IpStatus;

class CheckOnline : public QObject
{
    Q_OBJECT
public:
    explicit CheckOnline(QObject *parent = 0);
    void setCheckIpPort(const int &port);
    void addIp(const QString &ip);
    void delIp(const QString &ip);
    void start();

signals:
    void signalCheckFinished(const IpStatus &ipStatus);

private slots:
    void onConnectServerFinished();
//    void onDisconnectServerFinished();
    void onConnectServerError(QAbstractSocket::SocketError error);
    void checkIpList();

private:
    IpStatus m_ipStatusMap;
    QList<QTcpSocket*> m_wantConnectClientList;
    QStringList m_ipList;
    int m_port;
    bool m_areadlyStart;
};

#endif // CHECKONLINE_H
