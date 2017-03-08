#include "groupmessagemanager.h"

inline QString transMessageDateToString(const QDateTime &date)
{
    QString value = date.toString("yyyy-MM-dd-hh-mm-ss");
    return value;
}

inline QDateTime transStringToMessageDate(const QString &date)
{
    QDateTime value = QDateTime::fromString(date, "yyyy-MM-dd-hh-mm-ss");
    return value;
}

GroupMessageManager::GroupMessageManager(const QString &groupId, QObject *parent)
    : QObject(parent), m_groupId(groupId)
{
    QString sqlTable = QString("group%1").arg(m_groupId);
    m_sqlModel.setTable(sqlTable);
    m_sqlModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_sqlModel.select();

    //to do data from contact
    m_receiver.connectSocket("239.255.43.21", 45454);
    m_sender.setSocket("239.255.43.21", 45454);

    connect(&m_receiver, SIGNAL(signalNewMessage(Message)), this, SLOT(onNewMessage(Message)));
}

quint64 GroupMessageManager::sendMessage(const Message &msg)
{
    Message sendMsg = msg;
    sendMsg.common.groupId = m_groupId;
    sendMsg.common.state = MESSAGE_SENDING;
    bool recordFlag = recordMessage(sendMsg);
    if(!recordFlag)
        return 0;
    m_sqlModel.submitAll();

    emit signalMessageChanged(MESSAGE_ADD_HANDLE, sendMsg);
    quint64 sendCount = m_sender.sendMessage(sendMsg);
    if(sendCount <= 0)
    {
        sendMsg.common.state = MESSAGE_SEND_FAIL;
        updateMessage(sendMsg);
        m_sqlModel.submitAll();
        emit signalMessageChanged(MESSAGE_UPDATE_HANDLE, sendMsg);
    }
    return sendCount;
}

QStringList GroupMessageManager::getMessageList() const
{
    QStringList msgList;
    for(int i = m_sqlModel.rowCount() - 1, iend = 0; i >= iend; i--)
    {
        QModelIndex idIndex = m_sqlModel.index(i, 0);
        QString msgId = m_sqlModel.data(idIndex).toString();
        msgList.append(msgId);
    }

    return msgList;
}

void GroupMessageManager::onNewMessage(Message msg)
{
    Message receiveMsg = msg;
    receiveMsg.common.state = MESSAGE_RECEIVING;
    if(receiveMsg.common.type == TEXT_MESSAGE)
        receiveMsg.common.state = MESSAGE_RECEIVE_SUCCESS;

    bool recordFlag = recordMessage(receiveMsg);
    if(!recordFlag)
        qDebug() << "group " << m_groupId << " record receive msg " << msg.common.id << " fail";

    m_sqlModel.submitAll();
    emit signalMessageChanged(MESSAGE_ADD_HANDLE, receiveMsg);
}

bool GroupMessageManager::recordMessage(const Message &message)
{
    int row = insertLastRow(message.common.date);
    if(row < 0)
        return false;

    QModelIndex idIndex = m_sqlModel.index(row, 0);
    m_sqlModel.setData(idIndex, message.common.id);

    QModelIndex typeIndex = m_sqlModel.index(row, 1);
    m_sqlModel.setData(typeIndex, message.common.type);

    QModelIndex stateIndex = m_sqlModel.index(row, 2);
    m_sqlModel.setData(stateIndex, message.common.state);

    QModelIndex userIdIndex = m_sqlModel.index(row, 3);
    m_sqlModel.setData(userIdIndex, message.common.userId);

    QModelIndex groupIdIndex = m_sqlModel.index(row, 4);
    m_sqlModel.setData(groupIdIndex, message.common.groupId);

    QModelIndex dateIndex = m_sqlModel.index(row, 5);
    QString dateStr = transMessageDateToString(message.common.date);
    m_sqlModel.setData(dateIndex, dateStr);

    QModelIndex isreadIndex = m_sqlModel.index(row, 6);
    m_sqlModel.setData(isreadIndex, message.common.isRead);

    QModelIndex accessoryIndex = m_sqlModel.index(row, 7);
    m_sqlModel.setData(accessoryIndex, message.common.accessoryPath);

    QModelIndex contentIndex = m_sqlModel.index(row, 8);
    m_sqlModel.setData(contentIndex, message.content.toJson());

    return true;
}

bool GroupMessageManager::updateMessage(const Message &message)
{
    int row = findMessage(message.common.id);
    if(row == -1)
        return false;

    QModelIndex typeIndex = m_sqlModel.index(row, 1);
    m_sqlModel.setData(typeIndex, message.common.type);

    QModelIndex stateIndex = m_sqlModel.index(row, 2);
    m_sqlModel.setData(stateIndex, message.common.state);

    QModelIndex userIdIndex = m_sqlModel.index(row, 3);
    m_sqlModel.setData(userIdIndex, message.common.userId);

    QModelIndex groupIdIndex = m_sqlModel.index(row, 4);
    m_sqlModel.setData(groupIdIndex, message.common.groupId);

    QModelIndex dateIndex = m_sqlModel.index(row, 5);
    QString dateStr = transMessageDateToString(message.common.date);
    m_sqlModel.setData(dateIndex, dateStr);

    QModelIndex isreadIndex = m_sqlModel.index(row, 6);
    m_sqlModel.setData(isreadIndex, message.common.isRead);

    QModelIndex accessoryIndex = m_sqlModel.index(row, 7);
    m_sqlModel.setData(accessoryIndex, message.common.accessoryPath);

    QModelIndex contentIndex = m_sqlModel.index(row, 8);
    m_sqlModel.setData(contentIndex, message.content.toJson());

    return true;
}

int GroupMessageManager::findMessage(const QString &id)
{
    int row = -1;
    for(int i = m_sqlModel.rowCount() - 1, iend = 0; i <= iend; i--)
    {
        QModelIndex idIndex = m_sqlModel.index(i, 0);
        QString idValue = m_sqlModel.data(idIndex).toString();
        if(idValue == id)
        {
            row = i;
            break;
        }
    }

    return row;
}

int GroupMessageManager::insertLastRow(const QDateTime &time)
{
    int rowCount = m_sqlModel.rowCount();
    int row = rowCount;
    //find last 10 message time
    if(rowCount > 10)
    {
        for(int i = rowCount - 10, iend = rowCount; i < iend; i++)
        {
            QModelIndex dateIndex = m_sqlModel.index(i, 4);
            QString msgDateStr = m_sqlModel.data(dateIndex).toString();
            QDateTime msgDate = transStringToMessageDate(msgDateStr);
            if(msgDate > time)
                row = i;
        }
    }

    bool insertFlag = m_sqlModel.insertRow(row);
    if(!insertFlag)
        return -1;
    else
        return row;
}

