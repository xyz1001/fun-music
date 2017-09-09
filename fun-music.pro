#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T09:29:20
#
#-------------------------------------------------

QT += core gui
QT += network
QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fun-music
TEMPLATE = app

CONFIG += C++11

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/mp3getter.cpp \
           src/picturegetter.cpp \
           src/htmlgetter.cpp \
           src/htmlhandler.cpp \
           src/datagetter.cpp \
           src/handlerwithhtml.cpp \
           src/clipboardmonitor.cpp \
           src/settings.cpp \
           src/options.cpp \
           src/guidance.cpp \
           src/jsongetter.cpp \
           src/handlerwithjson.cpp \
           src/albumjsonparser.cpp \
           src/detailjsonparser.cpp \
           src/jsonparser.cpp \
           src/networkaccess.cpp \
           src/register.cpp \
           src/devicecheck.cpp

HEADERS  += src/mainwindow.h \
            src/mp3getter.h \
            src/picturegetter.h \
            src/htmlgetter.h \
            src/htmlhandler.h \
            src/datagetter.h \
            src/handlerwithhtml.h \
            src/clipboardmonitor.h \
            src/settings.h \
            src/options.h \
            src/musicinfo.h \
            src/guidance.h \
            src/jsongetter.h \
            src/handlerwithjson.h \
            src/albumjsonparser.h \
            src/detailjsonparser.h \
            src/jsonparser.h \
            src/constant.h \
            src/networkaccess.h \
            src/register.h \
            src/devicecheck.h

FORMS += src/mainwindow.ui \
         src/options.ui \
         src/guidance.ui \
         src/register.ui

RESOURCES += res/image.qrc \
             res/language.qrc \
             res/qss.qrc

TRANSLATIONS += res/language/ts/zh_CN.ts

VERSION = 3.0.1

RC_ICONS = icon.ico
