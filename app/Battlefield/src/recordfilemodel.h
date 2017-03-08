#ifndef RECORDFILEMODEL_H
#define RECORDFILEMODEL_H

#include <QAbstractListModel>
#include <QList>

class RecordFileModel : public QAbstractListModel
{
    Q_OBJECT

    enum {
        FILE_NAME_ROLE = Qt::UserRole,
        FILE_SIZE_ROLE,
        FILE_REMOVE
    };

public:
    explicit RecordFileModel(QObject *parent = 0);
    ~RecordFileModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    void addFile(const QString &fileName, const QString &fileSize);
    void clear();
    // flag=0:video; flag=1:photo
    void remove(int flag);

signals:
    void signalRemoveFile(bool);

private:
    QList<QList<QString> > m_files;
};

#endif // RECORDFILEMODEL_H
