#ifndef LOCALINFORMATIONCREATOR_H
#define LOCALINFORMATIONCREATOR_H

#include "datacreator.h"

//此类负责创建本机信息
class LocalInformationCreator : public DataCreator
{
    Q_OBJECT
public:
    LocalInformationCreator();

    //创建本机数据
    virtual void createData(int count);

    //删除数据
    virtual void clearData();
};

#endif // LOCALINFORMATIONCREATOR_H
