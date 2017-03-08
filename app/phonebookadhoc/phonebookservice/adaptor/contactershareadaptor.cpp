#include "contactershareadaptor.h"
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QUuid>

ContacterShareAdaptor *ContacterShareAdaptor::instance = NULL;

ContacterShareAdaptor *ContacterShareAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new ContacterShareAdaptor();
    }

    return instance;
}

ContacterShareAdaptor::ContacterShareAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/contactershare", "com.sinux.DBus.contactershare", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/contactershare",
                       "com.sinux.DBus.contactershare",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect("com.sinux.backupdate",
                       "/contactershare",
                       "com.sinux.DBus.contactershare",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/contactershare",
                       "com.sinux.DBus.contactershare",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));
}

ContacterShareAdaptor::~ContacterShareAdaptor()
{
}

void ContacterShareAdaptor::onSetPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterShareAdaptor::onSetPort" ;
    }
    else
        qDebug() << "ContacterShareAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

void ContacterShareAdaptor::onSetAddresses(const QStringList &addresses)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetAddresses", addresses);
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterShareAdaptor::onSetAddresses" ;
    }
    else
        qDebug() << "ContacterShareAdaptor onSetAddresses reply error:" << retValDbus.error();

    return;
}

int ContacterShareAdaptor::onConnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onConnect");
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterShareAdaptor::onConnect" << retValDbus.value();
    }
    else
        qDebug() << "ContacterShareAdaptor onConnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

bool ContacterShareAdaptor::onIsConnected()
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


int ContacterShareAdaptor::onDisconnect()
{
    QDBusReply<int> retValDbus = m_interface.call("onDisconnect");
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterShareAdaptor::onDisconnect" << retValDbus.value();
    }
    else
        qDebug() << "ContacterShareAdaptor onDisconnect reply error:" << retValDbus.error();

    return retValDbus.value();
}

int ContacterShareAdaptor::onSendFile(const QString &fileName)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", fileName);
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterShareAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "ContacterShareAdaptor onSendFile reply error:" << retValDbus.error();

    return retValDbus.value();
}

int ContacterShareAdaptor::onSendContacter(const Contacter &contacter)
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

void ContacterShareAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void ContacterShareAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
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

void ContacterShareAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}
