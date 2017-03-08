#include <SyberosServiceCache>
//#include <QDBusConnection>
//#include <QDBusError>
//#include <QThread>
//#include "contacterinterface.h"
//#include "groupinterface.h"
//#include "messagemanager.h"
#include "phonebookservice_workspace.h"
//#include "callhistoryinterface.h"
//#include <QDateTime>
//#include "chinesetopinyin.h"
//#include "bluetoothinterface.h"
//#include "backgroundsyncinterface.h"
//#include "connectiondatabase.h"
//#include <QDebug>
//#include <QDir>
//#include "tcpserver.h"
//#include <QtConcurrent/QtConcurrent>
#include <iostream>
//#include <sys/resource.h>
//#include "adaptor/downloadcontactadaptor.h"
//#include "adaptor/muticastadaptor.h"
//#include "adaptor/fileshareadaptor.h"
//#include "adaptor/contactershareadaptor.h"
//#include <QDBusMetaType>
//#include <csystemdeviceinfo.h>

using namespace std;
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    int ret = SYBEROS::SyberosServiceCache::qApplication(argc, argv, new phonebookservice_Workspace());

    //db.close();

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
