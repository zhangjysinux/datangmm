#ifndef DATACREATOR_H
#define DATACREATOR_H

#include <QObject>
#include <QSqlDatabase>

//此类为创建测试数据的基类,主要包含一些公用的函数,创建测试数据由具体子类实现
class DataCreator : public QObject
{
    Q_OBJECT
public:
    explicit DataCreator(QObject *parent = 0);
    virtual ~DataCreator();

    //创建数据借口,功能由具体实现类的createData实现
    virtual void createData(int count) = 0;

    //删除数据
    virtual void clearData() = 0;

    //打开数据库
    bool openDatabase();

    //关闭数据库
    void closeDatabase();

protected:
    //创建性
    QString createSurname();

    //创建名字
    QString createName();

    //创建设备号
    QString createDeviceNumber();

    //通过枚举获取title的字符串
    QString getTitleStringFromTitleType(int titleType);

    //创建分组的名
    QString createGroupName(int index);

private:
    QSqlDatabase m_db;
};

#endif // DATACREATOR_H
