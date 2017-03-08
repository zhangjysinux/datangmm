#ifndef P2PVOICEADAPTOR_H
#define P2PVOICEADAPTOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/commondata.h"

class P2PVoiceAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit P2PVoiceAdaptor(QObject *parent = 0);
    //p2p ptt
    int onCallP2P(int network, QString addr); // returns groupId
    int onHangupP2P(int network, QString addr);
    int onGetP2PState(int network, QString addr);    // return type is PttState
    int onSetP2PState(int network, QString addr, int state); // state is of type PttState

signals:
    //p2p ptt
    void signalRecordFinished(int network, const QString &addr, const QString &fileName);
    void signalCallP2PFinished(int network, const QString &addr, int state);

private slots:
    //p2p
    void onSignalWifiRecordFinished(const QString &addr, const QString &fileName);
    void onSignalWifiCallP2PFinished(const QString &addr, int state);

    void onSignalLteRecordFinished(const QString &addr, const QString &fileName);
    void onSignalLteCallP2PFinished(const QString &addr, int state);

    void onSignalAdHocRecordFinished(const QString &addr, const QString &fileName);
    void onSignalAdHocCallP2PFinished(const QString &addr, int state);

private:
    //interface
    QDBusInterface m_interfaceP2pWifi;
    QDBusInterface m_interfaceP2pAdHoc;
    QDBusInterface m_interfaceP2pLte;
};

#endif // P2PVOICEADAPTOR_H
