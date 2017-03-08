#ifndef CALLOUTVIEWMODEL_H
#define CALLOUTVIEWMODEL_H

#include <QObject>
#include "miniphonecalldata.h"

class MiniPhoneCallAdaptor;

class CallOutViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY signalNameChanged)
    Q_PROPERTY(QString telphone READ getTelphone WRITE setTelphone NOTIFY signalTelphoneChanged)
    Q_PROPERTY(QString callState READ getCallState WRITE setCallState NOTIFY signalCallStateChanged)

public:
    explicit CallOutViewModel(QObject *parent = 0);
    ~CallOutViewModel();

    QString getName();
    QString getTelphone();
    QString getCallState();

    void setName(const QString &);
    void setTelphone(const QString &);
    void setCallState(const QString &);

    Q_INVOKABLE void startCallOut(int net, const QString &name, const QString &telphone);
    Q_INVOKABLE void callResume();

signals:
    void signalNameChanged(const QString &);
    void signalTelphoneChanged(const QString &);
    void signalCallStateChanged(const QString &);

    void signalShowScreen();
    void signalHideScreen();

private slots:
    void onCallStateChanged(int type , MiniDialData mini);

private:
    QString m_name;
    QString m_telphone;
    QString m_callState;
    QString m_callHandle;

    MiniPhoneCallAdaptor *m_miniAdaptor;
};

#endif // CALLOUTVIEWMODEL_H
