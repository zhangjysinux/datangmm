#include "voipinterface.h"
#include "voipdbusdata.h"
#include "voipdbus.h"
#include "voipcalllistmanager.h"
#include "wavtomp3converter.h"
#include <QDebug>
#include <iostream>
#include <QDBusMessage>
#include <QDBusMetaType>
#include "voipaccount.h"
#include "voipcall.h"
#include <audiomanager.h>
#include <QDateTime>
#include <sys/stat.h>
#include <pjsua2.hpp>
#include <sstream>
#include <QNetworkInterface>
#include "adhocbus/adhocconnectadaptor.h"
#include <QProcess>
#include <QFile>
using namespace pj;
using namespace std;

extern AudioMediaRecorder *recorder;
extern VoipAccount *acc;

extern void initialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange);
extern int ChangeVidInput();
VoipInterface::VoipInterface(QObject* parent)
    : QObject(parent),
      m_bIsRegisted(false),
      m_dbusConnection(""),
      m_isInitialized(false),
      m_sipPort(5060),
      m_isMute(false),
      m_bIsVideoHold(false),
      m_bIsStart(true),
      m_iAudOrVideo(0)
{
    qDBusRegisterMetaType<VoipCallInfo>();
    if (!m_dbusConnection.isConnected()) {
        m_dbusConnection = QDBusConnection::sessionBus();
    }

    if (m_dbusConnection.isConnected() && !m_bIsRegisted)
    {
        bool registServer = m_dbusConnection.registerService(VoipAdhocServiceName);
        bool registerDBusTest = m_dbusConnection.registerObject("/", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
        if (registServer && registerDBusTest){
            m_bIsRegisted = true;
        }
        else
            qDebug() << "%%%%%%%%%%%%%%%%%% voipService registration failed!";
    }

    QObject::connect(PttMediaManager::instance(), SIGNAL(signalConvertFinished(uint,QString)),
                     this, SLOT(onSignalPttRecordFileFinished(uint,QString)));
}

VoipInterface::~VoipInterface()
{
    qDebug() << "VoipDBus::~VoipDBus" << endl;
}

void VoipInterface::emitIncomingCallSignal(int callId, const QString &remoteIpAddr)
{
    qDebug() << "emitIncomingCallSignal callId=" << callId << endl;

    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalIncomingCall");
    msg << callId << remoteIpAddr;
    m_dbusConnection.send(msg);
}

void VoipInterface::emitCallStateSignal(int callId, int callState)
{
    qDebug() << "emitCallStateSignal callId=" << callId << " callState=" << callState << endl;

    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalCallState");
    msg << callId << callState;
    m_dbusConnection.send(msg);
}

void VoipInterface::emitCallStatusChanged(int callId, bool isDisconnected)
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalCallStatusChanged");
    msg << callId << isDisconnected;
    m_dbusConnection.send(msg);
}

void VoipInterface::emitMuteChanged()
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalMuteChanged");
    m_dbusConnection.send(msg);
}

void VoipInterface::emitHandsFreeChanged()
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalHandsFreeChanged");
    m_dbusConnection.send(msg);
}

void VoipInterface::emitCallIsInConference(int callId)
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalCallIsInConference");
    msg << callId;
    m_dbusConnection.send(msg);
}

void VoipInterface::emitConferenceEstablished()
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalConferenceEstablished");
    m_dbusConnection.send(msg);
}

void VoipInterface::emitConferenceParticipantsChanged()
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalConferenceParticipantsChanged");
    m_dbusConnection.send(msg);
}
void VoipInterface::emitVoipFrameData()
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalVoipFrameData");
    //qDebug() << "dbus connect send voip frame data............." << endl;
    m_dbusConnection.send(msg);

    if(m_bIsVideoHold)
    {
        m_bIsVideoHold = false;
        qDebug() << "##############################################emitVidChanged = false"<< endl;
        QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalVidChanged");
        msg <<m_bIsVideoHold;
        m_dbusConnection.send(msg);


    }

    if(m_bIsVideoHold)
    {
        m_iAudOrVideo = 0;
    }else
    {
        //qDebug() << "##############################################recive audio = m_iAudOrVideo = 1"<< endl;
        m_iAudOrVideo = 1;
    }

}

