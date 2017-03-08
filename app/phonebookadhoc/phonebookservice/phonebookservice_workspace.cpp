#include "phonebookservice_workspace.h"
#include <QThread>
#include <QDebug>

#include <SyberosServiceCache>
#include <QDBusConnection>
#include <QDBusError>
#include "contacterinterface.h"
#include "groupinterface.h"
#include "messagemanager.h"
#include "phonebookservice_workspace.h"
#include "callhistoryinterface.h"
#include <QDateTime>
#include "chinesetopinyin.h"
#include "bluetoothinterface.h"
#include "backgroundsyncinterface.h"
#include "connectiondatabase.h"
#include <QDir>
#include "tcpserver.h"
#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <sys/resource.h>
#include "adaptor/downloadcontactadaptor.h"
#include "adaptor/muticastadaptor.h"
#include "adaptor/fileshareadaptor.h"
#include "adaptor/contactershareadaptor.h"
#include <QDBusMetaType>
#include <csystemdeviceinfo.h>
#include <qscreen.h>
#include <QGuiApplication>
#define isServer  0

QSqlDatabase db;
phonebookservice_Workspace::phonebookservice_Workspace()
    : CWorkspace()
{

//        CSystemDeviceInfo aa;
//        QString qstr = aa.uniqueDeviceId();
//        qDebug() << "CSystemDeviceInfo" << qstr;

//        CDeviceInfo di;
//        qDebug()<<"serialNumber"<<di.serialNumber();

    //create ralate directory
    QDir dir = QDir::rootPath();
    dir.cd("home");
    dir.cd("user");
    if(dir.mkdir("messagefiles"))
    {
        qDebug() << "create files: " << dir.absolutePath() + "/messagefiles";
    }
    dir.mkdir("log");
    dir.mkdir("PhonebookPortrait");
    dir.mkdir("grouptmp");
    qDBusRegisterMetaType<Group>();
    qDBusRegisterMetaType<GroupList>();
    qRegisterMetaType<ContacterList>("ContacterList");
    qRegisterMetaType<CallHistoryList>("CallHistoryList");
    qRegisterMetaType<MessageList>("MessageList");
    qRegisterMetaType<GroupList>("GroupList");
    qRegisterMetaType<PacketData>("PacketData");
    qRegisterMetaType<GroupIps>("GroupIps");
    qDBusRegisterMetaType<IdNetworkMap>();

    ConnectionDatabase::createConnection();

    qDebug()<< "createConnection over";

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    qDebug() << "bus create-----";
    if(!bus.registerService("com.sinux.contactadhoc"))
        qDebug() << "bus error:" << bus.lastError().message();

    ContacterInterface *contacter = ContacterInterface::getInstance();
    qDebug() << "contacter create-----";
    bool contacterReg = bus.registerObject("/contacter", contacter, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterReg)
        qDebug() << "bus error:" << "register ContacterInterface false";

    GroupInterface *group = GroupInterface::getInstance();
    qDebug() << "GroupInterface create-----";
    bool groupReg = bus.registerObject("/group", group, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!groupReg)
        qDebug() << "bus error:" << "register GroupInterface false";

    MessageManager *msgMgr = MessageManager::getInstance();
    qDebug() << "MessageManager create-----";
    bool msgMgrReg = bus.registerObject("/msgmgr", msgMgr, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!msgMgrReg)
        qDebug() << "bus error:" << "register MessageManager false";

    CallHistoryInterface *history = CallHistoryInterface::getInstance();
    qDebug() << "CallHistoryInterface create-----";
    bool historyReg = bus.registerObject("/callhistory", history, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if (!historyReg)
        qDebug() << "bus error:" << "register history false";

    BluetoothInterface *bluetooth = BluetoothInterface::getInstance();
    qDebug() << "BluetoothInterface create-----";
    bool bluetoothReg = bus.registerObject("/bluetoothadaptor", bluetooth, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if (!bluetoothReg)
        qDebug() << "bus error:" << "register bluetoothadaptor1 false";

    DownloadContactAdaptor *downloadContact = DownloadContactAdaptor::getInstance();
    qDebug() << "DownloadContactAdaptor create-----";
    bool downloadReg = bus.registerObject("/downloadcontactadaptor", downloadContact, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if (!downloadReg)
        qDebug() << "bus error:" << "register downloadContact false";

    FileShareAdaptor *fileShare = FileShareAdaptor::getInstance();
    qDebug() << "FileShareAdaptor create-----";
    bool fileShareReg = bus.registerObject("/phonebookfileshare", fileShare, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if (!fileShareReg)
        qDebug() << "bus error:" << "register phonebookfileshare false";

    ContacterShareAdaptor *contacterShare = ContacterShareAdaptor::getInstance();
    qDebug() << "ContacterShareAdaptor create-----";
    bool contacterShareReg = bus.registerObject("/phonebookcontactershare", contacterShare, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if (!contacterShareReg)
        qDebug() << "bus error:" << "register phonebookcontactershare false";

//    downloadContact->onStartDownload();

//    QThread::sleep(2);
    BackgroundSyncInterface *backgroundTransfer = BackgroundSyncInterface::getInstance();
    qDebug() << "backgroundTransfer create-----";
//    bool bTransferReg = bus.registerObject("/backgroundtransferadaptor", bTransfer, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
//    if (!bTransferReg)
//        qDebug() << "bus error:" << "register backgroundtransferadaptor false";

    QObject::connect(contacter, SIGNAL(signalContacterChangedToChangeHistory(int,Contacter)),
                     history, SLOT(onContacterEmitChangeSignal(int,Contacter)));

    QObject::connect(backgroundTransfer, SIGNAL(signalContacterReceived(int,ContacterList)),
                     contacter, SLOT(onContacterReceived(int,ContacterList)));
    QObject::connect(contacter, SIGNAL(signalContacterChangedToChangeRemoteContacter(int,ContacterList)),
                     backgroundTransfer, SLOT(onSendContacters(int,ContacterList)));

    QObject::connect(backgroundTransfer, SIGNAL(signalCallHistoryReceived(int,CallHistoryList)),
                     history, SLOT(onCallHistoryReceived(int,CallHistoryList)));
    QObject::connect(history, SIGNAL(signalCallHistoryChangedToChangeRemoteCallHistory(int,CallHistoryList)),
                     backgroundTransfer, SLOT(onSendCallHistorys(int,CallHistoryList)));

    QObject::connect(backgroundTransfer, SIGNAL(signalGroupReceived(int,GroupList)),
                     group, SLOT(onGroupReceived(int,GroupList)));
    QObject::connect(group, SIGNAL(signalGroupChangedToChangeRemoteGroup(int,GroupList)),
                     backgroundTransfer, SLOT(onSendGroups(int,GroupList)));

    QObject::connect(backgroundTransfer, SIGNAL(signalGroupMessageReceived(int,MessageList)),
                     msgMgr, SLOT(onMessageReceived(int,MessageList)));
    QObject::connect(msgMgr, SIGNAL(signalMessageChangedToChangeRemoteMessage(int,MessageList)),
                     backgroundTransfer, SLOT(onSendGroupMessages(int,MessageList)));

    QObject::connect(downloadContact, SIGNAL(signalContactersDownloadFinished(ContacterList)),
                     contacter, SLOT(onContacterDownload(ContacterList)));
    QObject::connect(downloadContact, SIGNAL(signalGroupsDownloadFinished(GroupList)),
                     group, SLOT(onGroupDownload(GroupList)));
    QObject::connect(downloadContact, SIGNAL(signalGroupBattlesDownloadFinished(GroupList)),
                     group, SLOT(onGroupDownload(GroupList)));

    QObject::connect(backgroundTransfer, SIGNAL(signalDbChanged()),
                     contacter, SLOT(onDbChanged()));
    QObject::connect(backgroundTransfer, SIGNAL(signalDbChanged()),
                     history, SLOT(onDbChanged()));
    QObject::connect(backgroundTransfer, SIGNAL(signalDbChanged()),
                     group, SLOT(onDbChanged()));


    backgroundTransfer->onStart();
    qDebug() << "backgroundTransfer->onStart()";

}

void phonebookservice_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    qDebug() << "onLaunchComplete:";

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}


