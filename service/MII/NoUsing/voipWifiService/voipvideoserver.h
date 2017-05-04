#ifndef VOIPVIDEOSERVER_H
#define VOIPVIDEOSERVER_H
#include <QThread>
#include <pthread.h>
#define BUFFSIZE 1500000
enum iostate
{
    invaild=0,
    enable_read,
    deny_read,
};

enum FrameType
{
    remote=0,
    local,
};

struct queuedata
{
    unsigned char* buf;
    int bsize;
    iostate state;
} ;

class VoipVideoServer  : public QThread
{    Q_OBJECT
     void run() Q_DECL_OVERRIDE;
public:
    //static VoipVideoServer &instance();
    VoipVideoServer();
private:
    static VoipVideoServer *m_instance;
private:
    int m_conn;
    int m_server_sockfd;
};

#endif // VOIPVIDEOSERVER_H
