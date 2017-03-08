#ifndef _MUTI_CAST_H_
#define _MUTI_CAST_H_
#include<stdio.h>
#include <string>
#include <vector>
#include <QObject>
class Server;
class Client;
class MutiCast:public QObject
{
    Q_OBJECT
public:
    MutiCast(int netType,std::string mcAddr,int mcPort,int udpPort,int groupId,QObject *parent = 0);
    ~MutiCast();
    bool init(std::vector<std::string > &vOtherMembers);
    bool start();
    bool sendData(int msgtype,int fileid,std::string fileName,std::string path);
    void setRecivePath(std::string recPath);
    bool stop();
    void setGroupId(int guoupId);
    void recvComplete(int msgtype,int fileid,int recvNum,int allNum,std::string recFileName, int bIsSuccesd);
    void sendComplete(int msgtype,int fileid,int sendNum,int allNum,std::string sendFileName, int bIsSuccesd);
    void sendFinished(bool bIsSuccesd);
signals:
    void signalRecvComplete(int msgtype,int fileid,int groupId,int recvNum,int allNum,std::string recFileName, int bIsSuccesd);
    void signalSendComplete(int msgtype,int fileid,int groupId,int sendNum,int allNum,std::string sendFileName, int bIsSuccesd);
    void singalSendFinished(bool bIsSuccesd);
private:
    void setGroupIps(std::vector<std::string > &vOtherMembers);
    Server* m_pServer;
    Client* m_pClient;
    int m_iGroupId;
    int m_iNetType;
};
#endif
