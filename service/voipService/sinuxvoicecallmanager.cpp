#include "sinuxvoicecallmanager.h"
#include <QDebug>
#include "csmanager/voicecallmanager.h"

SinuxVoicecallManager * SinuxVoicecallManager::m_instance = NULL;
SinuxVoicecallManager::SinuxVoicecallManager(QObject *parent) :
    QObject(parent)
{
    init();
}

void SinuxVoicecallManager::init()
{
    m_voiceCallManager = NULL;
    m_voiceCallManager = new VoiceCallManager;
    connect(m_voiceCallManager, SIGNAL(voiceCallsChanged()), this, SLOT(onCallHandlerChanged()));
    connect(m_voiceCallManager, SIGNAL(microphoneMutedChanged()), this, SIGNAL(signalMuteChanged()));
    connect(m_voiceCallManager, SIGNAL(confCallFinished(QString)), this, SIGNAL(signalConferenceEstablished()));
    connect(m_voiceCallManager, SIGNAL(dialError(QString)), this, SIGNAL(signalDialError(QString)));
    connect(m_voiceCallManager, SIGNAL(audioModeChanged()), this, SIGNAL(audioModeChanged()));
    connect(m_voiceCallManager, SIGNAL(speakerMutedChanged()), this, SIGNAL(signalMuteChanged()));
    m_voiceCallProviderModel = NULL;
    m_provider = m_voiceCallManager->defaultProviderId();
}

void SinuxVoicecallManager::onCallHandlerChanged()
{
    CTS_PRINT << "onCallHandlerChanged ";
    VoiceCallModel *handlers = m_voiceCallManager->voiceCalls();
    VoiceCallHandler *handler;
    if (handlers->count() > m_handlers.count())
    {
        handler = handlers->instance(handlers->count() - 1);
        m_handlers.push_back(handler);
        CTS_PRINT << "signalNewCall " << handlers->count() << m_handlers.count();
        emit signalNewCall(handler->handlerId());
    }
    else if (handlers->count() < m_handlers.count())
    {
        m_handlers.clear();
        for (int i = 0; i < handlers->count(); i++ )
        {
            handler = handlers->instance(i);
            m_handlers.push_back(handler);
        }
    }
    return;
}

void SinuxVoicecallManager::setHoldConference(bool on)
{
    return;
}

VoiceCallHandler *SinuxVoicecallManager::lookupHandler(QString handlerId)
{
    VoiceCallHandler *result = 0;
    for (QList<VoiceCallHandler *>::iterator it = m_handlers.begin();
         it != m_handlers.end(); ++it)
    {
        if ((*it)->handlerId() == handlerId)
        {
            result = *it;
            break;
        }
    }

    return result;
}

SinuxVoicecallManager *SinuxVoicecallManager::instance(QObject *parent)
{
    if(m_instance == NULL)
    {
        m_instance = new SinuxVoicecallManager();
    }
    return m_instance;
}

int SinuxVoicecallManager::makeCall(const QString &phoneNum)
{
    //m_provider = "telepathy-ring/tel/account0";
    m_provider = m_voiceCallManager->defaultProviderId();
    CTS_PRINT << "voiceCallManager" << m_provider << phoneNum;
    m_voiceCallManager->dial(m_provider, phoneNum);
    CTS_PRINT << "voiceCallManager return" ;
    return 1;
}

void SinuxVoicecallManager::setMute(bool on)
{
    CTS_PRINT << on << endl;

    m_voiceCallManager->setMuteMicrophone(on);
}

void SinuxVoicecallManager::setHandsFree(bool on)
{
    CTS_PRINT << on << endl;
    QString model;
    if (on)
        model = "ihf";
    else
        model = "earpiece";
    m_voiceCallManager->setAudioMode(model);
}

void SinuxVoicecallManager::mergeIntoConference()
{
    m_voiceCallManager->createConfCall(m_provider);
}

void SinuxVoicecallManager::hangupConference()
{
    m_voiceCallManager->hangupMultipartyCalls();
}

QString SinuxVoicecallManager::getAudioModeChanged()
{
    return m_voiceCallManager->audioMode();
}

bool SinuxVoicecallManager::isMute()
{
    return m_voiceCallManager->isMicrophoneMuted();
}



