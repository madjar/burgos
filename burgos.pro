# -------------------------------------------------
# Project created by QtCreator 2008-11-22T15:54:05
# -------------------------------------------------
QT += network \
    xml
TARGET = burgos
TEMPLATE = app
SOURCES += main.cpp \
    ftpmodel.cpp \
    proxymodel.cpp \
    burgos.cpp \
    scanftp.cpp \
    probeftp.cpp \
    peer.cpp \
    peermodel.cpp \
    messagehandler.cpp \
    ftp.cpp \
    domitem.cpp \
    ftphandler.cpp
HEADERS += ftpmodel.h \
    proxymodel.h \
    burgos.h \
    scanftp.h \
    probeftp.h \
    peer.h \
    peermodel.h \
    messagehandler.h \
    ftp.h \
    domitem.h \
    ftphandler.h
FORMS += burgos.ui
RESOURCES += burgos.qrc
TRANSLATIONS = burgos_fr.ts
