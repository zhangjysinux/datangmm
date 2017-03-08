#include "TerminalApp_workspace.h"
#include <QDebug>

TerminalApp_Workspace::TerminalApp_Workspace()
    : CWorkspace()
{
//    m_view = SYBEROS::SyberosGuiCache::qQuickView();
//    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
//    m_view->setSource(QUrl("qrc:/qml/main.qml"));
//    m_view->showFullScreen();
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    GetData *getData = new GetData;
    getData->m_contacterModel = new ContacterModel;
    getData->m_callHistoryModel = new CallHistoryModel;
    getData->m_messageModel = new MessageModel;

    QStringList role;
    role <<"name" <<"position" <<"size" <<"id" <<"type"<<"isSlect";
    getData->m_contacterModel->setRoles(role);
    getData->m_contacterModel->setModelList(0,3,"",getData->m_members);
    m_view->rootContext()->setContextProperty("contactermodel", getData->m_contacterModel);

    m_view->rootContext()->setContextProperty("getData", getData);
    m_view->rootContext()->setContextProperty("qApp",qApp);

//    PttMediaServiceAdaptor *m_pttAdaptor = new PttMediaServiceAdaptor;
//    m_view->rootContext()->setContextProperty("m_pttAdaptor", m_pttAdaptor);

    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_root = (QObject *)(m_view->rootObject());
    m_view->showFullScreen();
    getData->setView(m_view);
    getData->localPersonInfoInit();
}

void TerminalApp_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        {
            QString urlStr = params.at(0);
            QUrl url(urlStr);
            openByUrl(url);
        }
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}

void TerminalApp_Workspace::openByUrl(const QUrl& url){
    qDebug() << "----phonebook::openByUrl---------------url:" << url;
    QString scheme = url.scheme();
    QString path = url.path();
    QUrlQuery query(url.query());
    qDebug() << "---------------scheme:"<<scheme<<path<<query.toString();
    if(scheme == "sinuxbook"){
        if (path == "addContacter"){
            qDebug()<<"addContacter app  start";
            QString net = query.queryItemValue("net");
            QString number = query.queryItemValue("number");
            QMetaObject::invokeMethod(m_root, "addContacter",Q_ARG(QVariant, net),Q_ARG(QVariant, number));
        }
        else if (path == "viewContacter") {
            qDebug()<<"viewpersoninfo app  start";
            QString value = query.queryItemValue("userId");
            QMetaObject::invokeMethod(m_root, "viewContacter",Q_ARG(QVariant, value));
        }
        else if (path == "group") {
            QMetaObject::invokeMethod(m_root, "initGroup");
        }
        else if (path == "history") {
            QMetaObject::invokeMethod(m_root, "initHistory");
        }
    }
}

