#ifndef VIDEORECORDPAGEMODEL_H
#define VIDEORECORDPAGEMODEL_H

#include <QObject>

class VideoRecordPageModel : public QObject
{
    Q_OBJECT
public:
    explicit VideoRecordPageModel(QObject *parent = 0);

    QString getCurrentDateTime();

private:
    bool makeVideoDir();
};

#endif // VIDEORECORDPAGEMODEL_H
