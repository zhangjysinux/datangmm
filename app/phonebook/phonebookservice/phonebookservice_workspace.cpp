#include "phonebookservice_workspace.h"
#include <QThread>
#include <QDebug>

//#include <SyberosServiceCache>
//#include <QDBusConnection>
//#include <QDBusError>
//#include "contacterinterface.h"
//#include "groupinterface.h"
//#include "messagemanager.h"
//#include "phonebookservice_workspace.h"
//#include "callhistoryinterface.h"
//#include <QDateTime>
//#include "chinesetopinyin.h"
//#include "bluetoothinterface.h"
//#include "backgroundsyncinterface.h"
//#include "connectiondatabase.h"
//#include <QDir>
//#include "tcpserver.h"
//#include <QtConcurrent/QtConcurrent>
//#include <iostream>
//#include <sys/resource.h>
//#include "adaptor/downloadcontactadaptor.h"
//#include "adaptor/muticastadaptor.h"
//#include "adaptor/fileshareadaptor.h"
//#include "adaptor/contactershareadaptor.h"
//#include <QDBusMetaType>
//#include <csystemdeviceinfo.h>
//#include <qscreen.h>
//#include <QGuiApplication>

#include <cgui_application.h>
#define isServer  0
#define BodyNetServiceName "com.coi208.bodynetservice"
#define BodyNetPath "/Service"
#define BodyNetDbus "local.BodyNetService"

phonebookservice_Workspace::phonebookservice_Workspace()
    : CWorkspace()
{
//    m_interface = new QDBusInterface(BodyNetServiceName, BodyNetPath, BodyNetDbus, QDBusConnection::sessionBus(),this);

    m_bodyNetInterface = new LocalBodyNetServiceInterface(BodyNetServiceName,BodyNetPath,QDBusConnection::sessionBus(),this);
    connect(m_bodyNetInterface,SIGNAL(SignalShortCutKey(int)),this,SLOT(onSignalShortCutKey(int)));

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
/*******************************
 * 1--跳转至态势信息      03
 * 2--跳转至视频显示软件   04
 * 3--打开未读消息        07
 * 4--关闭当前消息查看界面  08
 * 5--截图（静默拍照）     05
 * 6--截图并发送给班长     06
 * 7--开始通话            09
 * 8--结束通话            0A
**********************************/
void phonebookservice_Workspace::onSignalShortCutKey(int keyValue)
{

    qDebug() << "onSignalShortCutKey" << keyValue;
    switch (keyValue) {
    case 1:
        qApp->openUrl("CommandApp:openSituation");
        break;
    case 2:
        qApp->openUrl("sinuxbook:openCamera");
        break;
    case 3:
        qApp->openUrl("sinuxbook:initP2pGroupRoom");
        break;
    case 4:
        qApp->openUrl("sinuxbook:quitGroupRoom");
        break;
    case 5:
        qApp->openUrl("sinuxbook:imageCapture");
        break;
    case 6:
        qApp->openUrl("sinuxbook:imageCaptureAndSendP2P");
        break;
    case 7:
        qApp->openUrl("sinuxbook:initGroupRoomAndStartPtt?groupId=200100000");
        break;
    case 8:
        qApp->openUrl("sinuxbook:initGroupRoomAndStopPtt?groupId=200100000");
        break;
    default:
        break;
    }
}


