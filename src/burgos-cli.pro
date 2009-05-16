QT -= gui
TARGET = burgos-cli
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
include(common.pri)
SOURCES += main-cli.cpp \
    cli.cpp
HEADERS += cli.h

unix:SOURCES += buffererrorhandler_cli.cpp
