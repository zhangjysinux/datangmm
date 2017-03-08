#include "datetimegetter.h"
#include <QDateTime>

DateTimeGetter::DateTimeGetter(QObject *parent) :
    QObject(parent)
{
}

QString DateTimeGetter::getDate()
{
    QString dateStr = QDateTime::currentDateTime().toString("yyyy年MM月dd日");
    return dateStr;
}

QString DateTimeGetter::getWeek()
{
    QString weekStr = QDateTime::currentDateTime().toString("dddd");
    return weekStr;
}

QString DateTimeGetter::getTime()
{
    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:");
    return timeStr;
}

int DateTimeGetter::getHour()
{
    QString hourStr = QDateTime::currentDateTime().toString("hh");
    return hourStr.toInt();
}

int DateTimeGetter::getMin()
{
    QString minStr = QDateTime::currentDateTime().toString("mm");
    return minStr.toInt();
}

int DateTimeGetter::getSec()
{
    QString secStr = QDateTime::currentDateTime().toString("ss");
    return secStr.toInt();
}