void VoipInterface::emitV4l2FrameData()
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalV4l2FrameData");
    //qDebug() << "dbus connect send v4l2 frame data............." << endl;
    m_dbusConnection.send(msg);
}

void VoipInterface::emitVidChanged(bool isVideoHold)
{
    if(m_bIsVideoHold && isVideoHold)
        return;

//    if(isVideoHold && m_bIsStart)
//    {
//        m_bIsStart = false;
//        return;
//    }


    qDebug() << "##############################################emitVidChanged ="<<isVideoHold<<endl;
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalVidChanged");
    msg <<isVideoHold;
    m_dbusConnection.send(msg);
    m_bIsVideoHold = isVideoHold;

    if(isVideoHold)
    {
        m_iAudOrVideo = 0;
    }else
    {
        m_iAudOrVideo = 1;
    }
}

void VoipInterface::emitAudOrVideo(int iAudOrVideo)
{
    m_iAudOrVideo = iAudOrVideo;
}

void VoipInterface::emitCallError(int callId)
{
    qDebug() << "emitCallError callId=" << callId << endl;

    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalCallError");
    msg << callId;
    m_dbusConnection.send(msg);
}

void VoipInterface::printArgs(QString arg0, QString arg1)
{
    qDebug() << "DBusTest::Call dbus service success!!! arg0 ====" << arg0 << ", arg1===" << arg1 << endl;
}

int VoipInterface::onInitialize(unsigned sipPort, unsigned rtpPort, unsigned rtpPortRange)
{

    if (m_isInitialized)
        return -1;

    m_isInitialized = true;
    m_sipPort = sipPort;
    m_rtpPort = rtpPort;
#ifdef voipAdHocService
    qDebug()<<"-----------------in voipAdHocService---6****1---" ;
    // by michael zheng change from 1 to 10
    m_rtpPortRange = 10;
    // end by michael zheng
#else
    qDebug()<<"-----------------in voipAdHocService---6****2---" ;
    m_rtpPortRange = rtpPortRange;
#endif


#ifdef voipAdHocService
    qDebug()<<"-----------------in voipAdHocService---6****3---" ;
    QTimer::singleShot(1000, this, SLOT(onDelayInitialize()));
#else
    qDebug()<<"-----------------in voipAdHocService---6****4---" ;
    initialize(sipPort, rtpPort, rtpPortRange);
#endif
    qDebug()<<"-----------------in voipAdHocService---6****5---" ;

    QFile file("/home/user/log/mylog.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
    {
        file.write("onInitialize");
        file.flush();
        file.close();
    }


    return 0;
}

int VoipInterface::onDelayInitialize()
{
    bool getFlag = false;
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
        QString addr = address.toString();
        qDebug() << "===================wait adhoc interface start";
        if(addr.contains("192.168.43.") || addr.contains("192.168.90."))
        {
            getFlag = true;
            qDebug() << "===================find adhoc interface: " << addr;
            break;
        }
    }

    if(getFlag)
    {
        initialize(m_sipPort, m_rtpPort, m_rtpPortRange);
    }
    else
    {
        QTimer::singleShot(1000, this, SLOT(onDelayInitialize()));
    }
}

