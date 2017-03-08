#ifndef GROUPMESSAGESENDER_H
#define GROUPMESSAGESENDER_H

#include <QObject>
#include "../../../commondata/datatype/messagedata.h"

class GroupMessageSenderAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit GroupMessageSenderAdaptor(QObject *parent = 0);
    //abstract func
    virtual qint64 sendMessage(const Message& message);
    virtual void logout();

    //
    void setNetworkType(int networkType);
    int getNetowrkType();

signals:
    void signalNewMessage(const Message& message);

protected:
    int m_networkType;

};

#endif // GROUPMESSAGESENDER_H
