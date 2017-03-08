#include "videorecordpagemodel.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>

VideoRecordPageModel::VideoRecordPageModel(QObject *parent) :
    QObject(parent)
{
    makeVideoDir();
}

QString VideoRecordPageModel::getCurrentDateTime()
{
    return QDateTime::currentDateTime().toString("yyMMddhhmmss");
}

bool VideoRecordPageModel::makeVideoDir()
{
    QDir dir;
    dir.mkdir("/data/data/com.sinux.battlefield/videos");
    dir.mkdir("/data/data/com.sinux.battlefield/images");
    return true;
}
