#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT       += core dbus

TEMPLATE = app
TARGET = voipBattleService
target.path = /usr/bin

INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include

SOURCES += main.cpp \
    voipcall.cpp \
    voipaccount.cpp \
    voipinterface.cpp \
    voipcalllistmanager.cpp \
    wavtomp3converter.cpp \
    voipvideoserver.cpp \
    pttmediamanager.cpp

HEADERS += \
    voipcall.h \
    voipaccount.h \
    voipinterface.h \
    voipdbusdata.h \
    voipdbus.h \
    voipcalllistmanager.h \
    wavtomp3converter.h \
    voipvideoserver.h \
    pttmediamanager.h

CONFIG += debug
CONFIG += link_pkgconfig
PKGCONFIG += libpjproject

LIBS += -laudio-manager
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libmp3lame.a
LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-armv7tnhl-meego-linux-gnueabi.a

#该文件用于systemd方式启动应用程序
systemdservice.files = service/voipBattleService.service
systemdservice.path  = /usr/lib/systemd/user

rings.files = rings/清晨乐章2.wav rings/朝气蓬勃2.wav
rings.path  = /usr/share/voipService

INSTALLS += target systemdservice rings
