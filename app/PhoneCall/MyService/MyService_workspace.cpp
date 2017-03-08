#include "MyService_workspace.h"
#include <QDebug>
#include <QThread>
#include <QtQml>

MyService_Workspace::MyService_Workspace()
    : CWorkspace()
{
    csDbus = DialInterface::getInstance();
    connect(csDbus,SIGNAL(signalStartApp(QString,QString)),this,SLOT(onCallHandlerIdChanged(QString,QString)));
    connect(csDbus,SIGNAL(signalStartMake(int,QString)),this,SLOT(onStartMake(int,QString)));
    connect(csDbus,SIGNAL(signalVideoCallInComming(QString,QString)),SLOT(onStartInCommingVideo(QString,QString)));
}

void MyService_Workspace::setDisplayState(bool state)
{
    if (state) {
        powerManager.setDisplayState(powerManager.DisplayOn); //控制点亮屏幕
    } else {
        powerManager.setDisplayState(powerManager.DisplayOff); //关闭屏幕
    }
}

void MyService_Workspace::onCallHandlerIdChanged(QString dialingNumber, QString handlerId)
{
    qDebug()<<"onCallHandlerIdChanged  in service";
//    bool isIncoming = csDbus->isIncoming(handlerId);
//    if (isIncoming)
//    {
        SYBEROS::CServiceApplication * app = static_cast<SYBEROS::CServiceApplication *>(QCoreApplication::instance());
        QString url = QString("sinuxtel:callInComing?value=%1&remoteIpAddr=%2").arg(handlerId).arg(dialingNumber);
        app->openUrl(url);
        setDisplayState(true);
        qDebug()<<"myurl = "<<url;
        // }
}

void MyService_Workspace::onStartMake(int net, QString phoneNum)
{
    SYBEROS::CServiceApplication * app = static_cast<SYBEROS::CServiceApplication *>(QCoreApplication::instance());
    QString url = QString("sinuxtel:startMake?net=%1&phoneNum=%2").arg(net).arg(phoneNum);
    app->openUrl(url);
}

void MyService_Workspace::onStartInCommingVideo(QString callId, QString remoteIpAddr)
{
    SYBEROS::CServiceApplication * app = static_cast<SYBEROS::CServiceApplication *>(QCoreApplication::instance());
    QString url = QString("sinuxtel:startVideoIn?callId=%1&phoneNum=%2").arg(callId).arg(remoteIpAddr);
    app->openUrl(url);
    setDisplayState(true);
}

void MyService_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)
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


