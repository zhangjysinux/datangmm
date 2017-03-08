#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDebug>
#include "contacterinterfaceadaptor.h"

ContacterInterfaceAdaptor *ContacterInterfaceAdaptor::m_instance = NULL;
ContacterInterfaceAdaptor::ContacterInterfaceAdaptor(QObject *parent)
    : QObject(parent),
      m_interface("com.sinux.contact", "/contacter", "com.sinux.DBus.contacter", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<Contacter>();
    qDBusRegisterMetaType<PhoneNumber>();
    qDBusRegisterMetaType<ContacterList>();

    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.contact", "/contacter", "com.sinux.DBus.contacter", "signalContacterChanged",
                       this, SLOT(onDbusSignalContacterChanged(int, QDBusVariant)));
}

ContacterInterfaceAdaptor *ContacterInterfaceAdaptor::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new ContacterInterfaceAdaptor();
    }

    return m_instance;
}

QList<quint32> ContacterInterfaceAdaptor::getContacters()
{
    QList<quint32> values;
    QDBusReply<QDBusVariant> contactersReply = m_interface.call("onGetContacters");
    if(contactersReply.isValid())
    {
        QDBusVariant dbusVariant = contactersReply.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        values = qdbus_cast<QList<quint32> >(argument);
    }
    else
    {
        qDebug() << "onGetContacters reply error:" << contactersReply.error();
    }

    return values;
}

Contacter ContacterInterfaceAdaptor::getContacter(const quint32 &id)
{
    Contacter value;
    QDBusReply<QDBusVariant> contacterReply = m_interface.call("onGetContacter", id);
    if(contacterReply.isValid())
    {
        QDBusVariant dbusVariant = contacterReply.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        value = qdbus_cast<Contacter>(argument);
    }
    else
    {
        qDebug() << "onGetContacter reply error:" << contacterReply.error();
    }

    return value;
}

Contacter ContacterInterfaceAdaptor::getLocalInfo()
{
    Contacter value;
    QDBusReply<QDBusVariant> localInfoReply = m_interface.call("onGetLocalInfo");
    if(localInfoReply.isValid())
    {
        QDBusVariant dbusVariant = localInfoReply.value();
        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
        value = qdbus_cast<Contacter>(argument);
    }
    else
    {
        qDebug() << "onGetContacter reply error:" << localInfoReply.error();
    }

    return value;
}

bool ContacterInterfaceAdaptor::updateLocalInfo(const Contacter &localInfo)
{
    bool flag = false;
    QVariant localInfoVar = QVariant::fromValue(localInfo);
    QDBusReply<bool> setLocalReply = m_interface.call("onUpdateLocalInfo", localInfoVar);
    if(setLocalReply.isValid())
        flag = setLocalReply.value();
    else
        qDebug() << "onSetLocalInfo reply error:" << setLocalReply.error();

    return flag;
}

bool ContacterInterfaceAdaptor::addContacter(const Contacter &contacter)
{
    bool flag = false;
    QVariant contacterVar = QVariant::fromValue(contacter);
    QDBusReply<bool> addReply = m_interface.call("onAddContacter", contacterVar);
    if(addReply.isValid())
        flag = addReply.value();
    else
        qDebug() << "onAddContacter reply error:" << addReply.error();

    return flag;
}

bool ContacterInterfaceAdaptor::updateContacter(const Contacter &contacter)
{
    bool flag = false;
    QVariant contacterVar = QVariant::fromValue(contacter);
    QDBusReply<bool> updateReply = m_interface.call("onUpdateContacter", contacterVar);
    if(updateReply.isValid())
        flag = updateReply.value();
    else
        qDebug() << "onUpdateContacter reply error:" << updateReply.error();

    return flag;
}

bool ContacterInterfaceAdaptor::removeContacter(const quint32 &id)
{
    bool flag = false;
    QDBusReply<bool> removeReply = m_interface.call("onRemoveContacter", id);
    if(removeReply.isValid())
        flag = removeReply.value();
    else
        qDebug() << "onRemoveContacter reply error:" << removeReply.error();

    return flag;
}

void ContacterInterfaceAdaptor::onDbusSignalContacterChanged(int type, QDBusVariant data)
{
    qDebug() << "-----------------------receiveContacterChange------------------";
    qDebug() << "contacter change type:" << type;
    QDBusArgument argument = data.variant().value<QDBusArgument>();
    Contacter value = qdbus_cast<Contacter>(argument);
    qDebug() << "change contacter id:" << value.id;
    qDebug() << "change contacter name:" << value.name;
    qDebug() << "change contacter title:" << value.title;
    qDebug() << "-----------------------end--------------------------------------";

    emit signalContacterChanged(type, value);
}

