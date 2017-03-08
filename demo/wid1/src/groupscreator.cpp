#include "groupscreator.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "common.h"

GroupsCreator::GroupsCreator()
{
}

void GroupsCreator::createData(int count)
{
    //首先创建表
    QSqlQuery query;
    if (!query.exec("create table groups (id varchar(20) primary key, "
               "name varchar(20), creator varchar(20), iconpath varchar(50), members varchar(200), "
               "leaders varchar(50), address varchar(50), port int, network varchar(20), "
               "type varchar(20))"))
    {
        qDebug() << "GroupsCreator::createData create table" << query.lastError();
    }

    //会话群20个
    for (int i = 1000;  i < count + 1000; i++)
    {
        query.prepare("INSERT INTO groups (id, name, creator, iconpath, members, leaders, address, port, network, type) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        query.addBindValue(QString("%1").arg(i));
        query.addBindValue(createGroupName(i - 1000));
        query.addBindValue("2");
        query.addBindValue("null");
        query.addBindValue("[\"1\", \"2\", \"3\", \"4\"]");
        query.addBindValue("[\"1\"]");
        query.addBindValue("224.0.0.55");
        query.addBindValue(50000);
        query.addBindValue("ad_hoc_network");
        query.addBindValue("conversion");
        if (!query.exec())
            qDebug() << "insert groups failed " << query.lastError();
    }

    //建制群20个
    for (int i = 2000; i < count + 2000; i++)
    {
        query.prepare("INSERT INTO groups (id, name, creator, iconpath, members, leaders, address, port, network, type) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        query.addBindValue(QString("%1").arg(i));
        query.addBindValue(QString("第%1军区").arg(i - 2000));
        query.addBindValue("2");
        query.addBindValue("null");
        query.addBindValue("[\"1\", \"2\", \"3\", \"4\"]");
        query.addBindValue("[\"1\"]");
        query.addBindValue("224.0.0.55");
        query.addBindValue(50000);
        query.addBindValue("ad_hoc_network");
        query.addBindValue("subordinate");
        if (!query.exec())
            qDebug() << query.lastError();
    }
}

void GroupsCreator::clearData()
{
    QSqlQuery query;
    query.exec("delete from groups");
}
