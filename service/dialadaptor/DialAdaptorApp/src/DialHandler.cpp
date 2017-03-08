#include "DialHandler.h"

#include <QTimer>
#include <QDBusReply>
#include <QDBusPendingCall>
#include <QDBusConnection>

using namespace QT_DTT;

#define DIAL_HANDLER_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_HANDLER_INTERFACE QLatin1String("com.sinux.CommTransService.Dial")

DialHandler::DialHandler(QString handlerId, QObject *parent)
    : QObject(parent)
    , m_interface(NULL)
    , m_handlerId(handlerId)
    , m_network(UNKNOW_NETWORK)
{
    CTS_PRINT << "DialHandler ID : " << m_handlerId << endl;
    initialize();
}

DialHandler::~DialHandler()
{
    CTS_PRINT << endl;

    if(m_interface != NULL)
    {
        delete m_interface;
        m_interface = NULL;
    }
}

void DialHandler::initialize()
{
    CTS_PRINT << endl;
    
    m_interface = new QDBusInterface(DIAL_HANDLER_SERVICE, m_handlerId, DIAL_HANDLER_INTERFACE, QDBusConnection::sessionBus());

    if(m_interface->isValid() == false)
    {
        CTS_PRINT << "connect service error!" << endl;
        delete m_interface;
        m_interface = NULL;
        QTimer::singleShot(2000, this, SLOT(initialize()));
    }
    else 
    {
        QDBusReply<QString> replyNumber = m_interface->call("dialNumber");
        if(replyNumber.isValid())
        {
            m_dialNumber = replyNumber.value();
            CTS_PRINT << "m_dialNumber = " << m_dialNumber << endl;
        }
        else
        {
            CTS_PRINT << "replyNumber error is " << replyNumber.error();
        }

        QDBusReply<bool> replyIsIncoming = m_interface->call("isIncoming");
        if(replyIsIncoming.isValid()) 
        {
            m_isIncoming = replyIsIncoming.value();
            CTS_PRINT << "m_isIncoming = " << m_isIncoming << endl;
        }
        else 
        {
            CTS_PRINT << "replyIsIncoming error is " << replyIsIncoming.error();
        }

        QDBusReply<int> replyNetwork = m_interface->call("network");
        if(replyNetwork.isValid())
        {
            m_network = (NetworkType)replyNetwork.value();
            CTS_PRINT << "network = " << m_network << endl;
        }
        else
        {
            CTS_PRINT << "replyNetwork error is " << replyNetwork.error();
        }
		
		QDBusConnection sessionBus = QDBusConnection::sessionBus();
		sessionBus.connect(DIAL_HANDLER_SERVICE, m_handlerId, DIAL_HANDLER_INTERFACE, "statusChanged", this, SLOT(onStatusChanged()));
		sessionBus.connect(DIAL_HANDLER_SERVICE, m_handlerId, DIAL_HANDLER_INTERFACE, "error", this, SLOT(onError(QString)));
		sessionBus.connect(DIAL_HANDLER_SERVICE, m_handlerId, DIAL_HANDLER_INTERFACE, "multipartyChanged", this, SLOT(onMultipartyChanged()));
		//QObject::connect(m_interface, SIGNAL(statusChanged()), this, SLOT(onStatusChanged()));
        //QObject::connect(m_interface, SIGNAL(error(QString)), this, SLOT(onError(QString)));
        //QObject::connect(m_interface, SIGNAL(multipartyChanged()), this, SLOT(onMultipartyChanged()));
    }
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
    QDBusReply<int> reply = m_interface->call("status");
    if(reply.isValid())
    {
        m_status = reply.value();
    }
    CTS_PRINT << "Status is " << m_status << endl;
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

    QDBusReply<bool> reply = m_interface->call("isMultiparty");
    if(reply.isValid())
    {
        m_isMultiparty = reply.value();
    }

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
	m_interface->asyncCall("answer");
}

void DialHandler::hangup()
{
	CTS_PRINT << endl;
	m_interface->asyncCall("hangup");
}

void DialHandler::hold(bool on)
{
	CTS_PRINT << "on = " << on << endl;
	m_interface->asyncCall("hold", on);
}
