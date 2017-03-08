#include "p2pvoiceadaptor.h"
#include <QDBusReply>
#include "../../commondata/callmanageradaptor/audiomanager.h"

#define serviceNameP2PWifi "com.sinux.voipWifiP2P"
#define serviceNameP2PAdHoc "com.sinux.voipAdHocP2P"
#define serviceNameP2PLte "com.sinux.voipLteP2P"
#define servicePath "/"
#define serviceInterfaceName "com.sinux.DBus.voip"

P2PVoiceAdaptor::P2PVoiceAdaptor(QObject *parent) :
    QObject(parent),
    m_interfaceP2pWifi(serviceNameP2PWifi,
                       servicePath,
                       serviceInterfaceName,
                       QDBusConnection::sessionBus()),
    m_interfaceP2pAdHoc(serviceNameP2PAdHoc,
                       servicePath,
                       serviceInterfaceName,
                       QDBusConnection::sessionBus()),
    m_interfaceP2pLte(serviceNameP2PLte,
                       servicePath,
                       serviceInterfaceName,
                       QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect(serviceNameP2PWifi,
                       servicePath,
                       serviceInterfaceName,
                       "signalRecordFinished",
                       this,
                       SLOT(onSignalWifiRecordFinished(QString,QString)));

    sessionBus.connect(serviceNameP2PAdHoc,
                       servicePath,
                       serviceInterfaceName,
                       "signalRecordFinished",
                       this,
                       SLOT(onSignalAdHocRecordFinished(QString,QString)));

    sessionBus.connect(serviceNameP2PLte,
                       servicePath,
                       serviceInterfaceName,
                       "signalRecordFinished",
                       this,
                       SLOT(onSignalLteRecordFinished(QString,QString)));

    sessionBus.connect(serviceNameP2PWifi,
                       servicePath,
                       serviceInterfaceName,
                       "signalCallP2PFinished",
                       this,
                       SLOT(onSignalWifiCallP2PFinished(QString,int)));

    sessionBus.connect(serviceNameP2PAdHoc,
                       servicePath,
                       serviceInterfaceName,
                       "signalCallP2PFinished",
                       this,
                       SLOT(onSignalAdHocCallP2PFinished(QString,int)));

    sessionBus.connect(serviceNameP2PLte,
                       servicePath,
                       serviceInterfaceName,
                       "signalCallP2PFinished",
                       this,
                       SLOT(onSignalLteCallP2PFinished(QString,int)));
}

int P2PVoiceAdaptor::onCallP2P(int network, QString addr)
{
    qDebug() << "P2PVoiceAdaptor::onCallP2P" << network << addr;

    QDBusReply<int> retValDbus;
    switch (network)
    {
    case WIFI_NETWORK:
    {
        retValDbus = m_interfaceP2pWifi.call("onCallP2P", addr);
        break;
    }
    case AD_HOC_NETWORK:
    {
        AudioManager audioManager;
        audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_SELFORGNET);

        retValDbus = m_interfaceP2pAdHoc.call("onCallP2P", addr);
        break;
    }
    case LTE_NETWORK:
    {
        retValDbus = m_interfaceP2pLte.call("onCallP2P", addr);
        break;
    }
    default:
        break;
    }

    if(retValDbus.isValid())
    {
        qDebug() << "P2PVoiceAdaptor::onCallP2P" << retValDbus.value();
    }
    else
    {
        qDebug() << "P2PVoiceAdaptor onCallP2P reply error:" << retValDbus.error();
    }

    return retValDbus.value();
}

int P2PVoiceAdaptor::onHangupP2P(int network, QString addr)
{
    qDebug() << "P2PVoiceAdaptor::onHangupP2P" << network << addr;

    QDBusReply<int> retValDbus;
    switch (network)
    {
    case WIFI_NETWORK:
    {
        retValDbus = m_interfaceP2pWifi.call("onHangupP2P", addr);
        break;
    }
    case AD_HOC_NETWORK:
    {
        retValDbus = m_interfaceP2pAdHoc.call("onHangupP2P", addr);
        break;
    }
    case LTE_NETWORK:
    {
        retValDbus = m_interfaceP2pLte.call("onHangupP2P", addr);
        break;
    }
    default:
        break;
    }

    if(retValDbus.isValid())
    {
        qDebug() << "P2PVoiceAdaptor::onHangupP2P" << retValDbus.value();
    }
    else
    {
        qDebug() << "P2PVoiceAdaptor onHangupP2P reply error:" << retValDbus.error();
    }

    return retValDbus.value();
}

