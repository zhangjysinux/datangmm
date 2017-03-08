#include "BaotongDial.h"
//#include "../Common/commonDef.h"
#include "callmanager.h"
#include <QDBusReply>
#include <QVariant>
#include <QDBusVariant>

#define DWT_SERVICE   QLatin1String("org.dwvt")
#define DWT_INTERFACE QLatin1String("org.dwvt.VoiceCall")
#define DWT_PATH      QLatin1String("/ril_0")

#define KEY_PRIORITY     "Priority"
#define KEY_CALLSTATE    "CallState"
#define KEY_PTTALLOW     "PttAllow"
#define KEY_PTTGETTED    "PttGetted"
#define KEY_PHONENUMBER  "PhoneNumber"
#define KEY_CALLEDNUMBER "CalledNumber"
#define KEY_CALLTYPE     "CallType"
#define KEY_DIALMODE     "DialMode"
#define KEY_CALLDIRECTION "CallDirection"
#define KEY_DISCONNREASON "DisconnectReason"
#define KEY_CHANNELSTATE  "ChannelState"
#define KEY_PTTOWNER     "PttOwner"

#define DIRECTION_CALLER "MO"  //主叫
#define DIRECTION_CALLED "MT"  //被叫

#define CALL_STATE_DIALING      QLatin1String("dialing")
#define CALL_STATE_ACTIVE       QLatin1String("active")
#define CALL_STATE_INCOMING     QLatin1String("incoming")
#define CALL_STATE_DISCONNECTED QLatin1String("disconnected")

BaotongDial::BaotongDial(QObject *parent)
    : QObject(parent)
