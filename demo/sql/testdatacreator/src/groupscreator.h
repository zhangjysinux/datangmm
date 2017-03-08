#ifndef GROUPSCREATOR_H
#define GROUPSCREATOR_H

#include "datacreator.h"

//此类负责创建群组
class GroupsCreator : public DataCreator
{
    Q_OBJECT
public:
    GroupsCreator();

    //创建群组,count为需要创建的数量
    virtual void createData(int count);

    //删除所有群组
    virtual void clearData();
};

#endif // GROUPSCREATOR_H
