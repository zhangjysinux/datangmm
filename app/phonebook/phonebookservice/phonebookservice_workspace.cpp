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
#define isServer  0

phonebookservice_Workspace::phonebookservice_Workspace()
    : CWorkspace()
{


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


