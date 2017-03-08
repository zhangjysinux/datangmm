#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>

class DateTimeGetter;
class QTimer;
class DbusServiceManager;

class MainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString date READ getDate WRITE setDate NOTIFY signalDateChanged)
    Q_PROPERTY(QString week READ getWeek WRITE setWeek NOTIFY signalWeekChanged)
    Q_PROPERTY(QString time READ getTime WRITE setTime NOTIFY signalTimeChanged)
    Q_PROPERTY(int hour READ getHour WRITE setHour NOTIFY signalHourChanged)
    Q_PROPERTY(int min READ getMin WRITE setMin NOTIFY signalMinchanged)
    Q_PROPERTY(int sec READ getSec WRITE setSec NOTIFY signalSecChanged)
    Q_PROPERTY(int unreadGroupMessage READ getUnreadGroupMessage WRITE setUnreadGroupMessage NOTIFY signalUnreadGroupMessageChanged)
    Q_PROPERTY(int unreadMessage READ getUnreadMessage WRITE setUnreadMessage NOTIFY signalUnreadMessageChanged)

signals:
    void signalDateChanged(const QString &date);
    void signalWeekChanged(const QString &week);
    void signalTimeChanged(const QString &time);
    void signalHourChanged(int hour);
    void signalMinchanged(int min);
    void signalSecChanged(int sec);
    void signalUnreadGroupMessageChanged(int messageCount);
    void signalUnreadMessageChanged(int messageCount);
    void signalGroupMessageButtonClicked();
    void signalMessageButtonClicked();

private slots:
    void onTimerTimeout();

public:
    explicit MainViewModel(QObject *parent = 0);
    ~MainViewModel();

    QString getDate();
    void setDate(const QString &date);

    QString getWeek();
    void setWeek(const QString &week);

    QString getTime();
    void setTime(const QString &time);

    int getHour();
    void setHour(int hour);

    int getMin();
    void setMin(int min);

    int getSec();
    void setSec(int sec);

    int getUnreadGroupMessage();
    void setUnreadGroupMessage(int messageCount);

    int getUnreadMessage();
    void setUnreadMessage(int messageCount);

    void updateDateTime();

    Q_INVOKABLE void execPhonebookApp();
    Q_INVOKABLE void execCallHistoryApp();
    Q_INVOKABLE void execDialApp();
    Q_INVOKABLE void execGroupMessageApp();
    Q_INVOKABLE void execMessageApp();
    Q_INVOKABLE void execSettingApp();

private:
    QString m_date;
    QString m_week;
    QString m_time;
    int m_hour;
    int m_min;
    int m_sec;

    int m_unreadGroupMessage; // 未读的群组消息数量
    int m_unreadMessage;      // 未读的短消息数量

    DateTimeGetter *m_dateTimeGetter;
    QTimer *m_timer;

    DbusServiceManager *m_dbusServiceManager;
};

#endif // MAINVIEWMODEL_H
