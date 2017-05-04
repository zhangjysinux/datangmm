#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT += core dbus sql network  multimedia
QT += bluetooth

TEMPLATE = app
TARGET = bluetoothModuleService
target.path = /usr/bin

CONFIG += link_pkgconfig c++11 
PKGCONFIG += syberos-application syberos-application-cache

INCLUDEPATH += /home/michael/SyberOS-SDK2/jail/opt/cross/armv7tnhl-meego-linux-gnueabi/sys-root/usr/include
LIBS += -lz -lrt
INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include
PKGCONFIG += libpjproject

SOURCES += main.cpp \
    interface/bluetoothinterface2.cpp

HEADERS += \
    interface/bluetoothinterface2.h \
    datatype/commondata.h \
    datatype/bluetoothdata.h

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
systemdservice.files = service/bluetoothModuleService.service
systemdservice.path  = /usr/lib/systemd/user

INSTALLS += target systemdservice
