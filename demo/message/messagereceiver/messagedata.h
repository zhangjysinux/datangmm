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
    SENDING,
    SEND_FAIL,
    SEND_SUCCESS,
    RECEIVING,
    RECEIVE_FAIL,
    RECEIVE_SUCCESS,
    UNDEFINE_STATE
};

struct MessageCommon
{
    QString id;
    int type;
    QString userId;
    QString groupId;
    QDateTime date;
    QString showMsg;
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
    out << message.common.id << message.common.type << message.common.userId
        << message.common.groupId << message.common.date << message.common.showMsg
        << message.content.toJson();
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Message &message)
{
    QString id;
    int type;
    QString userId;
    QString groupId;
    QDateTime date;
    QString showMsg;
    QByteArray content;
    in >> id >> type >> userId >> groupId >> date >> showMsg >> content;

    message.common.id = id;
    message.common.type = type;
    message.common.userId = userId;
    message.common.groupId = groupId;
    message.common.date = date;
    message.common.showMsg = showMsg;
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

