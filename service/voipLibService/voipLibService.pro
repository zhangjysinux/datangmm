#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:28:04
#
#-------------------------------------------------

QT       += core

TEMPLATE = app
TARGET = voipLibService
target.path = /usr/bin

#INCLUDEPATH += /srv/syberos/targets/syberos-target-armv7tnhl/usr/include

SOURCES += main.cpp

CONFIG += debug
CONFIG += link_pkgconfig
#PKGCONFIG += libpjproject

#LIBS += -laudio-manager
#LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libmp3lame.a
#LIBS += /parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpjmedia-armv7tnhl-meego-linux-gnueabi.a

#该文件用于systemd方式启动应用程序
systemdservice.files = service/voipLibService.service
systemdservice.path  = /usr/lib/systemd/user

#LIBS += -L./videolib
#INCLUDELIB += -I ./voidolib
#rings.files = rings/清晨乐章2.wav rings/朝气蓬勃2.wav
#rings.path  = /usr/share/voipService

libs.files =    videolib/libpostproc.so \
                videolib/libpostproc.so.53 \
                videolib/libpostproc.so.53.3.100 \
                videolib/libavcodec.so \
                videolib/libavcodec.so.56 \
                videolib/libavcodec.so.56.13.100 \
                videolib/libavdevice.so \
                videolib/libavdevice.so.56 \
                videolib/libavdevice.so.56.3.100 \
                videolib/libavfilter.so \
                videolib/libavfilter.so.5 \
                videolib/libavfilter.so.5.2.103 \
                videolib/libavformat.so \
                videolib/libavformat.so.56 \
                videolib/libavformat.so.56.15.102 \
                videolib/libavutil.so \
                videolib/libavutil.so.54 \
                videolib/libavutil.so.54.15.100 \
                videolib/libiconv.so.2 \
                videolib/libiconv.so.2.5.1 \
                videolib/libSDL2-2.0.so.0 \
                videolib/libSDL2-2.0.so.0.4.0 \
                videolib/libSDL2.a \
                videolib/libSDL2.la \
                videolib/libSDL2main.a \
                videolib/libSDL2.so \
                videolib/libSDL2_test.a \
                videolib/libswresample.so.1 \
                videolib/libswresample.so.1.1.100 \
                videolib/libswscale.so.3 \
                videolib/libswscale.so.3.1.101 \
                videolib/libv4l1.a \
                videolib/libv4l1.la \
                videolib/libv4l1.so \
                videolib/libv4l1.so.0 \
                videolib/libv4l1.so.0.0.0 \
                videolib/libv4l2.a \
                videolib/libv4l2.la \
                videolib/libv4l2rds.a \
                videolib/libv4l2rds.la \
                videolib/libv4l2rds.so \
                videolib/libv4l2rds.so.0 \
                videolib/libv4l2rds.so.0.0.0 \
                videolib/libv4l2.so \
                videolib/libv4l2.so.0 \
                videolib/libv4l2.so.0.0.0 \
                videolib/libv4lconvert.a \
                videolib/libv4lconvert.la \
                videolib/libv4lconvert.so \
                videolib/libv4lconvert.so.0 \
                videolib/libv4lconvert.so.0.0.0

libs.path  = /usr/lib

INSTALLS += target systemdservice libs
