#include "groupshareadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QUuid>

GroupShareAdaptor *GroupShareAdaptor::instance = NULL;

GroupShareAdaptor *GroupShareAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new GroupShareAdaptor();
    }

    return instance;
}

GroupShareAdaptor::GroupShareAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/groupshare", "com.sinux.DBus.groupshare", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/groupshare",
                       "com.sinux.DBus.groupshare",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/groupshare",
                       "com.sinux.DBus.groupshare",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/groupshare",
                       "com.sinux.DBus.groupshare",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

GroupShareAdaptor::~GroupShareAdaptor()
{
}

void GroupShareAdaptor::onSetPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupShareAdaptor::onSetPort" ;
    }
    else
        qDebug() << "GroupShareAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

void GroupShareAdaptor::onSetAddresses(const QStringList &addresses)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetAddresses", addresses);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupShareAdaptor::onSetAddresses" << addresses;
    }
    else
        qDebug() << "GroupShareAdaptor onSetAddresses reply error:" << retValDbus.error();

    return;
}

int GroupShareAdaptor::onConnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onConnect");
    if(retValDbus.isValid())
    {
        qDebug() << "GroupShareAdaptor::onConnect" << retValDbus.value();
    }
    else
        qDebug() << "GroupShareAdaptor onConnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

bool GroupShareAdaptor::onIsConnected()
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


int GroupShareAdaptor::onDisconnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        qDebug() << "GroupShareAdaptor::onDisconnect" << retValDbus.value();
    }
    else
        qDebug() << "GroupShareAdaptor onDisconnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

int GroupShareAdaptor::onSendFile(const QString &fileName)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", fileName);
    if(retValDbus.isValid())
    {
        qDebug() << "GroupShareAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "GroupShareAdaptor onSendFile reply error:" << retValDbus.error();

    return retValDbus.value();
}

int GroupShareAdaptor::onSendGroup(const Group &group)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << group;

    if(data.isEmpty())
        qDebug() << "onSendGroup Group to datastream error---";

    QString fileName = "/home/user/grouptmp/groupdata"+QUuid::createUuid().toString().remove("{").remove("}").remove("-")+".dat";
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open success: " << file.fileName();
        file.write(data);
        file.flush();
        file.close();

        //file send
        onSendFile(fileName);
    }
    else
    {
        qDebug() << "file open error: " << file.errorString();
    }
}

void GroupShareAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void GroupShareAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    if(size == maxSize)
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            qDebug() << "file open success: " << file.fileName();
            Group group;
            QByteArray data = file.readAll();
            QDataStream in(&data, QIODevice::ReadOnly);
            in >> group;
            qDebug() << "onSignalFileShareReceivePercent group: " << group.id << group.name << group.address << group.port;

            file.close();

            emit signalGroupReceived(group);
        }
        else
        {
            qDebug() << "file open error: " << file.errorString();
        }
    }
    emit signalReceivePercent(fileName, size, maxSize);
}

void GroupShareAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
