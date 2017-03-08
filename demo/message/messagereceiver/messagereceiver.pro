QT += core sql network
QT -= gui

TARGET = messagereceiver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    receiver.cpp

HEADERS += \
    messagedata.h \
    receiver.h

