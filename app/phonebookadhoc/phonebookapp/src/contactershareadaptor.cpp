#include "contactershareadaptor.h"
#include <QDBusReply>
#include <QDebug>

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
    m_interface(contacterServiceName, "/phonebookcontactershare", "com.sinux.DBus.phonebookcontactershare", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect(contacterServiceName,
                       "/phonebookcontactershare",
                       "com.sinux.DBus.phonebookcontactershare",
                       "signalConnectFinished",
                       this,
                       SLOT(onSignalFileShareConnectFinished(int)));

    sessionBus.connect(contacterServiceName,
                       "/phonebookcontactershare",
                       "com.sinux.DBus.phonebookcontactershare",
                       "signalReceivePercent",
                       this,
                       SLOT(onSignalFileShareReceivePercent(QString,qint64,qint64)));

    sessionBus.connect(contacterServiceName,
                       "/phonebookcontactershare",
                       "com.sinux.DBus.phonebookcontactershare",
                       "signalSendPercent",
                       this,
                       SLOT(onSignalFileShareSendPercent(QString,qint64,qint64)));

    sessionBus.connect(contacterServiceName,
                       "/phonebookcontactershare",
                       "com.sinux.DBus.phonebookcontactershare",
                       "signalContacterReceived",
                       this,
                       SLOT(onSignalFileShareContacterReceived(Contacter)));
}

ContacterShareAdaptor::~ContacterShareAdaptor()
{
}

void ContacterShareAdaptor::setPort(const int &port)
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



void ContacterShareAdaptor::setAddresses(const QStringList &addresses)
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

int ContacterShareAdaptor::connect()
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
        qDebug() << "ContacterShareAdaptor::onIsConnected" << retValDbus.value();
    }
    else
        qDebug() << "ContacterShareAdaptor onIsConnected reply error:" << retValDbus.error();

    return retValDbus.value();
}

int ContacterShareAdaptor::disconnect()
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

int ContacterShareAdaptor::sendFile(const QString &fileName)
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

int ContacterShareAdaptor::sendContacter(const Contacter &contacter)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendContacter", QVariant::fromValue(contacter));
    if(retValDbus.isValid())
    {
        qDebug() << "ContacterShareAdaptor::onSendContacter" << retValDbus.value();
    }
    else
        qDebug() << "ContacterShareAdaptor onSendContacter reply error:" << retValDbus.error();

    return retValDbus.value();
}

void ContacterShareAdaptor::onSignalFileShareConnectFinished(const int &status)
{
    emit signalConnectFinished(status);
}

void ContacterShareAdaptor::onSignalFileShareReceivePercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalReceivePercent(fileName, size, maxSize);
}

void ContacterShareAdaptor::onSignalFileShareSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    emit signalSendPercent(fileName, size, maxSize);
}

void ContacterShareAdaptor::onSignalFileShareContacterReceived(const Contacter &contacter)
{
    emit signalContacterReceived(CONTACTER_ADD_HANDLE, contacter);
}
