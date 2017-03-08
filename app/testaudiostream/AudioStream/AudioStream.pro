include(../syberos.pri)
QT += gui qml quick widgets multimedia network dbus

TEMPLATE = app

TARGET = AudioStream

CONFIG += link_pkgconfig

RESOURCES += res.qrc

PKGCONFIG += syberos-application syberos-application-cache

QML_FILES = qml/*.qml

OTHER_FILES += $$QML_FILES *.qm \
    qml/logic.js


# The .cpp file which was generated for your project.
SOURCES += src/main.cpp \
           src/AudioStream_Workspace.cpp

HEADERS += src/AudioStream_Workspace.h

# Installation path
target.path = $$INSTALL_DIR/bin

qm.files = *.qm
qm.path = $$INSTALL_DIR/qm

INSTALLS += target qm

