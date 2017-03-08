#include "multifileshareadaptor.h"
#include <QDBusReply>
#include <QDebug>

MultiFileShareAdaptor *MultiFileShareAdaptor::instance = NULL;

MultiFileShareAdaptor *MultiFileShareAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new MultiFileShareAdaptor();
    }

    return instance;
}

MultiFileShareAdaptor::MultiFileShareAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/multifileshare", "com.sinux.DBus.multifileshare", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/multifileshare",
                       "com.sinux.DBus.multifileshare",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/multifileshare",
                       "com.sinux.DBus.multifileshare",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/multifileshare",
                       "com.sinux.DBus.multifileshare",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

MultiFileShareAdaptor::~MultiFileShareAdaptor()
{
}

void MultiFileShareAdaptor::onSetConnectType(const int &netType)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetConnectType", netType);
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onSetConnectType" ;
    }
    else
        qDebug() << "FileShareAdaptor onSetConnectType reply error:" << retValDbus.error();

    return;
}

void MultiFileShareAdaptor::onSetPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onSetPort" ;
    }
    else
        qDebug() << "FileShareAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

void MultiFileShareAdaptor::onInitServer()
{
    QDBusReply<void> retValDbus = m_interface.call("onInitServer");
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onInitServer" ;
    }
    else
        qDebug() << "FileShareAdaptor onInitServer reply error:" << retValDbus.error();

    return;
}

void MultiFileShareAdaptor::onSetAddresses(const QStringList &addresses)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetAddresses", addresses);
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onSetAddresses" ;
    }
    else
        qDebug() << "FileShareAdaptor onSetAddresses reply error:" << retValDbus.error();

    return;
}

int MultiFileShareAdaptor::onConnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onConnect");
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onConnect" << retValDbus.value();
    }
    else
        qDebug() << "FileShareAdaptor onConnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

int MultiFileShareAdaptor::onDisconnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onDisconnect" << retValDbus.value();
    }
    else
        qDebug() << "FileShareAdaptor onDisconnect reply error:" << retValDbus.error();

    return retValDbus.value();
}
bool MultiFileShareAdaptor::onIsConnected()
{
    QDBusReply<bool> retValDbus = m_interface.call("onIsConnected");
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onIsConnected" << retValDbus.value();
    }
    else
        qDebug() << "FileShareAdaptor onIsConnected reply error:" << retValDbus.error();

    return retValDbus.value();
}
int MultiFileShareAdaptor::onSendFile(const QString &fileName)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", fileName);
    if(retValDbus.isValid())
    {
        qDebug() << "FileShareAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "FileShareAdaptor onSendFile reply error:" << retValDbus.error();
    return retValDbus.value();
}

void MultiFileShareAdaptor::onSetReceivePath(const QString &path)
{
    QDBusReply<int> retValDbus = m_interface.call("onSetReceivePath", path);
    if(retValDbus.isValid())
    {
        qDebug() << "MultiFileShareAdaptor::onSetReceivePath" << retValDbus.value();
    }
    else
        qDebug() << "MultiFileShareAdaptor onSetReceivePath reply error:" << retValDbus.error();
    //return retValDbus.value();
}

void MultiFileShareAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void MultiFileShareAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void MultiFileShareAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
