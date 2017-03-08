#include "callhistorycreator.h"
#include <QDebug>
#include <QTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "common.h"

CallHistoryCreator::CallHistoryCreator()
{
}

void CallHistoryCreator::createData(int count)
{
    QSqlQuery query;
    if (!query.exec("create table callhistory (id varchar(50) primary key, "
               "dialTarget varchar(200), dialType varchar(20), dialTime int, dialNetworkType varchar(200), "
               "dateStart varchar(50), dateEnd varchar(50),"
               "contactId1 varchar(50), contactId2 varchar(50), contactId3 varchar(50),"
               "contactId4 varchar(50), contactId5 varchar(50), contactId6 varchar(50),"
               "number1 varchar(50), number2 varchar(50), number3 varchar(50),"
               "number4 varchar(50), number5 varchar(50), number6 varchar(50))"))
    {
        qDebug() << "CallHistoryCreator::createData create table " << query.lastError();
    }

    for (int i = 0; i < count; i++)
    {
        query.prepare("INSERT INTO callhistory (id, dialTarget, dialType, dialTime, dialNetworkType, dateStart, dateEnd, "
                      "contactId1, contactId2, contactId3, contactId4, contactId5, contactId6, "
                      "number1, number2, number3, number4, number5, number6) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        qsrand(QTime::currentTime().msec());
        QString dataTime = QString("2016022911233%1").arg(qrand() % 10);

        query.addBindValue(QString("%1").arg(i));
        query.addBindValue(QString("[\"%1\"]").arg(qrand() % count));
        query.addBindValue(getDialType(qrand() % 3));
        query.addBindValue(qrand() % 100);
        query.addBindValue("PS_NETWORK_VIDEO_DIAL");
        query.addBindValue(dataTime);
        query.addBindValue(dataTime);
        query.addBindValue(QString("%1").arg(qrand() % count));
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue(QString("1819094%1").arg(qrand() % 9999));
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.exec();
    }
}

void CallHistoryCreator::clearData()
{
    QSqlQuery query;
    query.exec("delete from callhistory");
}

QString CallHistoryCreator::getDialType(int index)
{
    switch (index)
    {
    case 1:
        return "IN_DIAL";
    case 2:
        return "OUT_DIAL";
    default:
        return "MISS_DIAL";
    }
}
