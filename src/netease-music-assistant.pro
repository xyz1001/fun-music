#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T09:29:20
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = netease-music-assistant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mp3getter.cpp \
    picturegetter.cpp \
    htmlgetter.cpp \
    htmlhandler.cpp \
    datagetter.cpp \
    handlerwithhtml.cpp \
    clipboardmonitor.cpp \
    settings.cpp \
    options.cpp \
    guidance.cpp \
    jsongetter.cpp \
    handlerwithjson.cpp \
    albumjsonparser.cpp \
    detailjsonparser.cpp \
    jsonparser.cpp \
    networkaccess.cpp \
    register.cpp \
    devicecheck.cpp

HEADERS  += mainwindow.h \
    mp3getter.h \
    picturegetter.h \
    htmlgetter.h \
    htmlhandler.h \
    datagetter.h \
    handlerwithhtml.h \
    clipboardmonitor.h \
    settings.h \
    options.h \
    musicinfo.h \
    guidance.h \
    jsongetter.h \
    handlerwithjson.h \
    albumjsonparser.h \
    detailjsonparser.h \
    jsonparser.h \
    constant.h \
    networkaccess.h \
    register.h \
    devicecheck.h

FORMS    += mainwindow.ui \
    options.ui \
    guidance.ui \
    register.ui

CONFIG   += C++11

TRANSLATIONS    += myI18N_zh_CN.ts

RESOURCES += \
    res.qrc

VERSION = 3.0.1

RC_ICONS = icon.ico
