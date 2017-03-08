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
#include <QDebug>

enum MessageType
{
    UNDEFINE_MESSAGE = 0,
    TEXT_MESSAGE = 1,
    IMAGE_MESSAGE = 2,
    FILE_MESSAGE = 3,
    AUDIO_MESSAGE = 4,
    VIDEO_MESSAGE = 5,
    SYSTEM_MESSAGE = 6,
    ORDER_MESSAGE = 7,
    LOCATION_MESSAGE = 8,            // 位置
    LOCATION_MESSAGE_BACKGROUND = 9, //不显示
    LOCATION_MESSAGE_FILE = 10,      //态势文件
    CUSTOM_MESSAGE = 20
};
;
enum MessageState
{
    MESSAGE_SENDING = 1,
    MESSAGE_SEND_FAIL = 2,
    MESSAGE_SEND_SUCCESS = 3,
    MESSAGE_RECEIVING = 4,
    MESSAGE_RECEIVE_FAIL = 5,
    MESSAGE_RECEIVE_SUCCESS = 6,
    MESSAGE_DOWNLOAD_READY = 7,
    MESSAGE_DOWNLOADING = 8,
    MESSAGE_DOWNLOAD_SUCCESS = 9,
    MESSAGE_DOWNLOAD_FAIL = 10,
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

    int networkType;
    QString addr;
    int optId;

    //FOR LOCATION
    QString longitude;
    QString latitude;
    QString height;
    QByteArray locationArray;
};
typedef QList<Message> MessageList;
Q_DECLARE_METATYPE(Message)
Q_DECLARE_METATYPE(MessageList)
//Q_DECLARE_METATYPE(QList<Message>)

struct GroupChange
{
    QString groupId;
    int groupType;
};

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

    quint64 fileSize = msg.fileSize;
    int networkType = msg.networkType;
    QString addr = msg.addr;
    int optId = msg.optId;

    //FOR LOCATION
    QString longitude = msg.longitude;
    QString latitude = msg.latitude;
    QString height = msg.height;
    QByteArray locationArray = msg.locationArray;

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

    argument << fileSize;
    argument << networkType;
    argument << addr;
    argument << optId;
    argument << longitude;
    argument << latitude;
    argument << height;
    argument << locationArray;
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

    quint64 fileSize;
    int networkType;
    QString addr;
    int optId;
    //FOR LOCATION
    QString longitude;
    QString latitude;
    QString height;
    QByteArray locationArray;

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

    argument >> fileSize;
    argument >> networkType;
    argument >> addr;
    argument >> optId;
    argument >> longitude;
    argument >> latitude;
    argument >> height;
    argument >> locationArray;
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
    msg.fileSize = fileSize;
    msg.networkType = networkType;
    msg.addr = addr;
    msg.optId = optId;
    msg.longitude = longitude;
    msg.latitude = latitude;
    msg.height = height;
    msg.locationArray = locationArray;
    return argument;
}

inline QDataStream &operator<<(QDataStream &out,const Message &message)
{
    out << message.common.id << message.common.type << message.common.state
        << message.common.userId << message.common.groupId
        << message.common.date << message.common.showMsg
        << message.common.isRead << message.common.accessoryPath
        << message.content << message.contentName << message.contentType << message.senderRealName
        << message.fileSize << message.networkType << message.addr
        << message.optId
        << message.longitude << message.latitude << message.height << message.locationArray;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Message &message)
{
    in >> message.common.id >> message.common.type >> message.common.state
        >> message.common.userId >> message.common.groupId
        >> message.common.date >> message.common.showMsg
        >> message.common.isRead >> message.common.accessoryPath
        >> message.content >> message.contentName >> message.contentType >> message.senderRealName
        >> message.fileSize >> message.networkType >> message.addr
        >> message.optId
        >> message.longitude >> message.latitude >> message.height >> message.locationArray;
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

inline QByteArray convertGroupChangeToString(const GroupChange &groupChange)
{
    QJsonObject object;
    object.insert("groupId", groupChange.groupId);
    object.insert("groupType", groupChange.groupType);
    QJsonDocument document(object);
    QByteArray data = document.toJson();

    return data;
}

inline GroupChange convertStringToGroupChange(const QByteArray str)
{
    GroupChange groupChange;
    QJsonDocument document = QJsonDocument::fromJson(str);
    QJsonObject object = document.object();
    groupChange.groupId = object.value("groupId").toString();
    groupChange.groupType = object.value("groupType").toInt();

    return groupChange;
}

#endif // MESSAGEDATA

