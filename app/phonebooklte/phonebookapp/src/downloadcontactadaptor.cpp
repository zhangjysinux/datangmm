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
    m_interface(contacterServiceName, "/downloadcontactadaptor", "com.sinux.DBus.downloadcontactadaptor", QDBusConnection::sessionBus())
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    sessionBus.connect(contacterServiceName,
                       "/downloadcontactadaptor",
                       "com.sinux.DBus.downloadcontactadaptor",
                       "signalContactersDownload",
                       this,
                       SLOT(onDbusSignalContactersDownload(ContacterList)));
    sessionBus.connect(contacterServiceName,
                       "/downloadcontactadaptor",
                       "com.sinux.DBus.downloadcontactadaptor",
                       "signalGroupsDownload",
                       this,
                       SLOT(onDbusSignalGroupsDownload(GroupList)));
    sessionBus.connect(contacterServiceName,
                       "/downloadcontactadaptor",
                       "com.sinux.DBus.downloadcontactadaptor",
                       "signalDownloadPercent",
                       this,
                       SLOT(onDbusSignalDownloadPercent(QString,QString)));
}

void DownloadContactAdaptor::startDownload()
{
    QDBusReply<void> removeReply = m_interface.call("onStartDownload");
    if(removeReply.isValid())
        qDebug() << "call onStartDownload success...";
    else
        qDebug() << "onRemoveGroup reply error:" << removeReply.error();
}

void DownloadContactAdaptor::onDbusSignalContactersDownload(const ContacterList& contacterList)
{
    qDebug() << "contacter list count : " << contacterList.count();
    emit signalContacterDownload(contacterList);
}

void DownloadContactAdaptor::onDbusSignalGroupsDownload(const GroupList &groupList)
{
    qDebug() << "group list count : " << groupList.count();
    emit signalGroupsDownload(groupList);
}

void DownloadContactAdaptor::onDbusSignalDownloadPercent(const QString &percent, const QString &type)
{
    qDebug() << "percent: " << percent << type;
    emit signalDownloadPercent(percent, type);
}
