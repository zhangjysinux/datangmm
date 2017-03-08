#include "battlefield_Workspace.h"
#include <QDebug>
#include <QtQml>
#include "videorecordpageviewmodel.h"
#include "videoviewviewmodel.h"
#include "recordfilemodel.h"
#include "realtimevideoviewmodel.h"


battlefield_Workspace::battlefield_Workspace()
    : CWorkspace()
{
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    // 视频录制
    m_view->rootContext()->setContextProperty("videoRecord", new VideoRecordPageViewModel());

    // 视频查看
    VideoViewViewModel *videoView = new VideoViewViewModel();
    m_view->rootContext()->setContextProperty("videoView", videoView);
    m_view->rootContext()->setContextProperty("filesModel", videoView->m_recordFileModel);
    m_view->rootContext()->setContextProperty("photoModel", videoView->m_recordPhotoModel);

    // 实时视频
    m_view->rootContext()->setContextProperty("realtimeVideo", new RealtimeVideoViewModel());

    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

void battlefield_Workspace::onLaunchComplete(Option option, const QStringList& params)
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


