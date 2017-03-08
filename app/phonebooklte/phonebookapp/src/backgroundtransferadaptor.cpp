//#include "backgroundtransferadaptor.h"
//#include <QDebug>

//BackgroundTransferAdaptor *BackgroundTransferAdaptor::instance = NULL;

//BackgroundTransferAdaptor *BackgroundTransferAdaptor::getInstance()
//{
//    if(instance == NULL)
//        instance = new BackgroundTransferAdaptor();

//    return instance;
//}

//BackgroundTransferAdaptor::BackgroundTransferAdaptor(QObject *parent) :
//    QObject(parent)
//{
//}


//quint64 BackgroundTransferAdaptor::sendMessage(const BluetoothTransData &message)
//{
//    return 0;
//}

//quint64 BackgroundTransferAdaptor::sendContacter(const Contacter &contacter, int handleType)
//{
//    BluetoothTransData data;
//    data.type = BLUETOOTHTRANSDATATYPE_CONTACTSHARE;
//    data.handleType = handleType;

//    QByteArray transData;
//    QDataStream stream(&transData, QIODevice::WriteOnly);
//    stream << contacter;

//    data.transData = transData;
//    return sendMessage(data);
//}

//quint64 BackgroundTransferAdaptor::sendCallHistory(const CallHistory &callHistory, int handleType)
//{
//    BluetoothTransData data;
//    data.type = BLUETOOTHTRANSDATATYPE_HISTORYSHARE;
//    data.handleType = handleType;

//    QByteArray transData;
//    QDataStream stream(&transData, QIODevice::WriteOnly);
//    stream << callHistory;

//    data.transData = transData;
//    return sendMessage(data);
//}

//quint64 BackgroundTransferAdaptor::sendGroup(const Group &group, int handleType)
//{
//    BluetoothTransData data;
//    data.type = BLUETOOTHTRANSDATATYPE_HISTORYSHARE;
//    data.handleType = handleType;

//    QByteArray transData;
//    QDataStream stream(&transData, QIODevice::WriteOnly);
//    stream << group;

//    data.transData = transData;
//    return sendMessage(data);
//}

//quint64 BackgroundTransferAdaptor::sendGroupMessage(const Message &message, int handleType)
//{
//    return 0;
//}

//void BackgroundTransferAdaptor::onDbusSignalMessageReceived(QDBusVariant message)
//{
//    QDBusArgument argument = message.variant().value<QDBusArgument>();
//    BluetoothTransData msg = qdbus_cast<BluetoothTransData>(argument);
////    emit signalMessageReceived(msg);

//    if(msg.type == BLUETOOTHTRANSDATATYPE_CONTACTSHARE)
//    {
//        Contacter contacter;
//        QByteArray transData = msg.transData;
//        QDataStream stream(&transData, QIODevice::ReadOnly);
//        stream >> contacter;
//        emit signalContacterReceived(contacter, msg.handleType);
//    }
//    else if(msg.type == BLUETOOTHTRANSDATATYPE_HISTORYSHARE)
//    {
//        CallHistory history;
//        QByteArray transData = msg.transData;
//        QDataStream stream(&transData, QIODevice::ReadOnly);
//        stream >> history;
//        emit signalCallHistoryReceived(history, msg.handleType);
//    }
//    else if(msg.type == BLUETOOTHTRANSDATATYPE_GROUPSHARE)
//    {
//        Group group;
//        QByteArray transData = msg.transData;
//        QDataStream stream(&transData, QIODevice::ReadOnly);
//        stream >> group;
//        emit signalGroupReceived(group, msg.handleType);
//    }
//    qDebug() << "receive from server" << msg.type << msg.transData;
//}
