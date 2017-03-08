#include "videoviewmodel.h"
#include <QDir>
#include <QFileInfo>

VideoViewModel::VideoViewModel(QObject *parent) :
    QObject(parent)
{
}

QList<QList<QString> > VideoViewModel::getFiles(const QString &path)
{
    QList<QList<QString> > files;

    QDir dir(path);
    // 设置筛选项
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Reversed);

    QFileInfoList fileInfoList = dir.entryInfoList();
    for (QFileInfoList::iterator it = fileInfoList.begin(); it != fileInfoList.end(); it++)
    {
        QList<QString> file;
        file.append(it->fileName());
        file.append(QString().number(it->size() / 1024.0 / 1024.0));
        files.append(file);
    }

    return files;
}
