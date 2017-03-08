#ifndef VIDEOVIEWVIEWMODEL_H
#define VIDEOVIEWVIEWMODEL_H

#include <QObject>

class RecordFileModel;
class VideoViewModel;

class VideoViewViewModel : public QObject
{
    Q_OBJECT
public:
    explicit VideoViewViewModel(QObject *parent = 0);
    ~VideoViewViewModel();

    Q_INVOKABLE void updateFiles();
    Q_INVOKABLE void removeFiles();

    Q_INVOKABLE void updatePhotos();
    Q_INVOKABLE void removePhotos();

signals:
    void signalRemoveFile(bool isRemove);

public:
    RecordFileModel *m_recordFileModel;
    RecordFileModel *m_recordPhotoModel;

private:
    VideoViewModel *m_videoViewModel;

};

#endif // VIDEOVIEWVIEWMODEL_H
