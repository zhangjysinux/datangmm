#include "contactsdatacreator.h"
#include <QTime>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "common.h"
#include "chinesetopinyin.h"


ContactsDataCreator::ContactsDataCreator()
{
}

void ContactsDataCreator::createData(int count)
{
    //首先创建表
    QSqlQuery query;
    if (!query.exec("create table contacter (id varchar(50) primary key, "
               "surname varchar(20), name varchar(20), title varchar(20), network varchar(200), "
               "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
               "bluetooth varchar(50), namePinyin varchar(40), type int)"))
    {
        qDebug() << "ContactsDataCreator::createData(int count) create table error"
                 << query.lastError();
    }

    //循环插入数据
    for (int i = 0; i < count; i++)
    {
        if (!query.prepare("INSERT INTO contacter (id, surname, name, title, network, photopath, leaders, snnumber, bluetooth, namePinyin, type) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))
        {
            qDebug() << "ContactsDataCreator::createData(int count) insert data error"
                     << query.lastError();
            break;
        }

        QString surname = createSurname();
        QString name = createName();
        QString title = getTitle(i);

        query.addBindValue(QString("%1").arg(i));
        query.addBindValue(QString("%1").arg(surname));
        query.addBindValue(QString("%1").arg(name));
        query.addBindValue(QString("%1").arg(title));
        query.addBindValue(QString("{\"ad_hoc_network\":\"192.168.1.%1\",\"lte_network\":\"192.168.1.%2\"}").arg(i).arg(i + 50));
        query.addBindValue("null");
        query.addBindValue("[]");
        query.addBindValue(QString("%1").arg(createDeviceNumber()));
        query.addBindValue("00:00:00:00:00:00");
        query.addBindValue(QString("%1").arg(
                    ChineseToPinyin::getPinyinFirstLetter(surname+name) + " "
                    + surname+name + " "
                    + ChineseToPinyin::getInstance()->getPinyin(surname+name) + " "
                    + getTitleStringFromTitleType(title.toInt())));
        query.addBindValue(getType(i).toInt());

        if (!query.exec())
        {
            qDebug() << "ContactsDataCreator::createData(int count) exec error"
                     << query.lastError();
        }
    }
}

void ContactsDataCreator::createApointData(int id, const QString &surname, const QString &name, const QString& ip)
{
    //首先创建表
    QSqlQuery query;
    if (!query.exec("create table contacter (id varchar(50) primary key, "
               "surname varchar(20), name varchar(20), title varchar(20), network varchar(200), "
               "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
               "bluetooth varchar(50), namePinyin varchar(40), type int)"))
    {
        qDebug() << "ContactsDataCreator::createData(int count) create table error"
                 << query.lastError();
    }

    if (!query.prepare("INSERT INTO contacter (id, surname, name, title, network, photopath, leaders, snnumber, bluetooth, namePinyin, type) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))
    {
        qDebug() << "ContactsDataCreator::createData(int count) insert data error"
                 << query.lastError();
        return;
    }

    query.addBindValue(QString("%1").arg(id));
    query.addBindValue(surname);
    query.addBindValue(name);
    query.addBindValue("1");
    query.addBindValue(QString("{\"ad_hoc_network\":\"%1\"}").arg(ip));
    query.addBindValue("null");
    query.addBindValue("[]");
    query.addBindValue(createDeviceNumber());
    query.addBindValue("00:00:00:00:00:00");
    query.addBindValue(ChineseToPinyin::getPinyinFirstLetter(surname + name) + " "
                       + surname + name + " "
                       + ChineseToPinyin::getInstance()->getPinyin(surname + name) + " "
                       + getTitleStringFromTitleType(1));
    query.addBindValue(1);

    if (!query.exec())
    {
        qDebug() << "ContactsDataCreator::createData(int count) exec error"
                 << query.lastError();
    }
}

void ContactsDataCreator::clearData()
{
    QSqlQuery query;
    query.exec("delete from contacter");
}

QString ContactsDataCreator::getTitle(int num)
{
    QString title;

    if (num % 20 == 0)
        title = "0";
    else
        title = QString("%1").arg(qrand() % 4 + 1);

    return title;
}

QString ContactsDataCreator::getType(int num)
{
    if (num % 5 == 0)
        return "2";
    return "1";
}
