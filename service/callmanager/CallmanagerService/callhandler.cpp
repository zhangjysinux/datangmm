#include "callhandler.h"

#define DIAL_HANDLER_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_HANDLER_INTERFACE QLatin1String("com.sinux.CommTransService.Dial")

CallHandler::CallHandler(VoipCallHandler *handler,QObject *parent) :
    QObject(parent),m_callHandler(handler)
{
    QObject::connect(m_callHandler, SIGNAL(statusChanged()),
                     this, SLOT(onStatusChanged()));
    QObject::connect(m_callHandler, SIGNAL(multipartyChanged()),
                     this, SLOT(onMultipartyChanged()));

    m_dialNumber = m_callHandler->lineId();

    m_network = m_callHandler->netWork();
    switch (m_callHandler->netWork()) {
    case WIFI_NETWORK:
        m_handlerId = "WIFINETWORK/" + m_callHandler->handlerId();
        break;
    case AD_HOC_NETWORK:
        m_handlerId = "ADHOCNETWORK/" + m_callHandler->handlerId();
        break;
    case LTE_NETWORK:
        m_handlerId = "LTENETWORK/" + m_callHandler->handlerId();
        break;
    case BATTLE_NETWORK:
        m_handlerId = "BATTLENETWORK/" + m_callHandler->handlerId();
        break;
    default:
        break;
    }
    m_isIncoming = m_callHandler->isIncoming();
    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService(DIAL_HANDLER_SERVICE))
        qDebug() << "bus error:" << bus.lastError().message();

    bus.unregisterObject("/handle"+m_handlerId);
    CTS_PRINT << m_handlerId << "CallHandler init";
    bool contacterReg = bus.registerObject("/handle"+m_handlerId, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterReg)
        qDebug() << "bus error:" << "register  false" << m_handlerId << bus.lastError().message();

    if (m_callHandler->status() == STATUS_INCOMING)
        m_dialState = 3;
    else if (m_callHandler->status() == STATUS_DIALING)
        m_dialState = 2;
    startTime = QDateTime::currentDateTime();
    initialize();
}

CallHandler::~CallHandler()
{

}

int CallHandler::getCallError()
{
    return m_callHandler->getCallError();
}

void CallHandler::initialize()
{

}

QString CallHandler::dialNumber()
{
    return m_dialNumber;
}

QString CallHandler::handlerId()
{
    return m_handlerId;
}

int CallHandler::status()
{
    m_status = STATUS_NULL;
    m_status = m_callHandler->status();
/*
    STATUS_NULL,
    STATUS_ACTIVE,
    STATUS_HELD,
    STATUS_DIALING,
    STATUS_ALERTING,
    STATUS_INCOMING,
    STATUS_WAITING,
    STATUS_DISCONNECTED
*/
    return m_status;
}

bool CallHandler::isIncoming()
{
    return m_isIncoming;
}

bool CallHandler::isMultiparty()
{
    m_isMultiparty = false;
    m_isMultiparty = m_callHandler->isMultiparty();
    return m_isMultiparty;
}

int CallHandler::network()
{
    return (int)m_network;
}


void CallHandler::onStatusChanged()
{
    if (status() == STATUS_ACTIVE){
        startTime = QDateTime::currentDateTime();
        if (isIncoming())
            m_dialState = 1;
        else
            m_dialState = 2;
    }
    else if (status() == STATUS_NULL || status() == STATUS_DISCONNECTED)
        endTime = QDateTime::currentDateTime();
    Q_EMIT statusChanged(m_handlerId, status());
}

void CallHandler::onError(const QString &err)
{
    Q_EMIT error(err);
}

void CallHandler::onMultipartyChanged()
{
    Q_EMIT multipartyChanged(m_handlerId, isMultiparty(), network());
}

void CallHandler::answer()
{
    m_callHandler->answer();
}

void CallHandler::hangup()
{
    CTS_PRINT << endl;
    m_callHandler->hangup();
}

void CallHandler::hold(bool on)
{
    m_callHandler->hold(on);
}