int P2PVoiceAdaptor::onGetP2PState(int network, QString addr)
{
    qDebug() << "P2PVoiceAdaptor::onGetP2PState" << network << addr;

    QDBusReply<int> retValDbus;
    switch (network)
    {
    case WIFI_NETWORK:
    {
        retValDbus = m_interfaceP2pWifi.call("onGetP2PState", addr);
        break;
    }
    case AD_HOC_NETWORK:
    {
        retValDbus = m_interfaceP2pAdHoc.call("onGetP2PState", addr);
        break;
    }
    case LTE_NETWORK:
    {
        retValDbus = m_interfaceP2pLte.call("onGetP2PState", addr);
        break;
    }
    default:
        break;
    }

    if(retValDbus.isValid())
    {
        qDebug() << "P2PVoiceAdaptor::onGetP2PState" << retValDbus.value();
    }
    else
    {
        qDebug() << "P2PVoiceAdaptor onGetP2PState reply error:" << retValDbus.error();
    }

    return retValDbus.value();
}

int P2PVoiceAdaptor::onSetP2PState(int network, QString addr, int state)
{
    qDebug() << "P2PVoiceAdaptor::onSetP2PState" << network << addr << state;

    QDBusReply<int> retValDbus;
    switch (network)
    {
    case WIFI_NETWORK:
    {
        retValDbus = m_interfaceP2pWifi.call("onSetP2PState", addr, state);
        break;
    }
    case AD_HOC_NETWORK:
    {
        retValDbus = m_interfaceP2pAdHoc.call("onSetP2PState", addr, state);
        break;
    }
    case LTE_NETWORK:
    {
        retValDbus = m_interfaceP2pLte.call("onSetP2PState", addr, state);
        break;
    }
    default:
        break;
    }

    if(retValDbus.isValid())
    {
        qDebug() << "P2PVoiceAdaptor::onSetP2PState" << retValDbus.value();
    }
    else
    {
        qDebug() << "P2PVoiceAdaptor onSetP2PState reply error:" << retValDbus.error();
    }

    return retValDbus.value();
}

void P2PVoiceAdaptor::onSignalWifiRecordFinished(const QString &addr, const QString &fileName)
{
    qDebug() << "P2PVoiceAdaptor::onSignalRecordFinished" << addr << fileName;
    emit signalRecordFinished(WIFI_NETWORK, addr, fileName);
}

void P2PVoiceAdaptor::onSignalWifiCallP2PFinished(const QString &addr, int state)
{
    qDebug() << "P2PVoiceAdaptor::onSignalCallP2PFinished" << addr << state;
    emit signalCallP2PFinished(WIFI_NETWORK, addr, state);

    AudioManager audioManager;
    audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_SELFORGNET);
}

void P2PVoiceAdaptor::onSignalLteRecordFinished(const QString &addr, const QString &fileName)
{
    qDebug() << "P2PVoiceAdaptor::onSignalLteRecordFinished" << addr << fileName;
    emit signalRecordFinished(LTE_NETWORK, addr, fileName);
}

void P2PVoiceAdaptor::onSignalLteCallP2PFinished(const QString &addr, int state)
{
    qDebug() << "P2PVoiceAdaptor::onSignalCallP2PFinished" << addr << state;
    emit signalCallP2PFinished(LTE_NETWORK, addr, state);

//    AudioManager audioManager;
//    audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_SELFORGNET);
}

void P2PVoiceAdaptor::onSignalAdHocRecordFinished(const QString &addr, const QString &fileName)
{
    qDebug() << "P2PVoiceAdaptor::onSignalAdHocRecordFinished" << addr << fileName;
    emit signalRecordFinished(AD_HOC_NETWORK, addr, fileName);
}

void P2PVoiceAdaptor::onSignalAdHocCallP2PFinished(const QString &addr, int state)
{
    qDebug() << "P2PVoiceAdaptor::onSignalAdHocCallP2PFinished" << addr << state;
    emit signalCallP2PFinished(AD_HOC_NETWORK, addr, state);

//    AudioManager audioManager;
//    audioManager.setAudioMode(AudioManager::AM_AUDIO_MODE_SELFORGNET);
}
