#ifndef MINIPHONECALLDATA_H
#define MINIPHONECALLDATA_H
#include <QDBusArgument>

enum ChangeStates{

    HANDLER_ADD = 1,
    HANDLER_REMOVE = 2,
    HANDLER_UPDATA = 3,
    HANDLER_UNDEFINE = 0

};

typedef struct _miniDialData
{
    QString handlerId;
    QString dialNum;
    QString name;
    QString photoPath;
    bool isIncoming;
    int state;

public:
    _miniDialData()
    {
        handlerId = "";
        dialNum = "";
        name = "";
        photoPath = "";
        isIncoming = false;
        state = 0;
    }

    _miniDialData& operator=(const _miniDialData info)
    {
        handlerId = info.handlerId;
        dialNum = info.dialNum;
        name = info.name;
        photoPath = info.photoPath;
        isIncoming = info.isIncoming;
        state = info.state;
        return *this;
    }
}MiniDialData;
Q_DECLARE_METATYPE(MiniDialData)
inline QDBusArgument &operator<<(QDBusArgument &argument, const MiniDialData &data)
{
    argument.beginStructure();
    argument << data.handlerId << data.dialNum << data.name << data.photoPath << data.isIncoming << data.state;
    argument.endStructure();

    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, MiniDialData &data)
{
    argument.beginStructure();
    argument >> data.handlerId >> data.dialNum >> data.name >> data.photoPath >> data.isIncoming >> data.state;
    argument.endStructure();
    return argument;
}
#endif // MINIPHONECALLDATA_H
