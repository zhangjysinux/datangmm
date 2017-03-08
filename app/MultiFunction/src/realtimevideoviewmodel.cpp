#include "realtimevideoviewmodel.h"
#include "voipserviceadaptor.h"
#include "SharedMemory.h"
#include "screenshot.h"

RealtimeVideoViewModel::RealtimeVideoViewModel(QObject *parent) :
    QObject(parent),
    m_voip(NULL),
    m_callID(0),
    m_bVoipFirst(true),
    m_bV4L2First(true)
{
    m_contacterModel = new ContacterModel;
    notiMgr = new CNotificationManager();
    m_voip = new VoipServiceAdaptor();
    m_pSharedMemory = new SharedMemory();
    m_voip->initialize();
    connect(m_voip, SIGNAL(signalIncomingCall(int,QString)), this, SLOT(onCallInComming(int,QString)));
    connect(m_voip, SIGNAL(signalCallStatusChanged(int,bool)), this, SLOT(onCallStatusChanged(int,bool)));
    connect(m_voip, SIGNAL(signalVoipFrameData()), this, SLOT(onVoipFramData()));
    connect(m_voip, SIGNAL(signalV4l2FrameData()), this, SLOT(onV4l2FramData()));
    connect(m_voip, SIGNAL(signalCallState(int,int)), this, SLOT(onSignalCallState()));

}

RealtimeVideoViewModel::~RealtimeVideoViewModel()
{
    if (m_voip)
        delete m_voip;
}

void RealtimeVideoViewModel::onCallInComming(int callId, QString remoteIpAddr)
{
    m_isAccept = true;
    m_callID = callId;
    setCallIP(remoteIpAddr);
    emit signalCallInComing();


}

// true dis   false connected
void RealtimeVideoViewModel::onCallStatusChanged(int callId, bool isDisconnected)
{
    emit signalVideoCallStatusChanged(callId, isDisconnected);
}

void RealtimeVideoViewModel::onVoipFramData()
{
    if(m_bVoipFirst)
    {
        QString ip;
        emit signalStartVideoCall();
        m_bVoipFirst = false;
    }

    QImage image;
    bool ret = m_pSharedMemory->GetImageFromSharedMemory(image);
    if(ret)
    {
        m_screenShot->setImage(image);
    }
}

void RealtimeVideoViewModel::onSignalCallState(int callId, int callState)
{

}

void RealtimeVideoViewModel::onV4l2FramData()
{
    if(m_bV4L2First)
    {
        emit signalStartVideoCall();
        m_bV4L2First = false;
    }

    QImage image;
    bool ret = m_pSharedMemory->GetImageFromV4l2SharedMemory(image);
    //    if(ret)
    //    {
    //        m_screenShot->setImage(image);
    //    }
}

void RealtimeVideoViewModel::testthread()
{
    //    QImage image;
    //    bool ret = m_pSharedMemory->GetImageFromSharedMemory(image);
    //    if(ret)
    //    {
    //        m_screenShot->setImage(image);
    //    }
}

void RealtimeVideoViewModel::callAccept()
{
    m_bVoipFirst = true;
    m_bV4L2First = true;
    if (m_isAccept)
    {
        m_voip->answer(m_callID);
    }
}

void RealtimeVideoViewModel::callRefuse()
{
    m_voip->hangup(m_callID);
    m_isAccept = true;
}

int RealtimeVideoViewModel::callRemote(const QString &ip)
{
    m_bV4L2First = true;
    m_bVoipFirst = true;
    return m_voip->makeVideoCall(ip);
}

void RealtimeVideoViewModel::setAccept(bool isAccept)
{
    m_isAccept = isAccept;
    emit signalAcceptAbleChanged(m_isAccept);
}

bool RealtimeVideoViewModel::getAccept()
{
    return m_isAccept;
}

QString RealtimeVideoViewModel::getCallIP()
{
    return m_callIP;
}

void RealtimeVideoViewModel::setCallIP(QString callIP)
{
    m_callIP = callIP;
    emit signalCallIPChanged(callIP);
}

int RealtimeVideoViewModel::getCallId()
{
    return m_callID;
}

void RealtimeVideoViewModel::setCallId(int callId)
{
    m_callID = callId;
    emit signalCallIdChanged(callId);
}

void RealtimeVideoViewModel::setScreenShot(ScreenShot *screenShot)
{
    m_screenShot = screenShot;
}

void RealtimeVideoViewModel::getContacter(QString id)
{

    bool ok;
    Contacter contacter;
    QStringList qLocalList;
    int dec = id.toInt(&ok,10);
    ContacterInterfaceAdaptor *m_contacterAdaptor = ContacterInterfaceAdaptor::getInstance();
    if (dec == -1) {
        contacter = m_contacterAdaptor->getLocalInfo();
    } else {
        contacter = m_contacterAdaptor->getContacter(id);
    }
    if(contacter.id.isEmpty())
    {
        qDebug() << "unvalid userid:" << id;
    }
    QString phoneName;
    QString titleName;
    QString name;
    switch (contacter.title) {
    case BATTLION:
        titleName = "营长";
        break;
    case COMPANY:
        titleName = "连长";
        break;
    case PLATOON:
        titleName = "排长";
        break;
    case SQUAD:
        titleName = "班长";
        break;
    case SOLIDER:
        titleName = "战士";
        break;
    default:
        titleName = "未知";
        break;
    }
    name = contacter.surname + " " + contacter.name;

    emit signalSendNameToPersonInfo(name,contacter.photoPath);
    emit signalContacterInfo("职位",titleName);
    emit signalContacterInfo("编号",id);

    for (int i = 0;i < contacter.numbers.count();i++){
        switch (contacter.numbers[i].netType) {
        case LTE_NETWORK:
            phoneName = "LTE";
            break;
        case AD_HOC_NETWORK:
            phoneName = "自组网";
            break;
        case SATELLITE_NETWORK:
            phoneName = "天通网";
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            phoneName = "数字对讲";
            break;
        case BATTLE_NETWORK:
            phoneName = "战互网";
            break;
        case WIFI_NETWORK:
            phoneName = "WIFI";
            break;
        case UNKNOW_NETWORK:
            //break;
        default:
            phoneName = "未知网络";
            break;
        }
        emit signalContacterInfo(phoneName,contacter.numbers[i].number);
    }
    emit signalContacterInfo("设备序列号",contacter.snNumber);
}

void RealtimeVideoViewModel::getAddPersonInfo(QString searchString)
{
    QStringList role;
    role << "personName" << "position" << "size" << "id" << "contacterType";
    m_contacterModel->setRoles(role);
    m_contacterModel->setModelList(3 , searchString);
}
bool RealtimeVideoViewModel::getIsComming()
{
    return m_voip->getIsComming();
}
//void RealtimeVideoViewModel::setInCommingIp(QString InComingIp)
//{

//}
//bool RealtimeVideoViewModel::getInCommingIp()
//{

//}
