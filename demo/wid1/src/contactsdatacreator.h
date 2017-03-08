#ifndef CONTACTSDATACREATOR_H
#define CONTACTSDATACREATOR_H

#include "datacreator.h"

//此类创建联系人测试数据
class ContactsDataCreator : public DataCreator
{
    Q_OBJECT
public:
    ContactsDataCreator();

    //创建联系人数据,count为需要创建的数量
    virtual void createData(int count);

    //创建指定联系人数据,id为用户id,surname为姓,name为名
    void createApointData(int id, const QString &surname, const QString &name, const QString& ip);

    //删除所有数据
    virtual void clearData();

private:
    //根据数字获取称号
    QString getTitle(int num);

    QString getType(int num);
};

#endif // CONTACTSDATACREATOR_H
