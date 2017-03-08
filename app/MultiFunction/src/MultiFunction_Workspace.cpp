#include "MultiFunction_Workspace.h"
#include <QDebug>
#include <QtQml>
#include "videorecordpageviewmodel.h"
#include "videoviewviewmodel.h"
#include "recordfilemodel.h"
#include "realtimevideoviewmodel.h"
#include "screenshot.h"
#include "filesendmodel.h"
#include "groupfilesendmodel.h"
MultiFunction_Workspace::MultiFunction_Workspace()
    : CWorkspace()
{

    qmlRegisterType<ScreenShot>("Sinux.ScreenShot", 1, 0, "ScreenShot");

    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    // 视频录制
    m_view->rootContext()->setContextProperty("videoRecord", new VideoRecordPageViewModel());

    // 视频查看
    VideoViewViewModel *videoView = new VideoViewViewModel();
    m_view->rootContext()->setContextProperty("videoView", videoView);
    m_view->rootContext()->setContextProperty("filesModel", videoView->m_recordFileModel);

    FileSendModel* filesend = new FileSendModel();
    filesend->setRecordFileModel(videoView->GetRecordFileModel());
    m_view->rootContext()->setContextProperty("filesSendModel", filesend);

    GroupFileSendModel *groupFileSend = new GroupFileSendModel();
    groupFileSend->setRecordFileModel(videoView->GetRecordFileModel());
    m_view->rootContext()->setContextProperty("getData", groupFileSend);
    // 实时视频
    RealtimeVideoViewModel* pRealtimeVideoViewModel = new RealtimeVideoViewModel();
    m_view->rootContext()->setContextProperty("realtimeVideo", pRealtimeVideoViewModel);

    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();

    //connect(pRealtimeVideoViewModel,SIGNAL(signalCallInComing(QString)),SLOT(onVideoInCommingVideo(QString)));
    connect(pRealtimeVideoViewModel,SIGNAL(signalStartVideoCall()),SLOT(onStartVideoView()));

    QStringList role;
    role << "personName" << "position" << "size" << "id" << "contacterType";
    pRealtimeVideoViewModel->m_contacterModel->setRoles(role);
    pRealtimeVideoViewModel->m_contacterModel->setModelList(3 , "");
    m_view->rootContext()->setContextProperty("contacterModel" , pRealtimeVideoViewModel->m_contacterModel);

}

void MultiFunction_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


void MultiFunction_Workspace::onVideoInCommingVideo(QString remoteIpAddr)
{
    //    SYBEROS::CServiceApplication * app = static_cast<SYBEROS::CServiceApplication *>(QCoreApplication::instance());
    //    QString url = QString("MultiFunction:startVideoIn?callId=%1&phoneNum=%2").arg(callId).arg(remoteIpAddr);
    //    app->openUrl(url);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
}

void MultiFunction_Workspace::onStartVideoView()
{
    //m_view->setSource(QUrl("qrc:/qml/main.qml"));
    //m_view->setSource(QUrl("qrc:/qml/ContactPage.qml"));
    //openUrl();
}
