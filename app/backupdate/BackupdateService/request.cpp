#include "request.h"
#include <QFile>
#include <QDebug>

Request::Request(QObject *parent) :
    QObject(parent)
{
    m_downloadProcess = new QProcess();
    connect(m_downloadProcess, SIGNAL(readyRead()), this, SLOT(onReadDownload()));
    connect(m_downloadProcess, SIGNAL(finished(int)), this, SLOT(onDownloadFinished()));
    m_downloadProcess->setReadChannel(QProcess::StandardOutput);
}

void Request::setType(RemoteContactType type)
{
    m_type = type;
}

int Request::getType()
{
    return m_type;
}

void Request::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Request::setSendFileName(const QString &fileName)
{
    m_sendFileName = fileName;
}

void Request::setUrl(const QString &url)
{
    m_url = url;
}

void Request::start()
{
    QFile::remove(m_fileName);
    QString cmd = QString("./curl -b cookie.txt -o %1 %2  -w %{http_code}").arg(m_fileName).arg(m_url);
    qDebug() << "cmd----------> " << cmd;
    m_downloadProcess->start(cmd);
}

void Request::startLogin()
{
    QFile::remove(m_fileName);

    LoginRequestData data;
//    data.iris.clear();
//    for(int i=0; i<120*1024; i++)
//        data.iris.push_back('c');

    QString cmd = QString("./curl -o %1 -c cookie.txt -F iris=%2 -F wifi=%3 -F bluetooth=%4 -F fingerprint=%5 -F sim=%6 -F  csrfmiddlewaretoken=%7 --cookie \"%8\" --header \"%9\" http://219.144.206.252:58000  -w %{http_code}")
            .arg(m_fileName).arg(data.iris).arg(data.wifi).arg(data.bluetooth).arg(data.fingerprint).arg(data.sim).arg(data.csrfmiddlewaretoken).arg(data.cookie).arg(data.header);
    m_downloadProcess->start(cmd);
}

void Request::postStart()
{
    QString postData = QString::fromLocal8Bit(loadFile(m_fileName));
    postData.replace("\"", "\'");
    QFile::remove(m_fileName);

    QString cmd = QString("./curl -b cookie.txt -o %1 -F userdata=\"%2\" %3  -w %{http_code}").arg(m_fileName).arg(postData).arg(m_url);
    qDebug() << "cmd start post : " << cmd;
    m_downloadProcess->start(cmd);
}

void Request::postFileStart()
{
    QString cmd = QString("./curl -b cookie.txt -o %1 -F file=@%2 %3  -w %{http_code}").arg(m_fileName).arg(m_sendFileName).arg(m_url);
    qDebug() << "cmd start post : " << cmd;
    m_downloadProcess->start(cmd);
}

int Request::getHttpCode()
{
    return m_statusCode;
}

void Request::onReadDownload()
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

void Request::onDownloadFinished()
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

QString Request::removeSpecialWord(const QString &content)
{
    QString newContent = content;
    newContent.replace(QRegExp("u\'"), "\"")
            .replace(QRegExp("\'"), "\"")
            .replace(QRegExp("u'"), "\"")
            .replace(QRegExp("'"), "\"");
    return newContent;
}
