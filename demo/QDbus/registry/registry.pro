QT += core dbus
QT -= gui

TARGET = registry
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sinuxtest.cpp

HEADERS += \
    sinuxtest.h \
    structtest.h

