#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    ~Downloader();
    void setId(const QString &id);
    void downloadFile(const QString &fileName, const QString &remoteFileName);

signals:
    void downloadFileFinished(const QString &id, const QString &fileName);
    void downloadFileProgress(const QString &id, qint64 byteReceived,qint64 byteTotal);

public slots:
    void onDownloadProgress(qint64 byteReceived,qint64 byteTotal);
    void onReplyFinished(QNetworkReply* reply);
    void onReadyRead();

private:
    QNetworkAccessManager *m_networkAccessManager;
    QFile m_file;
    QString m_id;
};

#endif // DOWNLOADER_H
