#include "groupmessagesenderadaptor.h"

GroupMessageSenderAdaptor::GroupMessageSenderAdaptor(QObject *parent) :
    QObject(parent)
{
}

qint64 GroupMessageSenderAdaptor::sendMessage(const Message& message)
{

}

void GroupMessageSenderAdaptor::logout()
{

}

void GroupMessageSenderAdaptor::setNetworkType(int networkType)
{
    m_networkType = networkType;
}

int GroupMessageSenderAdaptor::getNetowrkType()
{
    return m_networkType;
}
