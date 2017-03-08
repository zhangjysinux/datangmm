#include "testPttUi_Workspace.h"
#include <QDebug>
#include <QQmlContext>
#include "voipserviceadaptor.h"

testPttUi_Workspace::testPttUi_Workspace()
    : CWorkspace()
{
    m_view = SYBEROS::SyberosGuiCache::qQuickView();

    VoipServiceAdaptor *adp = new VoipServiceAdaptor("com.sinux.voipWifi", this);
    m_view->rootContext()->setContextProperty("adp", adp);

    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

void testPttUi_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


