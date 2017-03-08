#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <QObject>
#include <QSqlTableModel>
#include "receiver.h"
#include "groupmessagemanager.h"
#include "messagedata.h"

class MessageManager : public QObject
{
    Q_OBJECT
public:
    static MessageManager *getInstance();
    Q_INVOKABLE QStringList getMessageList(const QString &groupId);
    Q_INVOKABLE quint64 sendMessage(const Message &msg);
    Q_INVOKABLE quint64 sendTextMessage(const QString &groupId, const QString &userId, const QString &text);
    void setQMLObject(QObject *qmlObject);

private slots:
    void onSignalMessageChanged(int type, Message msg);

signals:
    void signalMessageChanged(int type, Message msg);

private:
    explicit MessageManager(QObject *parent = 0);
    static MessageManager *m_instance;
    QHash<QString, GroupMessageManager*> m_msgManagerMap;
    QObject *m_qmlObject;
};

#endif // MESSAGEMANAGER_H
