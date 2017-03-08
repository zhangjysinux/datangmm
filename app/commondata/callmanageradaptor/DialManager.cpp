#include "DialManager.h"
#include "DialHandler.h"
#include <QtDBus/QDBusMetaType>
#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QTimer>
#include <QDBusReply>

using namespace QT_DTT;
#ifdef SWITCH
    #define DIAL_SERVICE QLatin1String("com.sinux.CommTransService")
    #define DIAL_INTERFACE QLatin1String("com.sinux.CommTransService.CallManager")
    #define DIAL_PATH QLatin1String("/CallManager")
    #define GETACTIVEMODE QLatin1String("onGetActiveMode")
    #define SETHOLDCONFERENCE QLatin1String("onSetHoldConference")
    #define HANGUPCONFERENCE QLatin1String("onHangupConference")
    #define GETAUDIOMODE QLatin1String("onGetAudioMode")
#else
    #define DIAL_SERVICE QLatin1String("com.dtt.CommTransService")
    #define DIAL_INTERFACE QLatin1String("com.dtt.CommTransService.DialManager")
    #define DIAL_PATH QLatin1String("/DialManager")
    #define GETACTIVEMODE QLatin1String("getActiveMode")
    #define SETHOLDCONFERENCE QLatin1String("setHoldConference")
    #define HANGUPCONFERENCE QLatin1String("hangupConference")
    #define GETAUDIOMODE QLatin1String("AudioMode")
#endif

// hedaphone disconnect dbus
#define OHM_POLICY_PATH "/org/freedesktop/ohm/policy"
#define OHM_POLICY_INTERFACE "org.freedesktop.ohm.Policy"
#define OHM_POLICY_MEMBER_HEADPHONESTATE "HeadPhoneState"

// bluetooth disconnect dbus
#define OHM_POLICY_INTERFACE "org.freedesktop.ohm.Policy"
#define OHM_POLICY_INFO_PATH "/org/freedesktop/ohm/policy/info"
#define OHM_POLICY_MEMBER_INFO "info"



