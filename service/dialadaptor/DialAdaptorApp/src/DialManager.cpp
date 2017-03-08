#include "DialManager.h"
#include "DialHandler.h"
#include <QtDBus/QDBusMetaType>
#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QTimer>
#include <QDBusReply>

using namespace QT_DTT;

#define DIAL_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_INTERFACE QLatin1String("com.sinux.CommTransService.DialManager")
#define DIAL_PATH QLatin1String("/DialManager")

DialManager * DialManager::m_instance = NULL;
DialManager::DialManager(QObject *parent) 
    : QObject(parent)
    , m_interface(NULL)
{
    CTS_PRINT << "constructor" << endl;

    qDBusRegisterMetaType<UserInfo>();
    initialize();
}

DialManager::~DialManager()
{
    CTS_PRINT << endl;
    if(m_interface != NULL)
    {
        delete m_interface;
        m_interface = NULL;
    }
	
	if(m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
	
	for(int i = 0; i < m_adCalls.size(); i++)
    {
        DialHandler *pHandler = m_adCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_adCalls.clear();

    for(int i = 0; i < m_lteCalls.size(); i++)
    {
        DialHandler *pHandler = m_lteCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_lteCalls.clear();

    for(int i = 0; i < m_satelliteCalls.size(); i++)
    {
        DialHandler *pHandler = m_satelliteCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_satelliteCalls.clear();

    for(int i = 0; i < m_digitalCalls.size(); i++)
    {
        DialHandler *pHandler = m_digitalCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_digitalCalls.clear();
}

void DialManager::initialize()
{
    CTS_PRINT << endl;

    m_interface = new QDBusInterface(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, QDBusConnection::sessionBus());

    qDebug() << "m_interface dbus" << m_interface->isValid();

    QString error;
    QDBusReply<QString> reply = m_interface->call("audioMode", 0);
    if(reply.isValid())
    {
        error = reply.value();
        qDebug() << "audioMode reply no error:" << reply.error();
    }
    else
    {
        qDebug() << "audioMode reply error:" << reply.error();
    }

    if(m_interface->isValid() == false)
    {
        delete m_interface;
        m_interface = NULL;
        QTimer::singleShot(2000, this, SLOT(initialize()));
    }
    else 
    {
		QDBusConnection sessionBus = QDBusConnection::sessionBus();
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "callHandlerIdChanged", this, SLOT(onCallHandlerIdChanged(QString, QString)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "dialError", this, SLOT(onDialError(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "audioModeChanged", this, SLOT(onAudioModeChanged(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "microphoneMutedChanged", this, SLOT(onMicrophoneMuteChanged(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "networkChanged", this, SLOT(onNetworkChanged(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "serviceDisconnect", this, SLOT(onServiceDisconnect(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "confCallFinished", this, SLOT(onConfCallFinished(int, QString)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "numberOfMultipartyParticipantsChanged", this, SLOT(onNumberOfMultipartyParticipantsChanged(int)));
    }
}

DialManager *DialManager::instance(QObject *parent)
{
	CTS_PRINT << endl;
	if(m_instance == NULL)
	{
		m_instance = new DialManager();
	}
	return m_instance;
}
void DialManager::dial(UserInfo user)
{
    CTS_PRINT << "userId = " << user.userId << endl; 
    CTS_PRINT << "network = " << user.network << endl;
    CTS_PRINT << "addr = " << user.addr << endl;
    CTS_PRINT << "appId = " << user.appId << endl;
    CTS_PRINT << "optId = " << user.optId << endl;
    CTS_PRINT << "userName = " << user.userName << endl;
    CTS_PRINT << "lte_ip = " << user.lte_ip << endl;
    CTS_PRINT << "ad_hoc_ip = " << user.ad_hoc_ip << endl;
    CTS_PRINT << "satellite_ip = " << user.satellite_ip << endl;
    CTS_PRINT << "satellite_phone_number = " << user.satellite_phone_number << endl;
    CTS_PRINT << "digital_number = " << user.digital_number << endl;
    CTS_PRINT << "battle_ip = " << user.battle_ip << endl;

    if(m_interface->isValid())
    {
        CTS_PRINT << "call dial interface." << endl;
        m_interface->asyncCall("dial", QVariant::fromValue<UserInfo>(user));
    }
}

void DialManager::onCallHandlerIdChanged(QString number, QString handlerId)
{
    CTS_PRINT << "number = " << number << ", handlerId = " << handlerId << endl;

    DialHandler *pHandler = new DialHandler(handlerId);
    QObject::connect(pHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
    QObject::connect(pHandler, SIGNAL(multipartyChanged(QString, bool)), this, SIGNAL(multipartyChanged(QString, bool)));

    int i = 0;
    for(i = 0; i < m_adCalls.size(); i++)
	{
	    DialTransStatus status = (DialTransStatus)m_adCalls.at(i)->status();
		if(status == STATUS_DIALING || status == STATUS_INCOMING)
     	{
     	    CTS_PRINT << "AdHoc voip call : handlerId = " << m_adCalls.at(i)->handlerId() << ", status = " << status;
			pHandler->hangup();
			return;
     	}
	}

	for(i = 0; i < m_lteCalls.size(); i++)
	{
	    DialTransStatus status = (DialTransStatus)m_lteCalls.at(i)->status();
	    if(status == STATUS_DIALING || status == STATUS_INCOMING)
     	{
     	    CTS_PRINT << "Lte voip call : handlerId = " << m_lteCalls.at(i)->handlerId() << ", status = " << status;
			pHandler->hangup();
			return;
     	}
	}

	for(i = 0; i < m_satelliteCalls.size(); i++)
	{
	    DialTransStatus status = (DialTransStatus)m_satelliteCalls.at(i)->status();
	    if(status == STATUS_DIALING || status == STATUS_INCOMING)
     	{
     	    CTS_PRINT << "satellite call : handlerId = " << m_satelliteCalls.at(i)->handlerId() << ", status = " << status;
			pHandler->hangup();
			return;
     	}
	}

	for(i = 0; i < m_digitalCalls.size(); i++)
	{
	    DialTransStatus status = (DialTransStatus)m_digitalCalls.at(i)->status();
	    if(status == STATUS_DIALING || status == STATUS_INCOMING)
     	{
     	    CTS_PRINT << "digital call : handlerId = " << m_digitalCalls.at(i)->handlerId() << ", status = " << status;
			pHandler->hangup();
			return;
     	}
	}
	
    switch(pHandler->network())
    {
    case AD_HOC_NETWORK:
        m_adCalls.append(pHandler);
        break;
    case LTE_NETWORK:
        m_lteCalls.append(pHandler);
        break;
    case SATELLITE_NETWORK:
        m_satelliteCalls.append(pHandler);
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        m_digitalCalls.append(pHandler);
        break;
    default:
        break;
    }
    Q_EMIT callHandlerIdChanged(number, handlerId);
}

void DialManager::onStatusChanged(QString handlerId, int status)
{
    CTS_PRINT << "handlerId : " << handlerId << ", status = " << status << endl;
    int i = 0;

    if(status == STATUS_NULL || status == STATUS_DISCONNECTED)
    {
        bool bRemoved = false;
        for(i = 0; i < m_adCalls.size(); i++)
        {
            if(m_adCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_adCalls.takeAt(i);
                pHandler->deleteLater();
                //pHandler = NULL;
                break;
            }
        }

        for(i = 0; i < m_lteCalls.size() && bRemoved == false; i++)
        {
            if(m_lteCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_lteCalls.takeAt(i);
                pHandler->deleteLater();
                //pHandler = NULL;
                break;
            }
        }

        for(i = 0; i < m_satelliteCalls.size() && bRemoved == false; i ++)
        {
            if(m_satelliteCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_satelliteCalls.takeAt(i);
                pHandler->deleteLater();
                //pHandler = NULL;
                break;
            }
        }

        for(i = 0; i < m_digitalCalls.size() && bRemoved == false; i++)
        {
            if(m_digitalCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_digitalCalls.takeAt(i);
                pHandler->deleteLater();
                //pHandler = NULL;
                break;
            }
        }
    }
    
    Q_EMIT statusChanged(handlerId, status);
}

void DialManager::onDialError(int err)
{
    CTS_PRINT << "error = " << err << endl;
    Q_EMIT dialError(err);
}

DialHandler *DialManager::getHandlerById(QString handlerId)
{
    CTS_PRINT << "handlerId = " << handlerId << endl;
    
    int i = 0;
    
	CTS_PRINT << "m_adCalls.size() = " << m_adCalls.size() << endl;
	CTS_PRINT << "m_lteCalls.size() = " << m_lteCalls.size() << endl;
	CTS_PRINT << "m_satelliteCalls.size() = " << m_satelliteCalls.size() << endl;
	CTS_PRINT << "m_satelliteCalls.size() = " << m_digitalCalls.size() << endl;
	
    for(i = 0; i < m_adCalls.size(); i++)
    {
        if(m_adCalls.at(i)->handlerId() == handlerId)
        {
            DialHandler *pHandler = m_adCalls.at(i);
            return pHandler;
        }
    }

    for(i = 0; i < m_lteCalls.size(); i++)
    {
        if(m_lteCalls.at(i)->handlerId() == handlerId)
        {
            DialHandler *pHandler = m_lteCalls.at(i);
            return pHandler;
        }
    }

    for(i = 0; i < m_satelliteCalls.size(); i ++)
    {
        if(m_satelliteCalls.at(i)->handlerId() == handlerId)
        {
            DialHandler *pHandler = m_satelliteCalls.at(i);
            return pHandler;
        }
    }

    for(i = 0; i < m_digitalCalls.size(); i++)
    {
        if(m_digitalCalls.at(i)->handlerId() == handlerId)
        {
            DialHandler *pHandler = m_digitalCalls.at(i);
            return pHandler;
        }
    }
	
	return NULL;
}

QList<DialHandler*> * DialManager::voiceCalls(NetworkType network)
{
    CTS_PRINT << "network = " << network << endl;
    
    switch(network)
    {
    case AD_HOC_NETWORK:
        return &m_adCalls;
    case LTE_NETWORK:
        return &m_lteCalls;
    case SATELLITE_NETWORK:
        return &m_satelliteCalls;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        return &m_digitalCalls;
    default:
        break;
    }
	return NULL;
}

void DialManager::addDialHandler(QString handlerId)
{
	onCallHandlerIdChanged("", handlerId);
}

void DialManager::setAudioMode(int network, const QString &mode)
{
	CTS_PRINT << "network = " << network << ", mode = " << mode << endl;
	
	if(mode != "ihf" && mode != "earpiece")
    {
        CTS_PRINT << "Error audio mode :" << mode << ", setAudioMode failed!" << endl;
        return;
    }

    m_interface->asyncCall("setAudioMode", network, mode);
}

void DialManager::setMuteMicrophone(int network, bool on)
{
	CTS_PRINT << "network = " << network << "Mute = " << on << endl;
	
	m_interface->asyncCall("setMuteMicrophone", network, on);
}

void DialManager::createConfCall(int network)
{
	CTS_PRINT << "network = " << network << endl;
	m_interface->asyncCall("createConfCall", network);
}

void DialManager::onAudioModeChanged(int network)
{
	CTS_PRINT << "network = " << network << endl;
	
	QString mode = "";
	QDBusReply<QString> reply = m_interface->call("audioMode");
	
	if(reply.isValid())
	{
		mode = reply.value();
		CTS_PRINT << "mode = " << mode << endl;
	}
	Q_EMIT audioModeChanged(network);
}

void DialManager::onMicrophoneMuteChanged(int network)
{
	CTS_PRINT << "network = " << network << endl;
	bool mute = false;
	QDBusReply<bool> reply = m_interface->call("isMicrophoneMuted");
	if(reply.isValid())
	{
		mute = reply.value();
		CTS_PRINT << "mute = " << mute << endl;
	}
	Q_EMIT microphoneMuteChanged(network);
}

QString DialManager::audioMode(int network) const
{
	CTS_PRINT << "network is " << network << endl;
	
	QString mode = "";
	QDBusReply<QString> reply = m_interface->call("audioMode", network);
	
	if(reply.isValid())
	{
		mode = reply.value();
		CTS_PRINT << "mode = " << mode << endl;
	}
	
	return mode;
}

bool DialManager::isMicrophoneMuted(int network) const
{
	CTS_PRINT << "network is " << network << endl;
	bool mute = false;
	QDBusReply<bool> reply = m_interface->call("isMicrophoneMuted", network);
	if(reply.isValid())
	{
		mute = reply.value();
		CTS_PRINT << "mute = " << mute << endl;
	}
	
	return mute;
}

void DialManager::onNetworkChanged(int network)
{
	CTS_PRINT << "network = " << network << endl;
	Q_EMIT networkChanged(network);
}

void DialManager::onServiceDisconnect(int network)
{
	CTS_PRINT << "network = " << network << endl;
	NetworkType type = (NetworkType)network;
	if(type == AD_HOC_NETWORK)
	{
		for(int i = 0; i < m_adCalls.size(); i++)
        {
            DialHandler *pHandler = m_adCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_adCalls.clear();
    }
	else if(type == LTE_NETWORK)
	{
        for(int i = 0; i < m_lteCalls.size(); i++)
        {
            DialHandler *pHandler = m_lteCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_lteCalls.clear();
	}
	else if(type == SATELLITE_NETWORK)
	{
        for(int i = 0; i < m_satelliteCalls.size(); i++)
        {
            DialHandler *pHandler = m_satelliteCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_satelliteCalls.clear();
	}
	else if(type == DIGITAL_WALKIE_TALKIE_NETWORK)
	{
        for(int i = 0; i < m_digitalCalls.size(); i++)
        {
            DialHandler *pHandler = m_digitalCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_digitalCalls.clear();
	}
	
	Q_EMIT serviceDisconnect(network);
}

void DialManager::onConfCallFinished(int network, QString message)
{
	CTS_PRINT << "network = " << network << ", message = " << message << endl;
	Q_EMIT confCallFinished(network, message);
}

int DialManager::numberOfMultipartyParticipants(int network)
{
	int number = 0;
	QDBusReply<int> reply = m_interface->call("numberOfMultipartyParticipants", network);
	
	if(reply.isValid())
	{
		number = reply.value();
	}
	
	CTS_PRINT << "network = " << network << ", number = " << number << endl;;
	return number;
}

void DialManager::onNumberOfMultipartyParticipantsChanged(int network)
{
	CTS_PRINT << "network = " << network << endl;
	numberOfMultipartyParticipants(network);   //for test
	Q_EMIT numberOfMultipartyParticipantsChanged(network);
}

void DialManager::hangupMultipartyCalls(int network)
{
    CTS_PRINT << "network = " << network;

	QDBusReply<void> reply = m_interface->call("hangupMultipartyCalls", network);

	if(reply.isValid() == false)
	{
	    CTS_PRINT << "reply error is " << reply.error();
	}
}

void DialManager::record(int network, bool on)
{
    CTS_PRINT << "network = " << network << ", on = " << on << endl;

	QDBusReply<void> reply = m_interface->call("record", network, on);

	if(reply.isValid() == false)
	{
	    CTS_PRINT << "reply error is " << reply.error();
	}
}
