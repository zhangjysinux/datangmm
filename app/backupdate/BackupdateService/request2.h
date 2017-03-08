#ifndef DOWNLOADREQUEST2_H
#define DOWNLOADREQUEST2_H

#include <QObject>
#include <QProcess>
#include "../../commondata/datatype/commondata.h"
//#include <curl/curl.h>
#include <QThread>

#define ServerAddress "http://219.144.206.252:58000"

enum RemoteContactType
{
    RemoteContactType_Contacts = 0,
    RemoteContactType_Groups,
    RemoteContactType_ContacterPhoto,
    RemoteContactType_ContacterNetwork,
    RemoteContactType_ContacterPassword,
    RemoteContactType_ContacterLocal,
    RemoteContactType_Login,
    RemoteContactType_GroupsBattle,

    RemoteContactType_PostContacts,
    RemoteContactType_PostGroups,
    RemoteContactType_PostContacterPhoto,
    RemoteContactType_PostContacterNetwork,
    RemoteContactType_PostContacterPassword,
    RemoteContactType_PostGroupsBattle,
    RemoteContactType_None
};

struct LoginRequestData
{
    QString iris;
    QString fingerprint;
    QString wifi;
    QString bluetooth;
    QString sim;
    QString csrfmiddlewaretoken;
    QString cookie;
    QString header;
    LoginRequestData()
    {
        iris = "etqyqy";
        fingerprint = "rtwyewyw";
        wifi = "20:16:d8:70:57:3c";
        bluetooth = "20:16:d8:70:57:3c";
        sim = "setwtwtw";
        csrfmiddlewaretoken = "JUa1a6DkOruYlje8MKMtfubgdsxV23xs";
        cookie = "csrftoken=JUa1a6DkOruYlje8MKMtfubgdsxV23xs";
        header = "X-CSRFToken: JUa1a6DkOruYlje8MKMtfubgdsxV23xs";
    }
};

enum RequestType
{
    RequestType_Login = 0,
    RequestType_Normal,
    RequestType_Post,
    RequestType_PostFile,
    RequestType_GetPhoto
};

class Request2 : public QThread
{
    Q_OBJECT
public:
    static Request2 * newInstance(QObject *parent = 0);
    ~Request2();
    void setType(RemoteContactType type);
    int getType();
    void setFileName(const QString &fileName);
    void setSendFileName(const QString &fileName);
    void setUrl(const QString &url);
//    void startDownload();
    void setLoginData(const LoginRequestData& loginRequestData);
//    void postStart();
//    void postFileStart();
    void startRequest();

    int getHttpCode();



//    int run();
//    int startLogin2();
private:
    void run();

signals:
    void signalDownloadOver(const QString &content);
    void signalPostOver(const QString &result);

public slots:

private slots:
    void onReadDownload();
    void onDownloadFinished();

private:
    explicit Request2(QObject *parent = 0);
    QProcess *m_downloadProcess;
    RemoteContactType m_type;
    QString m_fileName;
    QString m_sendFileName;
    QString m_url;
    QString removeSpecialWord(const QString &content);
    int m_statusCode;

public:
    QString downloadFileName;
    QString downloadContacterId;
    FILE *stream;
    QThread *Thread;
    LoginRequestData loginData;
    int requestType;
};

#endif // DOWNLOADREQUEST2_H
