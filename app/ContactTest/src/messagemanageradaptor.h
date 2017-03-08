#ifndef MESSAGEMANAGERADAPTOR_H
#define MESSAGEMANAGERADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "messagedata.h"

class MessageManagerAdaptor : public QObject
{
    Q_OBJECT
public:
    static MessageManagerAdaptor *getInstance();
    QStringList getMessageList(const QString &groupId);
    QStringList onGetMessageList(const QDateTime &startTime, const QDateTime &endTime, const QString &groupId);
    //startIndex from close start
    QStringList onGetMessageList(const int &startIndex, const int &endIndex, const QString &groupId);
    quint64 sendMessage(const Message &msg);
    quint64 sendTextMessage(const QString &groupId, const QString &text);
    Message getMessage(const QString &id, const QString &groupId);
    void getMessageDownload(const QString &id, const QString &groupId);
    bool setMessageIsread(const bool &isReadFlag, const QString &id, const QString& groupId);
    quint64 sendOtherMessage(const QString &groupId, const QString &fileName, const int &messageType);
    int getSpeakChance();
    int startSpeak();

signals:
    void signalMessageChanged(int type, Message msg);
    void signalMessageDownload(const Message &msg);

public slots:
    void onDbusSignalMessageChanged(int type, QDBusVariant data);
    void onDbusSignalMessageDownload(QDBusVariant data);

private:
    explicit MessageManagerAdaptor(QObject *parent = 0);

private:
    QDBusInterface m_interface;
    static MessageManagerAdaptor *m_instance;
};

#endif // MESSAGEMANAGERADAPTOR_H
