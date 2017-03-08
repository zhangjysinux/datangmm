#ifndef MESSAGEDATA
#define MESSAGEDATA

#include <QMetaType>
#include <QDateTime>
#include <QJsonDocument>
#include <QUuid>
#include <QJsonObject>
#include <QDataStream>
#include <QDBusArgument>
#include <QFile>

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
    MESSAGE_SENDING = 1,
    MESSAGE_SEND_FAIL = 2,
    MESSAGE_SEND_SUCCESS = 3,
    MESSAGE_RECEIVING = 4,
    MESSAGE_RECEIVE_FAIL = 5,
    MESSAGE_RECEIVE_SUCCESS = 6,
    MESSAGE_DOWNLOADING = 7,
    MESSAGE_DOWNLOAD_SUCCESS = 8,
    MESSAGE_DOWNLOAD_FAIL = 9,
    UNDEFINE_STATE = 0
};

enum MessageHandle
{
    MESSAGE_ADD_HANDLE = 1,
    MESSAGE_DELETE_HANDLE = 2,
    MESSAGE_UPDATE_HANDLE = 3,
    MESSAGE_INIT_HANDLE = 4,
    MESSAGE_UNDEFINE_HANDLE = 5,
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
    int isRead;
    QString accessoryPath;
    MessageCommon()
    {
        id = QUuid::createUuid().toString();
        type = UNDEFINE_MESSAGE;
        date = QDateTime::currentDateTime();
        isRead = 0;
    }
};
Q_DECLARE_METATYPE(MessageCommon)

struct Message
{
    MessageCommon common;
    QByteArray content;
    QString contentName;//if it is file,video,audeo... , it will be save as to path contentName
    QString contentType;//image: png jpeg... audio: mp3 wma... video: mp4 rmvb...
    QString senderRealName;//sender file name
    quint64 fileSize;
};
typedef QList<Message> MessageList;
Q_DECLARE_METATYPE(Message)
Q_DECLARE_METATYPE(MessageList)
//Q_DECLARE_METATYPE(QList<Message>)


// Marshall the data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Message &msg)
{
    argument.beginStructure();
    QString id = msg.common.id;
    int type = msg.common.type;
    int state = msg.common.state;
    QString userId = msg.common.userId;
    QString groupId = msg.common.groupId;
    QDateTime date = msg.common.date;
    QString showMsg = msg.common.showMsg;
    int isRead = msg.common.isRead;
    QString accessoryPath = msg.common.accessoryPath;
    QByteArray content = msg.content;
    QString contentName = msg.contentName;
    QString contentType = msg.contentType;
    QString senderRealName = msg.senderRealName;
    argument << id;
    argument << type;
    argument << state;
    argument << userId;
    argument << groupId;
    argument << date;
    argument << showMsg;
    argument << isRead;
    argument << accessoryPath;
    argument << content;
    argument << contentName;
    argument << contentType;
    argument << senderRealName;
    argument.endStructure();
    return argument;
}

// Retrieve the data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Message &msg)
{
    QString id;
    int type;
    int state;
    QString userId;
    QString groupId;
    QDateTime date;
    QString showMsg;
    int isRead;
    QString accessoryPath;
    QByteArray content;
    QString contentName;
    QString contentType;
    QString senderRealName;
    argument.beginStructure();
    argument >> id;
    argument >> type;
    argument >> state;
    argument >> userId;
    argument >> groupId;
    argument >> date;
    argument >> showMsg;
    argument >> isRead;
    argument >> accessoryPath;
    argument >> content;
    argument >> contentName;
    argument >> contentType;
    argument >> senderRealName;
    argument.endStructure();

    msg.common.id = id;
    msg.common.type = type;
    msg.common.state = state;
    msg.common.userId = userId;
    msg.common.groupId = groupId;
    msg.common.date = date;
    msg.common.showMsg = showMsg;
    msg.common.isRead = isRead;
    msg.common.accessoryPath = accessoryPath;
    msg.content = content;
    msg.contentName = contentName;
    msg.contentType = contentType;
    msg.senderRealName = senderRealName;
    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const Message &message)
{
    out << message.common.id << message.common.type << message.common.state
        << message.common.userId << message.common.groupId
        << message.common.date << message.common.showMsg
        << message.common.isRead << message.common.accessoryPath
        << message.content << message.contentName << message.contentType << message.senderRealName;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Message &message)
{
    in >> message.common.id >> message.common.type >> message.common.state
        >> message.common.userId >> message.common.groupId
        >> message.common.date >> message.common.showMsg
        >> message.common.isRead >> message.common.accessoryPath
        >> message.content >> message.contentName >> message.contentType >> message.senderRealName;
    return in;
//    QString id;
//    int type;
//    int state;
//    QString userId;
//    QString groupId;
//    QDateTime date;
//    QString showMsg;
//    int isRead;
//    QString accessoryPath;
//    QByteArray content;
//    QString contentName;
//    QString contentType;
//    in >> id >> type >> state >> userId >> groupId >> date
//       >> showMsg >> isRead >> accessoryPath >> content
//       >> contentName >> contentType;

//    message.common.id = id;
//    message.common.type = type;
//    message.common.state = state;
//    message.common.userId = userId;
//    message.common.groupId = groupId;
//    message.common.date = date;
//    message.common.showMsg = showMsg;
//    message.common.isRead = isRead;
//    message.common.accessoryPath = accessoryPath;
//    message.content = content;
//    message.contentName = contentName;
//    message.contentType = contentType;

}

inline QByteArray wrapTextMessage(const QString &text)
{
//    QJsonObject object;
//    object.insert("message", text);
//    QJsonDocument document(object);

//    return document;
    return text.toLocal8Bit();
}

inline QString parseTextMessage(const QString &content)
{
//    QJsonObject object = content.object();
//    QString value = object.value("message").toString();
//    return value;
    return content;
}

inline bool isMessageNeedSaveToFile(const Message& message)
{
    if(message.common.type == IMAGE_MESSAGE ||
       message.common.type == FILE_MESSAGE  ||
       message.common.type == AUDIO_MESSAGE ||
       message.common.type == VIDEO_MESSAGE )
    {
        return true;
    }
    return false;
}

inline bool saveFile(const QByteArray& content, const QString& fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(content);
        file.flush();
        file.close();
        return true;
    }
    return false;
}

inline QByteArray loadFile(const QString& fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        return file.readAll();
    }
    return "";
}

#endif // MESSAGEDATA

