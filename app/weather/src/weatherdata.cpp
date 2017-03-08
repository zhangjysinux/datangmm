#include "weatherdata.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QUuid>
#include <QFile>

WeatherData::WeatherData(QObject *parent) :
    QObject(parent)
{
    openDatabase();
}
WeatherData::~WeatherData()
{
    closeDatabase();
}

QString WeatherData::transWeatherDataToString(const QDateTime &date)
{
    QString value = date.toString("yyyyMMddhhmmss");
    return value;
}

QDateTime WeatherData::transStringToWeatherData(const QString &date)
{
    QDateTime value = QDateTime::fromString(date, "yyyyMMddhhmmss");
    return value;
}

QString WeatherData::addWeatherData()
{
    //相对湿度空气质量风向风力降水量气压
    QSqlQuery query;
    query.prepare("INSERT INTO weather (id, time, lon, lat, humidity, air, wind, windpower, precipitation, pressure) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    QString id = QUuid::createUuid().toString();
    query.addBindValue(id);
    QDateTime time = QDateTime::currentDateTime();
    QString timeinfo = transWeatherDataToString(time);
    query.addBindValue(timeinfo);
    float lonnum = qrand()%73+ 62 + qrand()/(RAND_MAX+0.0);
    query.addBindValue(QString("%1").arg(lonnum));
    float latnum = qrand()%50 + 3 + qrand()/(RAND_MAX+0.0);
    query.addBindValue(QString("%1").arg(latnum));
    int humidity = qrand()%50;
    query.addBindValue(QString("%1%").arg(humidity));
    int air = qrand()%300;
    query.addBindValue(QString("%1").arg(air));
    QString wind = getWindDirection();
    query.addBindValue(QString("%1风").arg(wind));
    int windpower = qrand()%12;
    query.addBindValue(QString("%1级").arg(windpower));
    float precipitation = qrand()%200 + 3 + qrand()/(RAND_MAX+0.0);
    precipitation = ((float)((int)(precipitation*10)))/10;
    query.addBindValue(QString("%1mm").arg(precipitation));
    float pressure = qrand()%200 + 860 + qrand()/(RAND_MAX+0.0);
    pressure = ((float)((int)(pressure*10)))/10;
    query.addBindValue(QString("%1pa").arg(pressure));
    if (!query.exec())
        qDebug() << "insert weather failed " << query.lastError();
    return id;
}

bool WeatherData::openDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("weather.db");
    m_db.close();
    if (!m_db.open()) {
        qDebug() << "Cannot open weather database";
        return false;
    }
    QSqlQuery query;
    if (!query.exec("create table weather (id varchar(20) primary key, "
                    "time varchar(20), lon varchar(20), lat varchar(20), humidity varchar(20), air varchar(20), wind varchar(20), windpower varchar(20),precipitation varchar(20),pressure varchar(20))"))
    {
        qDebug() << "WeatherCreator::createData create table" << query.lastError();
        return false;
    }
    return true;
}

void WeatherData::closeDatabase()
{
    m_db.close();
}

QString WeatherData::startAddWeatherData()
{
    return addWeatherData();
}

void WeatherData::getWeatherData(QString id)
{
    qDebug()<<"getWeatherData"<<id;
    if (id.isEmpty() || id.isNull()) {
        qDebug()<<"getWeatherData id is error";
        return;
    }
    QSqlQuery query;
    QString sql = QString("SELECT * FROM weather WHERE id = '%1'").arg(id);
    query.exec(sql);
    if(query.next()) {
        QDateTime timeinfo = transStringToWeatherData(query.value(1).toString());
        emit signalOneWeatherData("日期",timeinfo.toString("yyyy-MM-dd"));
        emit signalOneWeatherData("时间",timeinfo.toString("hh:mm:ss"));
        emit signalOneWeatherData("经度",query.value(2).toString());
        emit signalOneWeatherData("纬度",query.value(3).toString());
        emit signalOneWeatherData("相对湿度",query.value(4).toString());
        emit signalOneWeatherData("空气质量",query.value(5).toString());
        emit signalOneWeatherData("风向",query.value(6).toString());
        emit signalOneWeatherData("风力",query.value(7).toString());
        emit signalOneWeatherData("降水量",query.value(8).toString());
        emit signalOneWeatherData("气压",query.value(9).toString());
    }

}

void WeatherData::getWeatherList()
{
    //humidity , air , wind , windpower ,precipitation ,pressure
    QSqlQuery query;
    query.exec("SELECT * FROM weather");
    while (query.next()) {
        QString id = query.value(0).toString();
        QDateTime timeinfo = transStringToWeatherData(query.value(1).toString());
        QString time = timeinfo.toString("yyyy-MM-dd hh:mm");
        QString loninfo = query.value(2).toString();
        QString latinfo = query.value(3).toString();
        QString humidity = query.value(4).toString();
        QString air = query.value(5).toString();
        QString wind = query.value(6).toString();
        QString windpower = query.value(7).toString();
        QString precipitation = query.value(8).toString();
        QString pressure = query.value(9).toString();
        emit signalWeatherDataHistory(id,time,loninfo,latinfo,humidity,air,wind,windpower,precipitation,pressure);
    }
}

QString WeatherData::getWindDirection()
{
    int ret = qrand()%8;
    QString direction = "";
    switch (ret) {
    case 0:
        direction = "北";
        break;
    case 1:
        direction = "东北";
        break;
    case 2:
        direction = "东";
        break;
    case 3:
        direction = "东南";
        break;
    case 4:
        direction = "南";
        break;
    case 5:
        direction = "西南";
        break;
    case 6:
        direction = "西";
        break;
    case 7:
        direction = "西北";
        break;
    default:
        break;
    }
    return direction;
}
