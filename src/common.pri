QT += network \
    xml
DESTDIR = ..
CONFIG += debug_and_release \
    debug
TRANSLATIONS = burgos_fr.ts
SOURCES += probeftp.cpp \
    peer.cpp \
    messagehandler.cpp \
    ftp.cpp \
    domitem.cpp \
    ftphandler.cpp \
    ping.cpp \
    scanner.cpp \
    scanall.cpp
HEADERS += probeftp.h \
    peer.h \
    messagehandler.h \
    ftp.h \
    domitem.h \
    ftphandler.h \
    ping.h \
    scanner.h \
    scanall.h \
    def.h
unix { 
    SOURCES += buffererrorhandler.cpp
    HEADERS += buffererrorhandler.h
}
