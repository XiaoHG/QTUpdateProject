#-------------------------------------------------
#
# Project created by QtCreator 2021-04-20T09:29:24
#
#-------------------------------------------------

QT       += widgets gui

CONFIG(debug, debug | release){
    TARGET = UpdateClientUI_d
}else{
    TARGET = UpdateClientUI
}

TEMPLATE = lib

DEFINES += UPDATECLIENTUI_LIBRARY

DESTDIR = ../../lib
DLLDESTDIR = ../../bin

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
<<<<<<< HEAD
        updateclientui.cpp

HEADERS += \
        updateclientui.h \
        updateclientui_global.h  
=======
        updateclientui.cpp \
    updateclientfilecontroler.cpp \
    serverrequest.cpp \
    versioninfocontroler.cpp

HEADERS += \
        updateclientui.h \
        updateclientui_global.h \ 
    updateclientfilecontroler.h \
    serverrequest.h \
    versioninfocontroler.h
>>>>>>> 0dc6cca643f408d20ee0f1e43a94359f09ee6e70

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    ../../source/icon.qrc


