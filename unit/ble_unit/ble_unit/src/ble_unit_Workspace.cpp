#include "ble_unit_Workspace.h"
#include <QDebug>
#include <QQmlContext>
#include <QQuickItem>
#include "udpsender.h"
#include "udpreceiver.h"

ble_unit_Workspace::ble_unit_Workspace()
    : CWorkspace()
{
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));


    UdpSender *sender = UdpSender::getInstance();
    UdpReceiver *receiver = UdpReceiver::getInstance();
    m_view->engine()->rootContext()->setContextProperty("sender", sender);
    m_view->engine()->rootContext()->setContextProperty("receiver", receiver);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();

    QObject *rootObject = qobject_cast<QObject *>(m_view->rootObject());
    connect(receiver, SIGNAL(signalReceive(QVariant)), rootObject, SLOT(onReceive(QVariant)));
}

void ble_unit_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


