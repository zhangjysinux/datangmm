#include "pttmediaserviceadaptor.h"
#include <QDBusConnection>
#include <QDBusMetaType>
#include <QDBusReply>
#include <QDebug>

PttMediaServiceAdaptor::PttMediaServiceAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.pttMedia", "/", "com.sinux.DBus.pttMedia", QDBusConnection::sessionBus())
{
    if(!m_interface.isValid())
    {
        qDebug() << "PttServiceAdaptor qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.pttMedia", "/", "com.sinux.DBus.pttMedia",
                       "signalPttStateChanged", this, SLOT(onPttStateChanged(unsigned,int)));
}

unsigned PttMediaServiceAdaptor::joinGroup(QString groupAddr, unsigned port)
{
    unsigned error = 0;
    QDBusReply<unsigned> reply = m_interface.call("onJoinGroup", groupAddr, port);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onJoinGroup reply error:" << reply.error();
    }
    return error;
}

int PttMediaServiceAdaptor::leaveGroup(unsigned groupId)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onLeaveGroup", groupId);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onLeaveGroup reply error:" << reply.error();
    }
    return error;
}

int PttMediaServiceAdaptor::getPttState(unsigned groupId)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onGetPttState", groupId);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onGetPttState reply error:" << reply.error();
    }
    return error;
}

int PttMediaServiceAdaptor::setPttState(unsigned groupId, int state)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onSetPttState", groupId, state);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onSetPttState reply error:" << reply.error();
    }
    return error;
}

void PttMediaServiceAdaptor::onPttStateChanged(unsigned groupId, int state)
{
    emit signalPttStateChanged(groupId, state);
}
