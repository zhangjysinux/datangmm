#include "AdHoc_unit_Workspace.h"
#include <QDebug>
#include <QUdpSocket>
#include "adhocadaptor.h"
#include <QQuickView>
#include <QQuickItem>
#include <QtQml>

#define LOCAL1 "192.168.90.9"
#define LOCAL2 "192.168.90.10"
#define GROUP_ADDRESS "239.0.0.43"
#define MULTI_PORT 454545

AdHoc_unit_Workspace::AdHoc_unit_Workspace()
    : CWorkspace()
{
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    adhocAdaptor = AdHocAdaptor::getInstance();

    m_view->rootContext()->setContextProperty("myDbus", this);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

void AdHoc_unit_Workspace::testAdhoc()
{

    //----------------group unit------------//
    GroupInfo groupInfo;
    groupInfo.network = 1;//ad hoc network type
    groupInfo.id = GROUP_ADDRESS;
    groupInfo.addr = GROUP_ADDRESS;
    groupInfo.name = "Adhoc test group";
    groupInfo.creator = LOCAL1;
    groupInfo.memberSize = 2;
    for(int i = 0, iend = 2;i < iend; i++)
    {
        UserInfo uinfo;
        uinfo.userId = i + qrand()%1000;
        uinfo.network = AD_HOC_NETWORK;
        if(i == 0)
            uinfo.ad_hoc_ip = LOCAL1;
        else
            uinfo.ad_hoc_ip = LOCAL2;
        uinfo.appId = "adhoc_test";
        uinfo.optId = 123;
        uinfo.userName = "adhoc_test";
        groupInfo.baseMembers.append(uinfo);
    }
//    adhocAdaptor->testGroup();
//    adhocAdaptor->createGroupInfo(groupInfo);
    qDebug() << "creatGroup";
    //sleep(10);

//    groupInfo.name = "Adhoc test group update";
//    adhocAdaptor->updateGroupInfo(groupInfo);

//    adhocAdaptor->removeGroupInfo(groupInfo);
//    //----------------group unit------------//

//    //----------------group send------------//
//    adhocAdaptor->createGroupInfo(groupInfo);
//    qDebug() << "sendMessage";

//    GroupTransMsg msg;
//    msg.id = QUuid::createUuid().toString();
//    msg.groupId = groupInfo.id;
//    msg.name = groupInfo.name;
//    msg.network = groupInfo.network;
//    msg.addr = groupInfo.addr;
//    msg.type = MSG_TYPE_TEXT;
//    msg.starttime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//    msg.status = MSG_STATUS_SENDING;
//    msg.text = QString("hello world").toLocal8Bit();
//    msg.userCount = 2;
//    msg.baseMembers = groupInfo.baseMembers;
//    msg.threadId = 1023;
//    msg.optId = 33;
//    adhocAdaptor->sendMessage(msg);
    /*

    //----------------group send------------//

    //----------------multi cast------------//
    QHostAddress multiAddress(GROUP_ADDRESS);
    m_receiveSocket = new QUdpSocket(this);
    m_receiveSocket->bind(QHostAddress::AnyIPv4, MULTI_PORT, QUdpSocket::ShareAddress);
    m_receiveSocket->joinMulticastGroup(multiAddress);
    connect(m_receiveSocket, SIGNAL(readyRead()), this, SLOT(onReceiveMultiCast()));

    m_sendSocket = new QUdpSocket(this);
    m_sendSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 6);

    QString castStr("hello world");
    QByteArray array = castStr.toLocal8Bit();
    m_sendSocket->writeDatagram(array, multiAddress, MULTI_PORT);
    //---------------multi cast------------//
    */
}

int AdHoc_unit_Workspace::onSetPtt(bool on)
{
    if (on)
        adhocAdaptor->onSetPtt(GROUP_ADDRESS,true);
    else
        adhocAdaptor->onSetPtt(GROUP_ADDRESS,false);
}

void AdHoc_unit_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}

void AdHoc_unit_Workspace::onGroupChanged(int type, QT_DTT::GroupInfo info)
{
    CTS_PRINT << "group changed type:" << type;
    CTS_PRINT << "group changed id:" << info.id;
}

void AdHoc_unit_Workspace::onSendMsgStateChanged(QT_DTT::GroupTransMsg msg)
{
    CTS_PRINT << "on send msg state changed, msg id:" << msg.id;
    CTS_PRINT << "on send msg state changed, msg text:" << QString::fromLocal8Bit(msg.text);
}

void AdHoc_unit_Workspace::onReceiveMsg(QT_DTT::GroupTransMsg msg)
{
    CTS_PRINT << "on receive msg, msg id:" << msg.id;
    CTS_PRINT << "on receive msg, msg text:" << QString::fromLocal8Bit(msg.text);
}

void AdHoc_unit_Workspace::onReceiveMultiCast()
{
    while(m_receiveSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_receiveSocket->pendingDatagramSize());
        m_receiveSocket->readDatagram(datagram.data(), datagram.size());

        QBuffer buffer(&buffer);
        buffer.open(QIODevice::ReadOnly);

        QDataStream in(&buffer);
        QByteArray array;
        in >> array;
        buffer.close();

        QString text = QString::fromLocal8Bit(array);
        CTS_PRINT << "receive socket receive multicast:" << text;
    }
}
