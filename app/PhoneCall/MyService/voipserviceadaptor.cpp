#include <QDBusConnection>
#include <QDBusMetaType>
#include <QDBusReply>
#include <QDebug>
#include "voipserviceadaptor.h"

VoipServiceAdaptor::VoipServiceAdaptor(const QString serviceName, QObject *parent) :
    QObject(parent),
    m_interface(serviceName, "/", "com.sinux.DBus.voip", QDBusConnection::sessionBus())
{
    //    qDBusRegisterMetaType<VoipCallInfo>();

    if(!m_interface.isValid())
    {
        qDebug() << "VoipServiceAdaptor qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalIncomingCall", this, SLOT(onIncomingCall(int,QString)));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalCallState", this, SLOT(onCallState(int,int)));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalCallStatusChanged", this, SLOT(onCallStatusChanged(int,bool)));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalMuteChanged", this, SLOT(onMuteChanged()));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalHandsFreeChanged", this, SLOT(onHandsFreeChanged()));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalCallIsInConference", this, SLOT(onCallIsInConference(int)));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalConferenceEstablished", this, SLOT(onConferenceEstablished()));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalConferenceParticipantsChanged", this, SLOT(onConferenceParticipantsChanged()));
    sessionBus.connect(serviceName, "/", "com.sinux.DBus.voip", "signalVoipFrameData", this, SLOT(onVoipFrameData()));
}

int VoipServiceAdaptor::initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onInitialize", sipPort, rtpPort, rtpPortRange);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onInitialize reply error:" << reply.error();
    }
    return error;
}

int VoipServiceAdaptor::makeCall(QString ipAddr)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onMakeCall", ipAddr);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onMakeCall reply error:" << reply.error();
    }

    return error;
}

int VoipServiceAdaptor::answer(int callId)
{
    qDebug() << "answer " << callId;

    int error = -1;
    QDBusReply<int> answerReply = m_interface.call("onAnswer", callId);
    if(answerReply.isValid())
    {
        error = answerReply.value();
    }
    else
    {
        qDebug() << "onAnswer reply error:" << answerReply.error();
    }
    return error;
}

int VoipServiceAdaptor::hangup(int callId)
{
    qDebug() << "hangup " << callId;

    int error = -1;
    QDBusReply<int> hangupReply = m_interface.call("onHangup", callId);
    if(hangupReply.isValid())
    {
        error = hangupReply.value();
    }
    else
    {
        qDebug() << "onHangup reply error:" << hangupReply.error();
    }
    return error;
}

int VoipServiceAdaptor::setHold(int callId, bool on)
{
    qDebug() << "setHold " << callId << on;

    int error = -1;
    QDBusReply<int> reply = m_interface.call("onSetHold", callId, on);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onSetHold reply error:" << reply.error();
    }
    return error;
}

//VoipCallInfo VoipServiceAdaptor::getCallInfo(int callId)
//{
//    VoipCallInfo result;
//    QDBusReply<QDBusVariant> reply = m_interface.call("onGetCallInfo", callId);
//    if(reply.isValid())
//    {
//        QDBusVariant dbusVariant = reply.value();
//        QDBusArgument argument = dbusVariant.variant().value<QDBusArgument>();
//        result = qdbus_cast<VoipCallInfo>(argument);
//    }
//    else
//    {
//        qDebug() << "onGetCallInfo reply error:" << reply.error();
//    }
//    return result;
//}

int VoipServiceAdaptor::getCallStatus(int callId)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onGetCallStatus", callId);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onGetCallStatus reply error:" << reply.error();
    }
    return error;
}

QString VoipServiceAdaptor::getCallIpAddr(int callId)
{
    QString result;
    QDBusReply<QString> reply = m_interface.call("onGetCallIpAddr", callId);
    if(reply.isValid())
    {
        result = reply.value();
    }
    else
    {
        qDebug() << "onGetCallIpAddr reply error:" << reply.error();
    }
    return result;
}

