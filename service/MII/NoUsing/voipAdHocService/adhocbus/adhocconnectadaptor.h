#ifndef ADHOCCONNECTADAPTOR_H
#define ADHOCCONNECTADAPTOR_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusError>
#include <QDebug>
#include <QDBusArgument>
#include "adhocdata.h"
#include "voipdbus.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define serviceName "org.ofono"
#define servicePath "/manet"
#define serviceInterfaceName "org.ofono.Manet"

#define ContainsBuildConnection 0


class AdhocConnectAdaptor : public QObject
{
    Q_OBJECT
//    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.connect")
public:
    explicit AdhocConnectAdaptor(QObject *parent = 0);
//    static AdhocConnectAdaptor *getInstance();
signals:

public slots:
    QString onGetDevIp();
    //主控模块给自组网模块发送链路建立请求消息
    void LinkSetup(const Stru_mcmwnu_link_setup_req &link_setup);
    void LinkSetup();
    //主控模块给自组网模块回复链路状态确认消息
    void LinkStatusConform(stru_mcmwnu_link_status_cnf link_status_cnf);
    //主控模块给自组网模块发送链路删除请求消息
    void LinkDel(stru_mcmwnu_link_delt_req link_delt_req);
    void buildConnect(const QString &srcAddr, const QString &destAddr, int port);

    void buildVideoConnect(const QString &srcAddr, const QString &destAddr, int port);

    void deleteConnect();
    void deleteConnect(stru_mcmwnu_link_status_cnf link_status_cnf);

    void deleteVideoConnect();
    void deleteVideoConnect(stru_mcmwnu_link_status_cnf link_status_cnf);


private slots:
    //自组网模块给主控模块回复链路建立响应消息
    void onLinkSetupRsp(Stru_mcmwnu_link_setup_rsp link_setup);
    //自组网模块给主控模块发送链路状态指示消息
    void onLinkStatusRsp(Ril_stru_mcmwnu_link_status_ind link_status_rsp);
    //自组网模块给主控模块发送链路删除响应消息
    void onLinkDelRsp(stru_mcmwnu_link_delt_rsp link_delt_rsp);

private:
//    static AdhocConnectAdaptor *instance;
    QDBusInterface m_interface;
};

static unsigned int iptouint( const char *ip )
{
    return ntohl( inet_addr( ip ) );
}

//static void inttoip( int ip_num, char *ip )
//{
//    strcpy( ip, (char*)inet_ntoa( htonl( ip_num ) ) );
//}

#endif // ADHOCCONNECTADAPTOR_H
