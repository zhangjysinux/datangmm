#ifndef _BAOTONG_DIAL_H_
#define _BAOTONG_DIAL_H_

#include <QObject>
#include <QDebug>
#include <QDBusConnection>
#include <QVariant>
#include <QTimer>
#include <QDBusVariant>
#include "../../../app/commondata/datatype/callhistorydata.h"
#include "../../../app/commondata/datatype/commondata.h"
//#include "../Common/commonDef.h"

class DialManager;

class BaotongDial : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.CommTransService.Dial")
    Q_PROPERTY(bool PttAllow READ pttAllow)
    Q_PROPERTY(bool PttGetted READ pttGetted)
    Q_PROPERTY(int network READ network)
    Q_PROPERTY(QString phoneNumber READ phoneNumber)
    Q_PROPERTY(QString calledNumber READ calledNumber)

public:
    //BaotongDial(QDBusConnection conn, DialManager *pDialManager, QObject *parent = 0);
    BaotongDial(QObject *parent = 0);
    ~BaotongDial();

public Q_SLOTS:
    void onPropertyChanged(QString name, QDBusVariant value);
    void dial(QString number, int mode = 1, QString callType = "single", QString prority = "normal");
    void answer();
    void hangup();
    void pttSet(bool on);
    bool pttAllow();
    bool pttGetted();
    int isIncoming();
    QString status();
    int network();
    QString phoneNumber();
    QString calledNumber();
    QString pttOwner();

    void test(QString num);

private Q_SLOTS:
    void dialTimeout();

Q_SIGNALS:
    void DialError(int err);
    void StateChanged(QString state);
    void PttGettedChanged(bool isGetted);

private Q_SLOTS:
    void initialize();

private:
    QVariant getProperty(QString property);

private:
    QDBusInterface *m_interface;
    //QDBusConnection m_conn;
    DialManager * m_pDialManager;

    QString m_priority;
    QString m_callState;
    bool m_pttAllow;
    bool m_pttGetted;
    QString m_phoneNumber;
    QString m_calledNumber;
    QString m_callType;
    int m_dialMode;
    QString m_callDirection;

    int m_isIncoming;
    QTimer * m_pTimer;
    NetworkType m_network;
};


#endif



