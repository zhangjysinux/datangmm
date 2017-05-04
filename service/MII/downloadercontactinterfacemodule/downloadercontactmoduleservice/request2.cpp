#include "request2.h"
#include <QFile>
#include <QDebug>
#include <stdio.h>
//#include "curl/curl.h"
#include <string.h>
#include <ctype.h>
#include "iconv.h"
#include <iostream>
#include <QThread>

using std::cout;

Request2 * Request2::newInstance(QObject *parent)
{
    Request2 *request = new Request2(parent);
//    QThread *thread = new QThread();
//    request->moveToThread(thread);
//    request->Thread = thread;
//    thread->start();

    return request;
}

Request2::Request2(QObject *parent) :
    QThread(parent),
    stream(NULL)
{
    m_downloadProcess = new QProcess();
    connect(m_downloadProcess, SIGNAL(readyRead()), this, SLOT(onReadDownload()));
    connect(m_downloadProcess, SIGNAL(finished(int)), this, SLOT(onDownloadFinished()));
    m_downloadProcess->setReadChannel(QProcess::StandardOutput);

    // init curl
}

Request2::~Request2()
{
    wait();
//    curl_global_cleanup();
//    QThread *thread = this->thread();
//    qDebug() << "delete thread id: " << thread->currentThreadId() << Thread->currentThreadId();
//    Thread->wait();
//    Thread->deleteLater();
}

void Request2::setType(RemoteContactType type)
{
    m_type = type;
}

int Request2::getType()
{
    return m_type;
}

void Request2::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Request2::setSendFileName(const QString &fileName)
{
    m_sendFileName = fileName;
}

void Request2::setUrl(const QString &url)
{
    m_url = url;
}

//void Request2::startDownload()
//{

//}

void Request2::setLoginData(const LoginRequestData &loginRequestData)
{
    loginData = loginRequestData;
}

//void Request2::postStart()
//{

//}

//void Request2::postFileStart()
//{

//}

void Request2::startRequest()
{
    switch (requestType) {
    case RequestType_Login:
    {
        QFile::remove(m_fileName);

        QString cmd = QString("./curl -o %1 -c cookie.txt -F iris=%2 -F wifi=%3 -F bluetooth=%4 -F fingerprint=%5 -F sim=%6 -F  csrfmiddlewaretoken=%7 --cookie \"%8\" --header \"%9\" http://219.144.206.252:58000  -w %{http_code}")
                .arg(m_fileName).arg(loginData.iris).arg(loginData.wifi).arg(loginData.bluetooth).arg(loginData.fingerprint).arg(loginData.sim).arg(loginData.csrfmiddlewaretoken).arg(loginData.cookie).arg(loginData.header);
        m_downloadProcess->start(cmd);
        break;
    }
    case RequestType_Normal:
    {
        QFile::remove(m_fileName);
        QString cmd = QString("./curl -b cookie.txt -o %1 %2  -w %{http_code}").arg(m_fileName).arg(m_url);
        qDebug() << "cmd----------> " << cmd;
        m_downloadProcess->start(cmd);
        break;
    }
    case RequestType_Post:
    {
        QString postData = QString::fromLocal8Bit(loadFile(m_fileName));
        postData.replace("\"", "\'");
        QFile::remove(m_fileName);

        QString cmd = QString("./curl -b cookie.txt -o %1 -F userdata=\"%2\" %3  -w %{http_code}").arg(m_fileName).arg(postData).arg(m_url);
        qDebug() << "cmd start post : " << cmd;
        m_downloadProcess->start(cmd);
        break;
    }
    case RequestType_PostFile:
    {
        QString cmd = QString("./curl -b cookie.txt -o %1 -F file=@%2 %3  -w %{http_code}").arg(m_fileName).arg(m_sendFileName).arg(m_url);
        qDebug() << "cmd start post : " << cmd;
        m_downloadProcess->start(cmd);
        break;
    }
    case RequestType_GetPhoto:
    {
        start();
    }
    default:
        break;
    }
}

int Request2::getHttpCode()
{
    return m_statusCode;
}

