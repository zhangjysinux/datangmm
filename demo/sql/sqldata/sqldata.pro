QT += core sql dbus
QT -= gui

TARGET = sqldata
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    contacterinterface.cpp \
    groupinterface.cpp

HEADERS += \
    connection.h \
    contacterinterface.h \
    contacterdata.h \
    groupdata.h \
    groupinterface.h \
    commondata.h

RESOURCES +=

