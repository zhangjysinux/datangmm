#ifndef __ADHOC_UNIT_WORKSPACE__
#define __ADHOC_UNIT_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QUdpSocket>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>
#include "commondata.h"
#include "adhocadaptor.h"

using namespace SYBEROS;

class AdHoc_unit_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;
    QUdpSocket *m_receiveSocket;
    QUdpSocket *m_sendSocket;
    AdHocAdaptor *adhocAdaptor;
private slots:
    void onGroupChanged(int type, QT_DTT::GroupInfo info);
    void onSendMsgStateChanged(QT_DTT::GroupTransMsg msg);
    void onReceiveMsg(QT_DTT::GroupTransMsg msg);
    void onReceiveMultiCast();

public:
    AdHoc_unit_Workspace();
    Q_INVOKABLE void testAdhoc();
    Q_INVOKABLE int onSetPtt(bool on);

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

};


#endif //__ADHOC_UNIT_WORKSPACE__

