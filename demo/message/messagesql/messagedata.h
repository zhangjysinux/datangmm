#ifndef MESSAGEDATA
#define MESSAGEDATA

#include <QMetaType>
#include <QDateTime>
#include <QJsonDocument>
#include <QUuid>
#include <QJsonObject>
#include <QDataStream>

enum MessageType
{
    UNDEFINE_MESSAGE = 0,
    TEXT_MESSAGE = 1,
    IMAGE_MESSAGE = 2,
    FILE_MESSAGE = 3,
    AUDIO_MESSAGE = 4,
    VIDEO_MESSAGE = 5,
    CUSTOM_MESSAGE = 20
};

enum MessageState
{
    MESSAGE_SENDING,
    MESSAGE_SEND_FAIL,
    MESSAGE_SEND_SUCCESS,
    MESSAGE_RECEIVING,
    MESSAGE_RECEIVE_FAIL,
    MESSAGE_RECEIVE_SUCCESS,
    UNDEFINE_STATE
};

enum MessageHandle
{
    MESSAGE_ADD_HANDLE = 1,
    MESSAGE_DELETE_HANDLE = 2,
    MESSAGE_UPDATE_HANDLE = 3,
    MESSAGE_UNDEFINE_HANDLE = 4,
};

struct MessageCommon
{
    QString id;
    int type;
    int state;
    QString userId;
    QString groupId;
    QDateTime date;
    QString showMsg;
    bool isRead;
    QString accessoryPath;
    MessageCommon()
    {
        id = QUuid::createUuid().toString();
        type = UNDEFINE_MESSAGE;
        date = QDateTime::currentDateTime();
    }
};
Q_DECLARE_METATYPE(MessageCommon)

struct Message
{
    MessageCommon common;
    QJsonDocument content;
};
Q_DECLARE_METATYPE(Message)

inline QDataStream &operator<<(QDataStream &out,const Message &message)
{
    out << message.common.id << message.common.type << message.common.state
        << message.common.userId << message.common.groupId
        << message.common.date << message.common.showMsg
        << message.common.isRead << message.common.accessoryPath
        << message.content.toJson();
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Message &message)
{
    QString id;
    int type;
    int state;
    QString userId;
    QString groupId;
    QDateTime date;
    QString showMsg;
    bool isRead;
    QString accessoryPath;
    QByteArray content;
    in >> id >> type >> state >> userId >> groupId >> date
       >> showMsg >> isRead >> accessoryPath >> content;

    message.common.id = id;
    message.common.type = type;
    message.common.state = state;
    message.common.userId = userId;
    message.common.groupId = groupId;
    message.common.date = date;
    message.common.showMsg = showMsg;
    message.common.isRead = isRead;
    message.common.accessoryPath = accessoryPath;
    message.content = QJsonDocument::fromJson(content);
    return in;
}

inline QJsonDocument wrapTextMessage(const QString &text)
{
    QJsonObject object;
    object.insert("message", text);
    QJsonDocument document(object);

    return document;
}

inline QString parseTextMessage(const QJsonDocument &content)
{
    QJsonObject object = content.object();
    QString value = object.value("message").toString();
    return value;
}

#endif // MESSAGEDATA

