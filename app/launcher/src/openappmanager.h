#ifndef OPENAPPMANAGER_H
#define OPENAPPMANAGER_H

#include <QObject>

class OpenAppManager : public QObject
{
    Q_OBJECT
public:
    explicit OpenAppManager(QObject *parent = 0);

    bool callPhonebookApp();
    bool callCallHistoryApp();
    bool callDialApp();
    bool callGroupMessageApp();
    bool callMessageApp();
    bool callSettingApp();

private:
    bool callApp(const QString &path);
};

#endif // OPENAPPMANAGER_H