int VoipInterface::onMakeCall(QString ipAddress)
{
    m_bIsStart = true;
    qDebug() << "onMakeCall ipAddress=" << ipAddress << endl;

#ifdef voipAdHocService
    setInterface(0);
    AdhocConnectAdaptor().buildConnect(getAdHocIpAddress(),
                                       ipAddress,
                                       m_rtpPort);
#else
    setInterface(1);
#endif

    if(ipAddress.isEmpty())
        return -1;

    VoipCall *call = new VoipCall(*acc);
    VoipCallListManager::instance().addCall(call);
    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    m_iAudOrVideo = 0;
    stringstream ss;
    ss << m_sipPort;
    string dstUri = "sip:" + ipAddress.toStdString() + ":" + ss.str();
    try
    {
        call->makeCall(dstUri, prm);
    }
    catch(pj::Error &error)
    {
        qDebug() << "onMakeCall error: " << error.reason.data();
    }

    return call->getId();
}
int VoipInterface::onMakeVideoCall(QString ipAddress)
{
    m_bIsStart = true;
    qDebug() << "onMakeCall ipAddress=" << ipAddress << endl;

    if(ipAddress.isEmpty())
        return -1;

    VoipCall *call = new VoipCall(*acc);
    VoipCallListManager::instance().addCall(call);
    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 1;
    m_iAudOrVideo = 1;
    stringstream ss;
    ss << m_sipPort;
    string dstUri = "sip:" + ipAddress.toStdString() + ":" + ss.str();
    try
    {
        call->makeCall(dstUri, prm);
    }
    catch(pj::Error &error)
    {
        qDebug() << "onMakeCall error: " << error.reason.data();
    }

    return call->getId();
}
int VoipInterface::onAnswer(int callId)
{
    qDebug() << "onAnswer callId=" << callId << endl;

    m_iAudOrVideo = 0;
    
    return VoipCallListManager::instance().answerCall(callId);
}

int VoipInterface::onHangup(int callId)
{
    qDebug() << "onHangup callId=" << callId << endl;
#ifdef voipAdHocService
    AdhocConnectAdaptor().deleteConnect();
#endif

    return VoipCallListManager::instance().hangupCall(callId);
}

int VoipInterface::onSetHold(int callId, bool on)
{
    qDebug() << "SetHold callId=" << callId << endl;
    int result;

    try
    {
        result = VoipCallListManager::instance().setHold(callId, on);
    }
    catch(Error &err)
    {
        result = -1;
    }

    return result;
}

int VoipInterface::onSetHoldConference(bool on)
{
    qDebug() << "onGetCallStatus" << endl;
    return VoipCallListManager::instance().setHoldConference(on);
}

QDBusVariant VoipInterface::onGetCallInfo(int callId)
{
    qDebug() << "onGetCallInfo callId=" << callId << endl;

    Call *call = Call::lookup(callId);
    if (call == 0)
        return QDBusVariant();
    CallInfo ci = call->getInfo();

    VoipCallInfo callInfo;
    string remIpAddr = ci.remoteContact;
    size_t pos1;
    if ((pos1 = ci.remoteContact.find(':')) != string::npos)
    {
        size_t pos2;
        if ((pos2 = ci.remoteContact.find(':', pos1 + 1)) != string::npos)
        {

            remIpAddr = ci.remoteContact.substr(pos1 + 1, pos2 - pos1 - 1);
        }
    }
    callInfo.remIpAddr = QString::fromStdString(remIpAddr);
    callInfo.isIncoming = ((ci.role == PJSIP_ROLE_UAS) ? true : false);
    switch (ci.state)
    {
    case PJSIP_INV_STATE_DISCONNECTED:
        callInfo.state = CallStateDisconnected;
        break;
    case PJSIP_INV_STATE_CONFIRMED:
        callInfo.state = CallStateConnected;
        break;
    case PJSIP_INV_STATE_NULL:
        callInfo.state = CallStateUnknown;
        break;
    default:
        callInfo.state = CallStateConnecting;
        break;
    }

    QVariant value;
    value = QVariant::fromValue(callInfo);
    QDBusVariant dbusValue;
    dbusValue.setVariant(value);
    return dbusValue;
}

int VoipInterface::onGetCallStatus(int callId)
{
qDebug() << "onGetCallStatus" << endl;
    return VoipCallListManager::instance().getCallStatus(callId);
}

