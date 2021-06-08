#-------------------------------------------------
#
# Project created by QtCreator 2021-04-26T14:52:35
#
#-------------------------------------------------

QT  += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoUpdater
TEMPLATE = app

RC_FILE = app.rc

MOC_DIR = $$OUT_PWD/mocfile
OBJECTS_DIR = $$OUT_PWD/objfile
UI_DIR = $$OUT_PWD/uifile
RCC_DIR = $$OUT_PWD/rcfile

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
    autoresize/autoresize.cpp \
    errorstack/errorstack.cpp \
    ftpmanager/ftpmanager.cpp \
    loger/log.cpp \
    mainui/updaterui.cpp \
    updater/updater.cpp \
    xmlparse/xmlparser.cpp \
    main.cpp

HEADERS += \
    autoresize/autoresize.h \
    errorstack/errorstack.h \
    ftpmanager/ftpmanager.h \
    loger/log.h \
    mainui/updaterui.h \
    updater/updater.h \
    xmlparse/xmlparser.h \
    globaldef.hpp

FORMS += \
    mainwindow.ui

TRANSLATIONS += ./res/translater/zh_en.ts

RESOURCES += \
    res/icon.qrc \
    res/qt_language.qrc \
    res/stylesheet.qrc \
    res/image.qrc

