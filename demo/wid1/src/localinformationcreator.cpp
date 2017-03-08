#include "localinformationcreator.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "common.h"

LocalInformationCreator::LocalInformationCreator()
{
}

void LocalInformationCreator::createData(int count)
{
    Q_UNUSED(count)
    //首先删除源数据
    clearData();

    QSqlQuery query;
    query.exec("create table local (id varchar(50) primary key, "
               "surname varchar(20), name varchar(20), title varchar(20), network varchar(200), "
               "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
               "bluetooth varchar(50),type int)");

    query.prepare("INSERT INTO local (id, surname, name, title, network, photopath, leaders, snnumber, bluetooth, type) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    int id = qrand()%100;
    query.addBindValue(QString::number(id));
    query.addBindValue(createSurname());
    query.addBindValue(createName());
    query.addBindValue(QString("%1").arg(qrand()%4+1));
    query.addBindValue("{\"ad_hoc_network\":\"192.168.1.10\",\"satellite_network\":\"13127564876\"}");
    query.addBindValue("null");
    query.addBindValue("[]");
    query.addBindValue(createDeviceNumber());
    query.addBindValue("00:00:00:00:00:00");
    query.addBindValue("1");

    if (!query.exec())
    {
        qDebug() << "LocalInformationCreator::createData insert data" << query.lastError();
    }
}

void LocalInformationCreator::clearData()
{
    QSqlQuery query;
    query.exec("delete from local");
}
