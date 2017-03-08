#ifndef BACKGROUNDSYNCINTERFACE_H
#define BACKGROUNDSYNCINTERFACE_H

#include <QObject>
#include "../../commondata/bluetoothdata.h"
#include "../../commondata/datatype/contacterdata.h"
#include "../../commondata/datatype/groupdata.h"
#include "../../commondata/datatype/callhistorydata.h"
#include "../../commondata/datatype/messagedata.h"
#include "../../commondata/bluetoothbackgroundtransferinterface.h"

class BackgroundSyncInterface : public QObject
{
    Q_OBJECT
public:
    static BackgroundSyncInterface *getInstance();
public slots:
    bool onStart();
    qint64 onSendContacter(const int &handleType, const Contacter &contacter);
    qint64 onSendCallHistory(const int &handleType, const CallHistory &history);
    qint64 onSendGroup(const int &handleType, const Group &group);
    qint64 onSendGroupMessage(const int &handleType, const Message &groupMessage);
    qint64 onSendDb(const QString &fileName);

    qint64 onSendContacters(const int &handleType, const ContacterList &contacters);
    qint64 onSendCallHistorys(const int &handleType, const CallHistoryList &historys);
    qint64 onSendGroups(const int &handleType, const GroupList &groups);
    qint64 onSendGroupMessages(const int &handleType, const MessageList &groupMessages);

signals:
    //parse message
//    void signalContacterReceived(const int &handleType, const Contacter &contacter);
//    void signalCallHistoryReceived(const int &handleType, const CallHistory &callHistory);
//    void signalGroupReceived(const int &handleType, const Group &group);
//    void signalGroupMessageReceived(const int &handleType, const Message &groupMessage);

    void signalContacterReceived(const int &handleType, const ContacterList &contacters);
    void signalCallHistoryReceived(const int &handleType, const CallHistoryList &callHistorys);
    void signalGroupReceived(const int &handleType, const GroupList &groups);
    void signalGroupMessageReceived(const int &handleType, const MessageList &groupMessages);
    void signalDbChanged();

private slots:
    //parse message
    void onSignalDeviceDiscovered(const BluetoothAddressData &address);
    void onSignalMessageReceived(const BluetoothTransData &message);
    void onSignalDeviceConnected(const BluetoothAddressData &address);

private:
    explicit BackgroundSyncInterface(QObject *parent = 0);

    qint64 sendMessage(const QByteArray &data);

    bool initQueue();
    bool pushSendData(const QByteArray &data);
    bool popSendData();
    QByteArray frontSendData();
    bool isEmptySendData();

//    bool pushReceiveData(const QByteArray &data);
//    bool popReceiveData();
//    QByteArray frontReceiveData();
//    bool isEmptyReceiveData();

private:
    static BackgroundSyncInterface *instance;
    BluetoothBackgroundTransferInterface *m_transferInterface;
    BluetoothAddressData m_remoteServerAddr;
//    QTemporaryFile* m_dbFile;
};

#endif // BACKGROUNDSYNCINTERFACE_H
