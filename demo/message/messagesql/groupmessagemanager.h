#ifndef GROUPMESSAGEMANAGER_H
#define GROUPMESSAGEMANAGER_H

#include <QObject>
#include <QSqlTableModel>
#include "receiver.h"
#include "sender.h"

class GroupMessageManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupMessageManager(const QString &groupId, QObject *parent = 0);
    quint64 sendMessage(const Message &msg);
    QStringList getMessageList() const;

private slots:
    void onNewMessage(Message msg);

signals:
    void signalMessageChanged(int type, Message msg);

private:
    quint64 sendTextMessage(const QVariant &data);
    bool recordMessage(const Message &message);
    bool updateMessage(const Message &message);
    int findMessage(const QString &id);
    int insertLastRow(const QDateTime &time);

private:
    QString m_groupId;
    QSqlTableModel m_sqlModel;
    Receiver m_receiver;
    Sender m_sender;
};

#endif // GROUPMESSAGEMANAGER_H
