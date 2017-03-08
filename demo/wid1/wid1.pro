#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T10:37:02
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wid1
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    src/callhistorycreator.cpp \
    src/chinesetopinyin.cpp \
    src/contactsdatacreator.cpp \
    src/datacreator.cpp \
    src/groupmessagecreator.cpp \
    src/groupscreator.cpp \
    src/viewmodel.cpp \
    src/localinformationcreator.cpp

HEADERS  += widget.h \
    src/callhistorycreator.h \
    src/chinesetopinyin.h \
    src/common.h \
    src/contactsdatacreator.h \
    src/datacreator.h \
    src/groupmessagecreator.h \
    src/groupscreator.h \
    src/viewmodel.h \
    src/localinformationcreator.h

FORMS    += widget.ui

OTHER_FILES +=

RESOURCES += \
    qrc.qrc
