#include "pttmediainterface.h"
#include <QDBusMessage>
#include <QDBusMetaType>
#include <QDebug>
#include <iostream>
#include <audiomanager.h>

using namespace std;

PttMediaInterface::PttMediaInterface(QObject* parent)
    : QObject(parent),
      m_bIsRegisted(false),
      m_dbusConnection("")
{
    if (!m_dbusConnection.isConnected())
        m_dbusConnection = QDBusConnection::sessionBus();

    if (m_dbusConnection.isConnected() && !m_bIsRegisted)
    {
        bool registServer = m_dbusConnection.registerService("com.sinux.pttMedia");
        bool registerDBusTest = m_dbusConnection.registerObject("/", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
        if (registServer && registerDBusTest)
            m_bIsRegisted = true;
        else
            qDebug()<<"%%%%%%%%%%%%%%%%%% pttMediaService registration failed!";
    }
    QObject::connect(&PttMediaManager::instance(), SIGNAL(signalConvertFinished(uint,QString)),
                     this, SLOT(onSignalPttRecordFileFinished(uint,QString)));
}

PttMediaInterface::~PttMediaInterface()
{
}

void PttMediaInterface::onSignalPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    emitSignalPttRecordFileFinished(groupId, fileName);
}

void PttMediaInterface::emitPttStateChangedSignal(unsigned groupId, int state)
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.pttMedia", "signalPttStateChanged");
    msg << groupId << state;
    m_dbusConnection.send(msg);
}

void PttMediaInterface::emitSignalPttRecordFileFinished(unsigned groupId, const QString &fileName)
{
    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.pttMedia", "signalPttRecordFileFinished");
    msg << groupId << fileName;
    m_dbusConnection.send(msg);
}

void PttMediaInterface::printArgs(QString arg0, QString arg1)
{
    qDebug() << "DBusTest::Call dbus service success!!! arg0 ====" << arg0 << ", arg1===" << arg1 << endl;
}

unsigned PttMediaInterface::onJoinGroup(QString groupAddr, unsigned port)
{
    qDebug() << "onJoinGroup" << groupAddr << port;
    PttMediaStream *stream = PttMediaManager::instance().createStream(groupAddr.toStdString().c_str(), port);
    return reinterpret_cast<unsigned>(stream);
}

int PttMediaInterface::onLeaveGroup(unsigned groupId)
{
    qDebug() << "onLeaveGroup" << groupId;
    PttMediaStream *stream = reinterpret_cast<PttMediaStream *>(groupId);

    if (!PttMediaManager::instance().destoryStream(stream))
    {
        qDebug() << "onLeaveGroup error";
        return -1;
    }

    return 0;
}

int PttMediaInterface::onGetPttState(unsigned groupId)
{
    PttMediaStream *stream = reinterpret_cast<PttMediaStream *>(groupId);

    if (!PttMediaManager::instance().hasStream(stream))
    {
        qDebug() << "onSetPttState error" << stream;
        return -1;
    }

    PttMediaStream::MediaDirection dir = stream->getMediaDirection();
    int state = toPttState(dir);
    return state;
}

int PttMediaInterface::onSetPttState(unsigned groupId, int state)
{
    qDebug() << "onSetPttState" << groupId << state;
    PttMediaStream *stream = reinterpret_cast<PttMediaStream *>(groupId);

    if (!PttMediaManager::instance().hasStream(stream))
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

int PttMediaInterface::toPttState(PttMediaStream::MediaDirection dir)
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
