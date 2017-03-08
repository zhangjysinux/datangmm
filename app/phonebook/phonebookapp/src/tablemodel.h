#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include "contacterinterfaceadaptor.h"
#include "callhistoryinterfaceadaptor.h"
#include "messagemanageradaptor.h"

class ContacterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ContacterModel(QObject *parent = 0);
    void setModelList(const int &netType, const int &contacterType,
                      const QString& searchText, QStringList memberList);
    void setModelList(const int &netType, const int &contacterType,
                      const QString& searchText);

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
    QString getContacterName(QString contacterId);

    int count();
    void removeContacter(QString info,int rows);
    int getCurrentIndex(QString info);
    //bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QHash<int, QByteArray> m_roleNames;
    QMap<QString, Contacter> *contacters;

};

typedef QPair<int, QString> oneList;
typedef QList<oneList> historyList;

class CallHistoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CallHistoryModel(QObject *parent = 0);
    void setModelList(int type, const QString& searchText);

    historyList m_idList;
    CallHistoryInterfaceAdaptor *m_historyAdaptor;
    ContacterInterfaceAdaptor *m_contacterAdaptor;

 protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:
    void signalMeetingStart(QString startTime);
    void signalMeetingEnd(QString endTime);
    void signalMeetingMember(QString id);

public slots:
    void setRoles(const QStringList &names);
    QVariant getCallHistoryValue(int index, QString key);
    void getMeetingTime(QString historyId);
    int count();
    void removeTheCallHistory(QString info,int rows);

private:
    QHash<int, QByteArray> m_roleNames;
};

class MessageModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MessageModel(QObject *parent = 0);
    void setModelList(QString groupId);

    QStringList m_idList;
    MessageManagerAdaptor *m_messageAdaptor;
    ContacterInterfaceAdaptor *m_contacterAdaptor;

 protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:
    void signalFindImage(QString sources);
    void signalFreshEnd();
public slots:

    void setRoles(const QStringList &names);
    QVariant getMessageValue(int index, QString key);
    //int count();
    void removeMessage(QString info,int rows);
    void getImageList();
    int getCount();
    //int getCurrentIndex(QString info);

private:
    QString m_groupID;
    QHash<int, QByteArray> m_roleNames;
    QMap<QString, Message> *messages;

};

#endif // TABLEMODEL_H
