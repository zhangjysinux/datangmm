#ifndef UNREADMESSAGESETTER_H
#define UNREADMESSAGESETTER_H

#include <QObject>

class UnreadMessageSetter : public QObject
{
    Q_OBJECT

    // 定义Interface名称为"com.sinux.launcher.unreadMessageSetter"
    Q_CLASSINFO("D-Bus Interface", "com.sinux.launcher.unreadMessageSetter")

signals:
    void signalGroupMessageButtonClicked();
    void signalMessageButtonClicked();

public slots:
    // 设置未读群组消息的数量
    bool setUnreadGroupMessageCount(int count);

    // 设置未读短消息数量
    bool setUnreadMessageCount(int count);

public:
    explicit UnreadMessageSetter(QObject *parent = 0);

};

#endif // UNREADMESSAGESETTER_H
