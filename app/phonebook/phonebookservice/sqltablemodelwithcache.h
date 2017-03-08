#ifndef SQLTABLEMODELWITHCACHE_H
#define SQLTABLEMODELWITHCACHE_H

#include <QObject>
#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
public:
    SqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    void setQuery(const QSqlQuery &query);
    //find the column = data by compare, return model row
    int findDataInColumn(int column, const QString& columnData);
    //find the column base on fieldname = data by exec sql
    QStringList findDataInColumn(const QString& columnFieldName, const QString& columnFieldData);
    //update
    bool updateData(const QString &key, const QStringList &values);

    //get one row data and return list
    QStringList rowData(int row);
    //get column data with sql
    QStringList getColumnDataWithSql(const QString& sql, int column=0);
    //get columns data with sql
    QList<QStringList> getColumnsDataWithSql(const QString& sql);

    int getMinUseableId(int column);
};

class SqlTableModelWithCache : public QObject
{
    Q_OBJECT
public:
    explicit SqlTableModelWithCache(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    void setTable(const QString& tableName);
    void setEditStrategy(QSqlTableModel::EditStrategy strategy);
    bool select();
    bool submitAll();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRow(int arow, const QModelIndex &aparent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    //find the column = data by compare, return model row
    int findDataInColumn(int column, const QString& data);
    //find the column base on fieldname = data by exec sql
    QStringList findDataInColumn(const QString& columnFieldName, const QString& data);
    //get one row data and return list
    QStringList rowData(int row);
    //get column data with sql
    QStringList getColumnDataWithSql(const QString& sql, int column=0);
    //get columns data with sql
    QList<QStringList> getColumnsDataWithSql(const QString& sql);
    int getMinUseableId(int column);
    void clear();

private:
    SqlTableModel m_tableSqlModel;
    QHash<QString, int> m_idRowHash;
signals:

public slots:

};

#endif // SQLTABLEMODELWITHCACHE_H
