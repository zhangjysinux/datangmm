#-------------------------------------------------
#
# Project generated by QtCreator for SyberOS.
#
#-------------------------------------------------

QML_IMPORT_PATH = $$[QT_INSTALL_QML]

RESOURCES += res.qrc

CONFIG += link_pkgconfig

PKGCONFIG += syberos-application syberos-application-cache syberos-qt-system

# This is needed for using syberos-application and syberos-qt module
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../syberos_application \
			   $$[QT_INSTALL_HEADERS]/../framework

QT += gui qml quick dbus

TARGET = MultiFunction

#Please Do not modify these macros, otherwise your app will not installed correctly and will not run at all.
APP_DIR = /data/apps
APP_DATA = /data/data
INS_DIR = $$APP_DIR/com.sinux.MultiFunction
DATA_DIR = $$APP_DATA/com.sinux.MultiFunction

DEFINES += SOP_ID=\\\"com.sinux.MultiFunction\\\"
DEFINES += APP_DIR_ENVVAR=\\\"APPDIR_REGULAR\\\"
# Currently home screen sets the environment variable, so when run from
# elsewhere, use this work-around instead.
DEFINES += APP_DIR=\\\"$$APP_DIR\\\"

# The .cpp file which was generated for your project.
SOURCES += src/main.cpp \
           src/MultiFunction_Workspace.cpp \
    src/videorecordpageviewmodel.cpp \
    src/videorecordpagemodel.cpp \
    src/videoviewviewmodel.cpp \
    src/recordfilemodel.cpp \
    src/videoviewmodel.cpp \
    src/realtimevideoviewmodel.cpp \
    src/voipserviceadaptor.cpp \
    src/SharedMemory.cpp \
    src/screenshot.cpp \
    src/tablemodel.cpp \
    src/contacterinterfaceadaptor.cpp \
    src/filesendmodel.cpp \
    src/multifileshareadaptor.cpp \
    src/groupfilesendmodel.cpp \
    src/groupinterfaceadaptor.cpp \
    src/muticastadaptor.cpp

HEADERS += src/MultiFunction_Workspace.h \
    src/videorecordpageviewmodel.h \
    src/videorecordpagemodel.h \
    src/videoviewviewmodel.h \
    src/recordfilemodel.h \
    src/videoviewmodel.h \
    src/realtimevideoviewmodel.h \
    src/voipserviceadaptor.h \
    src/SharedMemory.h \
    src/screenshot.h \
    src/tablemodel.h \
    src/contacterinterfaceadaptor.h \
    ../commondata/datatype/contacterdata.h \
    src/filesendmodel.h \
    src/multifileshareadaptor.h \
    src/groupfilesendmodel.h \
    src/groupinterfaceadaptor.h \
    ../commondata/datatype/groupdata.h \
    src/muticastadaptor.h

# Installation path
target.path = $$INS_DIR/bin

res.files = res
res.path = $$INS_DIR/

meta.files = META-INF
meta.path = $$INS_DIR/

syber.files = sopconfig.xml
syber.path = $$INS_DIR/

INSTALLS += target res meta syber

QML_FILES = qml/*.qml

OTHER_FILES += $$QML_FILES *.qm \
    qml/MainPage.qml \
    qml/MainButton.qml \
    qml/VideoRecordPage.qml \
    qml/VideoViewPage.qml \
    qml/VideoPlayPage.qml \
    qml/SelectContactsPage.qml \
    qml/RealtimeVideoPage.qml \
    qml/ContactPage.qml \
    qml/MessageBox.qml \
    qml/WaitConfirm.qml \
    qml/PersonInfo.qml \
    qml/PersonList.qml \
    qml/SearchLine.qml \
    qml/GroupMain.qml
               


