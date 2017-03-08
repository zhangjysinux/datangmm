#ifndef COMMON_H
#define COMMON_H

enum TitleType
{
    BATTLION = 5,
    COMPANY = 4,
    PLATOON = 3,
    SQUAD = 2,
    SOLIDER = 1,
    UNDEFINE_LEADER = 0
};

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

#endif // COMMON_H
