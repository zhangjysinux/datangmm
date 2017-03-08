#include "laserrangdata.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QUuid>
#include <QFile>

LaserRangData::LaserRangData(QObject *parent) :
    QObject(parent)
{
    openDatabase();
}
LaserRangData::~LaserRangData()
{
    closeDatabase();
}

QString LaserRangData::transLaserDateToString(const QDateTime &date)
{
    QString value = date.toString("yyyyMMddhhmmss");
    return value;
}

QDateTime LaserRangData::transStringToLaserDate(const QString &date)
{
    QDateTime value = QDateTime::fromString(date, "yyyyMMddhhmmss");
    return value;
}

QString LaserRangData::addLaserRangData()
{
    QSqlQuery query;
    query.prepare("INSERT INTO laser (id, time, lon, lat, range) "
                  "VALUES (?, ?, ?, ?, ?)");
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    QString id = QUuid::createUuid().toString();
    query.addBindValue(id);
    QDateTime time = QDateTime::currentDateTime();
    QString timeinfo = transLaserDateToString(time);
    query.addBindValue(timeinfo);
    float lonnum = qrand()%73+ 62 + qrand()/(RAND_MAX+0.0);
    query.addBindValue(QString("%1").arg(lonnum));
    float latnum = qrand()%50 + 3 + qrand()/(RAND_MAX+0.0);
    query.addBindValue(QString("%1").arg(latnum));
    float range = qrand()%1000 + qrand()/(RAND_MAX+0.0);
    query.addBindValue(QString("%1m").arg(range));
    if (!query.exec())
        qDebug() << "insert laser failed " << query.lastError();
    return id;
}

bool LaserRangData::openDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("laser.db");
    m_db.close();
    if (!m_db.open()) {
        qDebug() << "Cannot open laser database";
        return false;
    }
    QSqlQuery query;
    if (!query.exec("create table laser (id varchar(20) primary key, "
                    "time varchar(20), lon varchar(20), lat varchar(20), range varchar(20))"))
    {
        qDebug() << "LaserCreator::createData create table" << query.lastError();
        return false;
    }
    return true;
}

void LaserRangData::closeDatabase()
{
    m_db.close();
}

QString LaserRangData::startAddLaserRangData()
{
    return addLaserRangData();
}

void LaserRangData::getLaserRang(QString id)
{
    qDebug()<<"getLaserRang"<<id;
    if (id.isEmpty() || id.isNull()) {
        qDebug()<<"getLaserRang id is error";
        return;
    }
    QSqlQuery query;
    QString sql = QString("SELECT * FROM laser WHERE id = '%1'").arg(id);
    query.exec(sql);
    if(query.next()) {
        QDateTime timeinfo = transStringToLaserDate(query.value(1).toString());
        emit signalOneLaserData("日期",timeinfo.toString("yyyy-MM-dd"));
        emit signalOneLaserData("时间",timeinfo.toString("hh:mm:ss"));
        emit signalOneLaserData("经度",query.value(2).toString());
        emit signalOneLaserData("纬度",query.value(3).toString());
        emit signalOneLaserData("距离",query.value(4).toString());
    }

}

void LaserRangData::getLaserList()
{
    QSqlQuery query;
    query.exec("SELECT * FROM laser");
    while (query.next()) {
        QString id = query.value(0).toString();
        QDateTime timeinfo = transStringToLaserDate(query.value(1).toString());
        QString time = timeinfo.toString("yyyy-MM-dd hh:mm");
        QString loninfo = query.value(2).toString();
        QString latinfo = query.value(3).toString();
        emit signalLaserHistory(id,time,loninfo,latinfo);
    }
}
