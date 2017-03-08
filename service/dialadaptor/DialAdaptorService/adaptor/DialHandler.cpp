#include "DialHandler.h"

#include <QTimer>
#include <QDBusReply>
#include <QDBusPendingCall>
#include <QDBusConnection>
#include <QThread>

#define DIAL_HANDLER_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_HANDLER_INTERFACE QLatin1String("com.sinux.CommTransService.Dial")

DialHandler::DialHandler(VoipCallHandler *handler, QObject *parent)
    : QObject(parent),m_callHandler(handler)/*
    , m_interface(NULL)*/
//    , m_handlerId(QString::number(callId))
//    , m_network(UNKNOW_NETWORK)
//    , m_dialNumber(dialNumber)
{
    CTS_PRINT << "DialHandler ID : " << m_handlerId << endl;

//    m_callHandler = new VoipCallHandler(isLte, callId, isIncomming, lineId);

    QObject::connect(m_callHandler, SIGNAL(statusChanged()),
                     this, SLOT(onStatusChanged()));
    QObject::connect(m_callHandler, SIGNAL(multipartyChanged()),
                     this, SLOT(onMultipartyChanged()));

    m_handlerId = m_callHandler->handlerId();

    m_dialNumber = m_callHandler->lineId();

    if(m_callHandler->isLte())
        m_network = LTE_NETWORK;
    else
        m_network = AD_HOC_NETWORK;

    m_isIncoming = m_callHandler->isIncoming();

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService(DIAL_HANDLER_SERVICE))
        qDebug() << "bus error:" << bus.lastError().message();

    bus.unregisterObject("/handle"+m_handlerId);
    bool contacterReg = bus.registerObject("/handle"+m_handlerId, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
//    QThread::msleep(5000);
    if(!contacterReg)
        qDebug() << "bus error:" << "register  false" << m_handlerId << bus.lastError().message();

    initialize();
}

DialHandler::~DialHandler()
{
    CTS_PRINT << endl;
}

void DialHandler::initialize()
{
    CTS_PRINT << endl;


}

QString DialHandler::dialNumber()
{
    CTS_PRINT << "dialNumber = " << m_dialNumber << endl;
    
    return m_dialNumber;
}

QString DialHandler::handlerId()
{
    CTS_PRINT << "handlerId is " << m_handlerId << endl;
    return m_handlerId;
}

int DialHandler::status()
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

bool DialHandler::isIncoming()
{
    CTS_PRINT << "isIncoming is " << m_isIncoming << endl;
    return m_isIncoming;
}

bool DialHandler::isMultiparty()
{
    m_isMultiparty = false;

    m_isMultiparty = m_callHandler->isMultiparty();
    CTS_PRINT << "isMultiparty is " << m_isMultiparty << endl;
    return m_isMultiparty;
}

int DialHandler::network()
{
    CTS_PRINT << "network is " << m_network << endl;
    return (int)m_network;
}


void DialHandler::onStatusChanged()
{
    CTS_PRINT << endl;
    qDebug() << "m_handlerId" << m_handlerId << status();
    Q_EMIT statusChanged(m_handlerId, status());
}

void DialHandler::onError(const QString &err)
{
    CTS_PRINT << "Error is " << err << endl;
    Q_EMIT error(err);
}

void DialHandler::onMultipartyChanged()
{
    CTS_PRINT << endl;
    //Q_EMIT multipartyChanged(isMultiparty());
	Q_EMIT multipartyChanged(m_handlerId, isMultiparty());
}

void DialHandler::answer()
{
	CTS_PRINT << endl;
//	m_interface->asyncCall("answer");
    m_callHandler->answer();
}

void DialHandler::hangup()
{
	CTS_PRINT << endl;
//	m_interface->asyncCall("hangup");
    m_callHandler->hangup();
}

void DialHandler::hold(bool on)
{
	CTS_PRINT << "on = " << on << endl;
//	m_interface->asyncCall("hold", on);
    m_callHandler->hold(on);
}
