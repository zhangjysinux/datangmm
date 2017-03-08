#-------------------------------------------------
#
# Project generated by QtCreator for SyberOS.
#
#-------------------------------------------------

# This is needed for using syberos-application module
include(../syberos.pri)

QT += core dbus
TARGET = DialAdaptorService
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += syberos-application syberos-application-cache

HEADERS += DialAdaptorService_workspace.h \
    control/pttmediaserviceadaptor.h \
    control/voipcallhandler.h \
    control/voipcallmanageradhoc.h \
    control/voipcallmanagerlte.h \
    control/voipdbusdata.h \
    control/voipserviceadaptor.h \
    adaptor/common.h \
    adaptor/DialHandler.h \
    adaptor/DialManager.h \
    control/wificallmanager.h

SOURCES += main.cpp \
           DialAdaptorService_workspace.cpp \
    control/pttmediaserviceadaptor.cpp \
    control/voipcallhandler.cpp \
    control/voipcallmanageradhoc.cpp \
    control/voipcallmanagerlte.cpp \
    control/voipserviceadaptor.cpp \
    adaptor/DialHandler.cpp \
    adaptor/DialManager.cpp \
    control/wificallmanager.cpp

target.path = $$INSTALL_DIR/bin

INSTALLS += target

OTHER_FILES +=

