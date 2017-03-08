#-------------------------------------------------
#
# Project generated by QtCreator for SyberOS.
#
#-------------------------------------------------

# This is needed for using syberos-application module
include(../syberos.pri)

QT += core dbus qml quick sql bluetooth
TARGET = TerminalcallService
TEMPLATE = app
DEFINES += TERMINAL

CONFIG += link_pkgconfig
PKGCONFIG += syberos-application syberos-application-cache
PKGCONFIG += syberos-qt-system

HEADERS += TerminalcallService_workspace.h \
    ../../PhoneCall/MyService/contacterinterfaceadaptor.h \
    ../../PhoneCall/MyService/dialinterface.h \
    ../../PhoneCall/MyService/miniphonecalldata.h \
    ../../PhoneCall/MyService/voipserviceadaptor.h \
    ../../commondata/callmanageradaptor/DialHandler.h \
    ../../commondata/callmanageradaptor/DialManager.h \
    ../../commondata/datatype/contacterdata.h \
    ../../commondata/datatype/dialdata.h \
    ../../commondata/bluetoothbackgroundtransferinterface.h \
    ../../commondata/bluetoothdata.h \
    ../../commondata/datatype/commondata.h

SOURCES += main.cpp \
	   TerminalcallService_workspace.cpp \
    ../../PhoneCall/MyService/contacterinterfaceadaptor.cpp \
    ../../PhoneCall/MyService/dialinterface.cpp \
    ../../PhoneCall/MyService/voipserviceadaptor.cpp \
    ../../commondata/callmanageradaptor/DialHandler.cpp \
    ../../commondata/callmanageradaptor/DialManager.cpp \
    ../../commondata/bluetoothbackgroundtransferinterface.cpp

target.path = $$INSTALL_DIR/bin

INSTALLS += target

