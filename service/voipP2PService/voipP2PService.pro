#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT       += core dbus network

TEMPLATE = app
TARGET = voipP2PService
target.path = /usr/bin

DEFINES += voipP2PService

INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include

SOURCES += main.cpp \
    voipinterface.cpp \
    wavtomp3converter.cpp \
    p2pmediamanager.cpp

HEADERS += \
    voipinterface.h \
    voipdbusdata.h \
    wavtomp3converter.h \
    p2pmediamanager.h \
    voipdbus.h

CONFIG += debug
CONFIG += link_pkgconfig
PKGCONFIG += libpjproject

LIBS += -laudio-manager
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libmp3lame.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-armv7tnhl-meego-linux-gnueabi.a

#该文件用于systemd方式启动应用程序
systemdservice.files = service/voipP2PService.service
systemdservice.path  = /usr/lib/systemd/user

INSTALLS += target systemdservice
