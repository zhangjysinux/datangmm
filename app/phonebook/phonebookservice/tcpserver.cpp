#include "tcpserver.h"

tcpserver::tcpserver(QObject *parent) :
    QObject(parent)
{
    connect(&m_transpondMgr, SIGNAL(signalNewServer(quint16)),
            this, SLOT(onNewServerListen(quint16)));
    connect(&m_transpondMgr, SIGNAL(signalServerMsg(quint16,QString)),
            this, SLOT(onServerMsg(quint16, QString)));
    onAddButtonClicked();
    qDebug()<<"tcpserver start";
    qDebug()<< "tcpserver" << QThread::currentThreadId();

}

void tcpserver::onAddButtonClicked()
{
    //quint16 port = ui->portBox->value();
    m_transpondMgr.createServer(50000);
    m_transpondMgr.createServer(50001);
}

void tcpserver::onNewServerListen(quint16 port)
{
    qDebug() <<"onNewServerListen"<< port;
//    QTextBrowser *textBrowser = new QTextBrowser(ui->tabWidget);
//    ui->tabWidget->addTab(textBrowser, QString::number(port));
//    m_logHash.insert(port, textBrowser);
}

void tcpserver::onServerMsg(quint16 port, QString msg)
{
    qDebug()<<"onServerMsg" << port << msg;
//    if(!m_logHash.contains(port))
//        return;

//    QTextBrowser *textBrowser = m_logHash[port];
//    textBrowser->append(msg);
}
