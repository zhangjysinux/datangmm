#include "voipinterface.h"
#include "voipdbusdata.h"
#include "voipdbus.h"
#include "wavtomp3converter.h"
#include <QDebug>
#include <iostream>
#include <QDBusMessage>
#include <QDBusMetaType>
#include <audiomanager.h>
#include <QDateTime>
#include <sys/stat.h>
#include <pjsua2.hpp>
#include <sstream>

using namespace pj;
using namespace std;

extern AudioMediaRecorder *recorder;

extern void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
extern int ChangeVidInput();
VoipInterface::VoipInterface(QObject* parent)
    : QObject(parent),
      m_bIsRegisted(false),
      m_dbusConnection(""),
      m_isInitialized(false),
      m_sipPort(5060),
      m_callManager(P2PManager::getInstance())
{
    qDBusRegisterMetaType<VoipCallInfo>();

    if (!m_dbusConnection.isConnected())
        m_dbusConnection = QDBusConnection::sessionBus();

    if (m_dbusConnection.isConnected() && !m_bIsRegisted)
    {
        bool registServer = m_dbusConnection.registerService(VoipServiceName);
        bool registerDBusTest = m_dbusConnection.registerObject("/", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
        if (registServer && registerDBusTest)
            m_bIsRegisted = true;
        else
            qDebug()<<"%%%%%%%%%%%%%%%%%% voipService registration failed!";
    }

    connect(m_callManager, SIGNAL(signalRecordFinished(QString,QString)),
            this, SIGNAL(signalRecordFinished(QString,QString)));
}

VoipInterface::~VoipInterface()
{
    qDebug() << "VoipDBus::~VoipDBus" << endl;
}

int VoipInterface::onInitialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{

    if (m_isInitialized)
        return -1;

    m_callManager->initialize(sipPort, rtpPort, rtpPortRange);
    m_isInitialized = true;
    m_sipPort = sipPort;
    return 0;
}

int VoipInterface::onCallP2P(QString addr)
{
    return m_callManager->makeCall(addr);
}

int VoipInterface::onHangupP2P(QString addr)
{
    return m_callManager->hangupCall(addr);
}

int VoipInterface::onGetP2PState(QString addr)
{
    return m_callManager->getPttState(addr);
}

int VoipInterface::onSetP2PState(QString addr, int state)
{
    int ret = m_callManager->setPttState(addr, state);
    if(state == 2)
    {
        onRecord(addr, true);
    }
    else
    {
        onRecord(addr, false);
    }
    return ret;
}

int VoipInterface::onRecord(QString addr, bool on)
{
    return m_callManager->record(addr, on);
}

