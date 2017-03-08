#include "downloader.h"
#include <QUrl>
#include <QNetworkReply>

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    m_networkAccessManager=new QNetworkAccessManager(this);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReplyFinished(QNetworkReply*)));
}

Downloader::~Downloader()
{
    qDebug() << "download deleted";
}

void Downloader::setId(const QString &id)
{
    m_id = id;
}

void Downloader::downloadFile(const QString &fileName, const QString &remoteFileName)
{
    m_file.setFileName(fileName);
    if(!m_file.open(QIODevice::WriteOnly))
    {
        qDebug() << "open download file name error: " << m_file.fileName();
    }
    else
    {
        qDebug() << "open download file name success: " << m_file.fileName();
    }

    QUrl url(remoteFileName);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkAccessManager->get(request);//通过发送数据，返回值保存在reply指针里.
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));//download文件时进度
    connect(reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
}

void Downloader::onDownloadProgress(qint64 byteReceived, qint64 byteTotal)
{
    qDebug() << "onDownloadProcess" << byteReceived << byteTotal;
    emit downloadFileProgress(m_id, byteReceived, byteTotal);
    if(byteReceived == byteTotal)
    {
        if(m_file.isOpen())
        {
            m_file.close();
        }
        emit downloadFileFinished(m_id, m_file.fileName());
    }
}

void Downloader::onReplyFinished(QNetworkReply *reply)
{
    qDebug() << "Downloader::onReplyFinished" << reply;
}

void Downloader::onReadyRead()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(this->sender());

    QByteArray bytes = reply->readAll();
    if(m_file.isOpen())
    {
        m_file.write(bytes);
        qDebug() << "m_file.write " << bytes.size();
    }
}
