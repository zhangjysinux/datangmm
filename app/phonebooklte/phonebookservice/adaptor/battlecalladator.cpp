#include "battlecalladator.h"
#include <QDBusReply>
#include <QDebug>

#define serviceName "com.sinux.backupdate"
#define servicePath "/battlecall"
#define serviceInterface "com.sinux.DBus.battlecall"

BattleCallAdator *BattleCallAdator::instance = NULL;

BattleCallAdator *BattleCallAdator::getInstance()
{
    if(instance == NULL)
        instance = new BattleCallAdator();

    return instance;
}

BattleCallAdator::BattleCallAdator(QObject *parent) :
    QObject(parent),
    m_interface(serviceName,
                servicePath,
                serviceInterface,
                QDBusConnection::sessionBus())
{
    qDebug() << serviceInterface
             << "interface connect"
             << m_interface.isValid();
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect(serviceName,
                       servicePath,
                       serviceInterface,
                       "signalBattleCallStateChanged",
                       this,
                       SLOT(onSignalBattleCallStateChanged(QString,int)));

}

void BattleCallAdator::onSignalBattleCallStateChanged(const QString &ip, int state)
{
    emit signalBattleCallStateChanged(BATTLE_NETWORK, ip, state);
}

int BattleCallAdator::onCallBattle(const QString &addr)
{
    QDBusReply<int> dbusReply = m_interface.call("onCallBattle",
                                                 addr);
    if(dbusReply.isValid())
    {
        qDebug() << "call BattleCallAdator::onCallBattle success" << dbusReply.value();
        return dbusReply.value();
    }
    else
    {
        qDebug() << "call BattleCallAdator::onCallBattle fail";
    }
    return -1;
}

int BattleCallAdator::onHangupBattle(const QString &addr)
{
    QDBusReply<int> dbusReply = m_interface.call("onHangupBattle",
                                                 addr);
    if(dbusReply.isValid())
    {
        qDebug() << "call BattleCallAdator::onHangupBattle success" << dbusReply.value();
        return dbusReply.value();
    }
    else
    {
        qDebug() << "call BattleCallAdator::onHangupBattle fail";
    }
    return -1;
}

int BattleCallAdator::onSetBattlePttState(const QString &addr, int pttState)
{
    QDBusReply<int> dbusReply = m_interface.call("onSetBattlePttState",
                                                 addr,
                                                 pttState);
    if(dbusReply.isValid())
    {
        qDebug() << "call BattleCallAdator::onSetBattlePttState success" << dbusReply.value();
        return dbusReply.value();
    }
    else
    {
        qDebug() << "call BattleCallAdator::onSetBattlePttState fail";
    }
    return -1;
}

int BattleCallAdator::onGetBattlePttState(const QString &addr)
{
    QDBusReply<int> dbusReply = m_interface.call("onGetBattlePttState",
                                                 addr);
    if(dbusReply.isValid())
    {
        qDebug() << "call BattleCallAdator::onGetBattlePttState success" << dbusReply.value();
        return dbusReply.value();
    }
    else
    {
        qDebug() << "call BattleCallAdator::onGetBattlePttState fail";
    }
    return -1;
}

int BattleCallAdator::onSetPort(int port)
{
    QDBusReply<int> dbusReply = m_interface.call("onSetPort",
                                                 port);
    if(dbusReply.isValid())
    {
        qDebug() << "call BattleCallAdator::onSetPort success" << dbusReply.value();
        return dbusReply.value();
    }
    else
    {
        qDebug() << "call BattleCallAdator::onSetPort fail";
    }
    return -1;
}

