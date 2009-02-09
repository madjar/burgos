TARGET = burgos
TEMPLATE = app
include(common.pri)
SOURCES += main.cpp \
    ftpmodel.cpp \
    proxymodel.cpp \
    burgos.cpp \
    peermodel.cpp \
    utils_gui.cpp
HEADERS += ftpmodel.h \
    proxymodel.h \
    burgos.h \
    peermodel.h
FORMS += burgos.ui
RESOURCES += burgos.qrc
