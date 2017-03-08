#ifndef DATETIMEGETTER_H
#define DATETIMEGETTER_H

#include <QObject>

class DateTimeGetter : public QObject
{
    Q_OBJECT
public:
    explicit DateTimeGetter(QObject *parent = 0);

    //获取日期
    QString getDate();
    //获取星期
    QString getWeek();
    //获取时间:只获取时和分
    QString getTime();
    //获取小时数
    int getHour();
    //获取分钟数
    int getMin();
    //获取秒数
    int getSec();
};

#endif // DATETIMEGETTER_H
