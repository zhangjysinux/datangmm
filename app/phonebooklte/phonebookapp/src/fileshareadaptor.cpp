#include "fileshareadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include "../../../commondata/datatype/commondata.h"


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
    m_interface(contacterServiceName, "/phonebookfileshare", "com.sinux.DBus.phonebookfileshare", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect(contacterServiceName,
                       "/phonebookfileshare",
                       "com.sinux.DBus.phonebookfileshare",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect(contacterServiceName,
                       "/phonebookfileshare",
                       "com.sinux.DBus.phonebookfileshare",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect(contacterServiceName,
                       "/phonebookfileshare",
                       "com.sinux.DBus.phonebookfileshare",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

FileShareAdaptor::~FileShareAdaptor()
{
}

void FileShareAdaptor::setPort(const int &port)
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

void FileShareAdaptor::setAddresses(const QStringList &addresses)
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

int FileShareAdaptor::connect()
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

int FileShareAdaptor::disconnect()
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

int FileShareAdaptor::sendFile(const QString &fileName)
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
}

void FileShareAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
