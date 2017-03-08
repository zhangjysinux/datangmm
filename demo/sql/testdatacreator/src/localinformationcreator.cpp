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
    if (!query.exec("create table local (id varchar(50) primary key, "
               "surname varchar(20), name varchar(20), title varchar(20), network varchar(200), "
               "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
               "bluetooth varchar(50))"))
    {
        qDebug() << "LocalInformationCreator::createData insert data" << query.lastError();
    }

    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    QString queryText = QString("INSERT INTO local VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9)")
            .arg(QString("'%1'").arg(qrand() % 100))
            .arg(QString("'%1'").arg(createSurname()))
            .arg(QString("'%1'").arg(createName()))
            .arg(QString("'%1'").arg(qrand() % 4 + 1))
            .arg(QString("'%1'").arg("{\"ad_hoc_network\":\"192.168.1.10\",\"satellite_network\":\"13127564876\"}"))
            .arg(QString("'%1'").arg("null"))
            .arg(QString("'%1'").arg("[]"))
            .arg(QString("'%1'").arg(createDeviceNumber()))
            .arg(QString("'%1'").arg("00:00:00:00:00:00"));

    if (!query.exec(queryText))
    {
        qDebug() << "LocalInformationCreator::createData insert data" << query.lastError();
    }
}

void LocalInformationCreator::clearData()
{
    QSqlQuery query;
    query.exec("delete from local");
}
