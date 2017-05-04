#-------------------------------------------------
#
# Project generated by QtCreator for SyberOS.
#
#-------------------------------------------------


QT += core dbus sql network  multimedia
TARGET = PointToPointService
TEMPLATE = app
target.path = /usr/bin

CONFIG += link_pkgconfig c++11
PKGCONFIG += syberos-application syberos-application-cache

INCLUDEPATH += /home/michael/SyberOS-SDK2/jail/opt/cross/armv7tnhl-meego-linux-gnueabi/sys-root/usr/include
LIBS += -lz -lrt
INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include
PKGCONFIG += libpjproject

HEADERS += interface/pointtopointinterface.h \
    datatype/commondata.h \
    fileshare.h

SOURCES += main.cpp \
	   interface/pointtopointinterface.cpp \
    fileshare.cpp
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
systemdservice.files = service/PointToPointService.service
systemdservice.path  = /usr/lib/systemd/user

INSTALLS += target systemdservice



