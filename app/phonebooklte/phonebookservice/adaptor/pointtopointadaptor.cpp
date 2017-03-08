#include "pointtopointadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QUuid>

PointToPointAdaptor *PointToPointAdaptor::instance = NULL;

PointToPointAdaptor *PointToPointAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new PointToPointAdaptor();
    }

    return instance;
}

PointToPointAdaptor::PointToPointAdaptor(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_interface("com.sinux.backupdate", "/pointtopoint", "com.sinux.DBus.pointtopoint", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/pointtopoint",
                       "com.sinux.DBus.pointtopoint",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/pointtopoint",
                       "com.sinux.DBus.pointtopoint",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/pointtopoint",
                       "com.sinux.DBus.pointtopoint",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

PointToPointAdaptor::~PointToPointAdaptor()
{
}

qint64 PointToPointAdaptor::sendMessage(const Message &message)
{
    onSendMessage(message);
}

void PointToPointAdaptor::onSetPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "PointToPointAdaptor::onSetPort" ;
    }
    else
        qDebug() << "PointToPointAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

void PointToPointAdaptor::onSetAddresses(const QStringList &addresses)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetAddresses", addresses);
    if(retValDbus.isValid())
    {
        qDebug() << "PointToPointAdaptor::onSetAddresses" ;
    }
    else
        qDebug() << "PointToPointAdaptor onSetAddresses reply error:" << retValDbus.error();

    return;
}

int PointToPointAdaptor::onConnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onConnect");
    if(retValDbus.isValid())
    {
        qDebug() << "PointToPointAdaptor::onConnect" << retValDbus.value();
    }
    else
        qDebug() << "PointToPointAdaptor onConnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

bool PointToPointAdaptor::onIsConnected()
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


int PointToPointAdaptor::onDisconnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        qDebug() << "PointToPointAdaptor::onDisconnect" << retValDbus.value();
    }
    else
        qDebug() << "PointToPointAdaptor onDisconnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

int PointToPointAdaptor::onSendFile(const QString &fileName)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", fileName);
    if(retValDbus.isValid())
    {
        qDebug() << "PointToPointAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "PointToPointAdaptor onSendFile reply error:" << retValDbus.error();

    return retValDbus.value();
}

int PointToPointAdaptor::onSendMessage(const Message &message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;

    if(data.isEmpty())
        qDebug() << "onSendMessage to datastream error---";

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

void PointToPointAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void PointToPointAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    if(size == maxSize)
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            qDebug() << "file open success: " << file.fileName();
            Message msg;
            QByteArray data = file.readAll();
            QDataStream in(&data, QIODevice::ReadOnly);
            in >> msg;
            qDebug() << "onSignalFileShareReceivePercent contacter: " << msg.common.id << msg.common.groupId << msg.content;

            file.close();
            QFile::remove(fileName);

            emit signalNewMessage(msg);
        }
        else
        {
            qDebug() << "file open error: " << file.errorString();
        }
    }
    emit signalReceivePercent(fileName, size, maxSize);
}

void PointToPointAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    if(size == maxSize)
        QFile::remove(fileName);
    emit signalSendPercent(fileName, size, maxSize);
}