QString VoipInterface::onGetCallIpAddr(int callId)
{
     qDebug() << "onGetCallIpAddr" << endl;
    return VoipCallListManager::instance().getCallIpAddr(callId);
}

bool VoipInterface::onIsCallIncoming(int callId)
{
    qDebug() << "onIsCallIncoming" << endl;
    return VoipCallListManager::instance().isCallIncoming(callId);
}

bool VoipInterface::onIsCallInConference(int callId)
{
    qDebug() << "onIsCallInConference" << endl;
    return VoipCallListManager::instance().isCallInConference(callId);
}

int VoipInterface::onHangupConference()
{
    qDebug() << "onHangupConference" << endl;

    return VoipCallListManager::instance().hangupConference();
}

int VoipInterface::onHangupAllCalls()
{
    qDebug() << "onHangupAllCalls" << endl;

    Endpoint::instance().hangupAllCalls();
    return 0;
}

int VoipInterface::onSetMute(bool on)
{
    qDebug() << "onSetMute" << endl;
    if(on)
    {
        if(m_isMute)
            return 0;

        VoipCallListManager::instance().setMute(true);
        m_isMute = true;
        emitMuteChanged();
    }
    else
    {
        if(!m_isMute)
            return 0;

        VoipCallListManager::instance().setMute(false);
        m_isMute = false;
        emitMuteChanged();
    }

    return 0;
}

bool VoipInterface::onIsMute()
{
    return m_isMute;
}

int VoipInterface::onSetHandsFree(bool on)
{
    qDebug() << "onSetHandsFree" << endl;
//    AudioManager audMgr;
//    AudioManager::AM_PORT_TYPE port = audMgr.getPort();

//    if(on)
//    {
//        if(port == AudioManager::AM_PORT_OUTPUT_IHF)
//            return 0;

//        audMgr.setPort(AudioManager::AM_PORT_OUTPUT_IHF);
//        emitHandsFreeChanged();
//    }
//    else
//    {
//        if(port != AudioManager::AM_PORT_OUTPUT_IHF)
//            return 0;

//        audMgr.setPort(AudioManager::AM_PORT_CALLON_EARPIECE);
//        emitHandsFreeChanged();
//    }
    qDebug() << "onSetHandsFree" << on;

    AudioManager::AM_PORT_TYPE port;

    if(on)
    {
        port = AudioManager::AM_PORT_OUTPUT_IHF;
    }
    else
    {
        port = AudioManager::AM_PORT_CALLON_EARPIECE;
    }

    if(VoipCallListManager::setPort(port))
    {
        emitHandsFreeChanged();
    }

    return 0;
}

bool VoipInterface::onIsHandsFree()
{
    qDebug() << "onIsHandsFree" << endl;
    AudioManager audMgr;
    bool isHandsFree = false;
    AudioManager::AM_PORT_TYPE type = audMgr.getPort();
    if(type == AudioManager::AM_PORT_OUTPUT_IHF)
        isHandsFree = true;
    else
        isHandsFree = false;

    qDebug() << "on is handsfree: " << isHandsFree << type;
    return isHandsFree;
}

int VoipInterface::onRecord(bool on)
{
    qDebug() << "onRecord" << endl;
    AudioMedia& cap_med = Endpoint::instance().audDevManager().getCaptureDevMedia();
    if (on)
    {
        if (recorder)
            return -1;
        recorder = new AudioMediaRecorder;
        QString timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
        const char *path = "/home/user/sinux/voicerecords/";
        mkdir(path, 0755);
        m_filename = path + timeString;
        QString wavFilename = m_filename + ".wav";
        recorder->createRecorder(wavFilename.toStdString());
        cap_med.startTransmit(*recorder);
        VoipCallListManager::instance().record();
    }
    else
    {
        if (!recorder)
            return -1;
        cap_med.stopTransmit(*recorder);
        delete recorder;
        recorder = 0;
        WavToMp3Converter *converter = new WavToMp3Converter(m_filename.toUtf8().data());
        converter->convert();
    }
    return 0;
}

