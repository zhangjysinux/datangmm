#ifndef UNREADMESSAGEADAPTOR_H
#define UNREADMESSAGEADAPTOR_H

#include <QObject>
#include <QMutex>

class UnreadMessageAdaptor : public QObject
{
    Q_OBJECT
public:
    static UnreadMessageAdaptor* getInstance();

public:
    // 设置未读群组消息数量,count:未读的数量
    bool setUnreadGroupMessage(int count);

    // 设置未读短消息数量,count:未读的数量
    bool setUnreadMessage(int count);

public slots:
    // 当群组消息按钮被按下时触发
    void onDbusSignalGroupMessageButtonClicked();

    // 当短消息按钮被按下时出发
    void onDbusSignalMessageButtonClicked();

private:
    explicit UnreadMessageAdaptor(QObject *parent = 0);

private:
    static UnreadMessageAdaptor *m_instance;
    static QMutex m_lock;
};

#endif // UNREADMESSAGEADAPTOR_H
