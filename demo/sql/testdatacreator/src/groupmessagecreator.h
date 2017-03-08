#ifndef GROUPMESSAGECREATOR_H
#define GROUPMESSAGECREATOR_H

#include "datacreator.h"
#include <QtSql/QSqlQuery>

//此类负责创建群组消息
class GroupMessageCreator : public DataCreator
{
public:
    GroupMessageCreator();

    //创建群组消息,暂时创建在group1000群里面
    virtual void createData(int count);

    //删除所有群组消息
    virtual void clearData();

private:
    void createTextData(QSqlQuery &query, int id);
    void createImageData(QSqlQuery &query, int id);
    void createAudioData(QSqlQuery &query, int id);
    void createVideoData(QSqlQuery &query, int id);

    QString getUserId();
};

#endif // GROUPMESSAGECREATOR_H
