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

    qDebug() << "PTT media service start" << endl;
    QObject::connect(PttMediaManager::instance(), SIGNAL(signalConvertFinished(uint,QString)),
                     this, SLOT(onSignalPttRecordFileFinished(uint,QString)));
}

VoipInterface::~VoipInterface()
{
    qDebug() << "VoipDBus::~VoipDBus" << endl;
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
