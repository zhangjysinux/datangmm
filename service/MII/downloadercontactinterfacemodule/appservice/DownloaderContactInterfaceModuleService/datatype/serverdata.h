#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <QString>
#include <QMap>

struct ContacterServer
{
    QString id;
    QString surname;
    QString name;
    QString title;
//    QString networkid;
    QString photoid;
    QString leaders;
    QString snnumber;
//    QString passwordid;
};

struct ContacterNetworkServer
{
    QString networkid;
    QString bluetooth;
    QString lteaddress;
    QString adhocaddress;
    QString battleaddress;
    QString wifiaddress;
    QString btaddress;
    QString ttaddress;
};

struct ContacterPhotoServer
{
    QString phototid;
    QByteArray photodata;
};

struct ContacterPasswordServer
{
    QString passwordid;
    QString iris;
    QString fingerprint;
    QString password;
};

struct GroupBattleServer
{
    QString battleid;
    QString name;
    QString creator;
    QString leaders;
    QString members;
    QString address;
    QString port;
    QString network;
};

struct GroupOrganizationServer
{
    QString groupid;
    QString name;
    QString creator;
    QString leaders;
    QString members;
    QString address;
    QString port;
    QString network;
};

typedef QList<ContacterServer> ContacterServerList;
typedef QList<GroupOrganizationServer> GroupOrganizationServerList;
typedef QList<ContacterPhotoServer> ContacterPhotoServerList;
typedef QList<ContacterNetworkServer> ContacterNetworkServerList;
typedef QMap<QString,ContacterNetworkServer> IdContacterNetworkServerMap;
typedef QMap<QString,QString> IdContacterPhotoNameMap;
typedef QList<GroupBattleServer> GroupBattleServerList;

#endif // SERVERDATA_H
