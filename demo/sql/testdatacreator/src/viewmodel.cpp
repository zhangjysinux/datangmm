#include "viewmodel.h"
#include "datacreator.h"
#include "contactsdatacreator.h"
#include "callhistorycreator.h"
#include "groupscreator.h"
#include "groupmessagecreator.h"
#include "localinformationcreator.h"


ViewModel::ViewModel(QObject *parent) :
    QObject(parent)
{
}

void ViewModel::startCreateData1()
{
    //联系人数据
    ContactsDataCreator contactsCreator;
    contactsCreator.createData(49);
    contactsCreator.createApointData(49, "李", "龙飞", "192.168.1.5");
    contactsCreator.createApointData(50, "李", "继承", "192.168.1.6");

    CallHistoryCreator callHistoryCreator;
    callHistoryCreator.createData(50);

    GroupsCreator groupsCreator;
    groupsCreator.createData(20);

    GroupMessageCreator groupMessageCreator;
    groupMessageCreator.createData(100);
}

void ViewModel::startCreateData2()
{
    ContactsDataCreator contactsCreator;
    contactsCreator.createData(499);
    contactsCreator.createApointData(499, "李", "龙飞", "192.168.1.5");
    contactsCreator.createApointData(500, "李", "继承", "192.168.1.6");

    CallHistoryCreator callHistoryCreator;
    callHistoryCreator.createData(500);

    GroupsCreator groupsCreator;
    groupsCreator.createData(20);

    GroupMessageCreator groupMessageCreator;
    groupMessageCreator.createData(500);
}

void ViewModel::startCreateData3()
{
    ContactsDataCreator contactsCreator;
    contactsCreator.createData(1999);
    contactsCreator.createApointData(1999, "李", "龙飞", "192.168.1.5");
    contactsCreator.createApointData(2000, "李", "继承", "192.168.1.6");

    CallHistoryCreator callHistoryCreator;
    callHistoryCreator.createData(2000);

    GroupsCreator groupsCreator;
    groupsCreator.createData(20);

    GroupMessageCreator groupMessageCreator;
    groupMessageCreator.createData(5000);
}

void ViewModel::startCreateLocalInfomation()
{
    LocalInformationCreator localCreator;
    localCreator.createData(0);
}

void ViewModel::clearAllData()
{
    ContactsDataCreator contactsCreator;
    contactsCreator.clearData();

    CallHistoryCreator callHistoryCreator;
    callHistoryCreator.clearData();

    GroupsCreator groupsCreator;
    groupsCreator.clearData();

    GroupMessageCreator groupMessageCreator;
    groupMessageCreator.clearData();
}
