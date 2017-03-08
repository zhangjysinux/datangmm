#include "CallmanagerService_workspace.h"
#include <QDebug>
#include <QThread>

CallmanagerService_Workspace::CallmanagerService_Workspace()
    : CWorkspace()
{

}

void CallmanagerService_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


