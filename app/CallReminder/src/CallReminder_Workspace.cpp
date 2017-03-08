#include "CallReminder_Workspace.h"
#include <QDebug>
#include <QtQml>
#include "callreminderviewmodel.h"
#include "calloutviewmodel.h"

CallReminder_Workspace::CallReminder_Workspace()
    : CWorkspace()
{
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->rootContext()->setContextProperty("callReminderVm", new CallReminderViewModel());
    m_view->rootContext()->setContextProperty("callOutViewModel", new CallOutViewModel());
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

void CallReminder_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


