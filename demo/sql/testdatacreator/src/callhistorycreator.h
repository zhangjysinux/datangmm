#ifndef CALLHISTORYCREATOR_H
#define CALLHISTORYCREATOR_H

#include "datacreator.h"

//此类负责生成通话记录数据
class CallHistoryCreator : public DataCreator
{
    Q_OBJECT
public:
    CallHistoryCreator();

    //创建通话记录,count为需要创建的数量
    virtual void createData(int count);

    //清空通话记录数据
    virtual void clearData();

private:
    QString getDialType(int index);
};

#endif // CALLHISTORYCREATOR_H