//    , m_conn(conn)
//    , m_pDialManager(pDialManager)
    , m_priority(QLatin1String("unkown"))
    , m_callState(QLatin1String("disconnected"))
    , m_pttAllow(false)
    , m_pttGetted(false)
    , m_phoneNumber(QLatin1String(""))
    , m_calledNumber(QLatin1String(""))
    , m_callType(QLatin1String("single"))
    , m_dialMode(1)
    , m_callDirection(QLatin1String(""))
    , m_isIncoming(-1)
    , m_interface(NULL)
    , m_pTimer(NULL)
{
    QDBusConnection m_conn = QDBusConnection::systemBus();
    if(!m_conn.registerService(DWT_SERVICE))
        qDebug() << "bus error:" << m_conn.lastError().message() << m_conn.lastError().type();


    m_conn.registerObject("/calls/dwvt", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    m_pTimer = new QTimer(this);
    m_pTimer->setSingleShot(true);
    QObject::connect(m_pTimer, SIGNAL(timeout()), this, SLOT(dialTimeout()));
    initialize();
}

BaotongDial::~BaotongDial()
{
    CTS_PRINT << endl;
}

void BaotongDial::initialize()
{
    //CTS_PRINT << endl;

    m_network = DIGITAL_WALKIE_TALKIE_NETWORK;

    m_interface = new QDBusInterface(DWT_SERVICE, DWT_PATH, DWT_INTERFACE, QDBusConnection::systemBus());

    if (m_interface->isValid() == false)
    {
        //CTS_PRINT << "m_interface->isValid() == false" << endl;
        delete m_interface;
        m_interface = NULL;

        QTimer::singleShot(2000, this, SLOT(initialize()));
    } else {
        CTS_PRINT << endl;

        QDBusConnection sysBus = QDBusConnection::systemBus();
        sysBus.connect(DWT_SERVICE, DWT_PATH, DWT_INTERFACE, "PropertyChanged", this, SLOT(onPropertyChanged(QString, QDBusVariant)));
    }

}

void BaotongDial::dial(QString number, int mode, QString callType, QString prority)
{
    CTS_PRINT << "number:" << number << "; mode:" << mode << "; callType:" << callType << "; prority:" << prority << endl;

    uchar abcd = 1;
    QDBusReply<void> reply = m_interface->call("Dial", number, prority, callType, QVariant::fromValue(abcd));

    if (reply.isValid())
    {
        m_pTimer->start(10*1000);
    } else {
        CTS_PRINT << "reply.error : " << reply.error();
        Q_EMIT DialError(DIAL_FAIL);
    }

}

void BaotongDial::answer()
{
    CTS_PRINT << endl;
    QDBusReply<void> reply = m_interface->call("Answer");
    if (reply.isValid() == false)
    {
        CTS_PRINT << "reply error is " << reply.error();
        //return invalidOperation("answer");
    }
}

void BaotongDial::hangup()
{
    CTS_PRINT << endl;
    QDBusReply<void> reply = m_interface->call("Hangup");
    if (reply.isValid() == false)
    {
        CTS_PRINT << "reply error is " << reply.error();
        // return invalidOperation("hangup");
    }
}

int BaotongDial::isIncoming()
{
    CTS_PRINT <<"isIncoming = " << m_isIncoming << endl;
    QVariant var = getProperty(KEY_CALLDIRECTION);
    if (var.isNull())
    {
        m_callDirection = "";
    } else {
        m_callDirection = var.toString();
    }

    if (m_callDirection.isEmpty())
    {
        m_isIncoming = -1;
    } else if (m_callDirection == DIRECTION_CALLER) {
        m_isIncoming = 0;
    } else if (m_callDirection == DIRECTION_CALLED) {
        m_isIncoming = 1;
    }
    return m_isIncoming;
}

//(dialing呼出/active通话/incoming呼入/disconnected)空闲
QString BaotongDial::status()
{
    CTS_PRINT << endl;
    QVariant var = getProperty(KEY_CALLSTATE);
    if (var.isNull())
    {
        m_callState = "";
        //return invalidOperation("status");
    } else {
        m_callState = var.toString();
    }
    CTS_PRINT << "CallState : " << m_callState << endl;
    return m_callState;
}

int BaotongDial::network()
{
    CTS_PRINT << "network is DIGITAL_WALKIE_TALKIE_NETWORK" << endl;
    if (m_isIncoming == -1)
    {
        //return invalidOperation("network");
    }
    return static_cast<int>(DIGITAL_WALKIE_TALKIE_NETWORK);
}

void BaotongDial::onPropertyChanged(QString name, QDBusVariant value)
{
    CTS_PRINT << "propertyName = " << name << endl;

    QVariant var = value.variant();
    //QVariant var = value;
    if (name == KEY_PRIORITY)
    {
        m_priority = var.toString();
        CTS_PRINT << "Priority : " << m_priority << endl;
    } else if (name == KEY_CALLSTATE) {
        m_callState = var.toString();
        CTS_PRINT << "CallState : " << m_callState << endl;

        if (m_callState == CALL_STATE_ACTIVE)
        {
            if (m_pTimer->isActive())
            {
                CTS_PRINT << "stop timer" << endl;
                m_pTimer->stop();
            }
        }

        Q_EMIT StateChanged(m_callState);
    } else if (name == KEY_PTTALLOW) {
        m_pttAllow = var.toBool();
        CTS_PRINT << "PttAllow : " << m_pttAllow << endl;
    } else if (name == KEY_PTTGETTED) {
        m_pttGetted = var.toBool();
        CTS_PRINT << "PttGetted : " << m_pttGetted << endl;
        Q_EMIT PttGettedChanged(m_pttGetted);
    } else if (name == KEY_PHONENUMBER) {
        m_phoneNumber = var.toString();
        CTS_PRINT << "PhoneNumber : " << m_phoneNumber << endl;
    } else if (name == KEY_CALLEDNUMBER) {
        m_calledNumber = var.toString();
        CTS_PRINT << "CalledNumber : " << m_calledNumber << endl;
    } else if (name == KEY_CALLTYPE) {
        m_callType = var.toString();
        CTS_PRINT << "CallType : " << m_callType << endl;
    } else if (name == KEY_DIALMODE) {
        m_dialMode = var.toInt();
        CTS_PRINT << "DialMode : " << m_dialMode << endl;
    } else if (name == KEY_CALLDIRECTION) {
        m_callDirection = var.toString();
        CTS_PRINT << "CallDirection : " << m_callDirection << endl;
    } else if (name == KEY_DISCONNREASON) {
        CTS_PRINT << KEY_DISCONNREASON << " : " << var.toInt() << endl;
    } else if (name == KEY_PTTOWNER) {
        CTS_PRINT << KEY_PTTOWNER << " : " << var.toString() << endl;
    } else if (name == KEY_CHANNELSTATE) {
        CTS_PRINT << KEY_CHANNELSTATE << " : " << var.toInt() << endl;
    }
}

bool BaotongDial::pttAllow()
{
    CTS_PRINT << endl;
    QVariant var = getProperty(KEY_PTTALLOW);
    if (var.isNull())
    {
        m_pttAllow = false;
        //return invalidOperation("pttAllow");
    } else {
        m_pttAllow = var.toBool();
    }
    CTS_PRINT << "PttAllow : " << m_pttAllow << endl;
    return m_pttAllow;
}

bool BaotongDial::pttGetted()
{
    CTS_PRINT << endl;
    QVariant var = getProperty(KEY_PTTGETTED);
    if (var.isNull())
    {
        m_pttGetted = false;
        //return invalidOperation("pttGetted");
    } else {
        m_pttGetted = var.toBool();
    }

    CTS_PRINT << "PttGetted : " << m_pttGetted << endl;
    return m_pttGetted;
}

void BaotongDial::pttSet(bool on)
{
    CTS_PRINT << "on = " << on << endl;
    QDBusReply<void> reply = m_interface->call("PttSet", on);

    if (reply.isValid() == false)
    {
        CTS_PRINT << "reply.error is " << reply.error();
        //return invalidOperation("pttSet");
    } else {
        CTS_PRINT << "reply is valid";
    }
}

void BaotongDial::dialTimeout()
{
    CTS_PRINT << "time out";
    Q_EMIT DialError(DIAL_FAIL);

    m_isIncoming = -1;
}

QString BaotongDial::phoneNumber()
{
    CTS_PRINT << endl;
    QVariant var = getProperty(KEY_PHONENUMBER);
    if (var.isNull())
    {
        m_phoneNumber = "";
        //return invalidOperation("phoneNumber");
    } else {
        m_phoneNumber = var.toString();
    }
    CTS_PRINT << "phoneNumber : " << m_phoneNumber << endl;
    return m_phoneNumber;
}

QString BaotongDial::calledNumber()
{
    CTS_PRINT << endl;
    QVariant var = getProperty(KEY_CALLEDNUMBER);
    if (var.isNull())
    {
        m_calledNumber = "";
        //return invalidOperation("calledNumber");
    } else {
        m_calledNumber = var.toString();
    }
    CTS_PRINT << "calledNumber : " << m_calledNumber << endl;
    return m_calledNumber;
}

void BaotongDial::test(QString num)
{
    CTS_PRINT << "num : " << num;
    //dial(num);
    //pttSet(true);
    //initialize();
    QVariant var;
    if (var.isNull())
    {
        CTS_PRINT << "var is null";
    } else {
        CTS_PRINT << "var is not null";
    }

    if (var.isValid() == false)
    {
        CTS_PRINT << "var is invalid";
    } else {
        CTS_PRINT << "var is valid";
    }
}

QString BaotongDial::pttOwner()
{
    CTS_PRINT << endl;

    QString owner = "";
    QVariant var = getProperty(KEY_PTTOWNER);

    if (var.isNull() == false)
    {
        owner = var.toString();
    }

    CTS_PRINT << KEY_PTTOWNER << " is " << owner;
    return owner;
}

QVariant BaotongDial::getProperty(QString property)
{
    CTS_PRINT << "property name is " << property;
    QVariant var;
    QList<QVariant> argumentList;
    QDBusPendingReply<QVariantMap> reply = m_interface->asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    reply.waitForFinished();

    if (!reply.isError())
    {
        QVariantMap map = reply.value();

        QVariantMap::const_iterator i;

        for (i = map.constBegin(); i != map.constEnd(); ++i)
        {
            if (i.key() == property)
            {
                var = i.value();
                if (i.key() == KEY_PRIORITY)
                    CTS_PRINT << KEY_PRIORITY << " is " << var.toString();
                else if (i.key() == KEY_CALLSTATE)
                    CTS_PRINT << KEY_CALLSTATE << " is " << var.toString();
                else if (i.key() == KEY_PTTALLOW)
                    CTS_PRINT << KEY_PTTALLOW << " is " << var.toBool();
                else if (i.key() == KEY_PTTGETTED)
                    CTS_PRINT << KEY_PTTGETTED << " is " << var.toBool();
                else if (i.key() == KEY_PHONENUMBER)
                    CTS_PRINT << KEY_PHONENUMBER << " is " << var.toString();
                else if (i.key() == KEY_CALLEDNUMBER)
                    CTS_PRINT << KEY_CALLEDNUMBER << " is " << var.toString();
                else if (i.key() == KEY_CALLTYPE)
                    CTS_PRINT << KEY_CALLTYPE << " is " << var.toString();
                else if (i.key() == KEY_DIALMODE)
                    CTS_PRINT << KEY_DIALMODE << " is " << var.toInt();
                else if (i.key() == KEY_CALLDIRECTION)
                    CTS_PRINT << KEY_CALLDIRECTION << " is " << var.toString();
                else if (i.key() == KEY_DISCONNREASON)
                    CTS_PRINT << KEY_DISCONNREASON << " is " << var.toInt();
                else if (i.key() == KEY_CHANNELSTATE)
                    CTS_PRINT << KEY_CHANNELSTATE << " is " << var.toInt();
                else if (i.key() == KEY_PTTOWNER)
                    CTS_PRINT << KEY_PTTOWNER << " is " << var.toString();

                break;
            }

        }
    }

    return var;
}