int VoipInterface::onMergeIntoConference()
{
    qDebug() << "onMergeIntoConference" << endl;
    bool success = VoipCallListManager::instance().mergeIntoConference();
    if (success)
    {
        emitConferenceEstablished();
        emitConferenceParticipantsChanged();
    }
    return 0;
}

int VoipInterface::onGetConferenceParticipants()
{
    return VoipCallListManager::instance().getConferenceParticipants();
}

int VoipInterface::onVidInputChanged(int callId)
{
    qDebug() << "onVidInputChanged" << endl;
   return ChangeVidInput();
}

int VoipInterface::onVidEnabledChanged(int callId, bool on)
{
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!onVidEnabledChanged callId=" << callId <<"on="<<on<<endl;
    int result;
    try
    {
        result = VoipCallListManager::instance().setVidChanged(callId, on);
    }
    catch(Error &err)
    {
        result = -1;
    }

    return result;
}
int VoipInterface::onGetAudOrVideo(int callId)
{
    qDebug() << "onGetAudOrVideo" << endl;
    return m_iAudOrVideo;
}

int VoipInterface::onGetCallError(int callId)
{
    qDebug() << "onGetCallError: " << callId;
    return VoipCallListManager::instance().getCallError(callId);
}

unsigned VoipInterface::onJoinGroup(QString groupAddr, unsigned port)
{
    qDebug() << "onJoinGroup" << groupAddr << port;
    unsigned groupId = PttMediaManager::instance()->getGroupId(groupAddr, port);
    if(groupId)
    {
        qDebug() << "onJoinGroup error: arealy has groupAddr" << groupAddr << port;
    }
    else
    {
        PttMediaStream *stream = PttMediaManager::instance()->createStream(groupAddr.toStdString().c_str(), port);
        qDebug() << "onJoinGroup success" << groupAddr << port;
        groupId = reinterpret_cast<unsigned>(stream);
    }
    return groupId;
}

int VoipInterface::onLeaveGroup(unsigned groupId)
{
    qDebug() << "onLeaveGroup" << groupId;
    PttMediaStream *stream = reinterpret_cast<PttMediaStream *>(groupId);

    if(PttMediaManager::instance()->hasStream(stream))
    {
        if (!PttMediaManager::instance()->destoryStream(stream))
        {
            qDebug() << "onLeaveGroup error";
            return -1;
        }
        else
        {
            qDebug() << "onLeaveGroup success";
        }
    }
    else
    {
        qDebug() << "onLeaveGroup did not contains groupid: " << groupId;
    }

    return 0;
}

int VoipInterface::onGetPttState(unsigned groupId)
{
    PttMediaStream *stream = reinterpret_cast<PttMediaStream *>(groupId);

    if (!PttMediaManager::instance()->hasStream(stream))
    {
        qDebug() << "onSetPttState error" << stream;
        return -1;
    }

    PttMediaStream::MediaDirection dir = stream->getMediaDirection();
    int state = toPttState(dir);
    return state;
}

int VoipInterface::onSetPttState(unsigned groupId, int state)
{
    qDebug() << "onSetPttState" << groupId << state;
    PttMediaStream *stream = reinterpret_cast<PttMediaStream *>(groupId);

    if (!PttMediaManager::instance()->hasStream(stream))
    {
        qDebug() << "onSetPttState error";
        return -1;
    }

    PttMediaStream::MediaDirection dir;
    if (state == PttStatePaused)
        dir = PttMediaStream::MediaDirNone;
    else if (state == PttStateListening)
        dir = PttMediaStream::MediaDirRecving;
    else if (state == PttStateSpeaking)
        dir = PttMediaStream::MediaDirSending;
    bool changed = stream->setMediaDirection(dir);
    if (changed)
        emitPttStateChangedSignal(groupId, state);

    return 0;
}

