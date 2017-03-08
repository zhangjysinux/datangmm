#ifndef CALLREMINDERVIEWMODEL_H
#define CALLREMINDERVIEWMODEL_H

#include <QObject>
#include "miniphonecalldata.h"

class MiniPhoneCallAdaptor;

class CallReminderViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString headImg READ getHeadImg WRITE setHeadImg NOTIFY signalHeadImgChanged)
    Q_PROPERTY(QString callInName READ getCallInName WRITE setCallInName NOTIFY signalCallInNameChanged)
    Q_PROPERTY(QString telphoneNumber READ getTelphoneNumber WRITE setTelphoneNumber NOTIFY signalTelphoneNumberChanged)

public:
    explicit CallReminderViewModel(QObject *parent = 0);
    ~CallReminderViewModel();

    QString getHeadImg();
    QString getCallInName();
    QString getTelphoneNumber();
    QString getCallHandleId();

    void setHeadImg(const QString &);
    void setCallInName(const QString &);
    void setTelphoneNumber(const QString &);
    void setCallHandleId(const QString &);

    Q_INVOKABLE void setCallInfo(const QString &img, const QString &name, const QString &tel);
    Q_INVOKABLE void callAccept();
    Q_INVOKABLE void callResume();
    Q_INVOKABLE void callComing(int type , MiniDialData mini);


signals:
    void signalHeadImgChanged(const QString &);
    void signalCallInNameChanged(const QString &);
    void signalTelphoneNumberChanged(const QString &);
    void signalCallHandleIdChanged(const QString &);

    void signalShowScreen();
    void signalHideScreen();

private:
    QString m_headImg;
    QString m_callInName;
    QString m_telphoneNumber;
    QString m_callhandleId;

    MiniPhoneCallAdaptor *m_miniAdaptor;
};

#endif // CALLREMINDERVIEWMODEL_H