DialManager * DialManager::m_instance = NULL;
DialManager::DialManager(QObject *parent) 
    : QObject(parent)
    , m_interface(NULL)
{
    CTS_PRINT << "constructor" << endl;

    qDBusRegisterMetaType<UserInfo>();
    qDBusRegisterMetaType<HandlerData>();
    initialize();
    m_videoHandler = NULL;
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

    for(int i = 0; i < m_wifiCalls.size(); i++)
    {
        DialHandler *pHandler = m_wifiCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_wifiCalls.clear();


    for(int i = 0; i < m_batCalls.size(); i++)
    {
        DialHandler *pHandler = m_batCalls.at(i);
        delete pHandler;
        pHandler = NULL;
    }
    m_batCalls.clear();

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
    CTS_PRINT << DIAL_SERVICE << endl;
    m_interface = new QDBusInterface(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, QDBusConnection::sessionBus());
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
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalVideoCallHandlerIdChanged", this, SLOT(onVideoCallHandlerIdChanged(QString,QString)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "dialError", this, SLOT(onDialError(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "audioModeChanged", this, SLOT(onAudioModeChanged(int)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "microphoneMutedChanged", this, SLOT(onMicrophoneMuteChanged(int)));
        //sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "networkChanged", this, SLOT(onNetworkChanged(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "serviceDisconnect", this, SLOT(onServiceDisconnect(int)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "confCallFinished", this, SLOT(onConfCallFinished(int, QString)));
		sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "numberOfMultipartyParticipantsChanged", this, SLOT(onNumberOfMultipartyParticipantsChanged(int)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalHandlerEnd", this, SLOT(onSignalHandlerEnd(HandlerData)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalMettingEnd", this, SLOT(onSignalMettingEnd(HandlerData)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalMettingHandlerDown", this, SLOT(onSignalMettingHandlerDown(QString,QString)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalVoipFrameData", this, SLOT(onSignalVoipFrameData()));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalV4l2FrameData", this, SIGNAL(signalV4l2FrameData()));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalVidChanged", this, SIGNAL(signalVidChanged(bool)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalPttStateChanged", this, SIGNAL(signalPttStateChanged(uint,int)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "signalPttRecordFileFinished", this, SIGNAL(signalPttRecordFileFinished(uint,QString)));
        sessionBus.connect(DIAL_SERVICE, DIAL_PATH, DIAL_INTERFACE, "PttGettedChanged", this, SIGNAL(PttGettedChanged(bool)));

    }
    g_headphoneState = false;
    g_blueHead = false;

    QDBusConnection::sessionBus().connect("", OHM_POLICY_PATH,
                                     OHM_POLICY_INTERFACE, OHM_POLICY_MEMBER_HEADPHONESTATE,this,
                                     SLOT(headphoneState(bool)));

    QDBusConnection::sessionBus().connect("", OHM_POLICY_INFO_PATH,
                                     OHM_POLICY_INTERFACE, OHM_POLICY_MEMBER_INFO,this,
                                     SLOT(info(QString)));

}


void DialManager::info(QString state)//, QStringList list)
{
    //Q_UNUSED(list)
    CTS_PRINT << state;
    qDebug() << Q_FUNC_INFO << " state=" << state;
    if (state.toInt() == 0) {
        g_blueHead = false;
        CTS_PRINT << "audioManager.stopTrans";
        audioManager.stopTrans();
//        emit bluetoothDisconnected();
    } else if (state.toInt() == 1) {
        g_blueHead = true;
        audioManager.stopTrans();
        audioManager.setPort(AudioManager::AM_PORT_CALLON_BLUETOOTH);
        if (m_satelliteCalls.size() > 0) {
            CTS_PRINT << "audioManager.startTrans AM_TT_AUDIO_ROUTE_BT_TT";
            audioManager.startTrans(AudioManager::AM_TT_AUDIO_ROUTE_BT_TT);
        }
        if (m_lteCalls.size() > 0) {
            CTS_PRINT << "audioManager.startTrans AM_TT_AUDIO_ROUTE_BT_LTE";
            audioManager.startTrans(AudioManager::AM_TT_AUDIO_ROUTE_BT_LTE);
        }
//        emit bluetoothConnected();
    }
}

void DialManager::headphoneState(bool state)
{
    if(state != g_headphoneState){
        CTS_PRINT << " state=" << state;
        if(state == true){
            CTS_PRINT << "audioManager.stopTrans";
            audioManager.stopTrans();
            audioManager.setPort(AudioManager::AM_PORT_CALLON_HEADPHONE);
            CTS_PRINT << "audioManager.startTrans AM_TT_AUDIO_ROUTE_TT";
            audioManager.startTrans(AudioManager::AM_TT_AUDIO_ROUTE_TT);
//          emit headphoneConnected;
        }
        else{
            CTS_PRINT << "audioManager.stopTrans";
            audioManager.stopTrans();
//          emit headphoneDisconnected;
        }
        g_headphoneState = state;
    }
}


void DialManager::onSignalVoipFrameData()
{
    emit signalVoipFrameData();
}
//void signalMettingEnd(HandlerData data);
//void signalMettingHandlerDown(QString handlerId, QString dialNum);
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

    //onNetworkChanged(NetworkType(user.network));
    if(m_interface->isValid())
    {
        CTS_PRINT << "call dial interface." << endl;
        m_interface->asyncCall("dial", QVariant::fromValue<UserInfo>(user));
    }
}

int DialManager::onMakeVideo(UserInfo user)
{
    int ret = -1;
    if(m_interface->isValid())
    {
        CTS_PRINT << "onMakeVideo interface." << endl;
        QDBusReply<int> reply = m_interface->asyncCall("onMakeVideo", QVariant::fromValue<UserInfo>(user));
        if(reply.isValid())
        {
            ret = reply.value();
        }
    }
    return ret;
}

void DialManager::onCallHandlerIdChanged(QString number, QString handlerId)
{
    CTS_PRINT << "number = " << number << ", handlerId = " << handlerId << endl;

    DialHandler *pHandler = new DialHandler(handlerId);
    QObject::connect(pHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
    QObject::connect(pHandler, SIGNAL(multipartyChanged(QString, bool)), this, SIGNAL(multipartyChanged(QString, bool)));

    /*
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
    for(i = 0; i < m_wifiCalls.size(); i++)
    {
        DialTransStatus status = (DialTransStatus)m_wifiCalls.at(i)->status();
        if(status == STATUS_DIALING || status == STATUS_INCOMING)
        {
            CTS_PRINT << "Lte voip call : handlerId = " << m_wifiCalls.at(i)->handlerId() << ", status = " << status;
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

    */
	
    switch(pHandler->network())
    {
    case AD_HOC_NETWORK:
        m_adCalls.append(pHandler);
        break;
    case LTE_NETWORK:
        m_lteCalls.append(pHandler);
        break;
    case WIFI_NETWORK:
        m_wifiCalls.append(pHandler);
        break;
    case BATTLE_NETWORK:
        m_batCalls.append(pHandler);
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
    onNetworkChanged(pHandler->network());
    Q_EMIT callHandlerIdChanged(number, handlerId);
}

void DialManager::onVideoCallHandlerIdChanged(QString dialNum, QString callId)
{
    m_videoHandler = new DialHandler(callId);
    QObject::connect(m_videoHandler, SIGNAL(statusChanged(QString, int)), this, SLOT(onStatusChanged(QString, int)));
    onNetworkChanged(m_videoHandler->network());
    emit signalVideoCallHandlerIdChanged(dialNum, callId);
}


void DialManager::onStatusChanged(QString handlerId, int status)
{
    CTS_PRINT << "handlerId : " << handlerId << ", status = " << status << endl;
    int i = 0;

    if(status == STATUS_NULL || status == STATUS_DISCONNECTED)
    {
        if ( m_videoHandler && m_videoHandler->handlerId() == handlerId)
        {
            m_videoHandler->deleteLater();
            m_videoHandler = NULL;
            Q_EMIT statusChanged(handlerId, status);
            onClearAudioModel();
            return;
        }

        bool bRemoved = false;
        for(i = 0; i < m_adCalls.size(); i++)
        {
            if(m_adCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_adCalls.takeAt(i);
                pHandler->deleteLater();
                //pHandler = NULL;
                if (m_adCalls.size() == 0)
                    onClearAudioModel();
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
                if (m_lteCalls.size() == 0){
                    CTS_PRINT << "LTE_NETWORK set AudioManager.stopTrans" ;
                    onClearAudioModel();
                    audioManager.stopTrans();
                }
                break;
            }
        }

        for(i = 0; i < m_wifiCalls.size() && bRemoved == false; i++)
        {
            if(m_wifiCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_wifiCalls.takeAt(i);
                pHandler->deleteLater();
                if (m_wifiCalls.size() == 0)
                    onClearAudioModel();
                //pHandler = NULL;
                break;
            }
        }

        for(i = 0; i < m_batCalls.size() && bRemoved == false; i++)
        {
            if(m_batCalls.at(i)->handlerId() == handlerId)
            {
                bRemoved = true;
                DialHandler *pHandler = m_batCalls.takeAt(i);
                pHandler->deleteLater();
                if (m_batCalls.size() == 0)
                    onClearAudioModel();
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
                if (m_satelliteCalls.size() == 0) {
                    CTS_PRINT << "SATELLITE_NETWORK set AudioManager.stopTrans" ;
                    onClearAudioModel();
                    audioManager.stopTrans();
                }
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
                if (m_digitalCalls.size() == 0)
                    onClearAudioModel();
                //pHandler = NULL;
                break;
            }
        }
    }
    
    Q_EMIT statusChanged(handlerId, status);
}

void DialManager::onClearAudioModel()
{
//    audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_NORMAL);
//    if (g_blueHead == true && g_headphoneState == false)
//        audioManager.setPort(AudioManager::AM_PORT_CALLON_BLUETOOTH);
//    else if (g_blueHead == false && g_headphoneState == true)
//        audioManager.setPort(AudioManager::AM_PORT_CALLON_HEADPHONE);
//    else if (g_blueHead == false && g_headphoneState == false)
//        audioManager.setPort(AudioManager::AM_PORT_OUTPUT_IHF);

}

void DialManager::onDialError(int err)
{
    CTS_PRINT << "error = " << err << endl;
    Q_EMIT dialError(err);
}

void DialManager::onSignalHandlerEnd(HandlerData data)
{
    emit signalHandlerEnd(data);
}

void DialManager::onSignalMettingEnd(HandlerData data)
{
    emit signalMettingEnd(data);
}

void DialManager::onSignalMettingHandlerDown(QString handlerId, QString dialNum)
{
    emit signalMettingHandlerDown(handlerId, dialNum);
}

void DialManager::baoTongDial(QString phoneNum)
{
    CTS_PRINT << phoneNum << endl;
    m_interface->asyncCall("baoTongDial", phoneNum);
    onNetworkChanged(DIGITAL_WALKIE_TALKIE_NETWORK);
}

void DialManager::baoTongHangUp()
{
    m_interface->asyncCall("baoTongHangUp");
}

void DialManager::baoTongSet(bool on)
{

    CTS_PRINT << on << endl;
    m_interface->asyncCall("baoTongSet", on);
    if (on)
    {
        CTS_PRINT << "audioManager.setPort";
        audioManager.setPort(AudioManager::AM_PORT_OUTPUT_IHF);
    }
}

bool DialManager::baoTongGet()
{
    bool on;
    on = false;
    QDBusReply<bool> reply = m_interface->asyncCall("baoTongGet");
    if (reply.isValid())
        on = reply.value();
    return on;
}

bool DialManager::baoTongPttAllow()
{
    bool on;
    on = false;
    QDBusReply<bool> reply = m_interface->asyncCall("baoTongPttAllow");
    if (reply.isValid())
        on = reply.value();
    return on;
}

DialHandler *DialManager::getHandlerById(QString handlerId)
{
    CTS_PRINT << "handlerId = " << handlerId << endl;
    
    int i = 0;
    
	CTS_PRINT << "m_adCalls.size() = " << m_adCalls.size() << endl;
	CTS_PRINT << "m_lteCalls.size() = " << m_lteCalls.size() << endl;
	CTS_PRINT << "m_satelliteCalls.size() = " << m_satelliteCalls.size() << endl;
    CTS_PRINT << "m_wifiCalls.size() = " << m_wifiCalls.size() << endl;
	
    if (m_videoHandler && handlerId == m_videoHandler->handlerId())
    {
        CTS_PRINT << "m_videoHandler";
        return m_videoHandler;
    }

    for(i = 0; i < m_adCalls.size(); i++)
    {
        qDebug() << "adcallssss " << m_adCalls.at(i)->handlerId() << i << m_adCalls.size();
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
    for(i = 0; i < m_wifiCalls.size(); i++)
    {
        if(m_wifiCalls.at(i)->handlerId() == handlerId)
        {
            CTS_PRINT << " wifi network";
            DialHandler *pHandler = m_wifiCalls.at(i);
            return pHandler;
        }
    }
    for(i = 0; i < m_batCalls.size(); i++)
    {
        if(m_batCalls.at(i)->handlerId() == handlerId)
        {
            CTS_PRINT << " bat network";
            DialHandler *pHandler = m_batCalls.at(i);
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
    case WIFI_NETWORK:
        return &m_wifiCalls;
    case BATTLE_NETWORK:
        return &m_batCalls;
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
    m_interface->asyncCall("onCreateConfCall", network);
}

void DialManager::onAudioModeChanged(int network)
{
	CTS_PRINT << "network = " << network << endl;
	
//	QString mode = "";
//	QDBusReply<QString> reply = m_interface->call("audioMode");
	
//	if(reply.isValid())
//	{
//		mode = reply.value();
//		CTS_PRINT << "mode = " << mode << endl;
//	}
	Q_EMIT audioModeChanged(network);
}

void DialManager::onMicrophoneMuteChanged(int network)
{
	CTS_PRINT << "network = " << network << endl;
//	bool mute = false;
//	QDBusReply<bool> reply = m_interface->call("isMicrophoneMuted");
//	if(reply.isValid())
//	{
//		mute = reply.value();
//		CTS_PRINT << "mute = " << mute << endl;
//	}
	Q_EMIT microphoneMuteChanged(network);
}

QString DialManager::audioMode(int network) const
{
	CTS_PRINT << "network is " << network << endl;
	
	QString mode = "";
    QDBusReply<QString> reply = m_interface->call(GETAUDIOMODE, network);
	
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
    CTS_PRINT << "onNetworkChanged set AudioManager = " << network << endl;
    switch ((NetworkType)network) {
    case AD_HOC_NETWORK:
        CTS_PRINT << "set AudioManager.setAudioMode to AM_AUDIO_MODE_SELFORGNET" ;
        if (audioManager.getAudioMode() != AudioManager::AM_AUDIO_MODE_SELFORGNET)
            audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_SELFORGNET);
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        CTS_PRINT << "set AudioManager.setAudioMode to AM_AUDIO_MODE_BAOTONG" ;
        if (audioManager.getAudioMode() != AudioManager::AM_AUDIO_MODE_BAOTONG)
            audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_BAOTONG);
        break;
    case LTE_NETWORK:
        CTS_PRINT << "set AudioManager.setAudioMode to AM_AUDIO_MODE_LTE" ;
        if (audioManager.getAudioMode() != AudioManager::AM_AUDIO_MODE_LTE)
            audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_LTE);
        break;
    case BATTLE_NETWORK:
        CTS_PRINT << "set AudioManager.setAudioMode to AM_AUDIO_MODE_ZHANHU" ;
        if (audioManager.getAudioMode() != AudioManager::AM_AUDIO_MODE_ZHANHU)
            audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_ZHANHU);
        break;
    case SATELLITE_NETWORK:
        CTS_PRINT << "set AudioManager.setAudioMode to AM_AUDIO_MODE_TIANTONG" ;
        if (audioManager.getAudioMode() != AudioManager::AM_AUDIO_MODE_TIANTONG)
            audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_TIANTONG);
        break;
    default:
        break;
    }
    audioManager.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
    if (network == SATELLITE_NETWORK && g_blueHead == true && g_headphoneState == false){
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.setPort to AM_PORT_CALLON_BLUETOOTH" ;
        if (audioManager.getPort() != AudioManager::AM_PORT_CALLON_BLUETOOTH)
            audioManager.setPort(AudioManager::AM_PORT_CALLON_BLUETOOTH);
    }
    else if (network == SATELLITE_NETWORK && g_blueHead == false && g_headphoneState == true){
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.setPort to AM_PORT_CALLON_HEADPHONE" ;
        if (audioManager.getPort() != AudioManager::AM_PORT_CALLON_HEADPHONE)
            audioManager.setPort(AudioManager::AM_PORT_CALLON_HEADPHONE);
    }
    else {
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.setPort to AM_PORT_CALLON_EARPIECE" ;
        if (audioManager.getPort() != AudioManager::AM_PORT_CALLON_EARPIECE)
            audioManager.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
    }
    if (network == SATELLITE_NETWORK && g_blueHead == false)
    {
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.stopTrans to stopTrans g_blueHead = false" ;
        audioManager.stopTrans();
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.startTrans to AM_TT_AUDIO_ROUTE_TT" ;
        audioManager.startTrans(AudioManager::AM_TT_AUDIO_ROUTE_TT);
    }
    else if (network == SATELLITE_NETWORK && g_blueHead == true)
    {
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.stopTrans to stopTrans g_blueHead = true" ;
        audioManager.stopTrans();
        CTS_PRINT << "SATELLITE_NETWORK set AudioManager.startTrans to stopTrans g_blueHead = true" ;
        audioManager.startTrans(AudioManager::AM_TT_AUDIO_ROUTE_BT_TT);
    }
    else if (network == LTE_NETWORK && g_blueHead == true)
    {
        CTS_PRINT << "LTE_NETWORK set AudioManager.stopTrans to stopTrans g_blueHead = true" ;
        audioManager.stopTrans();
        CTS_PRINT << "LTE_NETWORK set AudioManager.startTrans to stopTrans g_blueHead = true" ;
        audioManager.startTrans(AudioManager::AM_TT_AUDIO_ROUTE_BT_LTE);
    }

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
    else if(type == WIFI_NETWORK)
    {
        for(int i = 0; i < m_wifiCalls.size(); i++)
        {
            DialHandler *pHandler = m_wifiCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_wifiCalls.clear();
    }
    else if(type == BATTLE_NETWORK)
    {
        for(int i = 0; i < m_batCalls.size(); i++)
        {
            DialHandler *pHandler = m_batCalls.at(i);
            delete pHandler;
            pHandler = NULL;
        }
        m_batCalls.clear();
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

    QDBusReply<void> reply = m_interface->call(HANGUPCONFERENCE, network);

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

int DialManager::getActiveMode()
{
    int activeMode = 0 ;
    QDBusReply<int> reply = m_interface->call(GETACTIVEMODE);
    if(reply.isValid())
    {
        activeMode = reply.value();
        CTS_PRINT << "getActiveMode" << activeMode;
    }
    return activeMode;

}

int DialManager::setHoldConference(int network, bool on)
{
    CTS_PRINT << endl;
    int ret = 0 ;
    QDBusReply<int> reply = m_interface->call(SETHOLDCONFERENCE, network, on);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "setHoldConference" << ret;
    }
    return ret;
}

int DialManager::setVidEnabledChanged(int network, QString callId, bool on)
{
    int ret = -1 ;
    QDBusReply<int> reply = m_interface->call("setVidEnabledChanged", network, callId, on);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "setVidEnabledChanged" << ret;
    }
    return ret;
}

int DialManager::setVidInputChanged(int network, QString callId)
{

    CTS_PRINT << network << callId;
    int ret = -1 ;
    QDBusReply<int> reply = m_interface->call("setVidInputChanged", network, callId);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "setVidInputChanged" << ret;
    }
    return ret;
}

int DialManager::getGetAudOrVideo(int network, QString callId)
{
    int ret = -1 ;
    QDBusReply<int> reply = m_interface->call("getGetAudOrVideo", network, callId);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "getGetAudOrVideo" << ret;
    }
    return ret;
}

