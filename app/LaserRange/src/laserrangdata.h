#ifndef LASERRANGDATA_H
#define LASERRANGDATA_H

#include <QObject>
#include <QSqlDatabase>

class LaserRangData : public QObject
{
    Q_OBJECT
public:
    explicit LaserRangData(QObject *parent = 0);
    ~LaserRangData();
    Q_INVOKABLE QString startAddLaserRangData();
    Q_INVOKABLE void getLaserRang(QString id);
    Q_INVOKABLE void getLaserList();

protected:
    QString transLaserDateToString(const QDateTime &date);
    QDateTime transStringToLaserDate(const QString &date);

    //添加数据
    QString addLaserRangData();

    //打开数据库
    bool openDatabase();

    //关闭数据库
    void closeDatabase();

signals:
    void signalOneLaserData(QString keyhead,QString value);
    void signalLaserHistory(QString id,QString time,QString lon,QString lat);

public slots:

private:
    QSqlDatabase m_db;
};

#endif // LASERRANGDATA_H