void Request2::onReadDownload()
{
    if(m_downloadProcess->canReadLine())
    {
        QString lineData =  m_downloadProcess->readLine();
        QStringList processList = lineData.split("\r", QString::SkipEmptyParts);
        for(int i=0; i<processList.count(); i++)
        {
            QString process = processList.value(i);
            QStringList pList = process.split(" ", QString::SkipEmptyParts);

            double received = pList.value(3).toDouble();
            double total = pList.value(1).toDouble();
            double percent = 100 * received / total;

            qDebug() << "readline: " << pList << received << total << percent;
        }
    }
}

void Request2::onDownloadFinished()
{
    QString statusCodeStr = m_downloadProcess->readAll();
    m_statusCode = statusCodeStr.trimmed().toInt();
    qDebug() << "onDownloadFinished" << m_statusCode << statusCodeStr;

    QFile file(m_fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QString content = file.readAll();
        content = removeSpecialWord(content);
        qDebug() << "content" << content;
        file.close();

        emit signalDownloadOver(content);
    }
    else
    {
        qDebug() << "file.open error:" << file.errorString() << file.fileName();
    }
}

QString Request2::removeSpecialWord(const QString &content)
{
    QString newContent = content;
    newContent.replace(QRegExp("u\'"), "\"")
            .replace(QRegExp("\'"), "\"")
            .replace(QRegExp("u'"), "\"")
            .replace(QRegExp("'"), "\"");
    return newContent;
}

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    Request2 *out= static_cast<Request2*>(stream);
    if(out && !out->stream) {
        out->stream=fopen(out->downloadFileName.toLocal8Bit().data(), "wb");//打开文件进行写入
        if(!out->stream)
            return -1;
    }
    qDebug() << "fwrite:=----------" << size * nmemb << "Bytes";
    return fwrite(buffer, size, nmemb, out->stream);
}

static size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    int r;
    long len = 0;
    qDebug() << "data: " << (char*)ptr;
    Request2 *out= static_cast<Request2*>(stream);
    QString header = (char*)ptr;
    if(header.contains("filename=") && out)
    {
        QString fileName = header.section("filename=\"", -1);
        fileName = fileName.section("\"", 0, 0);
        QString path = "/home/user/sinux/phonebookphoto/";
        createPath(path);
        out->downloadFileName = path + fileName;
    }

    r = sscanf((char*)ptr, "Content-Length: %ld\n", &len);
    if (r) /* Microsoft: we don't read the specs */
        *((long *) stream) = len;

    return size * nmemb;
}

void Request2::run()
{
    downloadFileName.clear();

//    CURL *curl;
//    CURLcode res;
//    curl_global_init(CURL_GLOBAL_DEFAULT);
//    curl = curl_easy_init();  //初始化一个curl指针
//    if(curl) { //curl对象存在的情况下执行的操作
//        //设置远端地址
//        //         curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
//        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
//        curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
//        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
//        QString url = QString("http://219.144.206.252:58000/contact/image/?person_id=%1").arg(downloadContacterId);
//        curl_easy_setopt(curl, CURLOPT_URL, url.toLocal8Bit().data());
//        //执行写入文件流操作
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);//当有数据被写入，回调函数被调用，
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); //设置结构体的指针传递给回调函数
//        //启用时会汇报所有的信息，存放在STDERR或指定的CURLOPT_STDERR中
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//        //         struct curl_httppost *formpost = 0;
//        //         struct curl_httppost *lastptr  = 0;
//        //         curl_formadd(&formpost, &lastptr,
//        //                      CURLFORM_PTRNAME, "reqformat",
//        //                      CURLFORM_PTRCONTENTS, "plain",
//        //                      CURLFORM_END);

//        //写入文件
//        res = curl_easy_perform(curl);

//        char *ct;
//        long retcode = 0;
//        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
//        res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
//        m_statusCode = retcode;
//        //释放curl对象
//        curl_easy_cleanup(curl);
//        if(res != CURLE_OK)
//        {
//            cout << stderr << res << endl;
//        }
//        qDebug() << "close curl status code: " << m_statusCode;
//    }
//    if(stream)
//    {
//        //关闭文件流
//        fclose(stream);
//        stream = NULL;
//    }
//    //释放全局curl对象
//    curl_global_cleanup();

//    signalDownloadOver(downloadFileName);
    return ;
}

