QT += core network
QT -= gui

TARGET = messagesender
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sender.cpp

HEADERS += \
    messagedata.h \
    sender.h

