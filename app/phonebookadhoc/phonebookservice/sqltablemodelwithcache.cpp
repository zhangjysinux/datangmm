#include "sqltablemodelwithcache.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "connectiondatabase.h"

SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase db)
    :QSqlTableModel(parent, db)
{

}

int SqlTableModel::findDataInColumn(int column, const QString& columnData)
{
    for(int i = 0, iend = rowCount(); i < iend; i++)
    {
        QModelIndex idx = index(i, column);
        if(data(idx).toString() == columnData)
        {
            return idx.row();
        }
    }
    return -1;
}

QStringList SqlTableModel::findDataInColumn(const QString& columnFieldName, const QString& columnFieldData)
{
    QStringList rowData;
    if(columnFieldName.isEmpty() || columnFieldData.isEmpty())
        return rowData;
    QString sql = QString("SELECT * FROM %1 WHERE %2='%3'")
            .arg(tableName())
            .arg(columnFieldName)
            .arg(columnFieldData);
    QSqlQuery query(ConnectionDatabase::getGblDb());
    query.exec(sql);
    if(query.next())
    {
        for(int i=0; ;i++)
        {
            if(query.value(i).isValid())
            {
                rowData.push_back(query.value(i).toString());
            }
            else
            {
                break;
            }
        }
    }
    return rowData;
}

bool SqlTableModel::updateData(const QString &key, const QStringList &values)
{
    QString getTableFieldsOrder = QString("PRAGMA table_info(%1)").arg(tableName());
    QStringList tableFields;
    QSqlQuery query(ConnectionDatabase::getGblDb());
    if(query.exec(getTableFieldsOrder))
    {
        while(query.next())
        {
            tableFields.append(query.value("name").toString());
        }
    }
    else
    {
        qDebug() << "query.exec error" << getTableFieldsOrder;
    }

    if(!tableFields.isEmpty() && values.count() == tableFields.count())
    {
        QString update = QString("UPDATE %1 SET ").arg(tableName());
        for(int i=0, count=values.count(); i<count; i++)
        {
            const QString &field = tableFields.at(i);
            const QString &value = values.at(i);
            if(!value.isNull())
            {
                update.append(QString(" %1 = '%2' ,").arg(field).arg(value));
//                if(i < count-1)
//                    update.append(QString(" %1 = '%2' ,").arg(field).arg(value));
//                else
//                    update.append(QString(" %1 = '%2' ").arg(field).arg(value));
            }
        }
        update.remove(update.count()-1, 1);
        update.append(QString(" WHERE %1 = '%2' ").arg(tableFields.value(0)).arg(key));

        if(query.exec(update))
        {
            qDebug() << "updateData SqlTableModel success----";
            return true;
        }
        else
        {
            qDebug() << "updateData SqlTableModel error----" << query.lastError().text() << update;
            return false;
        }
    }
    else
    {
        qDebug() << "updateData SqlTableModel args error";
        return false;
    }
    return true;
}

QStringList SqlTableModel::rowData(int row)
{
    QStringList rData;
    int rCOunt = rowCount();
    if(row >=0 && row < rCOunt)
    {
        for(int i=0; ;i++)
        {
            if(index(row, i).isValid())
            {
                rData.push_back(index(row, i).data().toString());
            }
            else
            {
                break;
            }
        }
    }
    return rData;
}

QStringList SqlTableModel::getColumnDataWithSql(const QString& sql, int column)
{
    QStringList columnData;
    QSqlQuery query(ConnectionDatabase::getGblDb());
    QString s = sql;
    s.replace("__tablename__", tableName());
    if(query.exec(s))
    {
        while(query.next())
        {
            columnData.push_back(query.value(column).toString());
        }
    }
    if(query.lastError().type() != QSqlError::NoError){
        qDebug() << "query exec result: " << s << query.lastError().text();
    }
    return columnData;
}

QList<QStringList> SqlTableModel::getColumnsDataWithSql(const QString& sql)
{
    QList<QStringList> columnData;
    QSqlQuery query(ConnectionDatabase::getGblDb());
    QString s = sql;
    s.replace("__tablename__", tableName());
    if(query.exec(s))
    {
        while(query.next())
        {
            QStringList rowData;
            for(int i=0; ; i++)
            {
                QVariant val = query.value(i);
                if(val.isValid())
                    rowData.append(val.toString());
                else
                    break;
            }
            columnData.append(rowData);
        }
    }
    if(query.lastError().type() != QSqlError::NoError){
        qDebug() << "query exec result: " << s << query.lastError().text();
    }
    return columnData;
}