//int Request2::startLogin2()
//{
//    CURL *curl;
//    CURLcode res;
//    struct curl_slist *headers = NULL;
//    curl_global_init(CURL_GLOBAL_DEFAULT);
//    curl = curl_easy_init();  //初始化一个curl指针
//    if(curl)
//    {
//        //curl对象存在的情况下执行的操作
//        //设置远端地址
//        //初始化cookie引擎
//        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
//        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION, 1L);

//        //http请求头
//        headers = curl_slist_append(headers,"User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64; rv:25.0) Gecko/20100101 Firefox/25.0");    //模拟浏览器
//        headers = curl_slist_append(headers,"Host:passport.csdn.net");
//        headers = curl_slist_append(headers,"Accept:*/*");
//        headers = curl_slist_append(headers,"Accept-Language:zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3");
//        headers = curl_slist_append(headers,"Accept-Encoding:gzip, deflate");
//        headers = curl_slist_append(headers,"X-Requested-With:XMLHttpRequest");
//        headers = curl_slist_append(headers,"Referer:https://passport.csdn.net/account/loginbox?callback=logined&hidethird=1&from=http%3a%2f%2fwww.csdn.net%2f");
//        headers = curl_slist_append(headers,"Connection:keep-alive");

//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

//        LoginRequestData data;

//        curl_easy_setopt(curl, CURLOPT_URL, ServerAddress);
//        //执行写入文件流操作
//        //启用时会汇报所有的信息，存放在STDERR或指定的CURLOPT_STDERR中
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//        struct curl_httppost *formpost = 0;
//        struct curl_httppost *lastptr  = 0;
//        curl_formadd(&formpost, &lastptr,
//                     CURLFORM_PTRNAME, "iris",
//                     CURLFORM_PTRCONTENTS, data.iris.toLocal8Bit().data(),
//                     CURLFORM_END);
//        curl_formadd(&formpost, &lastptr,
//                     CURLFORM_PTRNAME, "wifi",
//                     CURLFORM_PTRCONTENTS, data.wifi.toLocal8Bit().data(),
//                     CURLFORM_END);
//        curl_formadd(&formpost, &lastptr,
//                     CURLFORM_PTRNAME, "bluetooth",
//                     CURLFORM_PTRCONTENTS, data.bluetooth.toLocal8Bit().data(),
//                     CURLFORM_END);
//        curl_formadd(&formpost, &lastptr,
//                     CURLFORM_PTRNAME, "fingerprint",
//                     CURLFORM_PTRCONTENTS, data.fingerprint.toLocal8Bit().data(),
//                     CURLFORM_END);
//        curl_formadd(&formpost, &lastptr,
//                     CURLFORM_PTRNAME, "sim",
//                     CURLFORM_PTRCONTENTS, data.sim.toLocal8Bit().data(),
//                     CURLFORM_END);
//        curl_formadd(&formpost, &lastptr,
//                     CURLFORM_PTRNAME, "csrfmiddlewaretoken",
//                     CURLFORM_PTRCONTENTS, data.csrfmiddlewaretoken.toLocal8Bit().data(),
//                     CURLFORM_END);
////        curl_formadd(&formpost, &lastptr,
////                     CURLFORM_PTRNAME, "",
////                     CURLFORM_PTRCONTENTS, data.csrfmiddlewaretoken.toLocal8Bit().data(),
////                     CURLFORM_END);
////        curl_formadd(&formpost, &lastptr,
////                     CURLFORM_PTRNAME, "",
////                     CURLFORM_PTRCONTENTS, data.csrfmiddlewaretoken.toLocal8Bit().data(),
////                     CURLFORM_END);
////        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
//        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");


//        //写入文件
//        res = curl_easy_perform(curl);

//        char *ct;
//        long retcode = 0;
//        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
//        res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
//        m_statusCode = retcode;
//        //释放curl对象
//        curl_easy_cleanup(curl);
//        if(res != CURLE_OK)
//        {
//            cout << stderr << res << endl;
//        }
//        qDebug() << "close curl: " << ct;
//    }
//    if(stream)
//    {
//        //关闭文件流
//        fclose(stream);
//        stream = NULL;
//    }
//    //释放全局curl对象
//    curl_global_cleanup();

//    signalDownloadOver("");
//    return 0;
//}
