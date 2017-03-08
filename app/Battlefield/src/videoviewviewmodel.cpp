#include "videoviewviewmodel.h"
#include "recordfilemodel.h"
#include "videoviewmodel.h"

VideoViewViewModel::VideoViewViewModel(QObject *parent) :
    QObject(parent),
    m_recordFileModel(NULL),
    m_recordPhotoModel(NULL),
    m_videoViewModel(NULL)
{
    m_recordFileModel = new RecordFileModel(this);
    m_recordPhotoModel = new RecordFileModel(this);
    m_videoViewModel = new VideoViewModel(this);

    connect(m_recordFileModel, SIGNAL(signalRemoveFile(bool)), this, SIGNAL(signalRemoveFile(bool)));
}

VideoViewViewModel::~VideoViewViewModel()
{
    if (m_videoViewModel)
        delete m_videoViewModel;

    if (m_recordPhotoModel)
        delete m_recordPhotoModel;

    if (m_recordFileModel)
        delete m_recordFileModel;
}

void VideoViewViewModel::updateFiles()
{
    m_recordFileModel->clear();

    QList<QList<QString> > files = m_videoViewModel->getFiles("videos/");
    for (QList<QList<QString> >::iterator it = files.begin(); it != files.end(); it++)
    {
        QString fileName, fileSize;
        fileName = it->at(0);
        fileSize = it->at(1);
        m_recordFileModel->addFile(fileName, fileSize);
    }
}

void VideoViewViewModel::removeFiles()
{
    m_recordFileModel->remove(0);
}

void VideoViewViewModel::updatePhotos()
{
    m_recordPhotoModel->clear();

    QList<QList<QString> > files = m_videoViewModel->getFiles("images/");
    for (QList<QList<QString> >::iterator it = files.begin(); it != files.end(); it++)
    {
        QString fileName, fileSize;
        fileName = it->at(0);
        fileSize = it->at(1);
        m_recordPhotoModel->addFile(fileName, fileSize);
    }
}

void VideoViewViewModel::removePhotos()
{
    m_recordPhotoModel->remove(1);
}

