#include "DialManager.h"
#include "DialHandler.h"
#include <QtDBus/QDBusMetaType>
#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QTimer>
#include <QDBusReply>
#include <QThread>

#define DIAL_SERVICE QLatin1String("com.sinux.CommTransService")
#define DIAL_INTERFACE QLatin1String("com.sinux.CommTransService.DialManager")
#define DIAL_PATH QLatin1String("/DialManager")

DialManager * DialManager::m_instance = NULL;
DialManager::DialManager(QObject *parent) 
    : QObject(parent)/*
    , m_interface(NULL)*/
{
    CTS_PRINT << "constructor" << endl;

    qDBusRegisterMetaType<UserInfo>();

    //register object
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.registerService(DIAL_SERVICE))
        qDebug() << "bus error:" << bus.lastError().message() << bus.lastError().type();

    bool contacterReg = bus.registerObject(DIAL_PATH, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if(!contacterReg)
        qDebug() << "bus error:" << "register /DialManager false";

    initialize();
}

DialManager::~DialManager()
{
    CTS_PRINT << endl;
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
}

void DialManager::initialize()
{
    CTS_PRINT << endl;

    m_adhocManager = VoipCallManagerAdHoc::instance();
    m_lteManager = VoipCallManagerLte::instance();

    m_adhocManager->setObjectName("adhoc");
    m_lteManager->setObjectName("lte");

    QObject::connect(m_adhocManager, SIGNAL(signalHandsFreeChanged()), this, SLOT(onAudioModeChanged()));
    QObject::connect(m_adhocManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    QObject::connect(m_adhocManager, SIGNAL(numberOfMultipartyParticipantsChanged()),
                     this, SLOT(onNumberOfMultipartyParticipantsChanged()));
    QObject::connect(m_adhocManager, SIGNAL(signalNewCall(int)), this, SLOT(onCallHandlerIdChanged(int)));

    QObject::connect(m_lteManager, SIGNAL(signalHandsFreeChanged()), this, SLOT(onAudioModeChanged()));
    QObject::connect(m_lteManager, SIGNAL(signalMuteChanged()), this, SLOT(onMicrophoneMuteChanged()));
    QObject::connect(m_lteManager, SIGNAL(numberOfMultipartyParticipantsChanged()),
                     this, SLOT(onNumberOfMultipartyParticipantsChanged()));
    QObject::connect(m_lteManager, SIGNAL(signalConferenceEstablished()),
                     this, SLOT(onConfCallFinished()));
    QObject::connect(m_lteManager, SIGNAL(signalNewCall(int)), this, SLOT(onCallHandlerIdChanged(int)));

    m_adhocManager->initialize(7010, 6010, 14);
    m_lteManager->initialize(5160, 5161, 20);
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

//    if(m_interface->isValid())
//    {
//        CTS_PRINT << "call dial interface." << endl;
//        m_interface->asyncCall("dial", QVariant::fromValue<UserInfo>(user));
//    }

    if(user.network == AD_HOC_NETWORK)
    {
        m_adhocManager->makeCall(user.addr);
    }
    else if(user.network == LTE_NETWORK)
    {
        m_lteManager->makeCall(user.addr);
    }
}

void DialManager::onCallHandlerIdChanged(int handlerId)
{
    QString number;

    VoipCallHandler * handler = NULL;
    DialHandler *pHandler = NULL;
    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        handler = m_adhocManager->lookupHandler(handlerId);
    }
    else if(sender->objectName() == "lte")
    {
        handler = m_lteManager->lookupHandler(handlerId);
    }

    if(pHandler == NULL)
        pHandler = new DialHandler(handler);

//    QThread::msleep(5000);

    number = pHandler->dialNumber();
    qDebug() << "onCallHandlerIdChanged number" << number << "handlerId" << handlerId;
//    CTS_PRINT << "number = " << number << ", handlerId = " << handlerId << endl;

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
	
    switch(pHandler->network())
    {
    case AD_HOC_NETWORK:
        m_adCalls.append(pHandler);
        break;
    case LTE_NETWORK:
        m_lteCalls.append(pHandler);
        break;
    default:
        break;
    }
    Q_EMIT callHandlerIdChanged(number, QString::number(handlerId));
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

//        for(i = 0; i < m_satelliteCalls.size() && bRemoved == false; i ++)
//        {
//            if(m_satelliteCalls.at(i)->handlerId() == handlerId)
//            {
//                bRemoved = true;
//                DialHandler *pHandler = m_satelliteCalls.takeAt(i);
//                pHandler->deleteLater();
//                //pHandler = NULL;
//                break;
//            }
//        }

//        for(i = 0; i < m_digitalCalls.size() && bRemoved == false; i++)
//        {
//            if(m_digitalCalls.at(i)->handlerId() == handlerId)
//            {
//                bRemoved = true;
//                DialHandler *pHandler = m_digitalCalls.takeAt(i);
//                pHandler->deleteLater();
//                //pHandler = NULL;
//                break;
//            }
//        }
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
//	CTS_PRINT << "m_satelliteCalls.size() = " << m_satelliteCalls.size() << endl;
//	CTS_PRINT << "m_satelliteCalls.size() = " << m_digitalCalls.size() << endl;
	
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
//    case SATELLITE_NETWORK:
//        return &m_satelliteCalls;
//    case DIGITAL_WALKIE_TALKIE_NETWORK:
//        return &m_digitalCalls;
    default:
        break;
    }
	return NULL;
}

