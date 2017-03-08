#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>

class CallHistoryCreator;
class ContactsDataCreator;
class GroupMessageCreator;
class GroupsCreator;
class LocalInformationCreator;

//此类为借口类,负责给用户或界面调用
class ViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ViewModel(QObject *parent = 0);
    ~ViewModel();

    //生成50个数据
    void startCreateData1();

    //生成500个数据
    void startCreateData2();

    //生成5000个数据
    void startCreateData3();

    //生成本地信息
    void startCreateLocalInfomation();

    //清空所有数据
    void clearAllData();

private:
    //初始化,创建对象,打开数据库
    void init();
    //清理对象,关闭数据库
    void uninit();

private:
    CallHistoryCreator *m_callHistory;
    ContactsDataCreator *m_contactsData;
    GroupMessageCreator *m_groupMessage;
    GroupsCreator *m_groups;
    LocalInformationCreator *m_localInfo;
};

#endif // VIEWMODEL_H
