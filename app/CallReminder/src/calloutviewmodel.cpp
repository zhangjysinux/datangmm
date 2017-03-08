#include "calloutviewmodel.h"
#include "miniphonecalladaptor.h"

CallOutViewModel::CallOutViewModel(QObject *parent) :
    QObject(parent),
    m_miniAdaptor(NULL)
{
    m_miniAdaptor = new MiniPhoneCallAdaptor(this);
    connect(m_miniAdaptor, SIGNAL(signalSendDataTomini(int,MiniDialData)), this,
            SLOT(onCallStateChanged(int,MiniDialData)));
}

CallOutViewModel::~CallOutViewModel()
{
    if (m_miniAdaptor)
        delete m_miniAdaptor;
}

QString CallOutViewModel::getName()
{
    return m_name;
}

QString CallOutViewModel::getTelphone()
{
    return m_telphone;
}

QString CallOutViewModel::getCallState()
{
    return m_callState;
}

void CallOutViewModel::setName(const QString &text)
{
    m_name = text;
    emit signalNameChanged(m_name);
}

void CallOutViewModel::setTelphone(const QString &text)
{
    m_telphone = text;
    emit signalTelphoneChanged(m_telphone);
}

void CallOutViewModel::setCallState(const QString &text)
{
    m_callState = text;
    emit signalCallStateChanged(m_callState);
}

void CallOutViewModel::startCallOut(int net, const QString &name, const QString &telphone)
{
    setName(name);
    setTelphone(telphone);
    setCallState("正在拨号");
    m_miniAdaptor->dial(net, telphone);
    emit signalShowScreen();
}

void CallOutViewModel::callResume()
{
    m_miniAdaptor->hangup(m_callHandle);
    emit signalHideScreen();
}

void CallOutViewModel::onCallStateChanged(int type , MiniDialData mini)
{
    if (type == HANDLER_ADD && !mini.isIncoming)
    {
        m_callHandle = mini.handlerId;
        setCallState("正在通话");
    }
}
