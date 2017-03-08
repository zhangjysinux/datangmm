#ifndef VIDEORECORDPAGEVIEWMODEL_H
#define VIDEORECORDPAGEVIEWMODEL_H

#include <QObject>

class VideoRecordPageModel;

class VideoRecordPageViewModel : public QObject
{
    Q_OBJECT
public:
    explicit VideoRecordPageViewModel(QObject *parent = 0);
    ~VideoRecordPageViewModel();

    // 删除自己,在main.qml中的onDestroy中调用
    Q_INVOKABLE void destroy();

    Q_INVOKABLE QString getCurrentDateTime();

private:
    VideoRecordPageModel *m_videoRecordPageModel;
};

#endif // VIDEORECORDPAGEVIEWMODEL_H
