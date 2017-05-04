#include "voipaccount.h"
#include "voipcall.h"
#include <QDebug>
#include "voipinterface.h"
#include <QDBusMessage>
#include <string>
#include <audiomanager.h>
#include "adhocbus/adhocconnectadaptor.h"

using namespace std;

//extern AudioMediaPlayer *player;
extern VoipInterface *service;

VoipAccount::VoipAccount()
{
    connect(this, SIGNAL(signalIncommingCallHandle(OnIncomingCallParam)),
                this, SLOT(onIncommingCallHandle(OnIncomingCallParam)));
}

VoipAccount::~VoipAccount()
{
}

void VoipAccount::onRegState(OnRegStateParam &prm)
{
    AccountInfo ai = getInfo();
    qDebug() << (ai.regIsActive ? "*** Register: code=" : "*** Unregister: code=")
             << prm.code << endl;
}

void VoipAccount::onIncomingCall(OnIncomingCallParam &iprm)
{
//    emit signalIncommingCallHandle(iprm);
    onIncommingCallHandle(iprm);
}

void VoipAccount::onIncommingCallHandle(const OnIncomingCallParam &iprm)
{
    VoipCall *call = new VoipCall(*this, iprm.callId);

    CallInfo ci = call->getInfo();

    //by michael zheng 2017.4.18
    qDebug()<<"----------ci.state:"<<ci.state<<"-----------";
//    if(ci.state==PJSIP_INV_STATE_DISCONNECTED)
//        return;
    // end by michael zheng

    m_callListMgr.addCall(call);

    if(ci.remVideoCount > 0)
    {
        service->emitAudOrVideo(1);
    }else
    {
        service->emitAudOrVideo(0);
    }

#ifdef voipAdHocService
    service->setInterface(0);
    qDebug() << "build connect: " << ci.remoteContact.data();

    QString addr = QString::fromStdString(ci.remoteContact);
    addr = addr.section(":", 1, 1);
    if (ci.remAudioCount > 0)
        AdhocConnectAdaptor().buildVideoConnect(service->getAdHocIpAddress(), addr, service->getAdHocPort());
    else
        AdhocConnectAdaptor().buildConnect(service->getAdHocIpAddress(), addr, service->getAdHocPort());
#else
    service->setInterface(1);
#endif

    qDebug() << "*** Incoming Call: " <<  ci.remoteUri.c_str() << " ["
             << ci.stateText.c_str() << "]" << endl;

    if (service)
    {
        size_t pos1;
        if ((pos1 = ci.remoteContact.find(':')) != string::npos)
        {
            size_t pos2;
            if ((pos2 = ci.remoteContact.find(':', pos1 + 1)) != string::npos)
            {

                string remoteIpAddr = ci.remoteContact.substr(pos1 + 1, pos2 - pos1 - 1);
                service->emitIncomingCallSignal(call->getId(), QString::fromStdString(remoteIpAddr));
            }
        }
    }

    //by michael zheng 2017.3.21
//    AudioMedia& playMed = Endpoint::instance().audDevManager().getPlaybackDevMedia();
    // end by michael zheng

    //    player->setPos(0);
//    player->startTransmit(playMed);

    //set ring louder
    if(!VoipCallListManager::instance().isAudioUsed())
    {
//        AudioManager audMgr;
//        audMgr.setPort(AudioManager::AM_PORT_OUTPzUT_IHF);
        //zhangjy cancel setport 3.21
//        VoipCallListManager::setPort(AudioManager::AM_PORT_OUTPUT_IHF);
    }

    call->setAudioUsed(true);
}