int VoipInterface::onCallP2P(QString addr)
{
//    callP2P(addr, port);
}

int VoipInterface::onHangupP2P(QString addr)
{

}

int VoipInterface::onGetP2PState(QString addr)
{

}

int VoipInterface::onSetP2PState(QString addr, int state)
{

}

int VoipInterface::toPttState(PttMediaStream::MediaDirection dir)
{
    int state;
    if (dir == PttMediaStream::MediaDirRecving)
        state = PttStateListening;
    else if (dir == PttMediaStream::MediaDirSending)
        state = PttStateSpeaking;
    else
        state = PttStatePaused;
    return state;
}

//int VoipInterface::toP2PState(P2PMediaStream::MediaDirection dir)
//{
//    int state;
//    if (dir == P2PMediaStream::MediaDirRecving)
//        state = PttStateListening;
//    else if (dir == P2PMediaStream::MediaDirSending)
//        state = PttStateSpeaking;
//    else
//        state = PttStatePaused;
//    return state;
//}

void VoipInterface::onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    qDebug() << "onSignalPttRecordFileFinished";
    emitSignalPttRecordFileFinished(groupId, fileName);
}

void VoipInterface::emitPttStateChangedSignal(unsigned groupId, int state)
{
    emit signalPttStateChanged(groupId, state);
}

void VoipInterface::emitSignalPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    emit signalPttRecordFileFinished(groupId, fileName);
}

void VoipInterface::emitSignalP2PStateChanged(QString addr, int state)
{
    emit signalP2PStateChanged(addr, state);
}

void VoipInterface::emitSignalP2PRecordFileFinished(QString addr, QString fileName)
{
    emit signalP2PRecordFileFinished(addr, fileName);
}

void VoipInterface::setInterface(int networkType)
{
    if(networkType == 0)
    {
        QString adhocAddress;
        foreach (QHostAddress address, QNetworkInterface::allAddresses())
        {
            QString addr = address.toString();
            if(addr.contains("192.168.43.") || addr.contains("192.168.90."))
            {
                adhocAddress = addr;
                break;
            }
        }

        QString addRoute = QString("/sbin/ip ro add default via %1").arg(adhocAddress);
        qDebug() << "VoipInterface::setInterface order : " << addRoute;

        QProcess process;
        process.start("/sbin/ip ro del default");
        process.waitForFinished();
        qDebug() << "system result1 :" << process.error() << process.errorString();

        process.start(addRoute);
        process.waitForFinished();
        qDebug() << "system result2 :" << process.error() << process.errorString();
    }
    else if(networkType == 1)
    {
        QString lteAddress;
        foreach (QHostAddress address, QNetworkInterface::allAddresses())
        {
            QString addr = address.toString();
            if(addr.contains("10.21.1.") || addr.contains("170.10.1"))
            {
                lteAddress = addr;
                break;
            }
        }
        QString addRoute = QString("/sbin/route add default gw %1").arg(lteAddress);
        qDebug() << "VoipInterface::setInterface order : " << addRoute;

        QProcess process;
        process.start("/sbin/ip ro del default");
        process.waitForFinished();
        qDebug() << "system result1 :" << process.error() << process.errorString();

        process.start(addRoute);
        process.waitForFinished();
        qDebug() << "system result2 :" << process.error() << process.errorString();
    }
    else
    {
        qDebug() << "VoipInterface::setInterface error" << networkType;
    }
    return;
}

QString VoipInterface::getAdHocIpAddress()
{
    QString adhocAddress;
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
    {
        QString addr = address.toString();
        if(addr.contains("192.168.43.") || addr.contains("192.168.90."))
        {
            adhocAddress = addr;
            break;
        }
    }
    return adhocAddress;
}

int VoipInterface::getAdHocPort()
{
    return m_rtpPort;
}

