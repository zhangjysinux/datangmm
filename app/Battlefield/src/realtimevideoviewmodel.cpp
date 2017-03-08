#include "realtimevideoviewmodel.h"
#include "voipserviceadaptor.h"

RealtimeVideoViewModel::RealtimeVideoViewModel(QObject *parent) :
    QObject(parent),
    m_voip(NULL),
    m_callID(0)
{
    m_voip = new VoipServiceAdaptor();
    m_voip->initialize();
}

RealtimeVideoViewModel::~RealtimeVideoViewModel()
{
    if (m_voip)
        delete m_voip;
}

void RealtimeVideoViewModel::call(const QString &ip)
{
    m_callID = m_voip->makeCall(ip);
}

void RealtimeVideoViewModel::hangup()
{
    if (m_callID != 0)
        m_voip->hangup(m_callID);

    m_callID = 0;
}
