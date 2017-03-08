#include "contacterpushadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QUuid>

ContacterPushAdaptor *ContacterPushAdaptor::instance = NULL;

ContacterPushAdaptor *ContacterPushAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new ContacterPushAdaptor();
    }

    return instance;
}

ContacterPushAdaptor::ContacterPushAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/contacterpush", "com.sinux.DBus.contacterpush", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/contacterpush",
                       "com.sinux.DBus.contacterpush",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/contacterpush",
                       "com.sinux.DBus.contacterpush",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/contacterpush",
                       "com.sinux.DBus.contacterpush",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

ContacterPushAdaptor::~ContacterPushAdaptor()
{
}

void ContacterPushAdaptor::onSetPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterPushAdaptor::onSetPort" ;
    }
    else
        qDebug() << "ContacterPushAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

void ContacterPushAdaptor::onSetAddresses(const QStringList &addresses)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetAddresses", addresses);
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterPushAdaptor::onSetAddresses" ;
    }
    else
        qDebug() << "ContacterPushAdaptor onSetAddresses reply error:" << retValDbus.error();

    return;
}

int ContacterPushAdaptor::onConnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onConnect");
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterPushAdaptor::onConnect" << retValDbus.value();
    }
    else
        qDebug() << "ContacterPushAdaptor onConnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

bool ContacterPushAdaptor::onIsConnected()
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


int ContacterPushAdaptor::onDisconnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterPushAdaptor::onDisconnect" << retValDbus.value();
    }
    else
        qDebug() << "ContacterPushAdaptor onDisconnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

int ContacterPushAdaptor::onSendFile(const QString &fileName)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", fileName);
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterPushAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "ContacterPushAdaptor onSendFile reply error:" << retValDbus.error();

    return retValDbus.value();
}

int ContacterPushAdaptor::onSendContacter(const Contacter &contacter)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << contacter;

    if(data.isEmpty())
        qDebug() << "onSendContacter to datastream error---";

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

void ContacterPushAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void ContacterPushAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    if(size == maxSize)
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            qDebug() << "file open success: " << file.fileName();
            Contacter contacter;
            QByteArray data = file.readAll();
            QDataStream in(&data, QIODevice::ReadOnly);
            in >> contacter;
            qDebug() << "onSignalFileShareReceivePercent contacter: " << contacter.id << contacter.name << contacter.surname;

            file.close();

            emit signalContacterReceived(contacter);
        }
        else
        {
            qDebug() << "file open error: " << file.errorString();
        }
    }
    emit signalReceivePercent(fileName, size, maxSize);
}

void ContacterPushAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