bool VoipServiceAdaptor::isCallIncoming(int callId)
{
    bool result = false;
    QDBusReply<bool> reply = m_interface.call("onIsCallIncoming", callId);
    if(reply.isValid())
    {
        result = reply.value();
    }
    else
    {
        qDebug() << "onIsCallIncoming reply error:" << reply.error();
    }
    return result;
}

bool VoipServiceAdaptor::isCallInConference(int callId)
{
    bool result = false;
    QDBusReply<bool> reply = m_interface.call("onIsCallInConference", callId);
    if(reply.isValid())
    {
        result = reply.value();
    }
    else
    {
        qDebug() << "onIsCallInConference reply error:" << reply.error();
    }
    return result;
}

int VoipServiceAdaptor::hangupConference()
{
    qDebug() << "hangupConference";

    int error = -1;
    QDBusReply<int> hangupReply = m_interface.call("onHangupConference");
    if(hangupReply.isValid())
    {
        error = hangupReply.value();
    }
    else
    {
        qDebug() << "onHangupConference reply error:" << hangupReply.error();
    }
    return error;
}

int VoipServiceAdaptor::hangupAllCalls()
{
    qDebug() << "hangupAllCalls";

    int error = -1;
    QDBusReply<int> hangupReply = m_interface.call("onHangupAllCalls");
    if(hangupReply.isValid())
    {
        error = hangupReply.value();
    }
    else
    {
        qDebug() << "onHangupAllCalls reply error:" << hangupReply.error();
    }
    return error;
}

int VoipServiceAdaptor::setMute(bool on)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onSetMute", on);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onSetMute reply error:" << reply.error();
    }
    return error;
}

bool VoipServiceAdaptor::isMute()
{
    bool result = false;
    QDBusReply<bool> reply = m_interface.call("onIsMute");
    if(reply.isValid())
    {
        result = reply.value();
    }
    else
    {
        qDebug() << "onIsMute reply error:" << reply.error();
    }
    return result;
}

int VoipServiceAdaptor::setHandsFree(bool on)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onSetHandsFree", on);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onSetHandsFree reply error:" << reply.error();
    }
    return error;
}

bool VoipServiceAdaptor::isHandsFree()
{
    bool result = false;
    QDBusReply<bool> reply = m_interface.call("onIsHandsFree");
    if(reply.isValid())
    {
        result = reply.value();
    }
    else
    {
        qDebug() << "onIsHandsFree reply error:" << reply.error();
    }
    return result;
}

int VoipServiceAdaptor::record(bool on)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onRecord", on);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onRecord reply error:" << reply.error();
    }
    return error;
}

int VoipServiceAdaptor::mergeIntoConference()
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onMergeIntoConference");
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onMergeIntoConference reply error:" << reply.error();
    }
    return error;
}

int VoipServiceAdaptor::getConferenceParticipants()
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onGetConferenceParticipants");
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onGetConferenceParticipants reply error:" << reply.error();
    }
    return error;
}

void VoipServiceAdaptor::onIncomingCall(int callId, QString remoteIpAddr)
{
    qDebug() << "on voip incoming call " << callId << remoteIpAddr;

    emit signalIncomingCall(callId, remoteIpAddr);
}

void VoipServiceAdaptor::onCallState(int callId, int callState)
{
    emit signalCallState(callId, callState);
}

void VoipServiceAdaptor::onCallStatusChanged(int callId, bool isDisconnected)
{
    emit signalCallStatusChanged(callId, isDisconnected);
}

void VoipServiceAdaptor::onMuteChanged()
{
    emit signalMuteChanged();
}

void VoipServiceAdaptor::onHandsFreeChanged()
{
    emit signalHandsFreeChanged();
}

void VoipServiceAdaptor::onCallIsInConference(int callId)
{
    emit signalCallIsInConference(callId);
}

void VoipServiceAdaptor::onConferenceEstablished()
{
    emit signalConferenceEstablished();
}

void VoipServiceAdaptor::onConferenceParticipantsChanged()
{
    emit signalConferenceParticipantsChanged();
}

void VoipServiceAdaptor::onVoipFrameData()
{
    emit signalVoipFrameData();
}

void VoipServiceAdaptor::onMyDebug(QString mess)
{
    qDebug()<<"myDebug_________________"<<mess;
}
