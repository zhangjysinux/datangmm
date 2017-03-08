#include "groupmessagecreator.h"
#include <QDebug>
#include <QDir>
#include <QTime>
#include <QDateTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "common.h"

GroupMessageCreator::GroupMessageCreator()
{
}

void GroupMessageCreator::createData(int count)
{
    QSqlQuery query;
    if (!query.exec(QString("create table group1000 (id varchar(50), "
               "type int, state int, userid varchar(20), groupid varchar(20), date int8, "
               "showmessage varchar(200),isread int, accessorypath varchar(50), "
               "content varchar(500), contentname varchar(50), "
               "contenttype varchar(20))")))
    {
          qDebug() << "GroupMessageCreator::createData create table" << query.lastError();
    }

    //循环插入数据
    for (int i = 0; i < count; i++)
    {
        if (i % 9 == 0)
        {
            createVideoData(query, i);
        }
        else if (i % 8 == 0)
        {
            createAudioData(query, i);
        }
        else if (i % 7 == 0)
        {
            createImageData(query, i);
        }
        else
        {
            createTextData(query, i);
        }
    }
}

void GroupMessageCreator::createTextData(QSqlQuery &query, int id)
{
    if (!query.prepare("INSERT INTO group1000 (id, type, state, userid, groupid, date,"
                       "showmessage, isread, accessorypath, content, contentname, contenttype) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))
    {
        qDebug() << "GroupMessageCreator::createTextData"
                 << query.lastError();
    }

    query.addBindValue(id);
    query.addBindValue(TEXT_MESSAGE);
    query.addBindValue(1);
    query.addBindValue(getUserId());
    query.addBindValue("1000");
    query.addBindValue(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    query.addBindValue("");
    query.addBindValue(1);
    query.addBindValue("");
    query.addBindValue(QString("this is text data: %1").arg(id));
    query.addBindValue("");
    query.addBindValue("");

    query.exec();
}

void GroupMessageCreator::createImageData(QSqlQuery &query, int id)
{
    if (!query.prepare("INSERT INTO group1000 (id, type, state, userid, groupid, date,"
                       "showmessage, isread, accessorypath, content, contentname, contenttype) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))
    {
        qDebug() << "GroupMessageCreator::createImageData"
                 << query.lastError();
    }

    query.addBindValue(id);
    query.addBindValue(IMAGE_MESSAGE);
    query.addBindValue(1);
    query.addBindValue(getUserId());
    query.addBindValue("1000");
    query.addBindValue(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    query.addBindValue("");
    query.addBindValue(1);
    query.addBindValue("");
    query.addBindValue(QString("this is image data: %1").arg(id));
    query.addBindValue(QString("file:///home/user/messagefiles/image.jpg"));
    query.addBindValue("jpg");

    query.exec();
}

void GroupMessageCreator::createAudioData(QSqlQuery &query, int id)
{
    if (!query.prepare("INSERT INTO group1000 (id, type, state, userid, groupid, date,"
                       "showmessage, isread, accessorypath, content, contentname, contenttype) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))
    {
        qDebug() << "GroupMessageCreator::createAudioData"
                 << query.lastError();
    }

    query.addBindValue(id);
    query.addBindValue(AUDIO_MESSAGE);
    query.addBindValue(1);
    query.addBindValue(getUserId());
    query.addBindValue("1000");
    query.addBindValue(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    query.addBindValue("");
    query.addBindValue(1);
    query.addBindValue("");
    query.addBindValue(QString("this is audio data: %1").arg(id));
    //query.addBindValue(QString("file:///data/data/com.sinux.phonebook/messagefiles/audio.mp3"));
    query.addBindValue(QString("file:///home/user/messagefiles/audio.mp3"));
    query.addBindValue("mp3");

    query.exec();
}

void GroupMessageCreator::createVideoData(QSqlQuery &query, int id)
{
    if (!query.prepare("INSERT INTO group1000 (id, type, state, userid, groupid, date,"
                       "showmessage, isread, accessorypath, content, contentname, contenttype) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))
    {
        qDebug() << "GroupMessageCreator::createVideoData"
                 << query.lastError();
    }

    query.addBindValue(id);
    query.addBindValue(VIDEO_MESSAGE);
    query.addBindValue(1);
    query.addBindValue(getUserId());
    query.addBindValue("1000");
    query.addBindValue(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    query.addBindValue("");
    query.addBindValue(1);
    query.addBindValue("");
    query.addBindValue(QString("this is video data: %1").arg(id));
    query.addBindValue(QString("file:///home/user/messagefiles/video.mp4"));
    query.addBindValue("mp4");

    query.exec();
}

void GroupMessageCreator::clearData()
{
    QSqlQuery query;
    query.exec("delete from group1000");
}

QString GroupMessageCreator::getUserId()
{
    qsrand(QTime::currentTime().msec());
    return QString::number((qrand() % 4) + 1);
}
