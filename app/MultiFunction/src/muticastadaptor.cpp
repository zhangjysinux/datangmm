#include "muticastadaptor.h"
#include <QDBusReply>
#include <QDebug>

MutiCastAdaptor *MutiCastAdaptor::instance = NULL;

MutiCastAdaptor *MutiCastAdaptor::getInstance()
{
    if(instance == NULL)
        instance = new MutiCastAdaptor();

    return instance;
}

MutiCastAdaptor::MutiCastAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/muticast", "com.sinux.DBus.muticast", QDBusConnection::sessionBus())
{
    qRegisterMetaType<QString>("QString");
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/muticast",
                       "com.sinux.DBus.muticast",
                       "signalRecvComplete",
                       this,
                       SLOT(onRecvComplete(int ,int ,int ,QString)));
    sessionBus.connect("com.sinux.backupdate",
                       "/muticast",
                       "com.sinux.DBus.muticast",
                       "signalSendComplete",
                       this,
                       SLOT(onSendComplete(int ,int ,int)));
}

int MutiCastAdaptor::createMuticast(const QString &mcIp,int mcPort,int udpPort)
{
    int error = -1;
    QDBusReply<int> reply = m_interface.call("onCreateMuticast", mcIp,mcPort,udpPort);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onCreateMuticast reply error:" << reply.error();
    }

    return error;
}

bool MutiCastAdaptor::init(int groupId,QStringList groupIps)
{
    if(groupIps.empty())
        return false;
    bool error = true;
//    QStringList vAddress;
//    QList<QString>::iterator itr = groupIps.begin();
//    for(;itr != groupIps.end();++itr);
//    {
//        vAddress.append(*itr);
//    }
    QDBusReply<bool> reply = m_interface.call("onInit", groupId,groupIps);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onInit reply error:" << reply.error();
    }
    return error;
}

bool MutiCastAdaptor::start(int groupId)
{
    int error = -1;
    QDBusReply<bool> reply = m_interface.call("onStart", groupId);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onStart reply error:" << reply.error();
    }
    return error;
}

bool MutiCastAdaptor::sendData(int groupId,const QString fileName,const QString path)
{
    int error = -1;
    QDBusReply<bool> reply = m_interface.call("onSendData", groupId,fileName,path);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onSendData reply error:" << reply.error();
    }

    return error;
}
void MutiCastAdaptor::setRecivePath(int groupId,QString recPath)
{
//    int error = -1;
    //QDBusReply<void> reply =
            m_interface.call("onSetRecivePath", groupId,recPath);
//    if(reply.isValid())
//    {
//        error = reply.value();
//    }
//    else
//    {
//        qDebug() << "onSetRecivePath reply error:" << reply.error();
//    }
}
bool MutiCastAdaptor::stop(int groupId)
{
    int error = -1;
    QDBusReply<bool> reply = m_interface.call("onStop", groupId);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onStop reply error:" << reply.error();
    }

    return error;
}
void MutiCastAdaptor::onRecvComplete(int groupId,int recvNum,int allNum, QString recFileName)
{
    emit signalRecvComplete(groupId,recvNum,allNum,recFileName);
}
void MutiCastAdaptor::onSendComplete(int groupId,int sendNum,int allNum)
{
    emit signalSendComplete(groupId,sendNum,allNum);
}
int MutiCastAdaptor::GetGroupId(QString groupId)
{
    int error = -1;
    QDBusReply<bool> reply = m_interface.call("onGetGroupId", groupId);
    if(reply.isValid())
    {
        error = reply.value();
    }
    else
    {
        qDebug() << "onStop reply error:" << reply.error();
    }

    return error;
}
