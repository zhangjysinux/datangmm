#include "bluetoothMoudleService_workspace.h"
#include <QDebug>
#include <QThread>

bluetoothMoudleService_Workspace::bluetoothMoudleService_Workspace()
    : CWorkspace()
{
//    int i;

//    // Repalce the following loop code with your service function.
//    for(i = 0; i < 1000000; i++)
//    {
//        qDebug() << "This is a service process. Print count = " << i;
//        QThread::sleep(1);
//    }
}

void bluetoothMoudleService_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


