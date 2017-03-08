#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "connection.h"
#include "messagemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    createConnection();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    MessageManager *msgManager = MessageManager::getInstance();
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("msgmanager", msgManager);
    msgManager->setQMLObject(engine.rootObjects().first());

    return app.exec();
}

