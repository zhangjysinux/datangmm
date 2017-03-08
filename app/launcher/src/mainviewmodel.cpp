#include "mainviewmodel.h"
#include <QTimer>
#include "datetimegetter.h"
#include "dbusservicemanager.h"
#include <QDebug>

MainViewModel::MainViewModel(QObject *parent) :
    QObject(parent),
    m_date("1900年00月00日"),
    m_week("星期一"),
    m_time("00:00:"),
    m_hour(0),
    m_min(0),
    m_sec(0),
    m_unreadGroupMessage(0),
    m_unreadMessage(0),
    m_dateTimeGetter(NULL),
    m_timer(NULL),
    m_dbusServiceManager(NULL)
{
    m_dbusServiceManager = new DbusServiceManager(this);
    m_dbusServiceManager->createService();

    connect(this, SIGNAL(signalGroupMessageButtonClicked()), m_dbusServiceManager,
            SIGNAL(signalGroupMessageButtonClicked()));
    connect(this, SIGNAL(signalMessageButtonClicked()), m_dbusServiceManager,
            SIGNAL(signalGroupMessageButtonClicked()));

    m_dateTimeGetter = new DateTimeGetter(this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    m_timer->start(1000);

    updateDateTime();
}

MainViewModel::~MainViewModel()
{
    if (m_dateTimeGetter)
        delete m_dateTimeGetter;

    if (m_timer)
    {
        if (m_timer->isActive())
            m_timer->stop();

        delete m_timer;
    }

    if (m_dbusServiceManager)
        delete m_dbusServiceManager;
}

void MainViewModel::onTimerTimeout()
{
    if (++m_sec >= 60)
        updateDateTime();
    else
        setSec(m_sec);
}

QString MainViewModel::getDate()
{
    return m_date;
}

void MainViewModel::setDate(const QString &date)
{
    m_date = date;
    emit signalDateChanged(date);
}

QString MainViewModel::getWeek()
{
    return m_week;
}

void MainViewModel::setWeek(const QString &week)
{
    m_week = week;
    emit signalWeekChanged(week);
}

QString MainViewModel::getTime()
{
    return m_time;
}

void MainViewModel::setTime(const QString &time)
{
    m_time = time;
    emit signalTimeChanged(time);
}

int MainViewModel::getHour()
{
    return m_hour;
}

void MainViewModel::setHour(int hour)
{
    m_hour = hour;
    emit signalHourChanged(hour);
}

int MainViewModel::getMin()
{
    return m_min;
}

void MainViewModel::setMin(int min)
{
    m_min = min;
    emit signalMinchanged(min);
}

int MainViewModel::getSec()
{
    return m_sec;
}

void MainViewModel::setSec(int sec)
{
    m_sec = sec;
    emit signalSecChanged(sec);
}

int MainViewModel::getUnreadGroupMessage()
{
    return m_unreadGroupMessage;
}

void MainViewModel::setUnreadGroupMessage(int messageCount)
{
    m_unreadGroupMessage = messageCount;
    emit signalUnreadGroupMessageChanged(m_unreadGroupMessage);
}

int MainViewModel::getUnreadMessage()
{
    return m_unreadMessage;
}

void MainViewModel::setUnreadMessage(int messageCount)
{
    m_unreadMessage = messageCount;
    emit signalUnreadMessageChanged(m_unreadMessage);
}

void MainViewModel::updateDateTime()
{
    setDate(m_dateTimeGetter->getDate());
    setWeek(m_dateTimeGetter->getWeek());
    setTime(m_dateTimeGetter->getTime());
    setHour(m_dateTimeGetter->getHour());
    setMin(m_dateTimeGetter->getMin());
    setSec(m_dateTimeGetter->getSec());
}

void MainViewModel::execPhonebookApp()
{

}

void MainViewModel::execCallHistoryApp()
{

}

void MainViewModel::execDialApp()
{

}

void MainViewModel::execGroupMessageApp()
{
    emit signalGroupMessageButtonClicked();
}

void MainViewModel::execMessageApp()
{
    emit signalMessageButtonClicked();
}

void MainViewModel::execSettingApp()
{

}
