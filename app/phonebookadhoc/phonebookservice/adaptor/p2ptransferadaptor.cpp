#include "p2ptransferadaptor.h"
#include <QDBusReply>
#include "../../../commondata/datatype/commondata.h"

P2pTransferAdaptor *P2pTransferAdaptor::instance = NULL;
P2pTransferAdaptor *P2pTransferAdaptor::getInstance()
{
    if(instance == NULL)
    {
        instance = new P2pTransferAdaptor();
    }

    return instance;
}
P2pTransferAdaptor::P2pTransferAdaptor(QObject *parent) :
    GroupMessageSenderAdaptor(parent),
    m_interface("com.sinux.backupdate", "/p2pTransfer", "com.sinux.DBus.p2pTransfer", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.backupdate",
                       "/p2pTransfer",
                       "com.sinux.DBus.p2pTransfer",
                       "signalFileSendPercent",
                       this,
                       SIGNAL(signalSendPercent(QString,qint64,qint64)));
    sessionBus.connect("com.sinux.backupdate",
                       "/p2pTransfer",
                       "com.sinux.DBus.p2pTransfer",
                       "signalFileReceivedPercent",
                       this,
                       SIGNAL(signalReceivePercent(QString,qint64,qint64)));

    sessionBus.connect("com.sinux.backupdate",
                       "/p2pTransfer",
                       "com.sinux.DBus.p2pTransfer",
                       "signalConnectStateChanged",
                       this,
                       SIGNAL(signalConnectStateChanged(QString,int)));
}

P2pTransferAdaptor::~P2pTransferAdaptor()
{
}

void P2pTransferAdaptor::setPort(const int &port)
{
    QDBusReply<void> retValDbus = m_interface.call("onSetPort", port);
    if(retValDbus.isValid())
    {
        qDebug() << "P2pTransferAdaptor::onSetPort" ;
    }
    else
        qDebug() << "P2pTransferAdaptor onSetPort reply error:" << retValDbus.error();

    return;
}

void P2pTransferAdaptor::initSocket(QString ip, int port)
{
    QDBusReply<void> retValDbus = m_interface.call("initSocket", ip, port);
    if(retValDbus.isValid())
    {
        qDebug() << "P2pTransferAdaptor::initSocket" ;
    }
    else
        qDebug() << "P2pTransferAdaptor initSocket reply error:" << retValDbus.error();

    return;
}

int P2pTransferAdaptor::sendFile(const QString &fileName)
{
    QDBusReply<int> retValDbus = m_interface.call("onSendFile", fileName);
    if(retValDbus.isValid())
    {
        qDebug() << "P2pTransferAdaptor::onSendFile" << retValDbus.value();
    }
    else
        qDebug() << "P2pTransferAdaptor onSendFile reply error:" << retValDbus.error();

    return retValDbus.value();
}

qint64 P2pTransferAdaptor::sendMessage(const Message &message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;

    if(data.isEmpty())
        qDebug() << "onSendMessage to datastream error---";

    QString fileName = "/home/user/grouptmp/groupdata"+QUuid::createUuid().toString().remove("{").remove("}").remove("-")+".dat";
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open success: " << file.fileName();
        file.write(data);
        file.flush();
        file.close();

        //file send
        sendFile(fileName);
    }
    else
    {
        qDebug() << "file open error: " << file.errorString();
    }
}

void P2pTransferAdaptor::connectToServer(QString ip)
{
    QDBusReply<void> retValDbus = m_interface.call("onConnectToServer", ip);
    if(retValDbus.isValid())
    {
        qDebug() << "P2pTransferAdaptor::onConnectToServer" ;
    }
    else
        qDebug() << "P2pTransferAdaptor onConnectToServer reply error:" << retValDbus.error();

    return;
}

//void P2pTransferAdaptor::initSocket(QString ip)
//{
//    QDBusReply<void> retValDbus = m_interface.call("initSocket", ip);
//    if(retValDbus.isValid())
//    {
//        qDebug() << "P2pTransferAdaptor::initSocket" ;
//    }
//    else
//        qDebug() << "P2pTransferAdaptor initSocket reply error:" << retValDbus.error();

//    return;
//}
