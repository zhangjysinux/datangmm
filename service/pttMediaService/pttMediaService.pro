#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT       += core dbus

TEMPLATE = app
TARGET = pttMediaService
target.path = /usr/bin

INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include

SOURCES += main.cpp \
    pttmediamanager.cpp \
    pttmediainterface.cpp \
    wavtomp3converter.cpp

HEADERS += \
    pttmediamanager.h \
    pttmediainterface.h \
    wavtomp3converter.h

CONFIG += debug
CONFIG += link_pkgconfig
PKGCONFIG += libpjproject

LIBS += -laudio-manager
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-armv7tnhl-meego-linux-gnueabi.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libmp3lame.a

#该文件用于systemd方式启动应用程序
systemdservice.files = service/pttMediaService.service
systemdservice.path  = /usr/lib/systemd/user

INSTALLS += target systemdservice
