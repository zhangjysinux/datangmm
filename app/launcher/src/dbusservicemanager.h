#ifndef DBUSSERVICEMANAGER_H
#define DBUSSERVICEMANAGER_H

#include <QObject>

class UnreadMessageSetter;

class DbusServiceManager : public QObject
{
    Q_OBJECT

signals:
    void signalGroupMessageButtonClicked();
    void signalMessageButtonClicked();

public:
    explicit DbusServiceManager(QObject *parent = 0);
    ~DbusServiceManager();

    void createService();

private:
    UnreadMessageSetter *m_unreadMessageSetter;
};

#endif // DBUSSERVICEMANAGER_H
