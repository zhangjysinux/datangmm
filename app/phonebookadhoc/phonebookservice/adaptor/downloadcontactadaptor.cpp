#include "downloadcontactadaptor.h"
#include <QDBusReply>
#include <QDebug>

DownloadContactAdaptor *DownloadContactAdaptor::instance = NULL;

DownloadContactAdaptor *DownloadContactAdaptor::getInstance()
{
    if(instance == NULL)
        instance = new DownloadContactAdaptor();

    return instance;
}

DownloadContactAdaptor::DownloadContactAdaptor(QObject *parent) :
    QObject(parent),
    m_interface("com.sinux.backupdate", "/downloadcontact", "com.sinux.DBus.downloadcontact", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalContactersDownloadFinished",
                       this,
                       SLOT(onSignalContactersDownloadFinished(ContacterList)));
    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalGroupsDownloadFinished",
                       this,
                       SLOT(onSignalGroupsDownloadFinished(GroupList)));
    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalGroupBattlesDownloadFinished",
                       this,
                       SLOT(onSignalGroupBattlesDownloadFinished(GroupList)));
    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalGetFinished",
                       this,
                       SLOT(onSignalGetFinished(int)));
    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalPostContactFinished",
                       this,
                       SLOT(onSignalPostContactFinished(int,Contacter)));
    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalPostGroupFinished",
                       this,
                       SLOT(onSignalPostGroupFinished(int,Group)));
    sessionBus.connect("com.sinux.backupdate",
                       "/downloadcontact",
                       "com.sinux.DBus.downloadcontact",
                       "signalPostGroupBattleFinished",
                       this,
                       SLOT(onSignalPostGroupBattleFinished(int,Group)));
}

void DownloadContactAdaptor::onStartGet()
{
    QDBusReply<void> retValDbus = m_interface.call("onStartGet");
    if(retValDbus.isValid())
    {
        qDebug() << "DownloadContactAdaptor::onStartGet" ;
    }
    else
    {
        qDebug() << "DownloadContactAdaptor onStartGet reply error:" << retValDbus.error();
    }

    return;
}

void DownloadContactAdaptor::onPostContact(const Contacter &contacter)
{
    QDBusReply<void> retValDbus = m_interface.call("onPostContact",
                                                   QVariant::fromValue(contacter));
    if(retValDbus.isValid())
    {
        qDebug() << "DownloadContactAdaptor::onPostContact" ;
    }
    else
    {
        qDebug() << "DownloadContactAdaptor onPostContact reply error:" << retValDbus.error();
    }

    return;
}

void DownloadContactAdaptor::onPostGroup(const Group &group)
{
    QDBusReply<void> retValDbus = m_interface.call("onPostGroup",
                                                   QVariant::fromValue(group));
    if(retValDbus.isValid())
    {
        qDebug() << "DownloadContactAdaptor::onPostGroup" ;
    }
    else
    {
        qDebug() << "DownloadContactAdaptor onPostGroup reply error:" << retValDbus.error();
    }

    return;
}

void DownloadContactAdaptor::onPostGroupBattle(const Group &group)
{
    QDBusReply<void> retValDbus = m_interface.call("onPostGroupBattle",
                                                   QVariant::fromValue(group));
    if(retValDbus.isValid())
    {
        qDebug() << "DownloadContactAdaptor::onPostGroup" ;
    }
    else
    {
        qDebug() << "DownloadContactAdaptor onPostGroup reply error:" << retValDbus.error();
    }

    return;
}

void DownloadContactAdaptor::onSignalContactersDownloadFinished(const ContacterList &contacterList)
{
    qDebug() << "DownloadContactAdaptor::onSignalContactersDownloadFinished";
    emit signalContactersDownloadFinished(contacterList);
}

void DownloadContactAdaptor::onSignalGroupsDownloadFinished(const GroupList &groupList)
{
    qDebug() << "DownloadContactAdaptor::onSignalGroupsDownloadFinished";
    emit signalGroupsDownloadFinished(groupList);
}

void DownloadContactAdaptor::onSignalGroupBattlesDownloadFinished(const GroupList &groupList)
{
    qDebug() << "DownloadContactAdaptor::onSignalGroupBattlesDownloadFinished";
    emit signalGroupBattlesDownloadFinished(groupList);
}

void DownloadContactAdaptor::onSignalGetFinished(int status)
{
    qDebug() << "DownloadContactAdaptor::onSignalGetFinished";
    emit signalGetFinished(status);
}

void DownloadContactAdaptor::onSignalPostContactFinished(int status, const Contacter &contacter)
{
    qDebug() << "DownloadContactAdaptor::onSignalPostContactFinished";
    emit signalPostContactFinished(status, contacter);
}

void DownloadContactAdaptor::onSignalPostGroupFinished(int status, const Group &group)
{
    qDebug() << "DownloadContactAdaptor::onSignalPostGroupFinished";
    emit signalPostGroupFinished(status, group);
}

void DownloadContactAdaptor::onSignalPostGroupBattleFinished(int status, const Group &group)
{
    qDebug() << "DownloadContactAdaptor::onSignalPostGroupBattleFinished";
    emit signalPostGroupBattleFinished(status, group);
}
