#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include "contacterinterfaceadaptor.h"
//#include "callhistoryinterfaceadaptor.h"

class ContacterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ContacterModel(QObject *parent = 0);
    void setModelList(const int &contacterType, const QString& searchText);

    QStringList m_idList;
    ContacterInterfaceAdaptor *m_contacterAdaptor;

 protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:
    void setRoles(const QStringList &names);
    QVariant getContacterValue(int index, QString key);
    int count();
    void removeContacter(QString info,int rows);

private:
    QHash<int, QByteArray> m_roleNames;
    QMap<QString, Contacter> *contacters;

};

/*
class CallHistoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CallHistoryModel(QObject *parent = 0);
    void setModelList(int type, const QString& searchText);

    QStringList m_idList;
    CallHistoryInterfaceAdaptor *m_historyAdaptor;
    ContacterInterfaceAdaptor *m_contacterAdaptor;

 protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:
    void setRoles(const QStringList &names);
    QVariant getCallHistoryValue(int index, QString key);
    int count();
    void removeCallHistory(QString info,int rows);

private:
    QHash<int, QByteArray> m_roleNames;
};
*/

#endif // TABLEMODEL_H
