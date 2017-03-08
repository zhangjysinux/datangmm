#include <SyberosServiceCache>
#include "BackupdateService_workspace.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include "fileshare.h"
#include "bluetoothinterface.h"
#include "backgroundtransfer.h"
#include "downloadcontactinterface.h"
#include "muticastinterface.h"
#include "fileshareinterface.h"
#include "groupshareinterface.h"
#include "multifileshareinterface.h"
#include "bluetoothinterface2.h"
#include "contactershareinterface.h"
#include "contacterpushinterface.h"
#include "downloadcontact.h"
#include "downloadcontactinterface.h"
#include "pointtopointinterface.h"
#include "p2ptransferinterface.h"
#include "battlecallinterface.h"
#include "messagesenderinterface.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
//    QThread::sleep(30);

    QFile file(":/parse.py");
    if(file.open(QIODevice::ReadOnly))
    {
        QFile savedFile("./parse.py");
        if(savedFile.open(QIODevice::WriteOnly))
        {
            savedFile.write(file.readAll());
            savedFile.flush();
            savedFile.close();
        }
        file.close();
    }

    {
    QFile file(":/curl");
    if(file.open(QIODevice::ReadOnly))
    {
        QFile savedFile("./curl");
        if(savedFile.open(QIODevice::WriteOnly))
        {
            savedFile.write(file.readAll());
            savedFile.flush();
            savedFile.close();
            system("chmod u+x curl");
        }
        file.close();
    }
    }

    {
    QFile file(":/python");
    if(file.open(QIODevice::ReadOnly))
    {
        QFile savedFile("./python");
        if(savedFile.open(QIODevice::WriteOnly))
        {
            savedFile.write(file.readAll());
            savedFile.flush();
            savedFile.close();
            system("chmod u+x python");
        }
        file.close();
    }
    }

    QDir dir = QDir::rootPath();
    dir.cd("home");
    dir.cd("user");
    if(dir.mkdir("receivedfiles"))
    {
        qDebug() << "create files: " << dir.absolutePath() + "/receivedfiles";
    }
    dir.mkdir("grouptmp");

    qDBusRegisterMetaType<Contacter>();
    qDBusRegisterMetaType<PhoneNumber>();
    qDBusRegisterMetaType<ContacterList>();
    qDBusRegisterMetaType<Group>();
    qDBusRegisterMetaType<GroupList>();
    qDBusRegisterMetaType<GroupIps>();
    qDBusRegisterMetaType<IdNetworkMap>();
    qRegisterMetaType<IdNetworkMap>("IdNetworkMap");

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService("com.sinux.backupdate"))
        qDebug() << "bus error:" << bus.lastError().message();

//    BluetoothInterface *bluetoothInterface = BluetoothInterface::getInstance();
//    bool bluetoothInterfaceReg = bus.registerObject("/bluetooth", bluetoothInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
//    if(!bluetoothInterfaceReg)
//        qDebug() << "bus error:" << "register BluetoothInterface false";
//    bluetoothInterface->onOpenBluetooth();

    BluetoothInterface2 *bluetoothInterface2 = BluetoothInterface2::getInstance();
    bool bluetoothInterfaceReg2 = bus.registerObject("/bluetooth", bluetoothInterface2, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!bluetoothInterfaceReg2)
        qDebug() << "bus error:" << "register BluetoothInterface false";
    bluetoothInterface2->onOpenBluetooth();
//    QThread::sleep(1);
//    bluetoothInterface->onStartFindRemoteBluetooth();

    //kill service or watch service

//    DownloadContactInterface * downloadInterface = DownloadContactInterface::getInstance();
//    bool downloadInterfaceReg = bus.registerObject("/downloadcontact", downloadInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
//    if(!downloadInterfaceReg)
//        qDebug() << "bus error:" << "register DownloadContactInterface false";
//    QTimer::singleShot(3000, downloadInterface, SLOT(onStartGet()));
//    downloadInterface->onStartDownload();

    MuticastInterface * muticastInterface = MuticastInterface::getInstance();
    bool muticastInterfaceReg = bus.registerObject("/muticast", muticastInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!muticastInterfaceReg)
        qDebug() << "bus error:" << "register MuticastInterface false";

    FileShareInterface * fileShareInterface = FileShareInterface::getInstance();
    bool fileShareInterfaceReg = bus.registerObject("/fileshare", fileShareInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!fileShareInterfaceReg)
        qDebug() << "bus error:" << "register fileShareInterface false";
    fileShareInterface->onSetPort(33245);

    GroupShareInterface * groupShareInterface = GroupShareInterface::getInstance();
    bool groupShareInterfaceReg = bus.registerObject("/groupshare", groupShareInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!groupShareInterfaceReg)
        qDebug() << "bus error:" << "register groupShareInterface false";

    groupShareInterface->onSetPort(33246);

    ContacterShareInterface * contacterShareInterface = ContacterShareInterface::getInstance();
    bool contacterShareInterfaceReg = bus.registerObject("/contactershare", contacterShareInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterShareInterfaceReg)
        qDebug() << "bus error:" << "register contacterShareInterface false";

    contacterShareInterface->onSetPort(33247);

    ContacterPushInterface * contacterPushInterface = ContacterPushInterface::getInstance();
    bool contacterPushInterfaceReg = bus.registerObject("/contacterpush", contacterPushInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterPushInterfaceReg)
        qDebug() << "bus error:" << "register contacterPushInterface false";

    contacterPushInterface->onSetPort(33248);

    PointToPointInterface * pointToPointInterface = PointToPointInterface::getInstance();
    bool pointToPointInterfaceReg = bus.registerObject("/pointtopoint", pointToPointInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!pointToPointInterfaceReg)
        qDebug() << "bus error:" << "register pointToPointInterface false";
    pointToPointInterface->onSetPort(33249);

    //p2p tcp
    MessageSenderInterface * msgInterface = MessageSenderInterface::getInstance();
    bool msgInterfaceReg = bus.registerObject("/messagesenderinterface", msgInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!msgInterfaceReg)
        qDebug() << "bus error:" << "register messagesenderinterface false";
    msgInterface->onSetPort(33250);

    BattleCallInterface *battleCallInterface = BattleCallInterface::getInstance();
    bool battleCallInterfaceReg = bus.registerObject("/battlecall", battleCallInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!battleCallInterfaceReg)
        qDebug() << "bus error:" << "register battleCallInterface false";
    battleCallInterface->onSetPort(8086);

    MultiFileShareInterface * muFileShareInterface = MultiFileShareInterface::getInstance();
    bool multiFileShareInterfaceReg = bus.registerObject("/multifileshare", muFileShareInterface, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!multiFileShareInterfaceReg)
        qDebug() << "bus error:" << "register multiFileShareInterface false";

    muFileShareInterface->onSetPort(10005);

    P2pTransferInterface *p2pTransfer = P2pTransferInterface::getInstance();
    bool p2pTransferReg = bus.registerObject("/p2pTransfer", p2pTransfer, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!p2pTransferReg)
        qDebug() << "bus error:" << "register P2pTransferInterface false";
    p2pTransfer->onSetPort(33251);

    int ret = SYBEROS::SyberosServiceCache::qApplication(argc, argv, new BackupdateService_Workspace());

    bluetoothInterface2->onDisconnect();

    return ret;
}

