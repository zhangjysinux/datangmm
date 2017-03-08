#ifndef DOWNLOADREQUEST_H
#define DOWNLOADREQUEST_H

#include <QObject>
#include <QProcess>
#include "../../commondata/datatype/commondata.h"
#include "request2.h"

class Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(QObject *parent = 0);
    void setType(RemoteContactType type);
    int getType();
    void setFileName(const QString &fileName);
    void setSendFileName(const QString &fileName);
    void setUrl(const QString &url);
    void start();
    void startLogin();

    void postStart();

    void postFileStart();

    int getHttpCode();

signals:
    void signalDownloadOver(const QString &content);
    void signalPostOver(const QString &result);

public slots:

private slots:
    void onReadDownload();
    void onDownloadFinished();

private:
    QProcess *m_downloadProcess;
    RemoteContactType m_type;
    QString m_fileName;
    QString m_sendFileName;
    QString m_url;
    QString removeSpecialWord(const QString &content);
    int m_statusCode;
};

#endif // DOWNLOADREQUEST_H
