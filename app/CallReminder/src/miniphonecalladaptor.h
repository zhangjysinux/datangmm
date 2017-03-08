#ifndef MINIPHONECALLADAPTOR_H
#define MINIPHONECALLADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusReply>
#include "miniphonecalldata.h"
class MiniPhoneCallAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit MiniPhoneCallAdaptor(QObject *parent = 0);
    static MiniPhoneCallAdaptor *getInstance();

    Q_INVOKABLE int dial(int net, QString phoneNum);
    Q_INVOKABLE void hangup(const QString &handId);
    Q_INVOKABLE void answer(const QString &handId);
    Q_INVOKABLE int setIsMini(bool on);  /// mini: true  fullscreen : false
    Q_INVOKABLE int showFullScreen();

signals:
    void signalSendDataTomini(int type , MiniDialData mini);

public slots:
    void onReceiveMiniData(int type , MiniDialData mini);
private:
    QDBusInterface m_interface;
    static MiniPhoneCallAdaptor *m_instance;

};

#endif // MINIPHONECALLADAPTOR_H