void DialManager::addDialHandler(QString handlerId)
{
    onCallHandlerIdChanged(handlerId.toInt());
}

void DialManager::setAudioMode(int network, const QString &mode)
{
	CTS_PRINT << "network = " << network << ", mode = " << mode << endl;
	
	if(mode != "ihf" && mode != "earpiece")
    {
        CTS_PRINT << "Error audio mode :" << mode << ", setAudioMode failed!" << endl;
        return;
    }

//    m_interface->asyncCall("setAudioMode", network, mode);


    if(mode == "ihf")
    {
        if(network == AD_HOC_NETWORK)
            m_adhocManager->setHandsFree(true);
        else if(network == LTE_NETWORK)
            m_lteManager->setHandsFree(true);
    }
    else
    {
        if(network == AD_HOC_NETWORK)
            m_adhocManager->setHandsFree(false);
        else if(network == LTE_NETWORK)
            m_lteManager->setHandsFree(false);
    }
}

void DialManager::setMuteMicrophone(int network, bool on)
{
	CTS_PRINT << "network = " << network << "Mute = " << on << endl;
	
//	m_interface->asyncCall("setMuteMicrophone", network, on);
    if(network == AD_HOC_NETWORK)
        m_adhocManager->setMute(on);
    else if(network == LTE_NETWORK)
        m_lteManager->setMute(on);
}

void DialManager::createConfCall(int network)
{
	CTS_PRINT << "network = " << network << endl;
//	m_interface->asyncCall("createConfCall", network);

    if(network == LTE_NETWORK)
    {
        m_lteManager->mergeIntoConference();
    }

}

void DialManager::onAudioModeChanged()
{	
//	QString mode = "";
//	QDBusReply<QString> reply = m_interface->call("audioMode");
	
//	if(reply.isValid())
//	{
//		mode = reply.value();
//		CTS_PRINT << "mode = " << mode << endl;
//	}

    int network = UNKNOW_NETWORK;

    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }

    CTS_PRINT << "network = " << network << endl;

    Q_EMIT audioModeChanged(network);
}

