QT -= gui
TARGET = burgos-cli
TEMPLATE = app
CONFIG += console
include(common.pri)
SOURCES += main-cli.cpp \
    cli.cpp \
    utils_cli.cpp
HEADERS += cli.h
