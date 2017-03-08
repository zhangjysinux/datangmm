#include "pttmediainterface.h"
#include "pttmediamanager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <pjsua2.hpp>
#include <iostream>
#include <QMutex>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    //log dir
    sleep(3);
    QDir dir = QDir::rootPath();
    dir.cd("home");
    dir.cd("user");
    dir.mkdir("log");
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication a(argc, argv);

    PttMediaManager mediaMgr;

    PttMediaInterface dBus;
    qDebug() << "PTT media service start" << endl;

//    unsigned groupId = dBus.onJoinGroup("224.0.0.76", 50003);
//    dBus.onSetPttState(groupId, 1);

    int ret = a.exec();

    return ret;
}


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString logFileName = "ptt";
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
        if(!file.open(QIODevice::WriteOnly))
        {
            return;
        }
    }
    localMsg = sprintStr.toLocal8Bit();
    file.write(localMsg);
    std::cout << msg.toLocal8Bit().data() << std::endl;
    file.flush();
}
