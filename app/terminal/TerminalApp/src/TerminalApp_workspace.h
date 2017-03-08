#ifndef __TERMINALAPP_WORKSPACE__
#define __TERMINALAPP_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>
#include <QQmlContext>
#include <QJsonObject>
#include "../../phonebook/phonebookapp/src/getdata.h"

using namespace SYBEROS;

class TerminalApp_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;
    QObject *m_root;
public:
    TerminalApp_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);
    void openByUrl(const QUrl& url);

};


#endif //__TERMINALAPP_WORKSPACE__

