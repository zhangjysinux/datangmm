#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QSqlDatabase>

class WeatherData : public QObject
{
    Q_OBJECT
public:
    explicit WeatherData(QObject *parent = 0);
    ~WeatherData();
    Q_INVOKABLE QString startAddWeatherData();
    Q_INVOKABLE void getWeatherData(QString id);
    Q_INVOKABLE void getWeatherList();
    QString getWindDirection();
protected:
    QString transWeatherDataToString(const QDateTime &date);
    QDateTime transStringToWeatherData(const QString &date);

    //添加数据
    QString addWeatherData();

    //打开数据库
    bool openDatabase();

    //关闭数据库
    void closeDatabase();

signals:
    void signalOneWeatherData(QString keyhead,QString value);
    void signalWeatherDataHistory(QString id,QString time,QString lon,QString lat,QString humidity,QString air,QString wind,QString windpower,QString precipitation,QString pressure);

public slots:

private:
    QSqlDatabase m_db;
};

#endif // WEATHERDATA_H
