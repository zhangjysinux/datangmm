#include <SyberosServiceCache>
#include <iostream>
#include "phonebookservice_workspace.h"

#include <QDBusConnection>
#include <QDBusError>
#include <QThread>
#include "contacterinterface.h"
#include "groupinterface.h"
#include "messagemanager.h"
#include "callhistoryinterface.h"
#include <QDateTime>
#include "chinesetopinyin.h"
#include "bluetoothinterface.h"
#include "backgroundsyncinterface.h"
#include "connectiondatabase.h"
#include <QDebug>
#include <QDir>
#include "tcpserver.h"
#include <QtConcurrent/QtConcurrent>
#include <sys/resource.h>
#include "adaptor/downloadcontactadaptor.h"
#include "adaptor/muticastadaptor.h"
#include "adaptor/fileshareadaptor.h"
#include "adaptor/contactershareadaptor.h"
#include <QDBusMetaType>
#include <csystemdeviceinfo.h>

using namespace std;
QSqlDatabase db;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);


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
        if(!bus.registerService("com.sinux.contact"))
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

    int ret = SYBEROS::SyberosServiceCache::qApplication(argc, argv, new phonebookservice_Workspace());

    db.close();

    return ret;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString logFileName = "phonebook";
    logFileName .append( QDateTime::currentDateTime().toString("yyyy_MM") )
                .append( ".log" );
    static QFile file("/home/user/log/" + logFileName);
    QByteArray localMsg = msg.toLocal8Bit();
    localMsg.append(QDateTime::currentDateTime().toString("(yyyy-MM-dd hh:mm:ss.zzz)"));
    QString sprintStr;
    switch (type) {
    case QtDebugMsg:
        sprintStr.sprintf("---Debug: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        sprintStr.sprintf("---Warning: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        sprintStr.sprintf("---Critical: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        sprintStr.sprintf("---Fatal: %s (%s:%u, %s)\r\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
    if(!file.isOpen())
    {
        if(!file.open(QIODevice::WriteOnly))
        {
            return;
        }
    }
    localMsg = sprintStr.toLocal8Bit();
    file.write(localMsg);
    cout << msg.toLocal8Bit().data() << endl;
    file.flush();
}
