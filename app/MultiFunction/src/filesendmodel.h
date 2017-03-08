#ifndef _FILE_SEND_MODEL_H_
#define _FILE_SEND_MODEL_H_
#include <QObject>
#include <vector>
#include <QString>
class MultiFileShareAdaptor;
class RecordFileModel;
class FileSendModel:public QObject
{
    Q_OBJECT
public:
    explicit FileSendModel(QObject *parent = 0);
    ~FileSendModel();
    void init();
    void setRecordFileModel(RecordFileModel* recordFileModel);
    Q_INVOKABLE bool sendFile();
    Q_INVOKABLE void setRemoteIp(const QString &remoteIp);
private:
    MultiFileShareAdaptor* m_multFileShare;
    RecordFileModel *m_recordFileModel;
    QString m_remIp;
};
#endif
