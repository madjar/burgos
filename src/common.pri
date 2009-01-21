QT += network \
    xml
DESTDIR = ..
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
    ftphandler.h
