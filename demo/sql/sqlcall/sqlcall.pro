QT += core sql dbus
QT -= gui

TARGET = sqldata
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    contacterinterfaceadaptor.cpp \
    groupinterfaceadaptor.cpp

HEADERS += \
    contacterdata.h \
    contacterinterfaceadaptor.h \
    commondata.h \
    groupdata.h \
    groupinterfaceadaptor.h

