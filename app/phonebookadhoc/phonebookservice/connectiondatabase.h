#ifndef CONNECTIONDATABASE_H
#define CONNECTIONDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSettings>
#include <QTimer>
#include "sqlfunction.h"

class ConnectionDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionDatabase(QObject *parent = 0);
    ConnectionDatabase();
    static QStringList getIp();
    static bool closeDb();
    static bool openDb();
    static QSqlDatabase& getGblDb();
    static QString getPersonSurname(int index);
    static QString getPersonName(int index);
    static QString getDeviceNum();
    static QString creatLocalPhoneInfo();
    static bool creatContacterInfo(int num);
    static bool createConnection();
    static bool loadSettingContacterInfo();
    static int getContacterPriority(QSettings *setting, QString typeStr);
    static bool loadSettingGroupInfo();
    static bool createConversionGroup();
    static void onSignalTimeOut();

private:
    static QTimer *m_timer;

};

#endif // CONNECTIONDB_H
