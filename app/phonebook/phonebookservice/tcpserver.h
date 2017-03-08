#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include "tcpserver/connection.h"
#include "tcpserver/transpondmgr.h"
class tcpserver : public QObject
{
    Q_OBJECT
public:
    explicit tcpserver(QObject *parent = 0);

signals:

public slots:
private slots:
    void onAddButtonClicked();
    void onNewServerListen(quint16 port);
    void onServerMsg(quint16 port, QString msg);

private:
    TranspondMgr m_transpondMgr;

};

#endif // TCPSERVER_H
