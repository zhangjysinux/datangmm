#include "MyApp_Workspace.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QUdpSocket>

MyApp_Workspace::MyApp_Workspace()
    : CWorkspace()
{
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();

//    QUdpSocket *socket = new QUdpSocket(this);
//    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
//    foreach(QNetworkInterface interface, list)
//    {
//        if(interface.name() == "wlan0")
//        {
//            QList<QNetworkAddressEntry> entryList = interface.addressEntries();
//            if(entryList.isEmpty())
//                qDebug() << "can't find wlan0";

//            foreach(QNetworkAddressEntry entry, entryList)
//            {
//                if(!entry.ip().toString().contains("wlan0"))
//                {
//                    qDebug() << "ip address:" << entry.ip().toString();
//                    //bool bindFlag = socket.bind(entry.ip(), 45454);
//                    //qDebug() << "bind flag:" << bindFlag;
//                }
//            }
//            break;
//        }
//    }

//    QByteArray datagram = "Multicast message phone1";
//    socket.writeDatagram(datagram.data(), datagram.size(), QHostAddress("224.0.0.55"), 45454);

//    udpSocket = new QUdpSocket(this);
//    bool bindFlag = udpSocket->bind(QHostAddress::AnyIPv4, 45454);
//    qDebug() << "bindflag:" << bindFlag;
//    bool joinFlag = udpSocket->joinMulticastGroup(QHostAddress("224.0.0.55"));
//    qDebug() << "joinflag:" << joinFlag;
//    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

//    datagram = "Multicast message phone2";
//    socket.writeDatagram(datagram.data(), datagram.size(), QHostAddress("224.0.0.55"), 45454);

    udpSocket = new QUdpSocket(this);
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface, list)
    {
        if(interface.name() == "wlan0")
        {
            QList<QNetworkAddressEntry> entryList = interface.addressEntries();
            if(entryList.isEmpty())
                qDebug() << "can't find wlan0";

            foreach(QNetworkAddressEntry entry, entryList)
            {
                if(!entry.ip().toString().contains("wlan0"))
                {
                    qDebug() << "ip address:" << entry.ip().toString();
                    bool bindFlag = udpSocket->bind(QHostAddress::AnyIPv4, 50000, QUdpSocket::ShareAddress);
                    qDebug() << "bind flag:" << bindFlag;
                    bool joinFlag = udpSocket->joinMulticastGroup(QHostAddress("224.0.0.55"), interface);
                    qDebug() << "join flag:" << joinFlag;
                    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
                    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
                    QByteArray datagram = "Multicast message phone" + QString::number(qrand()%100).toUtf8();
                    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress("224.0.0.55"), 50000);
                }
            }
            break;
        }
    }
}

void MyApp_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}

void MyApp_Workspace::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "receive datagram:" << datagram;
    }
}


