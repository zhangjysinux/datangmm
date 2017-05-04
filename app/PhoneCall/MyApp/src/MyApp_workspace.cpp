#include "MyApp_workspace.h"
#include <QDebug>
#include <QtQml>
#include "screenshot.h"
#include <QQuickItem>
MyApp_Workspace::MyApp_Workspace()
    : CWorkspace()
{

    qmlRegisterType<ScreenShot>("Sinux.ScreenShot", 1, 0, "ScreenShot");

    m_root = NULL;
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->rootContext()->setContextProperty("globalApp",	qApp);

    csDbus = DialInterfaceAdaptor::getInstance();
    m_view->rootContext()->setContextProperty("csDbus",csDbus);
    QStringList role;
    role << "personName" << "position" << "size" << "id" << "contacterType";
    csDbus->m_contacterModel = new ContacterModel;
    csDbus->m_contacterModel->setRoles(role);
    csDbus->m_contacterModel->setModelList(3 , "");
    m_view->rootContext()->setContextProperty("contacterModel" , csDbus->m_contacterModel);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_root = (QObject *)(m_view->rootObject());
    if (!csDbus->getIsMini())
        m_view->showFullScreen();
    connect(csDbus,SIGNAL(signalShowFullScreen()),this,SLOT(onShowFullScreen()));
}

void MyApp_Workspace::onLaunchComplete(Option option, const QStringList& params)
{


    qDebug() << "----appui_Workspace::onLaunchComplete------option:"  << option << ", params:" << params;
    switch (option) {
    case CWorkspace::URL:
    {
        QString urlStr = params.at(0);
        QUrl url(urlStr);
        openByUrl(url);
    }
        break;
    case CWorkspace::HOME:
        //csDbus->myDebug("home!!!!");
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
//        if(params.size() >= 3){
//            QString action = params.at(0);
//            QString mimetype = params.at(1);
//            QString file = params.at(2);
//            if(action == "share" && mimetype == "*/*"){
//                QMetaObject::invokeMethod(m_root, "entrySecondPage", Q_ARG(QVariant, file));
//            }
//        }
        break;
    case CWorkspace::MULTIDOCS:
//        if(params.size() >= 3){
//            QString action = params.at(0);
//            if(action == "share"){
//                QString text("");
//                for(int i = 1; i < params.size(); i++){
//                    text += params.at(i) + "\n";
//                }
//                QMetaObject::invokeMethod(m_root, "entrySecondPage", Q_ARG(QVariant, text));
//            }
//        }
        break;
    default:
        //QMetaObject::invokeMethod(m_root, "entryPhoneBook");
        //csDbus->onDialError(6);
        return;
        break;
    }
    csDbus->appAlreadyStart();
    csDbus->setDisplayState(true);
}

void MyApp_Workspace::open(const QStringList& params)
{
    qDebug() << "----appui_Workspace::open---------------params:" << params;
}

