#include "TiantongDial.h"
//#include "../Common/commonDef.h"
#include "callmanager.h"
#define DIAL_HANDLER_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_HANDLER_INTERFACE QLatin1String("com.sinux.CommTransService.Dial")

TiantongDial::TiantongDial(VoiceCallHandler *handler, QObject *parent) :
    QObject(parent),m_pVoiceCallHandler(handler)
{
    m_network = SATELLITE_NETWORK;
    m_dialState = 3;
    startTime = QDateTime::currentDateTime();
    initialize();
}

TiantongDial::~TiantongDial()
{
    qDebug() << "TiantongDial: ~TiantongDial" << endl;
}

void TiantongDial::initialize()
{
    CTS_PRINT << endl;
    m_network = SATELLITE_NETWORK;
    if (m_pVoiceCallHandler != NULL)
    {
        m_handlerId = m_pVoiceCallHandler->handlerId();
        QDBusConnection bus = QDBusConnection::sessionBus();
        if(!bus.registerService(DIAL_HANDLER_SERVICE))
            qDebug() << "bus error:" << bus.lastError().message();

        bus.unregisterObject("/handle"+m_handlerId);
        bool contacterReg = bus.registerObject("/handle"+m_handlerId, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
        if(!contacterReg)
            qDebug() << "bus error:" << "register  false" << m_handlerId << bus.lastError().message();
        m_dialNumber = m_pVoiceCallHandler->lineId();

        CTS_PRINT << "handlerId is " << m_handlerId << endl;
        CTS_PRINT << "dial number is " << m_dialNumber << endl;

        QObject::connect(m_pVoiceCallHandler, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
        QObject::connect(m_pVoiceCallHandler, SIGNAL(statusChanged()), this, SLOT(onStatusChanged()));
        QObject::connect(m_pVoiceCallHandler, SIGNAL(lineIdChanged()), this, SIGNAL(lineIdChanged()));
        QObject::connect(m_pVoiceCallHandler, SIGNAL(durationChanged()), this, SIGNAL(durationChanged()));
        QObject::connect(m_pVoiceCallHandler, SIGNAL(startedAtChanged()), this, SIGNAL(startedAtChanged()));
        QObject::connect(m_pVoiceCallHandler, SIGNAL(multipartyChanged()), this, SLOT(onMultipartyChanged()));
    }
}

void TiantongDial::answer()
{
    CTS_PRINT << endl;
    if (m_pVoiceCallHandler != NULL)
        m_pVoiceCallHandler->answer();
}

void TiantongDial::hangup()
{
    CTS_PRINT << endl;
    if (m_pVoiceCallHandler != NULL)
        m_pVoiceCallHandler->hangup();
}

void TiantongDial::hold(bool on)
{
    CTS_PRINT << "hold on = " << on << endl;
    if (m_pVoiceCallHandler != NULL)
        m_pVoiceCallHandler->hold(on);
}

void TiantongDial::splitChannelFromConf()
{
    CTS_PRINT << endl;
    if (m_pVoiceCallHandler != NULL)
        m_pVoiceCallHandler->splitChannelFromConf();
}

int TiantongDial::status()
{
    DialTransStatus status;
    switch (m_state)
    {
    case 1:
        status = STATUS_ACTIVE;
        break;
    case 2:
        status = STATUS_HELD;
        break;
    case 3:
        status = STATUS_DIALING;
        break;
    case 4:
        status = STATUS_ALERTING;
        break;
    case 5:
        status = STATUS_INCOMING;
        break;
    case 6:
        status = STATUS_WAITING;
        break;
    case 7:
        status = STATUS_DISCONNECTED;
        break;
    default:
        break;
    }
    return status;

//    CTS_PRINT << "status = " << m_pVoiceCallHandler->status() << endl;
//    return m_pVoiceCallHandler->status();
}

bool TiantongDial::isIncoming()
{
    CTS_PRINT << endl;
    bool bRet = false;
    if (m_pVoiceCallHandler != NULL)
    {
        bRet = m_pVoiceCallHandler->isIncoming();
        CTS_PRINT << "isIncoming = " << bRet << endl;
    }
    return bRet;
}

bool TiantongDial::isMultiparty()
{
    CTS_PRINT << endl;
    if (m_pVoiceCallHandler != NULL)
        return m_pVoiceCallHandler->isMultiparty();
}

bool TiantongDial::isEmergency()
{
    CTS_PRINT << endl;
    if (m_pVoiceCallHandler != NULL)
        return m_pVoiceCallHandler->isEmergency();
}

void TiantongDial::onStatusChanged()
{
    CTS_PRINT << endl;
    m_state = m_pVoiceCallHandler->status();
    if (m_state == STATUS_ACTIVE){
        startTime = QDateTime::currentDateTime();
        if (isIncoming())
            m_dialState = 1;
        else
            m_dialState = 2;
    }
    else if (status() == STATUS_NULL || status() == STATUS_DISCONNECTED)
        endTime = QDateTime::currentDateTime();
    Q_EMIT statusChanged(m_handlerId, m_pVoiceCallHandler->status());
}

void TiantongDial::onMultipartyChanged()
{
    Q_EMIT multipartyChanged(m_handlerId, m_pVoiceCallHandler->isMultiparty(), SATELLITE_NETWORK);
}

QString TiantongDial::dialNumber()
{
    return m_dialNumber;
}

QString TiantongDial::handlerId()
{
    return m_handlerId;
}

int TiantongDial::network()
{
    return m_network;
}

