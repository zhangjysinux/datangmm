#ifndef __PHONEBOOKAPP_WORKSPACE__
#define __PHONEBOOKAPP_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <QQmlContext>
#include <cgui_application.h>
#include <cworkspace.h>
#include <QJsonObject>
#include "getdata.h"


using namespace SYBEROS;

class phonebookapp_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;
    QObject *m_root;
public:
    phonebookapp_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);
    // 当应用是由openurl接口启动时，回调openByUrl
    void openByUrl(const QUrl& url);

};


#endif //__PHONEBOOKAPP_WORKSPACE__

