TEMPLATE = app

QT += qml quick sql network
CONFIG += c++11

SOURCES += main.cpp \
    receiver.cpp \
    sender.cpp \
    messagemanager.cpp \
    groupmessagemanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    connection.h \
    messagemanager.h \
    receiver.h \
    messagedata.h \
    sender.h \
    groupmessagemanager.h

