#ifndef _TIANTONG_DIAL_H_
#define _TIANTONG_DIAL_H_

#include <QObject>
#include <QDebug>
#include <QDBusConnection>
//#include "DialTransaction.h"

#include "voicecallmanager.h"
#include "voicecallhandler.h"
#include <QDateTime>

class TiantongDial : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.CommTransService.Dial")

public:
    TiantongDial(VoiceCallHandler *pVoiceCallHandler, QObject *parent = 0);
    ~TiantongDial();
    QDateTime startTime;
    QDateTime endTime;
    int m_dialState;

public Q_SLOTS:
    virtual void answer();
    virtual void hangup();
    virtual void hold(bool on);
    virtual void splitChannelFromConf();
    virtual void onStatusChanged();
    void onMultipartyChanged();
    QString dialNumber();
    QString handlerId();
    int network();

    virtual int status();
    virtual bool isIncoming();
    virtual bool isMultiparty();
    virtual bool isEmergency();

#if 1
Q_SIGNALS:
    void testStatusChanged();
    /**
     * @brief ͨ�����뷢���ı䣬���źű�����
     * 
     */
    void lineIdChanged();

    /**
     * @brief �෽ͨ��״̬�����ı䣬���źű�����
     * 
     */
    void multipartyChanged(QString handlerId, bool on, int network);

    /**
     * @brief ͨ��״̬�����ı�ʱ�����źű�����
     *
     */
    void statusChanged(QString handlerId, int State);

    void error(const QString &error);

    void durationChanged();
    void startedAtChanged();
#endif
private:
    int m_network;
    QString m_handlerId;
    QString m_dialNumber;
    int m_state;
    void initialize();

private:
    VoiceCallHandler *m_pVoiceCallHandler;
};


#endif



