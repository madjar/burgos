TARGET = burgos
TEMPLATE = app
include(common.pri)
SOURCES += main.cpp \
    ftpmodel.cpp \
    proxymodel.cpp \
    burgos.cpp \
    peermodel.cpp \
    scanwidget.cpp \
    logwidget.cpp \
    ftpwidget.cpp
HEADERS += ftpmodel.h \
    proxymodel.h \
    burgos.h \
    peermodel.h \
    logwidget.h \
    ftpwidget.h \
    scanwidget.h
FORMS += logwidget.ui \
    ftpwidget.ui \
    scanwidget.ui
RESOURCES += burgos.qrc
unix:SOURCES += buffererrorhandler_gui.cpp
