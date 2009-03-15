TARGET = burgos
TEMPLATE = app
include(common.pri)
SOURCES += main.cpp \
    ftpmodel.cpp \
    proxymodel.cpp \
    burgos.cpp \
    peermodel.cpp \
    scanwidget.cpp
HEADERS += ftpmodel.h \
    proxymodel.h \
    burgos.h \
    peermodel.h \
    scanwidget.h
FORMS += burgos.ui \
    scanwidget.ui
RESOURCES += burgos.qrc
unix:SOURCES += buffererrorhandler_gui.cpp
