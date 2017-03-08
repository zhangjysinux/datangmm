#ifndef _GROUP_FILE_SEND_MODEL_H_
#define _GROUP_FILE_SEND_MODEL_H_
#include <QString>
#include <QObject>
#include "groupinterfaceadaptor.h"
#include "contacterinterfaceadaptor.h"
#include "muticastadaptor.h"
class RecordFileModel;
class GroupFileSendModel:public QObject
{
    Q_OBJECT
public:
    explicit GroupFileSendModel(QObject *parent = 0);
    ~GroupFileSendModel();
    Q_INVOKABLE void getGroups(bool bIsDelete);
    Q_INVOKABLE void SetGroupId(QString groupId);
    Q_INVOKABLE void SendFile();
    void setRecordFileModel(RecordFileModel* recordFileModel);
public slots:
    void onGroupChanged(int type, Group group);
signals:
    void signalGroupChanged(int type,QString id,QString name,QString network, QString num);
    void signalSendGroupInfoToGroup(QString id,QString name, QString network, QString num);
    //void signalSendGroupInfoToGroup(QString id,QString name, QString network, QString num);
private:
    void GetOtherMember(QString groupId);
    GroupInterfaceAdaptor* m_groupAdaptor;
    ContacterInterfaceAdaptor *m_adaptor;
    MutiCastAdaptor *m_pMutiCastAdaptor;
    QStringList members;
    QStringList leaders;
    Group m_groupInfo;
    QStringList m_remoteAddress;
    QString m_localId;
    int m_groupId;
    RecordFileModel *m_recordFileModel;
};
#endif
