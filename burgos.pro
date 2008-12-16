# -------------------------------------------------
# Project created by QtCreator 2008-11-22T15:54:05
# -------------------------------------------------
QT += network
TARGET = burgos
TEMPLATE = app
SOURCES += main.cpp \
    node.cpp \
    model.cpp \
    file.cpp \
    ftp.cpp \
    proxymodel.cpp \
    burgos.cpp \
    scanftp.cpp \
    probeftp.cpp \
    peer.cpp \
    peermodel.cpp \
    messagehandler.cpp
HEADERS += node.h \
    model.h \
    file.h \
    ftp.h \
    proxymodel.h \
    burgos.h \
    scanftp.h \
    probeftp.h \
    peer.h \
    peermodel.h \
    messagehandler.h
FORMS += burgos.ui
RESOURCES += burgos.qrc
TRANSLATIONS = burgos_fr.ts \
	burgos_en.ts
