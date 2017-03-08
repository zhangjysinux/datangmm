#include "TerminalService_workspace.h"
#include <QDebug>
#include <QThread>

TerminalService_Workspace::TerminalService_Workspace()
    : CWorkspace()
{

}

void TerminalService_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