unsigned DialManager::joinGroup(int network, QString groupAddr, unsigned port)
{
    unsigned ret;
    if(!m_interface)
        return 0;
    QDBusReply<unsigned> reply = m_interface->call("joinGroup",network, groupAddr, port);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "joinGroup" << ret;
    }
    return ret;
}

int DialManager::leaveGroup(int network, unsigned groupId)
{
    qDebug() << "leaveGroup in dialManager" << network << groupId ;
    if(!m_interface)
        return 0;

    int ret = -1;
    QDBusReply<int> reply = m_interface->call("leaveGroup", network, groupId);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "leaveGroup" << ret;
    }
    return ret;
}

int DialManager::getPttState(int network, unsigned groupId)
{
    int ret = -1;
    if(!m_interface)
        return 0;
    QDBusReply<int> reply = m_interface->call("getPttState", network, groupId);
    if(reply.isValid())
    {
        ret = reply.value();
        CTS_PRINT << "getPttState" << ret;
    }
    return ret;
}

int DialManager::setPttState(int network, unsigned groupId, int state)
{
    int ret = -1;
    if(!m_interface)
        return 0;
    QDBusReply<int> reply = m_interface->call("setPttState", network, groupId, state);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}

int DialManager::voipInitialize(NetworkType network, int sipPort, int rtpPort, int rtpPortRange)
{
    int ret = -1;
    QDBusReply<int> reply = m_interface->call("voipInitialize", network, sipPort, rtpPort, rtpPortRange);
    if(reply.isValid())
    {
        ret = reply.value();
    }
    return ret;
}
