#-------------------------------------------------
#
# Project created by QtCreator 2021-05-06T10:37:57
#
#-------------------------------------------------

QT       += widgets xml network

TARGET = AutoUpdaterUI
TEMPLATE = lib

DEFINES += AUTOUPDATERUI_LIBRARY

CONFIG(release, debug|release){
    TARGET = AutoUpdaterUI
}
else{
    TARGET = AutoUpdaterUI_d
}

DESTDIR = ../../lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        autoupdaterui.cpp \
    cautoupdater.cpp \
    cxmlparser.cpp \
    ftpmanager.cpp

HEADERS += \
        autoupdaterui.h \
        autoupdaterui_global.h \ 
    cautoupdater.h \
    cxmlparser.h \
    ftpmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
