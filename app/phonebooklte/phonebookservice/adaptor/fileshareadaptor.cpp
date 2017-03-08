#include "fileshareadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include "cnotification.h"
#include "cnotificationmanager.h"

FileShareAdaptor *FileShareAdaptor::instance = NULL;

FileShareAdaptor *FileShareAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new FileShareAdaptor();
    }

    return instance;
}

FileShareAdaptor::FileShareAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/fileshare", "com.sinux.DBus.fileshare", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/fileshare",
                       "com.sinux.DBus.fileshare",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/fileshare",
                       "com.sinux.DBus.fileshare",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/fileshare",
                       "com.sinux.DBus.fileshare",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

FileShareAdaptor::~FileShareAdaptor()
{
}

void FileShareAdaptor::onSetPort(const int &port)
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

void FileShareAdaptor::onSetAddresses(const QStringList &addresses)
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

int FileShareAdaptor::onConnect()
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

bool FileShareAdaptor::onIsConnected()
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

int FileShareAdaptor::onDisconnect()
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

int FileShareAdaptor::onSendFile(const QString &fileName)
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

void FileShareAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void FileShareAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
    if(size == maxSize)
    {
        CNotificationManager notiMgr;
        CNotification genoti;
        genoti.setTitle("共享文件");
        genoti.setSubtitle("收到1个共享文件, "+fileName);
        genoti.setAttributes(CAbstractNotification::HideDisplayTime);
        genoti.setVibrationMode(CAbstractNotification::ForceVibrationMode);
        genoti.setMarqueeText("收到1个共享文件");

        QString result;
        result = notiMgr.sendNotification(genoti);
        qDebug() <<"notiMgr.sendNotification -----------------" <<result;
    }
}

void FileShareAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
