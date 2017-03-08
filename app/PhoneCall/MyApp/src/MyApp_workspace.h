#ifndef __MYAPP_WORKSPACE__
#define __MYAPP_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>
#include "dialinterfaceadaptor.h"
#include "tablemodel.h"
using namespace SYBEROS;

class MyApp_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;
    QObject *m_root;
    DialInterfaceAdaptor *csDbus;

public:
    MyApp_Workspace();

    void open(const QStringList& params);
    // 当应用是由openurl接口启动时，回调openByUrl
    void openByUrl(const QUrl& url);
    // 当应用是由某事件触发启动时，回调openByEvent，例如DBus触发
    void openByEvent(const QString& event, const QStringList& params);
    // 当应用是由用户希望打开某种Mimetype而启动时，回调openByDocument
    void openByDocument(const QString& action, const QString& mimetype, const QString& file);
    void openByMultiDocuments(const QString& action, const QStringList& filelist);
    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);
    // 应用在被调度到前台时回调此函数
    void onActive();
    // 应用被放置到后台时回调此函数
    void onDeactive();

public slots:
    void onShowFullScreen();
};


#endif //__MYAPP_WORKSPACE__

