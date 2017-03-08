#include "recordfilemodel.h"
#include <QFile>
#include <QDebug>

RecordFileModel::RecordFileModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

RecordFileModel::~RecordFileModel()
{

}

int RecordFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_files.size();
}

QVariant RecordFileModel::data(const QModelIndex &index, int role) const
{
    return m_files.at(index.row()).at(role - Qt::UserRole);
}

QHash<int, QByteArray> RecordFileModel::roleNames() const
{
    QHash<int,QByteArray> roleNames;
    roleNames.insert(FILE_NAME_ROLE, "fileName");
    roleNames.insert(FILE_SIZE_ROLE, "size");
    roleNames.insert(FILE_REMOVE, "remove");

    return roleNames;
}

bool RecordFileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == FILE_REMOVE)
    {
        m_files[index.row()][2] = value.toString();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void RecordFileModel::addFile(const QString &fileName, const QString &fileSize)
{
    for (QList<QList<QString> >::iterator it = m_files.begin(); it != m_files.end(); it++)
    {
        if (it->first() == fileName)
            return;
    }

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));

    QList<QString> fileInfo;
    fileInfo.append(fileName);
    fileInfo.append(fileSize);
    fileInfo.append("0");

    m_files.append(fileInfo);

    endInsertRows();
}

void RecordFileModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
    m_files.clear();
    endRemoveRows();
}

void RecordFileModel::remove(int flag)
{
    int index = 0;
    for (QList<QList<QString> >::iterator it = m_files.begin(); it != m_files.end();)
    {
        if (it->at(2) == "1")
        {
            beginRemoveRows(QModelIndex(), index, index);

            QString removePath;
            if (flag == 0)
            {
                removePath = QString("/data/data/com.sinux.battlefield/videos/%1").
                        arg(it->at(0));
            }
            else
            {
                removePath = QString("/data/data/com.sinux.battlefield/images/%1").
                        arg(it->at(0));
            }

            if (!QFile::remove(removePath))
                emit signalRemoveFile(false);
            else
                emit signalRemoveFile(true);

            it = m_files.erase(it);

            endRemoveRows();
        }
        else
        {
            it++;
            index++;
        }
    }
}
