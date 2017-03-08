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
    db.setDatabaseName("message.db");
    if (!db.open()) {
        qDebug() << "Cannot open contact database";
        return false;
    }

    QString groupId = "1001";
    QString groupTable = QString("group%1").arg(groupId);
    QSqlQuery query;
    query.exec(QString("create table %1 (id varchar(50), "
               "type int, state int, userid varchar(20), groupid varchar(20), date varchar(50), isread bool, "
               "accessorypath varchar(50), content varchar(500))").arg(groupTable));

    return true;
}

#endif
