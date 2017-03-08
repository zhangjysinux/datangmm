#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT       += core dbus network

TEMPLATE = app
TARGET = voipPTTService
target.path = /usr/bin

DEFINES += voipPTTService

INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include

SOURCES += main.cpp \
    voipinterface.cpp \
    wavtomp3converter.cpp \
    pttmediamanager.cpp

HEADERS += \
    voipinterface.h \
    voipdbusdata.h \
    voipdbus.h \
    wavtomp3converter.h \
    pttmediamanager.h

CONFIG += debug
CONFIG += link_pkgconfig
PKGCONFIG += libpjproject

LIBS += -laudio-manager
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libmp3lame.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpj-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjlib-util-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-audiodev-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-codec-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-videodev-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjnath-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjsip-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjsip-simple-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjsip-ua-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjsua2-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjsua-armv7tnhl-meego-linux-gnueabi.a


#该文件用于systemd方式启动应用程序
systemdservice.files = service/voipPTTService.service
systemdservice.path  = /usr/lib/systemd/user

INSTALLS += target systemdservice
