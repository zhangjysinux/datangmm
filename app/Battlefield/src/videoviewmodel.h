#ifndef VIDEOVIEWMODEL_H
#define VIDEOVIEWMODEL_H

#include <QObject>
#include <QList>

class VideoViewModel : public QObject
{
    Q_OBJECT
public:
    explicit VideoViewModel(QObject *parent = 0);

    QList<QList<QString> > getFiles(const QString &path);

};

#endif // VIDEOVIEWMODEL_H
