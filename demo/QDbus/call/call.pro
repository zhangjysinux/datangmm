QT += core dbus
QT -= gui

TARGET = call
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    signaltest.cpp

HEADERS += \
    structtest.h \
    signaltest.h