int SqlTableModel::getMinUseableId(int column)
{
    return 0;
//    QSqlQuery query(ConnectionDatabase::getGblDb());
//    QString s = QString("SELECT id from %1 ").arg(tableName());
//    if(query.exec(s))
//    {
//        int lastId = 2000;
//        while(query.next())
//        {
//            int id = query.value(column).toInt();
//            if(id > lastId+1)
//        }
//    }
//    if(query.lastError().type() != QSqlError::NoError){
//        qDebug() << "query exec result: " << s << query.lastError().text();
//    }
}

void SqlTableModel::setQuery(const QSqlQuery &query)
{
    QSqlTableModel::setQuery(query);
}



SqlTableModelWithCache::SqlTableModelWithCache(QObject *parent, QSqlDatabase db) :
    QObject(parent),
    m_tableSqlModel(0, db)
{
}


void SqlTableModelWithCache::setTable(const QString& tableName)
{
    m_tableSqlModel.setTable(tableName);
}

void SqlTableModelWithCache::setEditStrategy(QSqlTableModel::EditStrategy strategy)
{
    m_tableSqlModel.setEditStrategy(strategy);
}

bool SqlTableModelWithCache::select()
{
    m_idRowHash.clear();
    bool flag = m_tableSqlModel.select();
    while(m_tableSqlModel.canFetchMore())
        m_tableSqlModel.fetchMore();
    if(flag)
    {
        uint rowCount = m_tableSqlModel.rowCount();
        qDebug() << "select count : " << rowCount;
        for(int i=0; i<rowCount; i++)
        {
            m_idRowHash.insert(m_tableSqlModel.index(i,0).data().toString(), i);
        }
    }
    return flag;
}

bool SqlTableModelWithCache::submitAll()
{
    bool flag = m_tableSqlModel.submitAll();

    if(m_tableSqlModel.lastError().type() != QSqlError::NoError)
    {
        qDebug() << "submitAll error:" << m_tableSqlModel.lastError();
        if(m_tableSqlModel.lastError().text().contains("unable to open database file"))
        {
            qDebug() << "unable to open database";
        }
    }
    if(flag)
    {
        select();
    }
    return flag;
}

int SqlTableModelWithCache::rowCount(const QModelIndex &parent) const
{
    return m_tableSqlModel.rowCount(parent);
}
bool SqlTableModelWithCache::insertRow(int arow, const QModelIndex &aparent)
{
    bool flag = m_tableSqlModel.insertRow(arow, aparent);
    return flag;
}

bool SqlTableModelWithCache::removeRow(int row, const QModelIndex &parent)
{
    bool flag = m_tableSqlModel.removeRow(row, parent);
    if(flag)
    {
        QString id = m_tableSqlModel.index(row, 0).data().toString();
        m_idRowHash.remove(id);
    }
    return flag;
}

QModelIndex SqlTableModelWithCache::index(int row, int column, const QModelIndex &parent) const
{
    return m_tableSqlModel.index(row, column, parent);
}

QStringList SqlTableModelWithCache::rowData(int row)
{
    return m_tableSqlModel.rowData(row);
}

QVariant SqlTableModelWithCache::data(const QModelIndex &idx, int role) const
{
    return m_tableSqlModel.data(idx, role);
}

bool SqlTableModelWithCache::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool flag = m_tableSqlModel.setData(index, value, role);
    if(flag)
    {
        if(index.column() == 0)
        {
            QString id = value.toString();
            if(m_idRowHash.contains(id))
                m_idRowHash.remove(id);

            m_idRowHash.insert(id, index.row());
        }
    }
    return flag;
}

int SqlTableModelWithCache::findDataInColumn(int column, const QString& data)
{
    for(int i = 0, iend = m_tableSqlModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_tableSqlModel.index(i, column);
        if(m_tableSqlModel.data(index).toString() == data)
        {
            return index.row();
        }
    }
    return -1;
//    return m_idRowHash.value(data, -1);
}

QStringList SqlTableModelWithCache::findDataInColumn(const QString& columnFieldName, const QString& data)
{
    int row = m_idRowHash.value(data);
    QStringList rowD = m_tableSqlModel.rowData(row);
    return rowD;
}

QStringList SqlTableModelWithCache::getColumnDataWithSql(const QString& sql, int column)
{
    return m_tableSqlModel.getColumnDataWithSql(sql, column);
}

QList<QStringList> SqlTableModelWithCache::getColumnsDataWithSql(const QString& sql)
{
    return m_tableSqlModel.getColumnsDataWithSql(sql);
}

void SqlTableModelWithCache::clear()
{
    m_tableSqlModel.clear();
    m_idRowHash.clear();
}

int SqlTableModelWithCache::getMinUseableId(int column)
{
    m_tableSqlModel.getMinUseableId(column);
}
