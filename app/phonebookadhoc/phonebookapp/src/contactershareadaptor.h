#ifndef CONTACTERSHAREADAPTOR_H
#define CONTACTERSHAREADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../../commondata/datatype/contacterdata.h"

class ContacterShareAdaptor : public QObject
{
    Q_OBJECT
public:
    static ContacterShareAdaptor *getInstance();
    ~ContacterShareAdaptor();

signals:
    void signalConnectFinished(const int &status);
    void signalReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void signalContacterReceived(const int &handleType, const Contacter &contacter);

public:
    void setPort(const int &port);
    void setAddresses(const QStringList &addresses);
    int connect();
    bool onIsConnected();
    int disconnect();
    int sendFile(const QString &fileName);

    int sendContacter(const Contacter &contacter);

private slots:
    void onSignalFileShareConnectFinished(const int &status);
    void onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize);
    void onSignalFileShareContacterReceived(const Contacter &contacter);

private:
    explicit ContacterShareAdaptor(QObject *parent = 0);

private:
    static ContacterShareAdaptor *instance;
    QDBusInterface m_interface;

};

#endif // CONTACTERSHAREADAPTOR_H
