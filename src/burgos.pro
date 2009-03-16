TARGET = burgos
TEMPLATE = app
include(common.pri)
SOURCES += main.cpp \
    ftpmodel.cpp \
    proxymodel.cpp \
    burgos.cpp \
    peermodel.cpp \
    logwidget.cpp \
    ftpwidget.cpp
HEADERS += ftpmodel.h \
    proxymodel.h \
    burgos.h \
    peermodel.h \
    logwidget.h \
    ftpwidget.h
FORMS += logwidget.ui \
    ftpwidget.ui
RESOURCES += burgos.qrc
