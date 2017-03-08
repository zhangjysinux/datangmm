#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>
#include <QtQml>
#include "voipserviceadaptor.h"

VoipServiceAdaptor::VoipServiceAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.voip", "/", "com.sinux.DBus.voip", QDBusConnection::sessionBus()),
    m_callId(-1)
{
    if(!m_interface.isValid())
    {
        qDebug() << "VoipServiceAdaptor qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.voip", "/", "com.sinux.DBus.voip", "signalIncomingCall", this, SLOT(onIncomingCall(int)));
}

int VoipServiceAdaptor::makeCall(QString ip)
{
    qDebug() << "makeCall " << ip;

    int error = 1;
    QDBusReply<int> dialReply = m_interface.call("onMakeCall", ip);
    if(dialReply.isValid())
    {
        error = dialReply.value();
        m_callId = error;
    }
    else
    {
        qDebug() << "onDial reply error:" << dialReply.error();
    }

    return error;
}

int VoipServiceAdaptor::answer(int callId)
{
    qDebug() << "answer " << callId;

    int error = 1;
    QDBusReply<int> answerReply = m_interface.call("onAnswer", callId);
    if(answerReply.isValid())
    {
        error = answerReply.value();
    }
    else
    {
        qDebug() << "onAccept reply error:" << answerReply.error();
    }
    return error;
}

int VoipServiceAdaptor::hangup(int callId)
{
    qDebug() << "hangup " << callId;

    int error = 1;
    QDBusReply<int> hangupReply = m_interface.call("onHangup", callId);
    if(hangupReply.isValid())
    {
        error = hangupReply.value();
    }
    else
    {
        qDebug() << "onDecline reply error:" << hangupReply.error();
    }
    return error;
}

int VoipServiceAdaptor::hangupAllCalls()
{
    qDebug() << "hangupAllCalls";

    int error = 1;
    QDBusReply<int> hangupReply = m_interface.call("onHangupAllCalls");
    if(hangupReply.isValid())
    {
        error = hangupReply.value();
    }
    else
    {
        qDebug() << "onStopDial reply error:" << hangupReply.error();
    }
    return error;
}

int VoipServiceAdaptor::getCallId() const
{
    return m_callId;
}

void VoipServiceAdaptor::setCallId(int callId)
{
    m_callId = callId;
}

void VoipServiceAdaptor::onIncomingCall(int callId)
{
    qDebug() << "on voip incoming call callId=" << callId;

    m_callId = callId;

    emit signalIncomingCall(callId);
}
