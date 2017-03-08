#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <QDebug>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sqldata.db");
    if (!db.open()) {
        qDebug() << "Cannot open contact database";
        return false;
    }

    QSqlQuery query;
    //create local data
    query.exec("create table local (id int primary key, "
               "name varchar(20), title varchar(20), network varchar(200), "
               "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
               "bluetooth varchar(50))");

    query.prepare("INSERT INTO local (id, name, title, network, photopath, leaders, snnumber, bluetooth) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(100);
    query.addBindValue("Sinux");
    query.addBindValue("5");
    query.addBindValue("{\"ad_hoc_network\":\"192.168.1.10\"}");
    query.addBindValue("null");
    query.addBindValue("[]");
    query.addBindValue("xxxx-22xx-ddfg-eegg");
    query.addBindValue("00:00:00:00:00:00");
    query.exec();

    query.exec("create table contacter (id int primary key, "
               "name varchar(20), title varchar(20), network varchar(200), "
               "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
               "bluetooth varchar(50))");

    query.prepare("INSERT INTO contacter (id, name, title, network, photopath, leaders, snnumber, bluetooth) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(101);
    query.addBindValue("LiMing");
    query.addBindValue("4");
    query.addBindValue("{\"ad_hoc_network\":\"192.168.1.5\"}");
    query.addBindValue("null");
    query.addBindValue("[100]");
    query.addBindValue("xxxx-22xx-ddfg-eeff");
    query.addBindValue("00:00:00:00:00:00");
    query.exec();

    query.prepare("INSERT INTO contacter (id, name, title, network, photopath, leaders, snnumber, bluetooth) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(102);
    query.addBindValue("WangJun");
    query.addBindValue("4");
    query.addBindValue("{\"ad_hoc_network\":\"192.168.1.6\"}");
    query.addBindValue("null");
    query.addBindValue("[100]");
    query.addBindValue("xxxx-22xx-ddfg-eeqq");
    query.addBindValue("00:00:00:00:00:00");
    query.exec();

    query.prepare("INSERT INTO contacter (id, name, title, network, photopath, leaders, snnumber, bluetooth) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(103);
    query.addBindValue("ZhangJun");
    query.addBindValue("1");
    query.addBindValue("{\"ad_hoc_network\":\"192.168.1.7\"}");
    query.addBindValue("null");
    query.addBindValue("[102, 100]");
    query.addBindValue("xxxx-22xx-ddfg-eeww");
    query.addBindValue("00:00:00:00:00:00");
    query.exec();

    query.exec("create table groups (id int primary key, "
               "name varchar(20), iconpath varchar(50), members varchar(200), "
               "leaders varchar(50), address varchar(50), network varchar(20), "
               "type varchar(20))");

    query.prepare("INSERT INTO groups (id, name, iconpath, members, leaders, address, network, type) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(1001);
    query.addBindValue("SinuxGroup");
    query.addBindValue("null");
    query.addBindValue("[100, 101, 102, 103]");
    query.addBindValue("[100]");
    query.addBindValue("192.168.1.255");
    query.addBindValue("ad_hoc_network");
    query.addBindValue("conversion");
    query.exec();

    return true;
}

#endif
