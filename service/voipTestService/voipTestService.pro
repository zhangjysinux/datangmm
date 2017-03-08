#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT       += core dbus

TEMPLATE = app
TARGET = voipTestService
target.path = /usr/bin

SOURCES += main.cpp \
    voipserviceadaptor.cpp \
    voipcallhandler.cpp \
    voipcallmanagerlte.cpp \
    voipcallmanageradhoc.cpp \
    pttmediaserviceadaptor.cpp

HEADERS += \
    voipserviceadaptor.h \
    voipdbusdata.h \
    voipcallhandler.h \
    voipcallmanagerlte.h \
    voipcallmanageradhoc.h \
    pttmediaserviceadaptor.h

#该文件用于systemd方式启动应用程序
systemdservice.files = service/voipTestService.service
systemdservice.path  = /usr/lib/systemd/user

INSTALLS += target systemdservice
