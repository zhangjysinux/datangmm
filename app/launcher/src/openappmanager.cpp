#include "openappmanager.h"
#include <QDesktopServices>
#include <QUrl>

OpenAppManager::OpenAppManager(QObject *parent) :
    QObject(parent)
{
}

bool OpenAppManager::callPhonebookApp()
{
    return callApp("sinuxbook:");
}

bool OpenAppManager::callCallHistoryApp()
{

}

bool OpenAppManager::callDialApp()
{

}

bool OpenAppManager::callGroupMessageApp()
{

}

bool OpenAppManager::callMessageApp()
{
    return callApp("sms:");
}

bool OpenAppManager::callSettingApp()
{

}

bool OpenAppManager::callApp(const QString &path)
{
    return QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}