void MyApp_Workspace::openByUrl(const QUrl& url){
    qDebug() << "----appui_Workspace::openByUrl---------------url:" << url;
    QString scheme = url.scheme();
    QString path = url.path();
    QUrlQuery query(url.query());
    csDbus->myDebug(path);
    qDebug() << "---------------scheme:"<<scheme<<path<<query.toString();
    if(scheme == "sinuxtel"){
        if (path == "callInComing"){
            qDebug()<<"app  start";
            QString value = query.queryItemValue("value");
            QString remoteIpAddr = query.queryItemValue("remoteIpAddr");
            QMetaObject::invokeMethod(m_root, "callInComing", Q_ARG(QVariant, value),Q_ARG(QVariant, remoteIpAddr));
        }
        else if (path == "makeCall"){
            int netFlag = -1;
            QString net = query.queryItemValue("net");
            QString value = query.queryItemValue("value");
            qDebug()<<"---------------------"<<net<<value;

            if (net == "自组网" || net == "AD_HOC_NETWORK")
                netFlag = 1;
            else if (net == "LTE" || net == "LTE_NETWORK")
                netFlag = 16;
            else if (net == "天通网" || net == "SATELLITE_NETWORK")
                netFlag = 256;
            else if (net == "UNKNOW_NETWORK")
                netFlag = 0;
            else if (net == "战互网" || net == "BATTLE_NETWORK")
                netFlag = 65536;
            else if (net == "WIFI" || net == "WIFI_NETWORK")
                netFlag = 1048576;

            if (netFlag == -1)
                csDbus->onDialError(-1);
            else
            {
                qDebug() << "invokeMethod" << m_root;
                qDebug() << "invokeMethod makecall" << QMetaObject::invokeMethod(m_root, "makeCall", Q_ARG(QVariant, netFlag),Q_ARG(QVariant, value));
            }
        }
        else if (path == "startMake") {
            QString net = query.queryItemValue("net");
            QString phoneNum = query.queryItemValue("phoneNum");
            QMetaObject::invokeMethod(m_root, "startMake", Q_ARG(QVariant, net),Q_ARG(QVariant, phoneNum));
        }
        else if (path == "startVideoIn") {
            bool ok;
            int callId = query.queryItemValue("callId").toInt(&ok,10);
            QString phoneNum = query.queryItemValue("phoneNum");
            QMetaObject::invokeMethod(m_root, "startVideoIn", Q_ARG(QVariant, callId),Q_ARG(QVariant, phoneNum));
        }
        else if (path == "makeVideoCall") {
            QString phoneNum = query.queryItemValue("phoneNum");
            QString net = query.queryItemValue("net");
            int netFlag;
            if (net == "自组网" || net == "AD_HOC_NETWORK")
                netFlag = 1;
            else if (net == "LTE" || net == "LTE_NETWORK")
                netFlag = 16;
            else if (net == "天通网" || net == "SATELLITE_NETWORK")
                netFlag = 256;
            else if (net == "UNKNOW_NETWORK")
                netFlag = 0;
            else if (net == "战互网" || net == "BATTLE_NETWORK")
                netFlag = 65536;
            else if (net == "WIFI" || net == "WIFI_NETWORK")
                netFlag = 1048576;
            qDebug()<<"---------------------michael startVideoCall---------";
            QMetaObject::invokeMethod(m_root, "startVideoCall",Q_ARG(QVariant, netFlag), Q_ARG(QVariant, phoneNum));
        }
    }
}

void MyApp_Workspace::openByEvent(const QString& event, const QStringList& params){
    qDebug() << "----appui_Workspace::openByEvent---------------event:" << event<< ",params:" << params;
}

void MyApp_Workspace::openByDocument(const QString& action, const QString& mimetype, const QString& file){
    qDebug() << "----appui_Workspace::openByDocument---------------action:" << action << ",mimetype:" << mimetype << ",file:" << file;
    if(action == "share" && mimetype == "*/*"){
        QMetaObject::invokeMethod(m_root, "entrySecondPage", Q_ARG(QVariant, file));
    }
}

void MyApp_Workspace::openByMultiDocuments(const QString& action, const QStringList& filelist){
    qDebug() << "----appui_Workspace::openByMultiDocuments---------------action:" << action << ",filelist:" << filelist;
    if(action == "share"){
        QString text("");
        foreach (QString file, filelist) {
            text += file + "\n";
        }
        QMetaObject::invokeMethod(m_root, "entrySecondPage", Q_ARG(QVariant, text));
    }
}

void MyApp_Workspace::onActive(){
    qDebug() << "----appui_Workspace::onActive---------------";
//    if (csDbus->getHandlerCount() == 0 || csDbus->getMettingCount() == 0){
//        QMetaObject::invokeMethod(m_root, "entryPhoneBook");
//        qDebug()<<"onActive";
//    }
}

void MyApp_Workspace::onDeactive(){
    qDebug() << "----appui_Workspace::onDeactive---------------";
}

void MyApp_Workspace::onShowFullScreen()
{
    m_view->showFullScreen();
}


