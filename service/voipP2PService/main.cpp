#include "voipdbusdata.h"
#include "voipinterface.h"
#include <SDL2/SDL.h>
#include <QCoreApplication>
#include <QDebug>
#include <pjsua2.hpp>
#include <pjsua.h>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QMutex>

#include <sys/shm.h>
using namespace pj;
VoipInterface *service;

extern pthread_mutex_t m_lock;
extern pthread_cond_t cond;

void createPath(const QString &path);
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    //log dir
    QDir dir = QDir::rootPath();
    dir.cd("home");
    dir.cd("user");
    dir.mkdir("log");
    createPath("/home/user/sinux/voicerecords/");
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication a(argc, argv);

    //register type
    qRegisterMetaType<pj::OnIncomingCallParam>("OnIncomingCallParam");

    VoipInterface interface;
#ifdef voipWifiP2PService
    interface.onInitialize(30000, 30001, 100);
#elif voipAdHocP2PService
    interface.onInitialize(31000, 31001, 100);
#elif voipLteP2PService
    interface.onInitialize(32000, 32001, 100);
#endif
    qDebug() << "VOIP service start" << endl;

    int ret = a.exec();

    return ret;
}


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString logFileName = "adhocp2p";
    logFileName .append( QDateTime::currentDateTime().toString("yyyy_MM") )
                .append( ".log" );
    static QFile file("/home/user/log/" + logFileName);
    QByteArray localMsg = msg.toLocal8Bit();
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
        if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            return;
        }
    }
    localMsg = sprintStr.toLocal8Bit();
    file.write(localMsg);
    std::cout << msg.toLocal8Bit().data() << std::endl;
    file.flush();
}

void createPath(const QString &path)
{
    QStringList pathList = path.split("/", QString::SkipEmptyParts);
    QDir dir = QDir::rootPath();
    foreach(const QString &path, pathList)
    {
        if(dir.exists(path))
        {
            dir.cd(path);
        }
        else
        {
            if(dir.mkdir(path))
            {
                qDebug() << "create folder: " << dir.absolutePath() + "/" + path;
            }
        }
    }
}
