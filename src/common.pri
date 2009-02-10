QT += network \
    xml
DESTDIR = ..
CONFIG += debug_and_release \
    debug
TRANSLATIONS = burgos_fr.ts
SOURCES += scanftp.cpp \
    probeftp.cpp \
    peer.cpp \
    messagehandler.cpp \
    ftp.cpp \
    domitem.cpp \
    ftphandler.cpp
HEADERS += scanftp.h \
    probeftp.h \
    peer.h \
    messagehandler.h \
    ftp.h \
    domitem.h \
    ftphandler.h \
    ping.h
unix {
SOURCES += ping_unix.cpp buffererrorhandler.cpp
HEADERS += buffererrorhandler.h
}
