#include "muticastinterface.h"
#include "muticast/MutiCast.h"
#include "muticast/muticastmanager.h"
#include <QDBusMessage>
#include <QDBusMetaType>
MuticastInterface* MuticastInterface::instance = NULL;

MuticastInterface* MuticastInterface::getInstance()
{
    if(instance == NULL)
        instance = new MuticastInterface();

    return instance;
}

MuticastInterface::MuticastInterface(QObject *parent) :
    QObject(parent),
    m_dbusConnection("")
{
    qRegisterMetaType<std::string>("std::string");
}

int MuticastInterface::onCreateMuticast(const QString &mcIp,int mcPort,int udpPort)
{
    //MutiCast* pMutiCast = new MutiCast(mcIp.toStdString(),mcPort,udpPort);
    //connect(pMutiCast, SIGNAL(signalRecvComplete(int,int,int,std::string)), this, SLOT(onRecvComplete(int,int,int,std::string)));
    //connect(pMutiCast, SIGNAL(signalSendComplete(int,int,int)), this, SLOT(onSendComplete(int,int,int)));
    //return   MuticastManager::instance().createMuticastGroup(pMutiCast);
    return -1;
}

int MuticastInterface::onCreateMuticast(int netType,const QString &mcIp,int mcPort,int udpPort,const QString &groupId)
{
    int igroupId = MuticastManager::instance().getNewId();
    MutiCast* pMutiCast = new MutiCast(netType,mcIp.toStdString(),mcPort,udpPort,igroupId);
    MuticastManager::instance().createMuticastGroup(igroupId,pMutiCast);

    connect(pMutiCast, SIGNAL(signalRecvComplete(int,int,int,int,int,std::string,int)), this, SLOT(onRecvComplete(int,int,int,int,int,std::string,int)));
    connect(pMutiCast, SIGNAL(signalSendComplete(int,int,int,int,int,std::string,int)), this, SLOT(onSendComplete(int,int,int,int,int,std::string,int)));
    connect(pMutiCast, SIGNAL(singalSendFinished(bool)), this, SLOT(onSendFinished(bool)));

    m_mapGroupIds.insert(std::make_pair(igroupId,groupId));
    return   igroupId;
}
bool MuticastInterface::onInit(int groupId,QStringList groupIps)
{
    std::vector<std::string> vOtherMembers;

    for(QStringList::iterator it = groupIps.begin(); it != groupIps.end(); it++)
    {
        vOtherMembers.push_back(it->toStdString());
    }

    MutiCastGroup mutiCastGroup;
    if(!MuticastManager::instance().getMutiCastGroup(groupId,mutiCastGroup))
        return false;
    return mutiCastGroup.m_pMutiCast->init(vOtherMembers);
}
bool MuticastInterface::onStart(int groupId)
{
    MutiCastGroup mutiCastGroup;
    if(!MuticastManager::instance().getMutiCastGroup(groupId,mutiCastGroup))
        return false;
    return mutiCastGroup.m_pMutiCast->start();
}

bool MuticastInterface::onSendData(int msgtype,int fileid,int groupId,const QString fileName,const QString path)
{
    MutiCastGroup mutiCastGroup;
    if(!MuticastManager::instance().getMutiCastGroup(groupId,mutiCastGroup))
        return false;
    return mutiCastGroup.m_pMutiCast->sendData(msgtype,fileid,fileName.toStdString(),path.toStdString());
}
void MuticastInterface::onSetRecivePath(int groupId,QString recPath)
{
    MutiCastGroup mutiCastGroup;
    if(!MuticastManager::instance().getMutiCastGroup(groupId,mutiCastGroup))
        return ;
    mutiCastGroup.m_pMutiCast->setRecivePath(recPath.toStdString());
}
bool MuticastInterface::onStop(int groupId)
{
    MutiCastGroup mutiCastGroup;
    if(!MuticastManager::instance().getMutiCastGroup(groupId,mutiCastGroup))
        return false;
    return mutiCastGroup.m_pMutiCast->stop();
}
void MuticastInterface::onRecvComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,std::string recvFileName, int bIsSuccesd)
{

    QString rec = QString::fromStdString(recvFileName);

    //    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalIncomingCall");
    //    //QDBusMessage msg = QDBusMessage::createSignal("signalRecvComplete");
    //    msg << groupId << recvNum<<allNum<<rec;
    //    m_dbusConnection.send(msg);

    emit signalRecvComplete(msgtype,fileid,groupId,recvNum,allNum,rec,bIsSuccesd);

}

void MuticastInterface::onSendComplete(int msgtype,int fileid,int groupId,int sendNum,int allNum,std::string sendFileName,int bIsSuccesd)
{
    //    QDBusMessage msg = QDBusMessage::createSignal("/", "com.sinux.DBus.voip", "signalIncomingCall");
    //    msg << groupId << sendNum<<allNum;
    //    m_dbusConnection.send(msg);
    QString send = QString::fromStdString(sendFileName);
    signalSendComplete(msgtype,fileid,groupId,sendNum,allNum,send,bIsSuccesd);
}
void MuticastInterface::onSendFinished(bool bIsSuccesd)
{
    singalSendFinished(bIsSuccesd);
}

int MuticastInterface::onGetGroupId(QString groupId)
{
    std::map<int,QString>::iterator  itmap = m_mapGroupIds.begin();
    for(;itmap != m_mapGroupIds.end();++itmap)
    {
        if(itmap->second == groupId)
        {
            return itmap->first;
        }
    }
    return -1;
}
