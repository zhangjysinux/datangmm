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
    return QDir().mkdir("/home/user/sinux/com.sinux.MultiFunction/videos");
}