void DialManager::onMicrophoneMuteChanged()
{
//	CTS_PRINT << "network = " << network << endl;
//	bool mute = false;
//	QDBusReply<bool> reply = m_interface->call("isMicrophoneMuted");
//	if(reply.isValid())
//	{
//		mute = reply.value();
//		CTS_PRINT << "mute = " << mute << endl;
//	}

    int network = UNKNOW_NETWORK;

    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }

    Q_EMIT microphoneMutedChanged(network);
}

QString DialManager::audioMode(int network) const
{
//	CTS_PRINT << "network is " << network << endl;
	
	QString mode = "";
    bool handsFree;

    if(network == AD_HOC_NETWORK)
        handsFree = m_adhocManager->isHandsFree();
    else if(network == LTE_NETWORK)
        handsFree = m_lteManager->isHandsFree();

    if(handsFree)
        mode = "ihf";
    else
        mode = "earpiece";

//	QDBusReply<QString> reply = m_interface->call("audioMode", network);
	
//	if(reply.isValid())
//	{
//		mode = reply.value();
//		CTS_PRINT << "mode = " << mode << endl;
//	}
	
	return mode;
}

bool DialManager::isMicrophoneMuted(int network) const
{
	CTS_PRINT << "network is " << network << endl;
	bool mute = false;

    if(network == AD_HOC_NETWORK)
        mute = m_adhocManager->isMute();
    else if(network == LTE_NETWORK)
        mute = m_lteManager->isMute();

//	QDBusReply<bool> reply = m_interface->call("isMicrophoneMuted", network);
//	if(reply.isValid())
//	{
//		mute = reply.value();
//		CTS_PRINT << "mute = " << mute << endl;
//	}
	
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
	
	Q_EMIT serviceDisconnect(network);
}

void DialManager::onConfCallFinished()
{
    int network = LTE_NETWORK;
    QString message = "lte";
	CTS_PRINT << "network = " << network << ", message = " << message << endl;
	Q_EMIT confCallFinished(network, message);
}

int DialManager::numberOfMultipartyParticipants(int network)
{
	int number = 0;

    if(network == AD_HOC_NETWORK)
        number = m_adhocManager->numberOfMultipartyParticipants();
    else if(network == LTE_NETWORK)
        number = m_lteManager->numberOfMultipartyParticipants();

//	QDBusReply<int> reply = m_interface->call("numberOfMultipartyParticipants", network);
	
//	if(reply.isValid())
//	{
//		number = reply.value();
//	}
	
	CTS_PRINT << "network = " << network << ", number = " << number << endl;;
	return number;
}

void DialManager::onNumberOfMultipartyParticipantsChanged()
{
//	numberOfMultipartyParticipants(network);   //for test
    int network = UNKNOW_NETWORK;

    QObject *sender = this->sender();
    if(sender->objectName() == "adhoc")
    {
        network = AD_HOC_NETWORK;
    }
    else if(sender->objectName() == "lte")
    {
        network = LTE_NETWORK;
    }

    CTS_PRINT << "network = " << network << endl;

	Q_EMIT numberOfMultipartyParticipantsChanged(network);
}

void DialManager::hangupMultipartyCalls(int network)
{
    CTS_PRINT << "network = " << network;

    if(network == AD_HOC_NETWORK)
        m_adhocManager->hangupConference();
    else if(network == LTE_NETWORK)
        m_lteManager->hangupConference();

//	QDBusReply<void> reply = m_interface->call("hangupMultipartyCalls", network);

//	if(reply.isValid() == false)
//	{
//	    CTS_PRINT << "reply error is " << reply.error();
//	}
}

void DialManager::record(int network, bool on)
{
    CTS_PRINT << "network = " << network << ", on = " << on << endl;

//	QDBusReply<void> reply = m_interface->call("record", network, on);

//	if(reply.isValid() == false)
//	{
//	    CTS_PRINT << "reply error is " << reply.error();
//	}

    if(network == AD_HOC_NETWORK)
        m_adhocManager->record(on);
    else if(network == LTE_NETWORK)
        m_lteManager->record(on);
}
