#include "viewmodel.h"
#include "datacreator.h"
#include "contactsdatacreator.h"
#include "callhistorycreator.h"
#include "groupscreator.h"
#include "groupmessagecreator.h"
#include "localinformationcreator.h"


ViewModel::ViewModel(QObject *parent) :
    QObject(parent),
    m_callHistory(NULL),
    m_contactsData(NULL),
    m_groupMessage(NULL),
    m_groups(NULL),
    m_localInfo(NULL)
{
    init();
}

ViewModel::~ViewModel()
{
    uninit();
}

void ViewModel::startCreateData1()
{
//    m_contactsData->createData(49);
//    m_contactsData->createApointData(49, "李", "龙飞", "192.168.1.5");
//    m_contactsData->createApointData(50, "李", "继承", "192.168.1.6");
//    m_callHistory->createData(50);
    m_groups->createData(20);
//    m_groupMessage->createData(100);
}

void ViewModel::startCreateData2()
{
    m_contactsData->createData(499);
    m_contactsData->createApointData(499, "李", "龙飞", "192.168.1.5");
    m_contactsData->createApointData(500, "李", "继承", "192.168.1.6");
    m_callHistory->createData(500);
    m_groups->createData(20);
    m_groupMessage->createData(500);
}

void ViewModel::startCreateData3()
{
    m_contactsData->createData(1999);
    m_contactsData->createApointData(1999, "李", "龙飞", "192.168.1.5");
    m_contactsData->createApointData(2000, "李", "继承", "192.168.1.6");
    m_callHistory->createData(2000);
    m_groups->createData(20);
    m_groupMessage->createData(5000);
}

void ViewModel::startCreateLocalInfomation()
{
    m_localInfo->createData(0);
}

void ViewModel::clearAllData()
{
    m_callHistory->clearData();
    m_contactsData->clearData();
    m_groupMessage->clearData();
    m_groups->clearData();
    m_localInfo->clearData();
}

void ViewModel::init()
{
    m_callHistory = new CallHistoryCreator();
    m_contactsData = new ContactsDataCreator();
    m_groupMessage = new GroupMessageCreator();
    m_groups = new GroupsCreator();
    m_localInfo = new LocalInformationCreator();

    m_callHistory->openDatabase();
}

void ViewModel::uninit()
{
    m_callHistory->closeDatabase();

    if (m_callHistory) delete m_callHistory;
    if (m_contactsData) delete m_contactsData;
    if (m_groupMessage) delete m_groupMessage;
    if (m_groups) delete m_groups;
    if (m_localInfo) delete m_localInfo;
}
