QT += core network
QT -= gui

CONFIG += c++11

TARGET = padisco
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pafinder.cpp \
    padevice.cpp \
    papacket.cpp \
    pacontroller.cpp

HEADERS += \
    pafinder.h \
    padevice.h \
    papacket.h \
    pacontroller.h
