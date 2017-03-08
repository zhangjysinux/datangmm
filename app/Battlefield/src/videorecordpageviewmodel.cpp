#include "videorecordpageviewmodel.h"
#include "videorecordpagemodel.h"

VideoRecordPageViewModel::VideoRecordPageViewModel(QObject *parent) :
    QObject(parent),
    m_videoRecordPageModel(NULL)
{
    m_videoRecordPageModel = new VideoRecordPageModel(this);
}

VideoRecordPageViewModel::~VideoRecordPageViewModel()
{
    if (m_videoRecordPageModel)
        delete m_videoRecordPageModel;
}

void VideoRecordPageViewModel::destroy()
{
    delete this;
}

QString VideoRecordPageViewModel::getCurrentDateTime()
{
    return m_videoRecordPageModel->getCurrentDateTime();
}
