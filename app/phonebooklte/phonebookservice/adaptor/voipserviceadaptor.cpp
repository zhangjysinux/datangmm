#include "voipserviceadaptor.h"
#include <QDBusConnection>
#include <QDBusMetaType>
#include <QDBusReply>
#include <QDebug>

VoipServiceAdaptor *VoipServiceAdaptor::instance = NULL;

VoipServiceAdaptor *VoipServiceAdaptor::getInstance(const QString &serviceName)
{
    if(instance == NULL)
        instance = new VoipServiceAdaptor(serviceName);

    return instance;
}


VoipServiceAdaptor::VoipServiceAdaptor(const QString serviceName, QObject *parent) :
    QObject(parent),
    m_interface(serviceName, "/", "com.sinux.DBus.voip", QDBusConnection::sessionBus())
{
    qDebug() << "VoipServiceAdaptor::VoipServiceAdaptor" << serviceName;

    if(!m_interface.isValid())
    {
        qDebug() << "VoipServiceAdaptor qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalPttStateChanged", this, SLOT(onSignalPttStateChanged(uint,int)));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalPttRecordFileFinished", this, SLOT(onSignalPttRecordFileFinished(uint,QString)));
}


unsigned VoipServiceAdaptor::joinGroup(QString groupAddr, unsigned port)
{
    unsigned groupId = 0;
    QDBusReply<unsigned> reply = m_interface.call("onJoinGroup",groupAddr, port);
    if(reply.isValid())
    {
        groupId = reply.value();
    }
    else
    {
        qDebug() << "onJoinGroup reply error:" << reply.error();
    }
    qDebug()<< "joinGroup in voipserviceadaptor "<<groupAddr << port << groupId;
    return groupId;
}

int VoipServiceAdaptor::leaveGroup(unsigned groupId)
{
    qDebug()<< "leaveGroup"<<groupId;
    int ret = 0;
    QDBusReply<int> reply = m_interface.call("onLeaveGroup",groupId);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    else
    {
        qDebug() << "onLeaveGroup reply error:" << reply.error();
    }
    return ret;
}

int VoipServiceAdaptor::getPttState(unsigned groupId)
{
    int ret = 0;
    QDBusReply<int> reply = m_interface.call("onGetPttState",groupId);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    else
    {
        qDebug() << "onGetPttState reply error:" << reply.error();
    }
    return ret;
}

int VoipServiceAdaptor::setPttState(unsigned groupId, int state)
{
    qDebug()<< "setPttState"<<groupId << state;

    int ret = 0;
    QDBusReply<int> reply = m_interface.call("onSetPttState",groupId,state);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    else
    {
        qDebug() << "onSetPttState reply error:" << reply.error();
    }
    return ret;
}

void VoipServiceAdaptor::onSignalPttStateChanged(unsigned groupId, int state)
{
    qDebug() << "onSignalPttStateChanged: " << groupId << state;
    emit signalPttStateChanged(groupId, state);
}

void VoipServiceAdaptor::onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    qDebug() << "VoipServiceAdaptor::onSignalPttRecordFileFinished" << groupId << fileName;
    emit signalPttRecordFileFinished(groupId, fileName);
}
