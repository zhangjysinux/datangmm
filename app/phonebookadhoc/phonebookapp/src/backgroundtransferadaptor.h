//#ifndef BACKGROUNDTRANSFERADAPTOR_H
//#define BACKGROUNDTRANSFERADAPTOR_H

//#include <QObject>
//#include <QDBusInterface>
//#include <QDBusVariant>
//#include "bluetoothdata.h"
//#include "contacterdata.h"
//#include "groupdata.h"
//#include "callhistorydata.h"
//#include "messagedata.h"

//class BackgroundTransferAdaptor : public QObject
//{
//    Q_OBJECT
//public:
//    static BackgroundTransferAdaptor *getInstance();

//public slots:
//    quint64 sendMessage(const BluetoothTransData &message);
//    quint64 sendContacter(const Contacter &contacter, int handleType);
//    quint64 sendCallHistory(const CallHistory &callHistory, int handleType);
//    quint64 sendGroup(const Group &group, int handleType);
//    quint64 sendGroupMessage(const Message &message, int handleType);

//signals:
////    void signalMessageReceived(const BluetoothTransData &message);
//    void signalContacterReceived(const Contacter &contacter, int handleType);
//    void signalCallHistoryReceived(const CallHistory &callHistory, int handleType);
//    void signalGroupReceived(const Group &group, int handleType);
//    void signalGroupMessageReceived(const Message &message, int handleType);

//private slots:
//    void onDbusSignalMessageReceived(QDBusVariant message);

//private:
//    explicit BackgroundTransferAdaptor(QObject *parent = 0);

//private:
//    static BackgroundTransferAdaptor *instance;



//};

//#endif // BACKGROUNDTRANSFERADAPTOR_H
