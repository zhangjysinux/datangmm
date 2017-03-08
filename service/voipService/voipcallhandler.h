#ifndef VOIPCALLHANDLER_H
#define VOIPCALLHANDLER_H

#include <QObject>
#include "../../../app/commondata/datatype/callhistorydata.h"
#include "../../../app/commondata/datatype/commondata.h"
class VoipCallHandler : public QObject
{
    Q_OBJECT

    Q_ENUMS(VoipCallStatus)

    Q_PROPERTY(QString handlerId READ handlerId CONSTANT)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lineId READ lineId CONSTANT)
    Q_PROPERTY(bool isIncoming READ isIncoming CONSTANT)
    Q_PROPERTY(bool isMultiparty READ isMultiparty NOTIFY multipartyChanged)
    Q_PROPERTY(int netWork READ netWork CONSTANT)

public:
    enum VoipCallStatus {
        STATUS_NULL,
        STATUS_ACTIVE,
        STATUS_HELD,
        STATUS_DIALING,
        STATUS_ALERTING,
        STATUS_INCOMING,
        STATUS_WAITING,
        STATUS_DISCONNECTED
    };

    explicit VoipCallHandler(bool isLte, int callId, bool isIncoming, const QString &lineId = QString(),NetworkType netWork = UNKNOW_NETWORK, QObject *parent = 0);

    QString handlerId() const;
    int status() const;
    QString lineId() const;
    bool isIncoming() const;
    bool isMultiparty() const;
    bool isLte() const;
    NetworkType netWork() const;
    void SetVideo(bool bIsVideo);//true:video false:audio
    bool isVideo();
    int getCallError();
Q_SIGNALS:
    void statusChanged();
    void multipartyChanged();

public Q_SLOTS:
    void answer();
    void hangup();
    void hold(bool on);

private:
    void emitStatusChangedSignal();
    void emitMultipartyChanged();

    bool m_isLte;
    int m_callId;
    const bool m_isIncoming;
    const QString m_lineId;
    NetworkType m_netWork;
    bool m_isVideo;
//    friend class VoipCallManagerAdHoc;
//    friend class VoipCallManagerLte;
    friend class VoipCallManager;
};

#endif // VOIPCALLHANDLER_H
