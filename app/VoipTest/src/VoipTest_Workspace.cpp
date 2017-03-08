#include "VoipTest_Workspace.h"
#include "voipserviceadaptor.h"
#include <QDebug>
#include "voipserviceadaptor.h"
#include <QtQml>

VoipTest_Workspace::VoipTest_Workspace()
    : CWorkspace()
{
    //qmlRegisterType<VoipServiceAdaptor>("com.sinux.qml.voip",1,0,"VoipAPI");
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    VoipServiceAdaptor *voip = new VoipServiceAdaptor;
    m_view->rootContext()->setContextProperty("voip",voip);
    m_view->setSource(QUrl("qrc:/qml/Call.qml"));
    m_view->showFullScreen();
}

void VoipTest_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


