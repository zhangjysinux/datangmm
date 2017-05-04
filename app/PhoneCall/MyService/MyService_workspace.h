#ifndef __MYSERVICE_WORKSPACE__
#define __MYSERVICE_WORKSPACE__


#include <SyberosServiceCache>
#include <cservice_application.h>
#include <cworkspace.h>
#include <SyberosGuiCache>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickView>
#include "dialinterface.h"
#include <csystempowermanager.h> //包含头文件
using namespace QT_DTT;
using namespace SYBEROS;

class MyService_Workspace : public CWorkspace
{
    Q_OBJECT

public:
    QQuickView *m_view;
    QObject *m_root;
    MyService_Workspace();
    DialInterface *csDbus;
    DialManager *m_dialManager;
    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);
public slots:
    void onCallHandlerIdChanged(QString dialingNumber, QString handlerId);
    void onStartMake(int net, QString phoneNum);
    void onStartInCommingVideo(QString callId, QString remoteIpAddr);
    void setDisplayState(bool state);//添加函数供qml中调用
signals:
private:
    CSystemPowerManager powerManager; //定义变量
};


#endif //__MYSERVICE_WORKSPACE__

